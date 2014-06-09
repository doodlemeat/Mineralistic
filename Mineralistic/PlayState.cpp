#include "PlayState.h"
#include <iostream>
#include "GameObject.h"
#include "ObjectManager.h"
#include "WindowManager.h"
#include "GameState.h"
#include "GameStateAsset.h"
#include "ResourceHolder.h"
#include "World.h"
#include "Player.h"
#include "Thor/Input.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Box2DWorldDraw.h"
#include "Box2D/Dynamics/b2World.h"

PlayState::PlayState()
{
	mObjectManager = nullptr;
	mWorld = nullptr;
	mB2World = nullptr;
	mB2DebugDraw = nullptr;
}

PlayState::~PlayState()
{

}

void PlayState::entering()
{
	std::cout << "Entering play state" << std::endl;

	mObjectManager = new ObjectManager();

	mWorld = new World(sf::Vector2i(16, 16), sf::Vector2i(64, 64), mAssets->resourceHolder->getTexture("tileset.png"), mObjectManager);
	mWorld->addTileStop("Air", 0.f, sf::Vector2i(64, 0));
	mWorld->addTileStop("Stone", 1.f, sf::Vector2i(0, 0));

	mB2World = new b2World(b2Vec2(0.f, -20.f));
	mB2World->SetAllowSleeping(true);

	mB2DebugDraw = new Box2DWorldDraw(mAssets->windowManager->getWindow());
	mB2DebugDraw->SetFlags(b2Draw::e_shapeBit);
	mB2World->SetDebugDraw(mB2DebugDraw);

	/* Player object */
	Player *playerObject = new Player();
	mObjectManager->addObject(playerObject);

	playerObject->getSprite()->setTexture(mAssets->resourceHolder->getTexture("test.png"));
	playerObject->getSprite()->setOrigin(32, 32);

	thor::FrameAnimation *idleAnimation = new thor::FrameAnimation();
	idleAnimation->addFrame(1.f, sf::IntRect(0, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(64, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(128, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(192, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(256, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(320, 0, 64, 64));
	playerObject->addAnimation("Idle", idleAnimation, sf::seconds(0.6f));

	playerObject->getAnimator()->playAnimation("Idle", true);
	playerObject->getView()->setSize(mAssets->windowManager->getWindow()->getSize().x, mAssets->windowManager->getWindow()->getSize().y);

}

void PlayState::leaving()
{
	std::cout << "Leaving play state" << std::endl;

	delete mObjectManager;
	mObjectManager = nullptr;

	delete mWorld;
	mWorld = nullptr;

	delete mB2World;
	mB2World = nullptr;

	delete mB2DebugDraw;
	mB2DebugDraw = nullptr;
}

void PlayState::obscuring()
{
}

void PlayState::releaving()
{
}

bool PlayState::update(float dt)
{
	mB2World->Step(1.0f / 60.f, 8, 3);

	mWorld->update(dt);

	if (getActionMap()->isActive("Move_Right"))
	{
		mObjectManager->getObject("Player")->getSprite()->move(sf::Vector2f(15, 0));
	}
	if (getActionMap()->isActive("Move_Left"))
	{
		mObjectManager->getObject("Player")->getSprite()->move(sf::Vector2f(-15, 0));
	}
	if (getActionMap()->isActive("Move_Up"))
	{
		mObjectManager->getObject("Player")->getSprite()->move(sf::Vector2f(0, -15));
	}
	if (getActionMap()->isActive("Move_Down"))
	{
		mObjectManager->getObject("Player")->getSprite()->move(sf::Vector2f(0, 15));
	}

	mObjectManager->update(dt);

	if (getActionMap()->isActive("Zoom_Out"))
	{
		Player *player = dynamic_cast<Player*>(mObjectManager->getObject("Player"));
		if (player != nullptr)
		{
			player->getView()->zoom(2);
		}
	}
	if (getActionMap()->isActive("Zoom_In"))
	{
		Player *player = dynamic_cast<Player*>(mObjectManager->getObject("Player"));
		if (player != nullptr)
		{
			player->getView()->zoom(0.5f);
		}
	}
	return true;
}

void PlayState::draw()
{
	Player *player = dynamic_cast<Player*>(mObjectManager->getObject("Player"));
	if (player != nullptr)
	{
		mAssets->windowManager->getWindow()->setView(*player->getView());
	}

	mAssets->windowManager->drawWorld(mWorld);
	mAssets->windowManager->drawObjectManager(mObjectManager);

	mB2World->DrawDebugData();
	/* hud ... */

	mAssets->windowManager->getWindow()->setView(mAssets->windowManager->getWindow()->getDefaultView());

}

void PlayState::setupActions()
{
	getActionMap()->operator[]("Move_Right") = thor::Action(sf::Keyboard::D, thor::Action::Hold);
	getActionMap()->operator[]("Move_Left") = thor::Action(sf::Keyboard::A, thor::Action::Hold);
	getActionMap()->operator[]("Move_Up") = thor::Action(sf::Keyboard::W, thor::Action::Hold);
	getActionMap()->operator[]("Move_Down") = thor::Action(sf::Keyboard::S, thor::Action::Hold);
	getActionMap()->operator[]("Zoom_In") = thor::Action(sf::Keyboard::Up, thor::Action::PressOnce);
	getActionMap()->operator[]("Zoom_Out") = thor::Action(sf::Keyboard::Down, thor::Action::PressOnce);
}
