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
#include "Box2D/Box2D.h"
#include "Material.h"
#include "PhysicsScale.h"
#include "B2UserData.h"
#include "B2ContactListener.h"
#include "Chunk.h"
#include "ObjectGroup.h"
#include "Config.h"

PlayState::PlayState()
{
	mObjectManager = nullptr;
	mWorld = nullptr;
	mB2World = nullptr;
	mB2DebugDraw = nullptr;
	mDebugToggled = false;
}

PlayState::~PlayState()
{

}

void PlayState::entering()
{
	std::cout << "Entering play state" << std::endl;

	mB2World = new b2World(b2Vec2(0.f, 30.f));
	mB2World->SetAllowSleeping(true);

	mObjectManager = new ObjectManager();
	mObjectManager->setAudioSystem(mAssets->audioSystem);
	mObjectManager->setResourceHolder(mAssets->resourceHolder);
	mObjectManager->setB2World(mB2World);


	mContactListener = new B2ContactListener();
	mB2World->SetContactListener(mContactListener);

	mAssets->resourceHolder->getTexture("tileset.png").setSmooth(false);

	mWorld = new World(sf::Vector2i(8, 8), sf::Vector2i(64, 64), mAssets->resourceHolder->getTexture("tileset.png"), mObjectManager, mB2World);
	mWorld->registerMaterial(new Material("Stone", true, sf::IntRect(0, 0, 64, 64), 1));
	mWorld->registerMaterial(new Material("Air", false, sf::IntRect(64, 0, 64, 64)));
	mWorld->registerMaterial(new Material("PackedStone", true, sf::IntRect(128, 0, 64, 64), 2));
	mWorld->registerMaterial(new Material("Ruby", true, sf::IntRect(0, 64, 64, 64), 1));
	mWorld->registerMaterial(new Material("Bronze", true, sf::IntRect(64, 64, 64, 64), 1));
	mWorld->registerMaterial(new Material("Gold", true, sf::IntRect(128, 64, 64, 64), 1));
	mWorld->registerMaterial(new Material("Diamond", true, sf::IntRect(192, 64, 64, 64), 1));

	mWorld->addTileStop("Air", 0.f);
	mWorld->addTileStop("Stone", 0.5f);
	mWorld->addTileStop("PackedStone", 1.f);

	mB2DebugDraw = new Box2DWorldDraw(mAssets->windowManager->getWindow());
	mB2DebugDraw->SetFlags(b2Draw::e_jointBit | b2Draw::e_shapeBit);
	mB2World->SetDebugDraw(mB2DebugDraw);

	mDebug.setFont(mAssets->resourceHolder->getFont("consola.ttf"));
	mDebug.setString("");
	mDebug.setPosition(12, 12);
	mDebug.setCharacterSize(20);
	mDebug.setColor(sf::Color::White);

	ObjectGroup *hookGroup = new ObjectGroup("hooks");
	mObjectManager->addGroup(hookGroup);

	setupPlayer();
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

	delete mContactListener;
	mContactListener = nullptr;
}

void PlayState::obscuring()
{
}

void PlayState::releaving()
{
}

bool PlayState::update(float dt)
{
	if (getActionMap()->isActive("Toggle_Debug"))
	{
		mDebugToggled = !mDebugToggled;
	}
	mB2World->Step(1.0f / 60.f, 8, 3);

	mWorld->update(dt);

	mObjectManager->update(dt, getActionMap());

	Player* player = static_cast<Player*>(mObjectManager->getObject("Player"));

	sf::Vector2f worldPosition = WorldHelper::toWorldPositionFromSFMLPosition(player->getSprite()->getPosition());
	sf::Vector2i chunkPosition = WorldHelper::chunkPosition(worldPosition);

	std::string output = "";
	output += "v" + Config::MAJOR + "." + Config::MINOR + "\n";
	output += "x: " + std::to_string(worldPosition.x) + " c: " + std::to_string(chunkPosition.x) + "\n";
	output += "y: " + std::to_string(worldPosition.y) + " c: " + std::to_string(chunkPosition.y) + "\n";
	output += "m: ";
	(player->IsMining() ? output += "true" : output += "false");
	output += "\n";
	output += "h: ";
	(player->Hooked() ? output += "true" : output += "false");
	output += "\n";
	output += "s: ";
	(player->Standing() ? output += "true" : output += "false");
	output += "\n";
	output += "fl: ";
	(player->FacingLeft() ? output += "true" : output += "false");
	output += "\n";

	mDebug.setString(output);
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

	if (mDebugToggled)
	{
		mB2World->DrawDebugData();
	}
	/* hud ... */

	mAssets->windowManager->getWindow()->setView(mAssets->windowManager->getWindow()->getDefaultView());
	mAssets->windowManager->getWindow()->draw(mDebug);
}

void PlayState::setupActions()
{
	getActionMap()->operator[]("Jump") = thor::Action(sf::Keyboard::Up, thor::Action::Hold); // Jump, climb rope
	getActionMap()->operator[]("Climb_Down") = thor::Action(sf::Keyboard::Down, thor::Action::Hold);
	getActionMap()->operator[]("Walk_Left") = thor::Action(sf::Keyboard::Left, thor::Action::Hold);
	getActionMap()->operator[]("Walk_Right") = thor::Action(sf::Keyboard::Right, thor::Action::Hold);
	getActionMap()->operator[]("Throw_Hook") = thor::Action(sf::Keyboard::X, thor::Action::PressOnce);
	getActionMap()->operator[]("Toggle_Debug") = thor::Action(sf::Keyboard::F1, thor::Action::PressOnce);
	getActionMap()->operator[]("Mine") = thor::Action(sf::Keyboard::C, thor::Action::Hold);
}

