//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"

std::unique_ptr<sf::RenderWindow>	WindowManager::sInstance;

bool WindowManager::StaticInit()
{
	sInstance.reset(new sf::RenderWindow(sf::VideoMode(kWindowSize.x, kWindowSize.y), "Snowballed", sf::Style::Fullscreen));
	return true;
}


