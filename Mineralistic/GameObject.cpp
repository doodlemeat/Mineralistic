#include <iostream>
#include <typeinfo>
#include "SFML\Graphics\RenderTarget.hpp"
#include "GameObject.h"
#include <assert.h>

GameObject::GameObject()
{
	construct();
}

GameObject::GameObject(std::string pName)
{
	construct();
	assert(pName.length() > 0);
	mName = pName;
	mNameIsSet = true;
}

GameObject::~GameObject()
{
	delete mSprite;
	mSprite = nullptr;

	delete mAnimator;
	mAnimator = nullptr;

	auto animationIter = mFrameAnimations.begin();
	while (animationIter != mFrameAnimations.end())
	{
		thor::FrameAnimation *animation = animationIter->second;
		delete animation;
		animation = nullptr;
		++animationIter;
	}
}

void GameObject::construct()
{
	mDead = false;
	mGroup = nullptr;
	mNameIsSet = false;
	mSprite = new sf::Sprite();
	mAnimator = new thor::Animator<sf::Sprite, std::string>();
}
void GameObject::setGroup(ObjectGroup *pObjectGroup)
{
	mGroup = pObjectGroup;
}

bool GameObject::isName(std::string pIdentifier)
{
	return mName == pIdentifier;
}

std::string GameObject::getName()
{
	return mName;
}

void GameObject::setName(std::string pName)
{
	assert(pName.length() > 0);
	mName = pName;
	mNameIsSet = true;
}

bool GameObject::hasName()
{
	return mNameIsSet;
}

sf::Sprite * GameObject::getSprite()
{
	return mSprite;
}

thor::Animator<sf::Sprite, std::string> * GameObject::getAnimator()
{
	return mAnimator;
}

void GameObject::addAnimation(std::string pName, thor::FrameAnimation *pAnimation, sf::Time pTime)
{
	auto animationIter = mFrameAnimations.find(pName);
	if (animationIter != mFrameAnimations.end())
	{
		std::cout << "Failed to add animation " << pName << " in game object " << mName << ". Animation already exist." << std::endl;
		return;
	}

	mAnimator->addAnimation(pName, *pAnimation, pTime);
	mFrameAnimations.insert(std::make_pair(pName, pAnimation));
}

std::map<std::string, thor::FrameAnimation*> GameObject::getAnimations()
{
	return mFrameAnimations;
}

void GameObject::setManager(ObjectManager *pObjectManager)
{
	mObjectManager = pObjectManager;
}

void GameObject::setBody(b2Body *pBody)
{
	mBody = pBody;
}

b2Body *GameObject::getBody()
{
	return mBody;
}

void GameObject::update(float dt)
{

}

void GameObject::update(float dt, thor::ActionMap<std::string> *pActionMap /*= nullptr*/)
{

}

void GameObject::setDead(bool pValue)
{
	mDead = pValue;
}

bool GameObject::isDead()
{
	return mDead;
}
