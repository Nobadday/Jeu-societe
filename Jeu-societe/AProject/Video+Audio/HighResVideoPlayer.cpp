#include "HighResVideoPlayer.hpp"
#include <iostream>
#include <algorithm>

// ===== Audio Stream =====

HighResAudioStream::HighResAudioStream() : m_endOfStream(false), m_volume(1.0f)
{
	// SFML 2.6 : initialize avec 2 canaux et 44100 Hz (pas de SoundChannel)
	initialize(2, 44100);
	m_tempBuffer.reserve(44100);
}

void HighResAudioStream::addSamples(const int16_t* samples, std::size_t count)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_samples.insert(m_samples.end(), samples, samples + count);
}

void HighResAudioStream::setEndOfStream()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_endOfStream = true;
}

void HighResAudioStream::clearBuffer()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_samples.clear();
	m_endOfStream = false;
}

bool HighResAudioStream::isFinished() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_endOfStream && m_samples.empty();
}

void HighResAudioStream::setVolume(float volume)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_volume = std::max(0.0f, std::min(1.0f, volume));
	SoundStream::setVolume(m_volume * 100.0f);
}

bool HighResAudioStream::onGetData(Chunk& data)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	const std::size_t chunkSize = 44100 / 5 * 2;

	if (m_samples.empty())
	{
		if (m_endOfStream)
			return false;

		if (m_tempBuffer.size() < chunkSize)
			m_tempBuffer.resize(chunkSize);
		std::fill(m_tempBuffer.begin(), m_tempBuffer.begin() + chunkSize, 0);
		data.samples = m_tempBuffer.data();
		data.sampleCount = chunkSize;
		return true;
	}

	std::size_t samplesToPlay = std::min(chunkSize, m_samples.size());
	if (m_tempBuffer.size() < samplesToPlay)
		m_tempBuffer.resize(samplesToPlay);

	std::copy(m_samples.begin(), m_samples.begin() + samplesToPlay, m_tempBuffer.begin());
	m_samples.erase(m_samples.begin(), m_samples.begin() + samplesToPlay);

	data.samples = m_tempBuffer.data();
	data.sampleCount = samplesToPlay;
	return true;
}

void HighResAudioStream::onSeek(sf::Time) {}

// ===== Video Player =====

HighResVideoPlayer::HighResVideoPlayer(const HighResConfig& config)
	: m_format(nullptr)
	, m_videoContext(nullptr)
	, m_audioContext(nullptr)
	, m_swsContext(nullptr)
	, m_swrContext(nullptr)
	, m_videoFrame(nullptr)
	, m_rgbaFrame(nullptr)
	, m_audioFrame(nullptr)
	, m_videoBuffer(nullptr)
	, m_videoStreamIndex(-1)
	, m_audioStreamIndex(-1)
	, m_videoClock(0.0f)
	, m_audioClock(0.0f)
	, m_timePerFrame(0.0f)
	, m_lastFrameTime(0.0f)
	, m_originalWidth(0)
	, m_originalHeight(0)
	, m_renderWidth(0)
	, m_renderHeight(0)
	, m_isInitialized(false)
	, m_frameReady(false)
	, m_audioStarted(false)
	, m_videoEnded(false)
	, m_paused(true)  // ← CORRECTION : true au lieu de false
	, m_speed(1.0f)
	, m_audioEnabled(true)
	, m_fullscreen(false)
	, m_config(config)
	, m_duration(0.0f)
{
	m_audioBuffer.resize(8192 * 2);
	avformat_network_init();
}

HighResVideoPlayer::~HighResVideoPlayer()
{
	close();
	avformat_network_deinit();
}

