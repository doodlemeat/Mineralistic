#pragma once
#include <string>
#include <vector>

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

private:
	bool mNameIsSet;
	std::string mName;
	ObjectManager *mObjectManager;
	std::vector<GameObject*> mGameObjects;
};

