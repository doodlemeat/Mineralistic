#pragma once

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <Thor/Animation/Animator.hpp>
#include "Thor/Animation/FrameAnimation.hpp"
#include "Thor/Input/ActionMap.hpp"

class Component;
class ObjectGroup;
class ObjectManager;
class b2Body;

class GameObject : public sf::Drawable
{
public:
	GameObject();
	GameObject(std::string pName);
	~GameObject();

	void construct();
	void teleport(int x, int y);
	void setBody(b2Body *pBody);
	void setManager(ObjectManager *pObjectManager);
	void setGroup(ObjectGroup *pObjectGroup);
	void setDead(bool pValue);
	void setZOrder(int pValue);
	void setName(std::string pName);
	void addAnimation(std::string pName, thor::FrameAnimation *pAnimation, sf::Time pTime);

	virtual void update(float dt);
	virtual void update(float dt, thor::ActionMap<std::string> *pActionMap);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	
	bool isName(std::string pName);
	bool hasName();
	bool isDead();

	int getZOrder();

	std::string getName();

	b2Body *getBody();
	sf::Sprite *getSprite();
	thor::Animator<sf::Sprite, std::string> *getAnimator();
	std::map<std::string, thor::FrameAnimation*> getAnimations();

protected:
	bool mNameIsSet;
	bool mDead;

	int mZOrder;

	std::string mName;
	ObjectManager *mObjectManager;
	ObjectGroup *mGroup;
	b2Body *mBody;

	sf::Sprite *mSprite;
	thor::Animator<sf::Sprite, std::string> *mAnimator;
	std::map<std::string, thor::FrameAnimation*> mFrameAnimations;
};