bool HighResVideoPlayer::loadFromFile(const std::string& filename)
{
	close();
	m_filename = filename;

	if (avformat_open_input(&m_format, filename.c_str(), nullptr, nullptr) != 0)
	{
		std::cerr << "Erreur: impossible d'ouvrir " << filename << std::endl;
		return false;
	}

	if (avformat_find_stream_info(m_format, nullptr) < 0)
	{
		std::cerr << "Erreur: pas d'info stream" << std::endl;
		avformat_close_input(&m_format);
		return false;
	}

	if (m_format->duration != AV_NOPTS_VALUE)
	{
		m_duration = static_cast<float>(m_format->duration) / AV_TIME_BASE;
	}

	for (unsigned i = 0; i < m_format->nb_streams; i++)
	{
		if (m_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && m_videoStreamIndex == -1)
			m_videoStreamIndex = i;
		if (m_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && m_audioStreamIndex == -1)
			m_audioStreamIndex = i;
	}

	if (m_videoStreamIndex == -1)
	{
		std::cerr << "Erreur: pas de flux video" << std::endl;
		close();
		return false;
	}

	if (!initializeVideoDecoder())
	{
		close();
		return false;
	}

	if (m_audioStreamIndex != -1)
	{
		initializeAudioDecoder();
	}

	m_isInitialized = true;

	std::cout << "=== VIDEO CHARGEE ===" << std::endl;
	std::cout << "Resolution: " << m_originalWidth << "x" << m_originalHeight
		<< " -> " << m_renderWidth << "x" << m_renderHeight << std::endl;
	std::cout << "FPS: " << (1.0f / m_timePerFrame) << std::endl;
	std::cout << "Duree: " << m_duration << " secondes" << std::endl;
	std::cout << "=====================" << std::endl;

	return true;
}

bool HighResVideoPlayer::initializeVideoDecoder()
{
	const AVCodec* codec = avcodec_find_decoder(m_format->streams[m_videoStreamIndex]->codecpar->codec_id);
	if (!codec)
	{
		std::cerr << "Codec non trouve" << std::endl;
		return false;
	}

	m_videoContext = avcodec_alloc_context3(codec);
	avcodec_parameters_to_context(m_videoContext, m_format->streams[m_videoStreamIndex]->codecpar);

	m_videoContext->thread_count = 4;
	m_videoContext->thread_type = FF_THREAD_FRAME;

	if (avcodec_open2(m_videoContext, codec, nullptr) < 0)
	{
		std::cerr << "Erreur ouverture codec" << std::endl;
		return false;
	}

	m_originalWidth = m_videoContext->width;
	m_originalHeight = m_videoContext->height;

	calculateRenderSize();

	// SFML 2.6 : create() au lieu de resize()
	if (!m_texture.create(m_renderWidth, m_renderHeight))
	{
		std::cerr << "Erreur texture" << std::endl;
		return false;
	}

	m_texture.setSmooth(false);
	m_sprite.setTexture(m_texture.getTexture(), true);

	float scaleX = static_cast<float>(SCREEN_WIDTH) / m_renderWidth;
	float scaleY = static_cast<float>(SCREEN_HEIGHT) / m_renderHeight;
	float scale = std::min(scaleX, scaleY);
	m_sprite.setScale({ scale, scale });

	float posX = (SCREEN_WIDTH - m_renderWidth * scale) / 2.0f;
	float posY = (SCREEN_HEIGHT - m_renderHeight * scale) / 2.0f;
	m_sprite.setPosition({ posX, posY });

	m_swsContext = sws_getContext(
		m_originalWidth, m_originalHeight, m_videoContext->pix_fmt,
		m_renderWidth, m_renderHeight, AV_PIX_FMT_RGBA,
		SWS_FAST_BILINEAR, nullptr, nullptr, nullptr
	);

	m_videoFrame = av_frame_alloc();
	m_rgbaFrame = av_frame_alloc();
	m_videoBuffer = new uint8_t[m_renderWidth * m_renderHeight * 4];
	av_image_fill_arrays(m_rgbaFrame->data, m_rgbaFrame->linesize, m_videoBuffer,
		AV_PIX_FMT_RGBA, m_renderWidth, m_renderHeight, 1);

	m_timePerFrame = (float)av_q2d(m_format->streams[m_videoStreamIndex]->avg_frame_rate);
	m_timePerFrame = (m_timePerFrame > 0.0f) ? (1.0f / m_timePerFrame) : (1.0f / 30.0f);

	return true;
}

bool HighResVideoPlayer::initializeAudioDecoder()
{
	const AVCodec* codec = avcodec_find_decoder(m_format->streams[m_audioStreamIndex]->codecpar->codec_id);
	if (!codec) return false;

	m_audioContext = avcodec_alloc_context3(codec);
	avcodec_parameters_to_context(m_audioContext, m_format->streams[m_audioStreamIndex]->codecpar);

	if (avcodec_open2(m_audioContext, codec, nullptr) < 0)
		return false;

	m_swrContext = swr_alloc();
	AVChannelLayout stereoLayout = AV_CHANNEL_LAYOUT_STEREO;
	swr_alloc_set_opts2(
		&m_swrContext,
		&stereoLayout, AV_SAMPLE_FMT_S16, 44100,
		&m_audioContext->ch_layout, m_audioContext->sample_fmt, m_audioContext->sample_rate,
		0, nullptr
	);

	if (swr_init(m_swrContext) < 0)
		return false;

	m_audioFrame = av_frame_alloc();
	return true;
}

