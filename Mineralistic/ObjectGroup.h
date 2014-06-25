#pragma once
#include <string>
#include <vector>
#include "Thor/Input/ActionMap.hpp"

class GameObject;
class ObjectManager;

class ObjectGroup
{
public:
	ObjectGroup(std::string pName);
	ObjectGroup();
	~ObjectGroup();
	
	std::string getName();
	bool isName(std::string pName);
	void setName(std::string pName);
	bool hasName();

	GameObject *addGameObject(GameObject *pObject, std::string pName = "");
	std::vector<GameObject*> *getObjects();
	void setManager(ObjectManager *pObjectManager);
	void update(float dt);
	void update(float dt, thor::ActionMap<std::string> *pActionMap);
private:
	bool mNameIsSet;
	std::string mName;
	ObjectManager *mObjectManager;
	std::vector<GameObject*> mGameObjects;
};

