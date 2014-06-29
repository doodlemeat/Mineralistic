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
#include "Tile.h"
#include "Logger.h"
#include <string>
#include "Math.h"
#include "House.h"
#include "Chimney.h"
#include "Thor/Math/Random.hpp"

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

	mDebugToggled = false;

	mB2World = new b2World(b2Vec2(0.f, 30.f));
	mB2World->SetAllowSleeping(true);


	mContactListener = new B2ContactListener();
	mB2World->SetContactListener(mContactListener);

	mAssets->resourceHolder->getTexture("tileset.png").setSmooth(false);

	mObjectManager = new ObjectManager();
	mObjectManager->setAudioSystem(mAssets->audioSystem);
	mObjectManager->setResourceHolder(mAssets->resourceHolder);
	mObjectManager->setB2World(mB2World);

	mWorld = new World(mAssets->resourceHolder->getTexture("tileset.png"), mObjectManager, mB2World);
	mObjectManager->setWorld(mWorld);

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
	ObjectGroup *monsterGroup = new ObjectGroup("monsters");
	mObjectManager->addGroup(monsterGroup);
	ObjectGroup *torchGroup = new ObjectGroup("torches");
	mObjectManager->addGroup(torchGroup);
	
	registerMaterials();
	setupPlayer();
	setupHouse();

	mObjectManager->getObject("Player")->setZOrder(10);
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
	mB2World->Step(1.0f / 60.f, 8, 3);

	mWorld->update(dt);

	mObjectManager->update(dt, getActionMap());

	if (getActionMap()->isActive("Toggle_Debug"))
	{
		mDebugToggled = !mDebugToggled;
	}
	if (mDebugToggled)
	{
		Player* player = static_cast<Player*>(mObjectManager->getObject("Player"));

		sf::Vector2f worldPosition = WorldHelper::toWorldPositionFromSFMLPosition(player->getSprite()->getPosition());
		sf::Vector2i chunkPosition = WorldHelper::toChunkPositionFromWorldPosition(worldPosition);


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
		output += "c: " + std::to_string(mWorld->getLoadedChunks().size()) + "\n";
		output += "\n";
		output += "seed: " + std::to_string(mWorld->getSeed()) + "\n";
		output += "octaves: " + std::to_string(mWorld->getOctaveCount()) + "\n";
		output += "freq: " + std::to_string(mWorld->getFrequency()) + "\n";
		output += "q: ";
		switch (mWorld->getNoiseQuality())
		{
		case 0:
			output += "fast";
			break;
		case 1:
			output += "std";
			break;
		case 2:
			output += "best";
			break;
		}
		output += "\n";
		output += "e: " + std::to_string(mObjectManager->getGroup("monsters")->getObjects()->size());
		output += "t: " + std::to_string(mObjectManager->getGroup("torches")->getObjects()->size());

		mDebug.setString(output);
	}

	if (getActionMap()->isActive("Quit"))
	{
		return false;
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
	mAssets->windowManager->getWindow()->draw(*mWorld->getBlockParticleSystem());
	mAssets->windowManager->drawWorldLightOverlay(mWorld);
	mAssets->windowManager->getWindow()->setView(mAssets->windowManager->getWindow()->getDefaultView());

	if (mDebugToggled)
	{
		mAssets->windowManager->getWindow()->draw(mDebug);
	}
}

void PlayState::setupActions()
{
	getActionMap()->operator[]("Jump") = thor::Action(sf::Keyboard::Up, thor::Action::Hold); // Jump, climb rope
	getActionMap()->operator[]("Climb_Down") = thor::Action(sf::Keyboard::Down, thor::Action::Hold); // Climb rope, mine
	getActionMap()->operator[]("Walk_Left") = thor::Action(sf::Keyboard::Left, thor::Action::Hold);
	getActionMap()->operator[]("Walk_Right") = thor::Action(sf::Keyboard::Right, thor::Action::Hold);
	getActionMap()->operator[]("Throw_Hook") = thor::Action(sf::Keyboard::X, thor::Action::PressOnce);
	getActionMap()->operator[]("Toggle_Debug") = thor::Action(sf::Keyboard::F1, thor::Action::PressOnce);
	getActionMap()->operator[]("Mine") = thor::Action(sf::Keyboard::C, thor::Action::Hold);
	getActionMap()->operator[]("Torch") = thor::Action(sf::Keyboard::Z, thor::Action::PressOnce);
	getActionMap()->operator[]("Zoom_In") = thor::Action(sf::Keyboard::Num1, thor::Action::PressOnce);
	getActionMap()->operator[]("Zoom_Out") = thor::Action(sf::Keyboard::Num2, thor::Action::PressOnce);
	getActionMap()->operator[]("Quit") = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
}