void HighResVideoPlayer::calculateRenderSize()
{
	switch (m_config.sizeMode)
	{
	case VideoSizeMode::Original:
		// Taille originale de la vidéo
		m_renderWidth = m_originalWidth;
		m_renderHeight = m_originalHeight;
		break;

	case VideoSizeMode::Custom:
		// Taille personnalisée avec conservation du ratio
		if (m_config.customWidth > 0 && m_config.customHeight > 0)
		{
			m_renderWidth = m_config.customWidth;
			m_renderHeight = m_config.customHeight;
		}
		else if (m_config.customWidth > 0)
		{
			// Seulement largeur spécifiée, calcule hauteur proportionnelle
			float ratio = static_cast<float>(m_originalHeight) / m_originalWidth;
			m_renderWidth = m_config.customWidth;
			m_renderHeight = static_cast<int>(m_config.customWidth * ratio);
		}
		else if (m_config.customHeight > 0)
		{
			// Seulement hauteur spécifiée, calcule largeur proportionnelle
			float ratio = static_cast<float>(m_originalWidth) / m_originalHeight;
			m_renderWidth = static_cast<int>(m_config.customHeight * ratio);
			m_renderHeight = m_config.customHeight;
		}
		else
		{
			// Aucune dimension spécifiée, utilise l'originale
			m_renderWidth = m_originalWidth;
			m_renderHeight = m_originalHeight;
		}
		break;

	case VideoSizeMode::FitToScreen:
		// Ajuste à la taille de l'écran en conservant le ratio
		{
			float scaleX = static_cast<float>(SCREEN_WIDTH) / m_originalWidth;
			float scaleY = static_cast<float>(SCREEN_HEIGHT) / m_originalHeight;
			float scale = std::min(scaleX, scaleY);
			m_renderWidth = static_cast<int>(m_originalWidth * scale);
			m_renderHeight = static_cast<int>(m_originalHeight * scale);
		}
		break;

	case VideoSizeMode::DownscaleFactor:
	default:
		// Comportement original avec facteur de downscale
		if (m_originalWidth > 1920 || m_originalHeight > 1080)
		{
			m_renderWidth = m_originalWidth / m_config.downscaleFactor;
			m_renderHeight = m_originalHeight / m_config.downscaleFactor;
		}
		else
		{
			m_renderWidth = m_originalWidth / m_config.downscaleFactor;
			m_renderHeight = m_originalHeight / m_config.downscaleFactor;
		}
		break;
	}

	// Limite à la taille maximale de texture
	if (m_renderWidth > m_config.maxTextureSize)
	{
		float ratio = static_cast<float>(m_config.maxTextureSize) / m_renderWidth;
		m_renderWidth = m_config.maxTextureSize;
		m_renderHeight = static_cast<int>(m_renderHeight * ratio);
	}
	
	if (m_renderHeight > m_config.maxTextureSize)
	{
		float ratio = static_cast<float>(m_config.maxTextureSize) / m_renderHeight;
		m_renderHeight = m_config.maxTextureSize;
		m_renderWidth = static_cast<int>(m_renderWidth * ratio);
	}

	// S'assure que les dimensions sont paires (requis pour certains codecs)
	m_renderWidth = (m_renderWidth / 2) * 2;
	m_renderHeight = (m_renderHeight / 2) * 2;
}

void HighResVideoPlayer::resyncAudioVideo()
{
	m_audioStream.clearBuffer();
	m_audioClock = m_videoClock;
}

