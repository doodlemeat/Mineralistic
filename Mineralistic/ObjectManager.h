#pragma once

#include <vector>
#include "Thor/Input/ActionMap.hpp"

class ObjectGroup;
class GameObject;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	
	bool hasGroup(std::string pName);
	bool hasObject(std::string pName);

	ObjectGroup *getGroup(std::string pName);
	GameObject *getObject(std::string pName);

	void addObject(GameObject *pObject);
	void addGroup(ObjectGroup *pGroup);
	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	std::vector<ObjectGroup*> getGroups();
	std::vector<GameObject*> getObjects();

private:
	std::vector<ObjectGroup*> mObjectGroups;
	std::vector<GameObject*> mGameObjects;
};

