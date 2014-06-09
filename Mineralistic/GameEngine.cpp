#include "GameEngine.h"
#include "GameStateAsset.h"
#include "GameState.h"
#include "WindowManager.h"
#include "ResourceHolder.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Thor/Input/ActionMap.hpp>

GameEngine::GameEngine(std::string pTitle, int pWindowWidth, int pWindowHeight)
{
	this->mRunning = true;

	this->mActionMap = new thor::ActionMap<std::string>();
	this->mActionMap->operator[]("Closed_Window") = thor::Action(sf::Event::Closed);

	this->mResourceHolder = new ResourceHolder();

	this->mWindowManager = new WindowManager(pTitle, sf::Vector2i(pWindowWidth, pWindowHeight));

	this->mStateAssets = new GameStateAsset();
	this->mStateAssets->gameEngine = this;
	this->mStateAssets->windowManager = mWindowManager;
	this->mStateAssets->resourceHolder = mResourceHolder;
}

GameEngine::~GameEngine()
{
}

void GameEngine::changeState(GameState* pGameState)
{
	if (!mActiveStates.empty())
	{
		popState();
	}
	pushState(pGameState);
}

void GameEngine::popState()
{
	if (!mActiveStates.empty())
	{
		auto it = --mActiveStates.end();
		GameState* gameState = (*it);
		gameState->leaving();
		
		thor::ActionMap<std::string> *actionMap = gameState->getActionMap();
		delete actionMap;
		actionMap = nullptr;

		delete gameState;
		gameState = nullptr;
		mActiveStates.pop_back();
	}

	notifyRevealedStates();
}

void GameEngine::pushState(GameState* pGameState)
{
	pGameState->setStateAssets(mStateAssets);
	pGameState->createActionMap();
	pGameState->setupActions();
	mActiveStates.push_back(pGameState);
	
	notifyObscuredStates();

	pGameState->entering();
	pGameState->update(0); // To be sure that update() runs before draw() when changing
}

void GameEngine::notifyRevealedStates()
{
	
}

void GameEngine::notifyObscuredStates()
{

}

void GameEngine::draw()
{
	for (auto &state : mActiveStates)
	{
		state->draw();
	}
}

void GameEngine::clearEvents()
{
	for (auto &state : mActiveStates)
	{
		state->getActionMap()->clearEvents();
	}
}

bool GameEngine::updateActiveStates(float dt)
{
	bool returnValue = true;
	for (std::size_t i = 0; i < mActiveStates.size(); i++)
	{
		if (i == mActiveStates.size() - 1)
		{
			if (!mActiveStates[i]->update(dt))
			{
				if (i != 0)
				{
					popState();
				}
				else
				{
					returnValue = false;
				}
			}
		}
	}
	return returnValue;
}

GameStateAsset *GameEngine::getStateAsset()
{
	return mStateAssets;
}

void GameEngine::run()
{
	while (mRunning)
	{
		this->updateEvents();

		if (mActionMap->isActive("Closed_Window"))
		{
			mRunning = false;
			break;
		}

		if (!updateActiveStates(0.016666666f))
		{
			mRunning = false;
			break;
		}

		this->mWindowManager->getWindow()->clear();
		this->draw();
		this->mWindowManager->getWindow()->display();
	}
}

void GameEngine::exit()
{
	while (mActiveStates.size() > 0)
	{
		popState();
	}

	this->mWindowManager->getWindow()->close();

	delete this->mResourceHolder;
	this->mResourceHolder = nullptr;

	delete this->mActionMap;
	this->mActionMap = nullptr;

	delete this->mWindowManager;
	this->mWindowManager = nullptr;

	delete this->mStateAssets;
	this->mStateAssets = nullptr;
}

void GameEngine::updateEvents()
{
	mActionMap->clearEvents();
	this->clearEvents();

	sf::Event event;
	while (mWindowManager->getWindow()->pollEvent(event))
	{
		sf::Event localEvent = event;
		mActionMap->pushEvent(localEvent);
		this->pushEvents(localEvent);
	}
}

void GameEngine::pushEvents(sf::Event pEvent)
{
	for (auto &state : mActiveStates)
	{
		state->getActionMap()->pushEvent(pEvent);
	}
}