void HighResVideoPlayer::update(float deltaTime)
{
	if (!m_isInitialized || m_paused)
		return;

	if (m_videoEnded && m_config.enableLoop)
	{
		restart();
		return;
	}

	if (m_videoEnded)
		return;

	float effectiveDelta = std::min(deltaTime * m_speed, 0.1f);
	m_videoClock += effectiveDelta;

	int maxFramesToDecode = (m_speed > 3.0f) ? static_cast<int>(m_speed * 2) : 5;
	int framesDecoded = 0;

	while (m_videoClock >= m_timePerFrame && !m_videoEnded && framesDecoded < maxFramesToDecode)
	{
		AVPacket packet;
		int readResult = av_read_frame(m_format, &packet);

		if (readResult < 0)
		{
			m_videoEnded = true;
			m_audioStream.setEndOfStream();
			std::cout << "Fin de la video" << std::endl;
			break;
		}

		if (packet.stream_index == m_videoStreamIndex)
		{
			avcodec_send_packet(m_videoContext, &packet);
			if (avcodec_receive_frame(m_videoContext, m_videoFrame) == 0)
			{
				sws_scale(m_swsContext, m_videoFrame->data, m_videoFrame->linesize,
					0, m_originalHeight, m_rgbaFrame->data, m_rgbaFrame->linesize);

				m_texture.clear();
				sf::Texture temp;
				// SFML 2.6 : create() au lieu de resize()
				temp.create(m_renderWidth, m_renderHeight);
				temp.update(m_rgbaFrame->data[0]);
				sf::Sprite tempSprite(temp);
				m_texture.draw(tempSprite);
				m_texture.display();
				m_frameReady = true;
				m_videoClock -= m_timePerFrame;
				m_lastFrameTime = m_videoClock;
				framesDecoded++;
			}
		}

		if (packet.stream_index == m_audioStreamIndex && m_audioContext)
		{
			avcodec_send_packet(m_audioContext, &packet);
			while (avcodec_receive_frame(m_audioContext, m_audioFrame) == 0)
			{
				uint8_t* out[] = { (uint8_t*)m_audioBuffer.data() };
				int count = swr_convert(m_swrContext, out,
					static_cast<int>(m_audioBuffer.size() / 2),
					(const uint8_t**)m_audioFrame->data,
					m_audioFrame->nb_samples);

				if (count > 0)
				{
					m_audioStream.addSamples(m_audioBuffer.data(), count * 2);

					if (!m_audioStarted && m_audioEnabled)
					{
						m_audioStream.play();
						m_audioStarted = true;
					}
				}
			}
		}
		else if (packet.stream_index == m_audioStreamIndex && m_speed > 5.0f)
		{
			if (m_audioStarted && m_audioEnabled)
			{
				m_audioStream.pause();
			}
		}

		av_packet_unref(&packet);
	}
}

bool HighResVideoPlayer::seekToTime(float seconds)
{
	if (!m_isInitialized || m_duration <= 0)
		return false;

	seconds = std::max(0.0f, std::min(seconds, m_duration));

	int64_t seekTarget = static_cast<int64_t>(seconds / av_q2d(m_format->streams[m_videoStreamIndex]->time_base));

	if (av_seek_frame(m_format, m_videoStreamIndex, seekTarget, AVSEEK_FLAG_BACKWARD) < 0)
	{
		std::cerr << "Erreur lors du seek" << std::endl;
		return false;
	}

	avcodec_flush_buffers(m_videoContext);
	if (m_audioContext)
	{
		avcodec_flush_buffers(m_audioContext);
	}

	m_videoClock = seconds;
	m_audioClock = seconds;
	resyncAudioVideo();

	if (m_audioStarted && m_audioEnabled && !m_paused)
	{
		m_audioStream.play();
	}

	m_videoEnded = false;

	std::cout << "Seek vers " << seconds << "s" << std::endl;
	return true;
}

void HighResVideoPlayer::seek(float progress)
{
	progress = std::max(0.0f, std::min(1.0f, progress));
	seekToTime(progress * m_duration);
}

void HighResVideoPlayer::seekRelative(float seconds)
{
	float targetTime = m_lastFrameTime + seconds;
	seekToTime(targetTime);
}

void HighResVideoPlayer::restart()
{
	std::cout << "Redemarrage de la video..." << std::endl;
	
	float savedSpeed = m_speed;
	float savedVolume = getVolume();
	bool savedAudioEnabled = m_audioEnabled;
	bool savedLoop = m_config.enableLoop;
	
	m_audioStream.stop();
	m_audioStream.clearBuffer();
	
	close();
	loadFromFile(m_filename);
	
	m_config.enableLoop = savedLoop;
	setVolume(savedVolume);
	setAudioEnabled(savedAudioEnabled);
	
	// SFML 2.6 n'a pas setPitch(), on ignore la vitesse audio
	m_speed = savedSpeed;
	
	m_audioStarted = false;
	m_paused = true;
	
	std::cout << "Video redemarree (vitesse: " << m_speed << "x)" << std::endl;
}

