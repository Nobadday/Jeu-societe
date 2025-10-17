#include "common.h"
#include "SceneManager.hpp"
#include <process.h>



int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "My window");
    window.setFramerateLimit(100u);
    sf::Clock clock;
    float dt = 0;
    //SceneManager* sceneManager = new SceneManager();
    SceneManager sceneManager(window);
	sceneManager.SetScene(SCENE_2);

    std::cout << "Current scene is : " << sceneManager.GetScene();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
                
            sceneManager.PollEvent(event);
        }
		sceneManager.Update(dt);

		//Drawing         
        sceneManager.Draw();

        dt = clock.restart().asSeconds();      
        //std::cout << "dt = " << dt << std::endl;
    } 

}
