#include "WindowManager.h"
#include <SFML/Graphics.hpp>
#include "ObjectManager.h"
#include "ObjectGroup.h"
#include "GameObject.h"
#include "World.h"
#include "Chunk.h"
#include <iostream>

WindowManager::WindowManager(std::string pTitle, sf::Vector2i pWindowSize)
{
	mWindow = new sf::RenderWindow(sf::VideoMode(pWindowSize.x, pWindowSize.y), pTitle, sf::Style::None);
	mWindow->setVerticalSyncEnabled(true);
	mWindow->setKeyRepeatEnabled(false);
	mWindow->setMouseCursorVisible(false);
}

WindowManager::~WindowManager()
{
	delete mWindow;
	mWindow = nullptr;
}

sf::RenderWindow *WindowManager::getWindow()
{
	return mWindow;
}

void WindowManager::drawObjectManager(ObjectManager *pObjectManager)
{
	for (auto &group : pObjectManager->getGroups())
	{
		std::vector<GameObject*> *objects = group->getObjects();
		for (auto &object : *objects)
		{
			mWindow->draw(*object);
		}
	}

	for (auto &object : pObjectManager->getObjects())
	{
		mWindow->draw(*object);
	}
}

void WindowManager::drawWorld(World *pWorld)
{
	for (auto &chunk : pWorld->getLoadedChunks())
	{
		mWindow->draw(*chunk);
	}
}

void WindowManager::drawWorldLightOverlay(World *pWorld)
{
	for (auto &chunk : pWorld->getLoadedChunks())
	{
		chunk->setDrawLight(true);
		mWindow->draw(*chunk);
		chunk->setDrawLight(false);
	}
}