void PlayState::setupPlayer()
{
	Player *playerObject = new Player();
	playerObject->setManager(mObjectManager);
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

	thor::FrameAnimation *mineDownAnimation = new thor::FrameAnimation();
	mineDownAnimation->addFrame(1.f, sf::IntRect(0, 384, 64, 64));
	mineDownAnimation->addFrame(1.f, sf::IntRect(64, 384, 64, 64));
	mineDownAnimation->addFrame(1.f, sf::IntRect(128, 384, 64, 64));
	mineDownAnimation->addFrame(1.f, sf::IntRect(192, 384, 64, 64));
	mineDownAnimation->addFrame(1.f, sf::IntRect(256, 384, 64, 64));
	mineDownAnimation->addFrame(1.f, sf::IntRect(320, 384, 64, 64));

	thor::FrameAnimation *shovelRightAnimation = new thor::FrameAnimation();
	shovelRightAnimation->addFrame(1.f, sf::IntRect(0, 448, 64, 64));
	shovelRightAnimation->addFrame(1.f, sf::IntRect(64, 448, 64, 64));
	shovelRightAnimation->addFrame(1.f, sf::IntRect(128, 448, 64, 64));
	shovelRightAnimation->addFrame(1.f, sf::IntRect(192, 448, 64, 64));

	thor::FrameAnimation *shovelLeftAnimation = new thor::FrameAnimation();
	shovelLeftAnimation->addFrame(1.f, sf::IntRect(0, 512, 64, 64));
	shovelLeftAnimation->addFrame(1.f, sf::IntRect(64, 512, 64, 64));
	shovelLeftAnimation->addFrame(1.f, sf::IntRect(128, 512, 64, 64));
	shovelLeftAnimation->addFrame(1.f, sf::IntRect(192, 512, 64, 64));

	thor::FrameAnimation *shovelDownAnimation = new thor::FrameAnimation();
	shovelDownAnimation->addFrame(1.f, sf::IntRect(0, 576, 64, 64));
	shovelDownAnimation->addFrame(1.f, sf::IntRect(64, 576, 64, 64));

	playerObject->addAnimation("Idle", idleAnimation, sf::seconds(0.6f));
	playerObject->addAnimation("Swing", swingAnimation, sf::seconds(0.6f));
	playerObject->addAnimation("Mine_Left", mineLeftAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Mine_Right", mineRightAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Mine_Down", mineDownAnimation, sf::seconds(0.7f));
	playerObject->addAnimation("Walk_Left", walkLeftAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Walk_Right", walkRightAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Shovel_Left", shovelLeftAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Shovel_Right", shovelRightAnimation, sf::seconds(0.4f));
	playerObject->addAnimation("Shovel_Down", shovelDownAnimation, sf::seconds(0.3f));

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
	playerObject->teleport(0, -6);

	Tile* tile = mWorld->getTileByWorldPosition(WorldHelper::toWorldPositionFromSFMLPosition(playerObject->getSprite()->getPosition()));
	mObjectManager->spawnTorch(tile->getRelative(2, 0)->getPosition());
	mObjectManager->spawnTorch(tile->getRelative(0 , -2)->getPosition());
	mObjectManager->spawnTorch(tile->getRelative(-2, 0)->getPosition());
}

void PlayState::registerMaterials()
{
	MaterialDef stepStoneDef;
	stepStoneDef.stepSounds.push_back("Step_1");
	stepStoneDef.stepSounds.push_back("Step_2");
	stepStoneDef.stepSounds.push_back("Step_3");
	stepStoneDef.stepSounds.push_back("Step_4");
	stepStoneDef.stepSounds.push_back("Step_5");
	stepStoneDef.stepSounds.push_back("Step_6");
	stepStoneDef.stepSounds.push_back("Step_7");
	stepStoneDef.stepSounds.push_back("Step_8");
	stepStoneDef.stepSounds.push_back("Step_9");
	stepStoneDef.breakingSound = "Pick";
	stepStoneDef.breakSound = "Break_Stone";

	MaterialDef stepGrassDef;
	stepGrassDef.stepSounds.push_back("Step_Grass_1");
	stepGrassDef.stepSounds.push_back("Step_Grass_2");
	stepGrassDef.stepSounds.push_back("Step_Grass_3");
	stepGrassDef.stepSounds.push_back("Step_Grass_4");
	stepGrassDef.breakingSound = "Shovel";
	stepGrassDef.breakSound = "Break_Dirt";

	MaterialDef stoneDef(stepStoneDef);
	stoneDef.name = "Stone";
	stoneDef.collidable = true;
	stoneDef.resistance = 1;
	stoneDef.breakBlockType = PICKAXE;
	stoneDef.textureRect = sf::IntRect(0, 0, 64, 64);
	mWorld->registerMaterial(new Material(&stoneDef));

	MaterialDef airDef;
	airDef.name = "Air";
	airDef.textureRect = sf::IntRect(64, 0, 64, 64);
	mWorld->registerMaterial(new Material(&airDef));

	MaterialDef packedStoneDef(stepStoneDef);
	packedStoneDef.breakBlockType = PICKAXE;
	packedStoneDef.name = "Packed Stone";
	packedStoneDef.collidable = true;
	packedStoneDef.textureRect = sf::IntRect(128, 0, 64, 64);
	packedStoneDef.resistance = 2;
	mWorld->registerMaterial(new Material(&packedStoneDef));

	MaterialDef skyDef;
	skyDef.name = "Sky";
	skyDef.textureRect = sf::IntRect(192, 0, 64, 64);
	mWorld->registerMaterial(new Material(&skyDef));

	MaterialDef grassDef(stepGrassDef);
	grassDef.name = "Grass";
	grassDef.breakBlockType = SHOVEL;
	grassDef.collidable = true;
	grassDef.resistance = 0.4;
	grassDef.textureRect = sf::IntRect(256, 0, 64, 64);
	mWorld->registerMaterial(new Material(&grassDef));

	MaterialDef dirtDef(stepGrassDef);
	dirtDef.name = "Dirt";
	dirtDef.breakBlockType = SHOVEL;
	dirtDef.collidable = true;
	dirtDef.resistance = 0.3;
	dirtDef.lumpable = true;
	dirtDef.minLumpSize = 4;
	dirtDef.maxLumpSize = 10;
	dirtDef.textureRect = sf::IntRect(320, 0, 64, 64);
	mWorld->registerMaterial(new Material(&dirtDef));

	// Lumpables
	/*
		- Bronze
		- Iron
		- Silver
		- Gold
		- Ruby
		- Diamond
	*/
	MaterialDef baseLumpable(stepStoneDef);
	baseLumpable.collidable = true;
	baseLumpable.lumpable = true;
	baseLumpable.breakBlockType = PICKAXE;

	MaterialDef bronzeDef(&baseLumpable);
	bronzeDef.name = "Bronze";
	bronzeDef.textureRect = sf::IntRect(64, 64, 64, 64);
	bronzeDef.upperLimitY = 5;
	bronzeDef.minLumpSize = 3;
	bronzeDef.maxLumpSize = 5;

	MaterialDef ironDef(&baseLumpable);
	ironDef.name = "Iron";
	ironDef.textureRect = sf::IntRect(320, 64, 64, 64);
	ironDef.upperLimitY = 80;
	ironDef.minLumpSize = 2;
	ironDef.maxLumpSize = 4;

	MaterialDef silverDef(&baseLumpable);
	silverDef.name = "Silver";
	silverDef.textureRect = sf::IntRect(256, 64, 64, 64);
	silverDef.upperLimitY = 140;
	silverDef.minLumpSize = 4;
	silverDef.maxLumpSize = 5;

	MaterialDef goldDef(&baseLumpable);
	goldDef.name = "Gold";
	goldDef.textureRect = sf::IntRect(128, 64, 64, 64);
	goldDef.upperLimitY = 200;
	goldDef.minLumpSize = 1;
	goldDef.maxLumpSize = 8;

	MaterialDef rubyDef(&baseLumpable);
	rubyDef.name = "Ruby";
	rubyDef.textureRect = sf::IntRect(0, 64, 64, 64);
	rubyDef.upperLimitY = 240;
	rubyDef.minLumpSize = 3;
	rubyDef.maxLumpSize = 4;

	MaterialDef diamondDef(&baseLumpable);
	diamondDef.name = "Diamond";
	diamondDef.textureRect = sf::IntRect(192, 64, 64, 64);
	diamondDef.upperLimitY = 300;
	diamondDef.minLumpSize = 1;
	diamondDef.maxLumpSize = 5;

	mWorld->registerMaterial(new Material(&bronzeDef));
	mWorld->registerMaterial(new Material(&ironDef));
	mWorld->registerMaterial(new Material(&silverDef));
	mWorld->registerMaterial(new Material(&goldDef));
	mWorld->registerMaterial(new Material(&rubyDef));
	mWorld->registerMaterial(new Material(&diamondDef));

	mWorld->addTileStop("Air", 0.f);
	mWorld->addTileStop("Stone", 0.5f);
	mWorld->addTileStop("Packed Stone", 1.f);

	thor::ParticleSystem *pSystem = mWorld->getBlockParticleSystem();
	
	sf::Texture particleTexture = mAssets->resourceHolder->getTexture("tileset.png");
	
	// Scale down image using the nearest neighbor interpolation

	float scaleFactor = 0.25; // 50%
	sf::Image image = particleTexture.copyToImage();

	int scaledWidth = (int)(scaleFactor * image.getSize().x);
	int scaledHeight = (int)(scaleFactor * image.getSize().y);

	sf::Image newImage;
	newImage.create(scaledWidth, scaledHeight, sf::Color::Black);

	float xRatio = (float)image.getSize().x / (float)scaledWidth;
	float yRatio = (float)image.getSize().y / (float)scaledHeight;

	int xPos;
	int yPos;
	for (int x = 0; x < scaledWidth; x++)
	{
		for (int y = 0; y < scaledHeight; y++)
		{
			xPos = std::floor(x * xRatio);
			yPos = std::floor(y * yRatio);
			newImage.setPixel(x, y, image.getPixel(xPos, yPos));
		}
	}

	std::string filename = "block_break_particles";
	newImage.saveToFile(mAssets->resourceHolder->getTexturePath() + filename + ".png");

	pSystem->setTexture(mAssets->resourceHolder->getTexture(filename + ".png"));

	for (auto &material : mWorld->getMaterialList())
	{
		unsigned int index = pSystem->addTextureRect(Math::scaleRect(material->getTextureRect(), 0.25));
		material->setParticleRectIndex((int)index);
	}
}

void PlayState::setupHouse()
{
	House *house = new House();
	house->setName("House");
	house->setManager(mObjectManager);
	mObjectManager->addObject(house);

	house->getSprite()->setTexture(mAssets->resourceHolder->getTexture("house.png"));

	// Player tile
	Player *player = static_cast<Player*>(mObjectManager->getObject("Player"));
	Tile *tile = mWorld->getTileByWorldPosition(WorldHelper::toWorldPositionFromSFMLPosition(player->getSprite()->getPosition()))->getRelative(6, 0);
	sf::Vector2f housePositionWorldPos;
	housePositionWorldPos.x = tile->getPosition().x;
	housePositionWorldPos.y = tile->getPosition().y;
	house->getSprite()->setPosition(WorldHelper::toSFMLPositionFromWorldPosition(housePositionWorldPos));

	sf::Vector2f worldPosition = tile->getRelative(-1, 1)->getPosition();
	mObjectManager->spawnTorch(worldPosition);
	worldPosition = tile->getRelative(4, 1)->getPosition();
	mObjectManager->spawnTorch(worldPosition);

	// Chimney
	Chimney *chimney = new Chimney();
	chimney->getSprite()->setTexture(mAssets->resourceHolder->getTexture("chimney.png"));
	chimney->getSprite()->setOrigin(chimney->getSprite()->getGlobalBounds().width / 2.f, chimney->getSprite()->getGlobalBounds().height);
	sf::Vector2f SFMLPos = WorldHelper::toSFMLPositionFromWorldPosition(tile->getRelative(1, 0)->getPosition(), false);
	chimney->getSprite()->setPosition(SFMLPos);
	chimney->setName("Chimney");
	mObjectManager->addObject(chimney);
	house->setChimney(chimney);

}
