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
	
	std::vector<GameObject*> *objects = getGroup("hooks")->getObjects();
	for (auto &hook : *objects)
	{
		hook->update(dt);
	}
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
