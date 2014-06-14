#include "ObjectManager.h"
#include "GameObject.h"
#include "ObjectGroup.h"
#include <iostream>
#include "Thor\Input\ActionMap.hpp"
#include "Hook.h"
#include "ResourceHolder.h"
#include "World.h"


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

	for (auto &object : pGroup->getObjects())
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
	getObject("Player")->update(dt, pActionMap);
	
	for (auto &hook : getGroup("hooks")->getObjects())
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

void ObjectManager::spawnHook(sf::Vector2f pWorldPosition)
{
	Hook *hook = new Hook();
	hook->getSprite()->setTexture(mResourceHolder->getTexture("hook.png"));
	hook->getSprite()->setPosition(WorldHelper::toSFMLPositionFromWorldPosition(pWorldPosition, true));
	hook->getSprite()->setOrigin(32, 32);
	getGroup("hooks")->addGameObject(hook);
}

void ObjectManager::setResourceHolder(ResourceHolder *pResourceHolder)
{
	mResourceHolder = pResourceHolder;
}
