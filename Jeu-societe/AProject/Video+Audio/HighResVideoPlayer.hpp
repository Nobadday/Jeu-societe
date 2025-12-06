#ifndef _INC_HIGHRESVIDEOPLAYER_HPP
#define _INC_HIGHRESVIDEOPLAYER_HPP

#include "VideoCommun.hpp"

// Audio stream avec Volume - SFML 2.6

class HighResAudioStream : public sf::SoundStream
{
private:
    std::deque<int16_t> m_samples;
    mutable std::mutex m_mutex;
    bool m_endOfStream;
    std::vector<int16_t> m_tempBuffer;
    float m_volume;

public:
    HighResAudioStream();
    void addSamples(const int16_t* samples, std::size_t count);
    void setEndOfStream();
    void clearBuffer();
    bool isFinished() const;
    void setVolume(float volume);
    float getVolume() const { return m_volume; }

protected:
    virtual bool onGetData(Chunk& data) override;
    virtual void onSeek(sf::Time timeOffset) override;
};

// Modes de dimensionnement vidéo
enum class VideoSizeMode
{
    Original,          // Taille originale de la vidéo
    Custom,            // Taille personnalisée (utilise customWidth/customHeight)
    DownscaleFactor,   // Utilise le facteur de downscale
    FitToScreen        // Ajuste à la taille de l'écran en conservant le ratio
};

// Configuration
struct HighResConfig
{
    VideoSizeMode sizeMode = VideoSizeMode::DownscaleFactor;
    int downscaleFactor = 1;
    int customWidth = 0;      // Largeur personnalisée (utilisé si sizeMode == Custom)
    int customHeight = 0;     // Hauteur personnalisée (utilisé si sizeMode == Custom)
    int maxTextureSize = 4096;
    bool enableLoop = false;
};

// Lecteur vidéo SANS HUD
class HighResVideoPlayer
{
private:
    // FFmpeg
    AVFormatContext* m_format;
    AVCodecContext* m_videoContext;
    AVCodecContext* m_audioContext;
    SwsContext* m_swsContext;
    SwrContext* m_swrContext;

    AVFrame* m_videoFrame;
    AVFrame* m_rgbaFrame;
    AVFrame* m_audioFrame;
    uint8_t* m_videoBuffer;
    std::vector<int16_t> m_audioBuffer;

    // Streams
    int m_videoStreamIndex;
    int m_audioStreamIndex;

    // Rendu SFML - MISE À JOUR SFML 2.6
    sf::RenderTexture m_texture;
    sf::Sprite m_sprite;
    HighResAudioStream m_audioStream;

    // Timing - AMÉLIORATION pour sync audio-vidéo
    float m_videoClock;
    float m_audioClock;
    float m_timePerFrame;
    float m_lastFrameTime;

    // Dimensions
    int m_originalWidth;
    int m_originalHeight;
    int m_renderWidth;
    int m_renderHeight;

    // État
    bool m_isInitialized;
    bool m_frameReady;
    bool m_audioStarted;
    bool m_videoEnded;
    bool m_paused;
    float m_speed;
    bool m_audioEnabled;
    bool m_fullscreen;

    HighResConfig m_config;
    std::string m_filename;
    float m_duration;

    // Méthodes privées
    bool initializeVideoDecoder();
    bool initializeAudioDecoder();
    void calculateRenderSize();
    bool seekToTime(float seconds);
    void restart();
    void resyncAudioVideo();

public:
    HighResVideoPlayer(const HighResConfig& config = HighResConfig());
    ~HighResVideoPlayer();

    bool loadFromFile(const std::string& filename);
    void update(float deltaTime);
    void close();

    // Accesseurs
    const sf::Sprite& getSprite() const { return m_sprite; }
    bool isInitialized() const { return m_isInitialized; }
    bool isReady() const { return m_frameReady; }
    bool isFinish() const;
    float getCurrentTime() const { return m_videoClock; }
    float getDuration() const { return m_duration; }
    int getRenderWidth() const { return m_renderWidth; }
    int getRenderHeight() const { return m_renderHeight; }
    int getOriginalWidth() const { return m_originalWidth; }
    int getOriginalHeight() const { return m_originalHeight; }

    // Contrôles
    void setPaused(bool paused);
    void setSpeed(float speed);
    void setAudioEnabled(bool enabled);
    void setVolume(float volume);
    void setConfig(const HighResConfig& config);
    void setVideoSize(int width, int height); // Nouvelle méthode pour changer la taille
    
    float getVolume() const;
    
    void seek(float progress);
    void seekRelative(float seconds);
    void toggleFullscreen();
    void setLoop(bool loop) { m_config.enableLoop = loop; }
    void pollEvent(const sf::Event& event);
    void play();
};

// pour changer la taille de la Video il suffit de faire un setScale sur le sprite retourné par getSprite()
// 
// // Exemples d'utilisation :
// HighResVideoPlayer videoPlayer;
// videoPlayer.loadFromFile("video.mp4");
// 
// videoPlayer.play();
// 
// 
// Dans la boucle principale :
// 
// float deltaTime = clock.restart().asSeconds();
// videoPlayer.update(deltaTime);
// 
// Dans la boucle de rendu :
// 
// sf::sprite videoSprite = videoPlayer.getSprite();
// 
// videoSprite.setScale(2.0f, 2.0f); // Exemple : doubler la taille
// 
// window.draw(videoSprite);
// 
// 

#endif // !_INC_HIGHRESVIDEOPLAYER_HPP

