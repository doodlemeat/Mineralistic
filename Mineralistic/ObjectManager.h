#pragma once

#include <vector>

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
	void update(float dt);
	std::vector<ObjectGroup*> getGroups();
	std::vector<GameObject*> getObjects();

private:
	std::vector<ObjectGroup*> mObjectGroups;
	std::vector<GameObject*> mGameObjects;
};