void PlayState::setupPlayer()
{
	Player *playerObject = new Player();
	mObjectManager->addObject(playerObject);

	sf::Sprite *currentTileMarkup = new sf::Sprite();
	currentTileMarkup->setTexture(mAssets->resourceHolder->getTexture("current_tile.png"));
	playerObject->setCurrentTileMark(currentTileMarkup);

	sf::Sprite *rope = playerObject->getRope();
	mAssets->resourceHolder->getTexture("rope.png").setRepeated(true);
	rope->setTexture(mAssets->resourceHolder->getTexture("rope.png"));

	playerObject->setWorld(mWorld);

	playerObject->getSprite()->setTexture(mAssets->resourceHolder->getTexture("player.png"));
	playerObject->getSprite()->setOrigin(32, 32);

	thor::FrameAnimation *idleAnimation = new thor::FrameAnimation();
	idleAnimation->addFrame(1.f, sf::IntRect(0, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(64, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(128, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(192, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(256, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(320, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(384, 0, 64, 64));
	idleAnimation->addFrame(1.f, sf::IntRect(448, 0, 64, 64));

	thor::FrameAnimation *swingAnimation = new thor::FrameAnimation();
	swingAnimation->addFrame(1.f, sf::IntRect(0, 64, 64, 64));
	swingAnimation->addFrame(1.f, sf::IntRect(64, 64, 64, 64));
	swingAnimation->addFrame(1.f, sf::IntRect(128, 64, 64, 64));
	swingAnimation->addFrame(1.f, sf::IntRect(192, 64, 64, 64));
	swingAnimation->addFrame(1.f, sf::IntRect(256, 64, 64, 64));
	swingAnimation->addFrame(1.f, sf::IntRect(320, 64, 64, 64));

	thor::FrameAnimation *mineLeftAnimation = new thor::FrameAnimation();
	mineLeftAnimation->addFrame(1.f, sf::IntRect(0, 128, 64, 64));
	mineLeftAnimation->addFrame(1.f, sf::IntRect(64, 128, 64, 64));
	mineLeftAnimation->addFrame(1.f, sf::IntRect(128, 128, 64, 64));

	thor::FrameAnimation *mineRightAnimation = new thor::FrameAnimation();
	mineRightAnimation->addFrame(1.f, sf::IntRect(0, 192, 64, 64));
	mineRightAnimation->addFrame(1.f, sf::IntRect(64, 192, 64, 64));
	mineRightAnimation->addFrame(1.f, sf::IntRect(128, 192, 64, 64));

	thor::FrameAnimation *walkRightAnimation = new thor::FrameAnimation();
	walkRightAnimation->addFrame(1.f, sf::IntRect(0, 256, 64, 64));
	walkRightAnimation->addFrame(1.f, sf::IntRect(64, 256, 64, 64));
	walkRightAnimation->addFrame(1.f, sf::IntRect(128, 256, 64, 64));

	thor::FrameAnimation *walkLeftAnimation = new thor::FrameAnimation();
	walkLeftAnimation->addFrame(1.f, sf::IntRect(0, 320, 64, 64));
	walkLeftAnimation->addFrame(1.f, sf::IntRect(64, 320, 64, 64));
	walkLeftAnimation->addFrame(1.f, sf::IntRect(128, 320, 64, 64));

	playerObject->addAnimation("Idle", idleAnimation, sf::seconds(0.6f));
	playerObject->addAnimation("Swing", swingAnimation, sf::seconds(0.6f));
	playerObject->addAnimation("Mine_Left", mineLeftAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Mine_Right", mineRightAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Walk_Left", walkLeftAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Walk_Right", walkRightAnimation, sf::seconds(0.4f));

	playerObject->getAnimator()->playAnimation("Idle", true);
	sf::Vector2f newwindowSize = sf::Vector2f(mAssets->windowManager->getWindow()->getSize());
	playerObject->getView()->setSize(newwindowSize);

	/* Box2D body */
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	b2Vec2 bodyPos = PhysicsScale::gameToPhys(playerObject->getSprite()->getPosition());
	bodyDef.position.Set(bodyPos.x, bodyPos.y);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;

	b2Body *body = mB2World->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	sf::Vector2f boundingBox(10, 28);
	b2Vec2 boundingBoxPhysic = PhysicsScale::gameToPhys(boundingBox);
	boxShape.SetAsBox(boundingBoxPhysic.x, boundingBoxPhysic.y);

	b2PolygonShape sensorShape;
	boundingBox = sf::Vector2f(8, 4);
	b2Vec2 boundingBoxPhysic2 = PhysicsScale::gameToPhys(boundingBox);
	b2Vec2 center = body->GetPosition() + PhysicsScale::gameToPhys(sf::Vector2f(0, 28));
	sensorShape.SetAsBox(boundingBoxPhysic2.x, boundingBoxPhysic2.y, center, 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.friction = 0;
	body->CreateFixture(&fixtureDef);

	fixtureDef.shape = &sensorShape;
	fixtureDef.isSensor = true;

	UDPlayer *b2userData = new UDPlayer();
	b2userData->setType(B2UDType::PLAYER);
	b2userData->setData(playerObject);
	playerObject->setB2UserData(b2userData);

	fixtureDef.userData = b2userData;
	
	body->CreateFixture(&fixtureDef);

	playerObject->setBody(body);

}