void HighResVideoPlayer::toggleFullscreen()
{
	m_fullscreen = !m_fullscreen;

	if (m_fullscreen)
	{
		float scaleX = static_cast<float>(SCREEN_WIDTH) / m_renderWidth;
		float scaleY = static_cast<float>(SCREEN_HEIGHT) / m_renderHeight;
		m_sprite.setScale({ scaleX, scaleY });
		m_sprite.setPosition({ 0, 0 });
		std::cout << "Mode plein ecran active" << std::endl;
	}
	else
	{
		float scaleX = static_cast<float>(SCREEN_WIDTH) / m_renderWidth;
		float scaleY = static_cast<float>(SCREEN_HEIGHT) / m_renderHeight;
		float scale = std::min(scaleX, scaleY);
		m_sprite.setScale({ scale, scale });

		float posX = (SCREEN_WIDTH - m_renderWidth * scale) / 2.0f;
		float posY = (SCREEN_HEIGHT - m_renderHeight * scale) / 2.0f;
		m_sprite.setPosition({ posX, posY });
		std::cout << "Mode fenetre active" << std::endl;
	}
}

void HighResVideoPlayer::setVolume(float volume)
{
	m_audioStream.setVolume(volume);
}

void HighResVideoPlayer::setConfig(const HighResConfig& config)
{
	m_config = config;
}

float HighResVideoPlayer::getVolume() const
{
	return m_audioStream.getVolume();
}

void HighResVideoPlayer::close()
{
	if (!m_isInitialized && !m_format)
		return;

	m_audioStream.stop();

	if (m_videoFrame) av_frame_free(&m_videoFrame);
	if (m_rgbaFrame) av_frame_free(&m_rgbaFrame);
	if (m_audioFrame) av_frame_free(&m_audioFrame);
	if (m_videoBuffer) delete[] m_videoBuffer;
	if (m_swsContext) sws_freeContext(m_swsContext);
	if (m_swrContext) swr_free(&m_swrContext);
	if (m_videoContext) avcodec_free_context(&m_videoContext);
	if (m_audioContext) avcodec_free_context(&m_audioContext);
	if (m_format) avformat_close_input(&m_format);

	m_videoFrame = nullptr;
	m_rgbaFrame = nullptr;
	m_audioFrame = nullptr;
	m_videoBuffer = nullptr;
	m_swsContext = nullptr;
	m_swrContext = nullptr;
	m_videoContext = nullptr;
	m_audioContext = nullptr;
	m_format = nullptr;

	m_isInitialized = false;
	m_frameReady = false;
	m_audioStarted = false;
	m_videoEnded = false;
}

void HighResVideoPlayer::setPaused(bool paused)
{
	m_paused = paused;
	if (m_paused)
		m_audioStream.pause();
	else if (m_audioEnabled && m_audioStarted)
		m_audioStream.play();
}

void HighResVideoPlayer::setSpeed(float speed)
{
	if (speed < 0.25f) speed = 0.25f;
	if (speed > 5.0f) speed = 5.0f;

	m_speed = speed;
	
	// SFML 2.6 : setPitch() n'existe pas, on ajuste uniquement la vitesse vidéo
	// L'audio joue à vitesse normale
}

void HighResVideoPlayer::setAudioEnabled(bool enabled)
{
	m_audioEnabled = enabled;
	if (!enabled && m_audioStarted)
		m_audioStream.pause();
	else if (enabled && m_audioStarted && !m_paused)
		m_audioStream.play();
}

void HighResVideoPlayer::pollEvent(const sf::Event& event)
{
	// SFML 2.6 : event.type au lieu de event.getIf<>()
	if (event.type == sf::Event::KeyPressed)
	{
		// SFML 2.6 : sf::Keyboard::Right au lieu de sf::Keyboard::Key::Right
		if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Add)
		{
			setSpeed(m_speed + 0.25f);
			std::cout << "Vitesse: " << m_speed << "x" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Subtract)
		{
			setSpeed(m_speed - 0.25f);
			std::cout << "Vitesse: " << m_speed << "x" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::Space)
		{
			setSpeed(1.0f);
			std::cout << "Vitesse: 1.0x (normale)" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1)
		{
			setSpeed(0.25f);
			std::cout << "Vitesse: 0.25x" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2)
		{
			setSpeed(0.5f);
			std::cout << "Vitesse: 0.5x" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3)
		{
			setSpeed(1.0f);
			std::cout << "Vitesse: 1.0x" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4)
		{
			setSpeed(2.0f);
			std::cout << "Vitesse: 2.0x" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Num5 || event.key.code == sf::Keyboard::Numpad5)
		{
			setSpeed(4.0f);
			std::cout << "Vitesse: 4.0x" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::A)
		{
			setAudioEnabled(!m_audioEnabled);
			std::cout << "Audio: " << (m_audioEnabled ? "Active" : "Desactive") << std::endl;
		}
		else if (event.key.code == sf::Keyboard::P)
		{
			setPaused(!m_paused);
			std::cout << "Video: " << (m_paused ? "En pause" : "En lecture") << std::endl;
		}
		else if (event.key.code == sf::Keyboard::L)
		{
			setLoop(!m_config.enableLoop);
			std::cout << "Loop: " << (m_config.enableLoop ? "ON" : "OFF") << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Up)
		{
			float newVolume = std::min(1.0f, getVolume() + 0.1f);
			setVolume(newVolume);
			std::cout << "Volume: " << static_cast<int>(newVolume * 100) << "%" << std::endl;
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			float newVolume = std::max(0.0f, getVolume() - 0.1f);
			setVolume(newVolume);
			std::cout << "Volume: " << static_cast<int>(newVolume * 100) << "%" << std::endl;
		}
	}
}

