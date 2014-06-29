#include "ObjectManager.h"
#include "GameObject.h"
#include "ObjectGroup.h"
#include <iostream>
#include "Thor\Input\ActionMap.hpp"
#include "Hook.h"
#include "ResourceHolder.h"
#include "World.h"
#include "Box2D\Box2D.h"
#include "PhysicsScale.h"
#include "Math.h"
#include "Tile.h"
#include "Monster.h"
#include "Torch.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	auto groupIter = mObjectGroups.begin();
	while (groupIter != mObjectGroups.end())
	{
		ObjectGroup *group = (*groupIter);
		delete group;
		group = nullptr;
		++groupIter;
	}

	auto objectIter = mGameObjects.begin();
	while (objectIter != mGameObjects.end())
	{
		GameObject *object = (*objectIter);
		delete object;
		object = nullptr;
		++objectIter;
	}
}

bool ObjectManager::hasGroup(std::string pName)
{
	for (auto &group : mObjectGroups)
	{
		if (group->isName(pName))
		{
			return true;
		}
	}
	return false;
}

bool ObjectManager::hasObject(std::string pName)
{
	for (auto &object : mGameObjects)
	{
		if (object->isName(pName))
		{
			return true;
		}
	}
	return false;
}

ObjectGroup *ObjectManager::getGroup(std::string pName)
{
	for (auto &group : mObjectGroups)
	{
		if (group->isName(pName))
		{
			return group;
		}
	}
	std::cout << "A group with name " << pName << " do not exist" << std::endl;
	return nullptr;
}

GameObject *ObjectManager::getObject(std::string pName)
{
	for (auto &object : mGameObjects)
	{
		if (object->isName(pName))
		{
			return object;
		}
	}
	std::cout << "An object with name " << pName << " do not exist" << std::endl;
	return nullptr;
}

void ObjectManager::addGroup(ObjectGroup *pGroup)
{
	if (this->hasGroup(pGroup->getName()))
	{
		std::cout << "Failed to add group. A group with name " << pGroup->getName() << " do already exist" << std::endl;
		return;
	}

	if (!pGroup->hasName())
	{
		std::cout << "Failed to add object group. Object group has no name." << std::endl;
		return;
	}

	pGroup->setManager(this);

	std::vector<GameObject*> *objects = pGroup->getObjects();
	for (auto &object : *objects)
	{
		object->setManager(this);
	}

	std::cout << "Added group " << pGroup->getName() << " to the object manager" << std::endl;
	mObjectGroups.push_back(pGroup);
}

void ObjectManager::addObject(GameObject *pObject)
{
	if (this->hasObject(pObject->getName()))
	{
		std::cout << "Failed to add game object. A game object with name " << pObject->getName() << " do already exist" << std::endl;
		return;
	}

	if (!pObject->hasName())
	{
		std::cout << "Failed to add game object. Game object has no name." << std::endl;
		return;
	}

	pObject->setManager(this);
	mGameObjects.push_back(pObject);
}

std::vector<ObjectGroup*> ObjectManager::getGroups()
{
	return mObjectGroups;
}

std::vector<GameObject*> ObjectManager::getObjects()
{
	return mGameObjects;
}

void ObjectManager::update(float dt, thor::ActionMap<std::string> *pActionMap)
{
	// Check if object are dead and remove them
	auto groupIter = mObjectGroups.begin();
	while (groupIter != mObjectGroups.end())
	{
		ObjectGroup *group = (*groupIter);
		std::vector<GameObject*> *objects = group->getObjects();
		auto objectIter = objects->begin();
		while (objectIter != objects->end())
		{
			GameObject *obj = (*objectIter);
			if (obj->isDead())
			{
				delete obj;
				obj = nullptr;
				objectIter = objects->erase(objectIter);
			}
			else
			{
				++objectIter;
			}
		}
		++groupIter;
	}

	getObject("Player")->update(dt, pActionMap);
	getGroup("hooks")->update(dt);
	getGroup("monsters")->update(dt);
	getGroup("torches")->update(dt);
}

AudioSystem *ObjectManager::getAudioSystem()
{
	return mAudioSystem;
}

void ObjectManager::setAudioSystem(AudioSystem *pAudiosystem)
{
	mAudioSystem = pAudiosystem;
}

