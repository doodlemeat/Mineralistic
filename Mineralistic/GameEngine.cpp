#include "GameEngine.h"
#include "GameStateAsset.h"
#include "GameState.h"
#include "WindowManager.h"
#include "ResourceHolder.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Thor/Input/ActionMap.hpp>
#include "Audiosystem.h"

GameEngine::GameEngine(std::string pTitle, int pWindowWidth, int pWindowHeight)
{
	mRunning = true;

	mActionMap = new thor::ActionMap<std::string>();
	mActionMap->operator[]("Closed_Window") = thor::Action(sf::Event::Closed);

	mResourceHolder = new ResourceHolder();
	mAudioSystem = new AudioSystem();
	mAudioSystem->createMusic("Ambient_1", "../assets/music/ambient_1.wav");

	mAudioSystem->createSound("Throw_Hook", mResourceHolder->getSound("throw_hook.ogg"));
	mAudioSystem->createSound("Hook_Attached", mResourceHolder->getSound("hook_attached.ogg"));
	mAudioSystem->createSound("Climb_Rope", mResourceHolder->getSound("rope_climb.ogg"));

	mAudioSystem->createSound("Pick", mResourceHolder->getSound("pick.ogg"));
	mAudioSystem->createSound("Shovel", mResourceHolder->getSound("shovel.ogg"));
	mAudioSystem->createSound("Break_Stone", mResourceHolder->getSound("break_stone.ogg"));
	mAudioSystem->createSound("Break_Dirt", mResourceHolder->getSound("break_dirt.ogg"));
	mAudioSystem->createSound("Mineral_Gathered", mResourceHolder->getSound("mineral_gathered.ogg"));

	mAudioSystem->createSound("Monster_Groan_1", mResourceHolder->getSound("monster_groan_1.ogg"));
	mAudioSystem->createSound("Monster_Groan_2", mResourceHolder->getSound("monster_groan_2.ogg"));

	mAudioSystem->createSound("Menu_Item", mResourceHolder->getSound("menu_item.ogg"));
	mAudioSystem->createSound("Step_1", mResourceHolder->getSound("step_1.ogg"));
	mAudioSystem->createSound("Step_2", mResourceHolder->getSound("step_2.ogg"));
	mAudioSystem->createSound("Step_3", mResourceHolder->getSound("step_3.ogg"));
	mAudioSystem->createSound("Step_4", mResourceHolder->getSound("step_4.ogg"));
	mAudioSystem->createSound("Step_5", mResourceHolder->getSound("step_5.ogg"));
	mAudioSystem->createSound("Step_6", mResourceHolder->getSound("step_6.ogg"));
	mAudioSystem->createSound("Step_7", mResourceHolder->getSound("step_7.ogg"));
	mAudioSystem->createSound("Step_8", mResourceHolder->getSound("step_8.ogg"));
	mAudioSystem->createSound("Step_9", mResourceHolder->getSound("step_9.ogg"));
	mAudioSystem->createSound("Step_Grass_1", mResourceHolder->getSound("step_grass_1.ogg"));
	mAudioSystem->createSound("Step_Grass_2", mResourceHolder->getSound("step_grass_2.ogg"));
	mAudioSystem->createSound("Step_Grass_3", mResourceHolder->getSound("step_grass_3.ogg"));
	mAudioSystem->createSound("Step_Grass_4", mResourceHolder->getSound("step_grass_4.ogg"));

	mAudioSystem->createSound("Place_Torch", mResourceHolder->getSound("place_torch.ogg"));
	mAudioSystem->createSound("Remove_Torch", mResourceHolder->getSound("remove_torch.ogg"));

	mWindowManager = new WindowManager(pTitle, sf::Vector2i(pWindowWidth, pWindowHeight));

	mStateAssets = new GameStateAsset();
	mStateAssets->gameEngine = this;
	mStateAssets->windowManager = mWindowManager;
	mStateAssets->resourceHolder = mResourceHolder;
	mStateAssets->audioSystem = mAudioSystem;
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
		mAudioSystem->update();
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

	delete mAudioSystem;
	mAudioSystem = nullptr;

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