void HighResVideoPlayer::play()
{
    if (!m_isInitialized)
    {
        std::cerr << "Erreur: Video non initialisee. Appelez loadFromFile() d'abord." << std::endl;
        return;
    }

    // Si la vidéo est terminée, on la redémarre
    if (m_videoEnded)
    {
        restart();
    }

    // Dépause la vidéo si elle était en pause
    if (m_paused)
    {
        setPaused(false);
    }

    // Démarre l'audio si nécessaire
    if (!m_audioStarted && m_audioEnabled && m_audioStreamIndex != -1)
    {
        m_audioStream.play();
        m_audioStarted = true;
    }

    std::cout << "Lecture de la video lancee" << std::endl;
}

bool HighResVideoPlayer::isFinish() const
{
	return m_videoEnded;
}

void HighResVideoPlayer::setVideoSize(int width, int height)
{
	if (!m_isInitialized)
	{
		std::cerr << "Erreur: Video non initialisee" << std::endl;
		return;
	}

	// Sauvegarde l'état actuel
	float currentTime = m_videoClock;
	bool wasPaused = m_paused;

	// Met à jour la configuration
	m_config.sizeMode = VideoSizeMode::Custom;
	m_config.customWidth = width;
	m_config.customHeight = height;

	// Recalcule et réinitialise
	int oldWidth = m_renderWidth;
	int oldHeight = m_renderHeight;
	
	calculateRenderSize();

	if (oldWidth != m_renderWidth || oldHeight != m_renderHeight)
	{
		// Libère les anciennes ressources
		if (m_swsContext) sws_freeContext(m_swsContext);
		if (m_rgbaFrame) av_frame_free(&m_rgbaFrame);
		if (m_videoBuffer) delete[] m_videoBuffer;

		// Recrée la texture
		m_texture.clear();
		if (!m_texture.create(m_renderWidth, m_renderHeight))
		{
			std::cerr << "Erreur: impossible de recreer la texture" << std::endl;
			return;
		}

		m_texture.setSmooth(false);
		m_sprite.setTexture(m_texture.getTexture(), true);

		// Recalcule l'échelle et la position
		float scaleX = static_cast<float>(SCREEN_WIDTH) / m_renderWidth;
		float scaleY = static_cast<float>(SCREEN_HEIGHT) / m_renderHeight;
		float scale = std::min(scaleX, scaleY);
		m_sprite.setScale({ scale, scale });

		float posX = (SCREEN_WIDTH - m_renderWidth * scale) / 2.0f;
		float posY = (SCREEN_HEIGHT - m_renderHeight * scale) / 2.0f;
		m_sprite.setPosition({ posX, posY });

		// Recrée le contexte de conversion
		m_swsContext = sws_getContext(
			m_originalWidth, m_originalHeight, m_videoContext->pix_fmt,
			m_renderWidth, m_renderHeight, AV_PIX_FMT_RGBA,
			SWS_FAST_BILINEAR, nullptr, nullptr, nullptr
		);

		// Recrée le buffer RGBA
		m_rgbaFrame = av_frame_alloc();
		m_videoBuffer = new uint8_t[m_renderWidth * m_renderHeight * 4];
		av_image_fill_arrays(m_rgbaFrame->data, m_rgbaFrame->linesize, m_videoBuffer,
			AV_PIX_FMT_RGBA, m_renderWidth, m_renderHeight, 1);

		std::cout << "Taille video modifiee: " << m_renderWidth << "x" << m_renderHeight << std::endl;
	}

	// Restaure l'état
	if (!wasPaused)
		setPaused(false);
}