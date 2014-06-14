#pragma once

#include <vector>
#include "Thor/Input/ActionMap.hpp"

class ObjectGroup;
class AudioSystem;
class GameObject;
class ResourceHolder;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	
	bool hasGroup(std::string pName);
	bool hasObject(std::string pName);

	ObjectGroup *getGroup(std::string pName);
	GameObject *getObject(std::string pName);

	void setAudioSystem(AudioSystem *pAudiosystem);
	void setResourceHolder(ResourceHolder *pResourceHolder);
	AudioSystem *getAudioSystem();

	void addObject(GameObject *pObject);
	void addGroup(ObjectGroup *pGroup);
	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	std::vector<ObjectGroup*> getGroups();
	std::vector<GameObject*> getObjects();

	void spawnHook(sf::Vector2f pWorldPosition);
private:
	std::vector<ObjectGroup*> mObjectGroups;
	std::vector<GameObject*> mGameObjects;
	AudioSystem *mAudioSystem;
	ResourceHolder *mResourceHolder;
};

