#include "Transition.hpp"

TransitionClass::TransitionClass(Transition _transition)
{
    //Shader
    if (!chromaKeyShader.loadFromMemory(
        R"(
        uniform sampler2D texture;
        uniform vec3 keyColor; // Couleur à rendre transparente (ex: vert)
        uniform float threshold; // Seuil de tolérance

        void main()
        {
            vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
            float dist = distance(pixel.rgb, keyColor);
            
            // Si la couleur est proche de keyColor, rendre transparent
            if (dist < threshold)
            {
                pixel.a = 0.0;
            }
            
            gl_FragColor = pixel * gl_Color;
        }
        )", sf::Shader::Fragment))
    {
        std::cout << "Erreur : Impossible de charger le shader chroma key" << std::endl;
    }
    else
    {
        // Définir la couleur à rendre transparente (vert dans cet exemple)
        chromaKeyShader.setUniform("keyColor", sf::Glsl::Vec3(0.0f, 1.0f, 0.0f)); // RGB vert
        chromaKeyShader.setUniform("threshold", 0.7f); // Ajuster selon vos besoins
    }


    SetTransition(_transition);
}

TransitionClass::~TransitionClass()
{
    
}

void TransitionClass::PlayTransition()
{
    videoPlayer.play();
}
void TransitionClass::SetTransition(Transition _transition)
{
    //Its just an if because we will have only 2 transition
    //Upgrade it to switch for next project
    if (_transition == FADED_IN)
    {
        videoPlayer.loadFromFile("Assets/Video/TRANSITION_1.mp4");
    }
    else
    {
        videoPlayer.loadFromFile("Assets/Video/TRANSITION_2.mp4");
    }
}
void TransitionClass::Update(float _dt)
{
    videoPlayer.update(_dt);
}
void TransitionClass::Draw(sf::RenderWindow& _window)
{
    sf::Sprite vid = videoPlayer.getSprite();
    _window.draw(vid, &chromaKeyShader);
}
bool TransitionClass::IsFinished(void)
{
    return videoPlayer.isFinish();
}