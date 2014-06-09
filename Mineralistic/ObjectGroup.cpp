#include "ObjectGroup.h"
#include "GameObject.h"

#include <assert.h>

ObjectGroup::ObjectGroup(std::string pName)
{
	assert(mName.length() > 0);
	mNameIsSet = true;
	mName = pName;
}

ObjectGroup::ObjectGroup()
{
	mNameIsSet = false;
	mName = "";
}

ObjectGroup::~ObjectGroup()
{
	auto it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		GameObject *object = (*it);
		delete object;
		object = nullptr;
		++it;
	}
}

std::string ObjectGroup::getName()
{
	return mName;
}

bool ObjectGroup::isName(std::string pName)
{
	return mName == pName;
}

void ObjectGroup::setName(std::string pName)
{
	assert(pName.length() > 0);
	mName = pName;
	mNameIsSet = true;
}

GameObject *ObjectGroup::addGameObject(GameObject *pObject, std::string pName)
{
	return nullptr;
}

std::vector<GameObject*> ObjectGroup::getObjects()
{
	return mGameObjects;
}

bool ObjectGroup::hasName()
{
	return mNameIsSet;
}

void ObjectGroup::setManager(ObjectManager *pObjectManager)
{
	mObjectManager = pObjectManager;
}