Hook *ObjectManager::spawnHook(sf::Vector2f pWorldPosition)
{
	Hook *hook = new Hook();
	hook->getSprite()->setTexture(mResourceHolder->getTexture("hook.png"));
	hook->getSprite()->setPosition(WorldHelper::toSFMLPositionFromWorldPosition(pWorldPosition, true));
	hook->getSprite()->setOrigin(32, 32);
	hook->setGroup(getGroup("hooks"));

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	b2Vec2 pos = PhysicsScale::gameToPhys(hook->getSprite()->getPosition());
	bodyDef.position.Set(pos.x, pos.y);
	b2Body *body = mB2World->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = PhysicsScale::gameToPhys(8);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
	hook->setBody(body);

	getGroup("hooks")->addGameObject(hook);
	return hook;
}

void ObjectManager::setResourceHolder(ResourceHolder *pResourceHolder)
{
	mResourceHolder = pResourceHolder;
}

b2Joint *ObjectManager::createDistanceJointBetween(b2Body *pBodyA, b2Body *pBodyB, float pLength)
{
	b2DistanceJointDef jointDef;
	jointDef.bodyA = pBodyA;
	jointDef.bodyB = pBodyB;
	jointDef.type = e_distanceJoint;
	jointDef.dampingRatio = 1;
	jointDef.frequencyHz = 0;
	jointDef.length = PhysicsScale::gameToPhys(pLength);
	jointDef.localAnchorA = PhysicsScale::gameToPhys(sf::Vector2f(0, -32));
	b2Joint *joint = mB2World->CreateJoint(&jointDef);
	return joint;
}

void ObjectManager::setB2World(b2World *pWorld)
{
	mB2World = pWorld;
}

b2World *ObjectManager::getB2World()
{
	return mB2World;
}

void ObjectManager::setWorld(World *pWorld)
{
	mWorld = pWorld;
}

World *ObjectManager::getWorld()
{
	return mWorld;
}

void ObjectManager::spawnMonster(sf::Vector2f pWorldPosition)
{
	Monster *monster = new Monster();
	monster->getSprite()->setTexture(mResourceHolder->getTexture("mumba.png"));
	monster->getSprite()->setPosition(WorldHelper::toSFMLPositionFromWorldPosition(pWorldPosition, true));
	monster->getSprite()->setOrigin(32, 32);
	monster->setGroup(getGroup("monsters"));

	thor::FrameAnimation *walkLeft = new thor::FrameAnimation();
	walkLeft->addFrame(1.f, sf::IntRect(0, 0, 64, 64));
	walkLeft->addFrame(1.f, sf::IntRect(64, 0, 64, 64));
	walkLeft->addFrame(1.f, sf::IntRect(128, 0, 64, 64));
	walkLeft->addFrame(1.f, sf::IntRect(192, 0, 64, 64));

	thor::FrameAnimation *walkRight = new thor::FrameAnimation();
	walkRight->addFrame(1.f, sf::IntRect(0, 64, 64, 64));
	walkRight->addFrame(1.f, sf::IntRect(64, 64, 64, 64));
	walkRight->addFrame(1.f, sf::IntRect(128, 64, 64, 64));
	walkRight->addFrame(1.f, sf::IntRect(192, 64, 64, 64));

	monster->addAnimation("Walk_Left", walkLeft, sf::seconds(0.3f));
	monster->addAnimation("Walk_Right", walkRight, sf::seconds(0.3f));
	monster->getAnimator()->playAnimation("Walk_Left", true);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	b2Vec2 pos = PhysicsScale::gameToPhys(monster->getSprite()->getPosition());
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = true;
	b2Body *body = mB2World->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	sf::Vector2f boundingBox(10, 28);
	b2Vec2 boundingBoxPhysic = PhysicsScale::gameToPhys(boundingBox);
	boxShape.SetAsBox(boundingBoxPhysic.x, boundingBoxPhysic.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.friction = 1;
	body->CreateFixture(&fixtureDef);
	monster->setBody(body);

	getGroup("monsters")->addGameObject(monster);
}

void ObjectManager::spawnTorch(sf::Vector2f pWorldPosition)
{
	Torch *torch = new Torch(16, 16);
	torch->getSprite()->setTexture(mResourceHolder->getTexture("torch.png"));
	torch->getSprite()->setPosition(WorldHelper::toSFMLPositionFromWorldPosition(pWorldPosition, true));
	torch->getSprite()->setOrigin(32, 32);
	torch->setGroup(getGroup("torches"));
	torch->setManager(this);
	Tile *tile = mWorld->getTileByWorldPosition(pWorldPosition);
	tile->setTorch(torch);
	torch->setTile(tile);

	getGroup("torches")->addGameObject(torch);
}

void ObjectManager::orderGameObjects()
{
	std::sort(mGameObjects.begin(), mGameObjects.end(), 
		[](GameObject *a, GameObject *b) -> bool
	{
		return a->getZOrder() < b->getZOrder();
	});
}
