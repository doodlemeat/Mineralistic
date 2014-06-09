#pragma once

#include <vector>
#include <typeinfo>
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <Thor/Animation/Animator.hpp>
#include "Thor/Animation/FrameAnimation.hpp"

class Component;
class ObjectGroup;
class ObjectManager;

class GameObject : public sf::Drawable
{
public:
	GameObject();
	GameObject(std::string pName);
	~GameObject();

	void construct();
	void setManager(ObjectManager *pObjectManager);
	void setGroup(ObjectGroup *pObjectGroup);
	void addAnimation(std::string pName, thor::FrameAnimation *pAnimation, sf::Time pTime);

	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	
	bool isName(std::string pName);
	bool hasName();
	void setName(std::string pName);
	std::string getName();

	sf::Sprite *getSprite();
	thor::Animator<sf::Sprite, std::string> *getAnimator();
	std::map<std::string, thor::FrameAnimation*> getAnimations();

protected:
	bool mNameIsSet;
	std::string mName;
	ObjectManager *mObjectManager;
	ObjectGroup *mGroup;

	sf::Sprite *mSprite;
	thor::Animator<sf::Sprite, std::string> *mAnimator;
	std::map<std::string, thor::FrameAnimation*> mFrameAnimations;
};

