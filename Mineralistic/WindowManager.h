#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

namespace sf
{
	class RenderWindow;
}

class World;
class ObjectManager;

class WindowManager
{
public:
	WindowManager(std::string pTitle, sf::Vector2i pWindowSize);
	~WindowManager();

	sf::RenderWindow *getWindow();
	void drawObjectManager(ObjectManager *pObjectManager);
	void drawWorld(World *pWorld);
private:
	sf::RenderWindow *mWindow;
};

