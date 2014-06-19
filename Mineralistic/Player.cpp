#include "Player.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "GameObject.h"
#include "Box2D/Box2D.h"
#include "Thor/Input/ActionMap.hpp"
#include "PhysicsScale.h"
#include "World.h"
#include "ObjectManager.h"
#include <iostream>
#include "Audiosystem.h"
#include "Tile.h"
#include "Logger.h"
#include "Hook.h"
#include "Material.h"
#include "Torch.h"
#include "Math.h"
#include "SFML/Graphics/CircleShape.hpp"

Player::Player() : GameObject("Player")
{
	mCurrentAnimation = IDLE;
	mView = new sf::View();
	mFootContacts = 0;
	mJumpTimer.restart();
	mHooked = false;
	mIsMining = false;
	mJointHack = false;
	mCurrentHook = nullptr;
	mRope = new sf::Sprite();
	mLastTile = nullptr;
	mFacingLeft = true;
	mStanding = true;
}

Player::~Player()
{
	delete mView;
	mView = nullptr;
	
	delete mB2UserData;
	mB2UserData = nullptr;
	
	delete mCurrentTileMark;
	mCurrentTileMark = nullptr;

	delete mRope;
	mRope = nullptr;
}

void Player::update(float dt, thor::ActionMap<std::string> *pActionMap)
{
	if (!mHooked)
	{
		b2Vec2 velocity = mBody->GetLinearVelocity();
		mBody->SetLinearVelocity(b2Vec2(0, velocity.y));
	}

	sf::Vector2f p = mWorld->getTileByWorldPosition(WorldHelper::toWorldPositionFromSFMLPosition(mSprite->getPosition()))->getPosition();
	mCurrentTileMark->setPosition(WorldHelper::toSFMLPositionFromWorldPosition(p));

	if (pActionMap->isActive("Jump") && canJump() && mJumpTimer.getElapsedTime().asMilliseconds() > 100 && !mHooked)
	{
		mJumpTimer.restart();
		mBody->ApplyLinearImpulse(PhysicsScale::gameToPhys(sf::Vector2f(0, -150)), mBody->GetWorldCenter(), true);
	}
	else if (pActionMap->isActive("Jump") && mHooked)
	{
		// Climb rope
		b2DistanceJoint *joint = static_cast<b2DistanceJoint*>(mCurrentHook->getJoint());
		float length = PhysicsScale::physToGame(joint->GetLength());
		length -= 2.f;
		joint->SetLength(PhysicsScale::gameToPhys(length));
		if (!mObjectManager->getAudioSystem()->getSound("Climb_Rope")->isPlaying())
		{
			mObjectManager->getAudioSystem()->playSound("Climb_Rope", true);
		}
	}
	else if (pActionMap->isActive("Climb_Down") && mHooked)
	{
		b2DistanceJoint *joint = static_cast<b2DistanceJoint*>(mCurrentHook->getJoint());
		float length = PhysicsScale::physToGame(joint->GetLength());
		length += 2.f;
		joint->SetLength(PhysicsScale::gameToPhys(length));
		if (!mObjectManager->getAudioSystem()->getSound("Climb_Rope")->isPlaying())
		{
			mObjectManager->getAudioSystem()->playSound("Climb_Rope", true);
		}
	}
	else
	{
		mObjectManager->getAudioSystem()->getSound("Climb_Rope")->stop();
	}

	if (pActionMap->isActive("Walk_Left"))
	{
		mFacingLeft = true;
		mCurrentAnimation = WALK_LEFT;
		mStanding = false;
	}
	else if (pActionMap->isActive("Walk_Right"))
	{
		mFacingLeft = false;
		mCurrentAnimation = WALK_RIGHT;
		mStanding = false;
	}
	else
	{
		mCurrentAnimation = IDLE;
		mStanding = true;
	}

	if (mIsMining)
	{
		if (mFacingLeft)
		{
			mCurrentAnimation = MINE_LEFT;
		}
		else
		{
			mCurrentAnimation = MINE_RIGHT;
		}
	}

	if (mHooked)
	{
		mCurrentAnimation = SWING;
	}

	// Toggle hooked
	if (mHooked && pActionMap->isActive("Swing"))
	{
		mHooked = false;
		mCurrentAnimation = IDLE;
	}

	// Mining
	if (pActionMap->isActive("Mine") && !mHooked)
	{
		mIsMining = true;
		Tile *tileRelative = nullptr;
		bool tileFound = false;

		if (mFacingLeft)
		{
			try
			{
				Tile *tile = mWorld->getTileByWorldPosition(WorldHelper::toWorldPositionFromSFMLPosition(mSprite->getPosition()));
				tileRelative = tile->getRelative(sf::Vector2i(-1, 0));
				tileFound = true;
			}
			catch (WorldException& e)
			{
			}
		}
		else
		{
			try
			{
				Tile *tile = mWorld->getTileByWorldPosition(WorldHelper::toWorldPositionFromSFMLPosition(mSprite->getPosition()));
				tileRelative = tile->getRelative(sf::Vector2i(1, 0));
				tileFound = true;
			}
			catch (WorldException& e)
			{
			}
		}

		if (tileFound)
		{
			int timeToBreak = 0;
			if (tileRelative->getMaterial()->isCollidable())
			{
				if (!mObjectManager->getAudioSystem()->getSound("Pick")->isPlaying())
				{
					std::cout << "IsNotPlaying";
					mObjectManager->getAudioSystem()->playSound("Pick", 300);
				}

				timeToBreak = tileRelative->getMaterial()->getResistance();
				if (mLastTile != tileRelative)
				{
					mBreakTileTimer.restart();
				}
			}
			else
			{
				mBreakTileTimer.stop();
				mObjectManager->getAudioSystem()->getSound("Pick")->stop();
			}

			mLastTile = tileRelative;

			if (mBreakTileTimer.isRunning() && mBreakTileTimer.getElapsedTime().asSeconds() >= timeToBreak)
			{
				mBreakTileTimer.stop();
				mObjectManager->getAudioSystem()->playSound("Break", false);
				tileRelative->breakNaturally();
				mLastTile = nullptr;
			}
		}
	}
	else
	{
		mBreakTileTimer.stop();
		mLastTile = nullptr;
		mIsMining = false;
		mObjectManager->getAudioSystem()->getSound("Pick")->stop();
	}

	// Move left
	if (pActionMap->isActive("Walk_Left"))
	{
		sf::Vector2f velocity(-50, 0);
		if (mHooked)
		{
			velocity.x = -5;
		}
		mBody->ApplyLinearImpulse(PhysicsScale::gameToPhys(velocity), mBody->GetWorldCenter(), true);
	}

	// Move right
	if (pActionMap->isActive("Walk_Right"))
	{
		sf::Vector2f velocity(50, 0);
		if (mHooked)
		{
			velocity.x = 5;
		}
		mBody->ApplyLinearImpulse(PhysicsScale::gameToPhys(velocity), mBody->GetWorldCenter(), true);
	}

	sf::Vector2f newPosition = PhysicsScale::physToGame(mBody->GetPosition()) - sf::Vector2f(0, 4);
	mSprite->setPosition(newPosition);


	if (mHooked)
	{
		updateHook();
	}

	if (pActionMap->isActive("Throw_Hook"))
	{
		// If player is hooked, cancel the current hook and check if player can hook somewhere else
		if (mHooked)
		{
			quitHook();
		}
		else
		{
			mObjectManager->getAudioSystem()->playSound("Throw_Hook", false);
			try
			{
				Tile *targetTile = mWorld->getClosestTileInDirection(WorldHelper::toWorldPositionFromSFMLPosition(mSprite->getPosition()), World::NORTH, 10);
				Hook *spawnedHook = mObjectManager->spawnHook(targetTile->getPosition());
				hookTo(spawnedHook);
				mObjectManager->getAudioSystem()->playSound("Hook_Attached", false);
			}
			catch (WorldException &e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}

	if (pActionMap->isActive("PutTorch"))
	{
		Tile *current = mWorld->getTileByWorldPosition(WorldHelper::toWorldPositionFromSFMLPosition(mSprite->getPosition()));
		if (current->getTorch() != nullptr)
		{
			current->getTorch()->setDead(true);
			current->setTorch(nullptr);
		}
		else
		{
			Torch *torch = mObjectManager->spawnTorch(current->getPosition(), current);
		}
	}

	switch (mCurrentAnimation)
	{
	case IDLE:
		if (getAnimator()->getPlayingAnimation() != "Idle")
		{
			getAnimator()->playAnimation("Idle", true);
		}
		break;
	case MINE_LEFT:
		if (!getAnimator()->isPlayingAnimation() || (getAnimator()->getPlayingAnimation() != "Mine_Left"))
		{
			getAnimator()->playAnimation("Mine_Left", true);
		}
		break;
	case MINE_RIGHT:
		if (!getAnimator()->isPlayingAnimation() || (getAnimator()->getPlayingAnimation() != "Mine_Right"))
		{
			getAnimator()->playAnimation("Mine_Right", true);
		}
		break;
	case WALK_LEFT:
		if (mStanding)
		{
			getAnimator()->stopAnimation();
		}
		if (!mStanding)
		{
			if (!getAnimator()->isPlayingAnimation() || (getAnimator()->getPlayingAnimation() != "Walk_Left"))
			{
				getAnimator()->playAnimation("Walk_Left", true);
			}
		}
		break;
	case WALK_RIGHT:
		if (mStanding)
		{
			getAnimator()->stopAnimation();
		}
		if (!mStanding)
		{
			if (!getAnimator()->isPlayingAnimation() || (getAnimator()->getPlayingAnimation() != "Walk_Right"))
			{
				getAnimator()->playAnimation("Walk_Right", true);
			}
		}
		break;
	case SWING:
		if (!getAnimator()->isPlayingAnimation() || getAnimator()->getPlayingAnimation() != "Swing")
		{
			getAnimator()->playAnimation("Swing", true);
		}
		break;
	}

	getAnimator()->update(sf::seconds(dt));
	getAnimator()->animate(*mSprite);

	mView->setCenter(mSprite->getPosition());
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//target.draw(*mCurrentTileMark);
	target.draw(*mSprite);
	if (mHooked)
	{
		target.draw(*mRope);
	}
}

sf::View * Player::getView()
{
	return mView;
}

void Player::setWorld(World *pWorld)
{
	mWorld = pWorld;
}

World * Player::getWorld()
{
	return mWorld;
}

void Player::setB2UserData(B2UserData *pUserData)
{
	mB2UserData = pUserData;
}

void Player::increaseFootContacts()
{
	mFootContacts++;
}

void Player::decreaseFootContacts()
{
	mFootContacts--;
}

bool Player::canJump()
{
	return mFootContacts > 0;
}

void Player::setCurrentTileMark(sf::Sprite *pCurrentTileMark)
{
	mCurrentTileMark = pCurrentTileMark;
}

void Player::hookTo(Hook *pHook)
{
	mCurrentHook = pHook;

	sf::Vector2f from = mSprite->getPosition() + sf::Vector2f(0, -32);
	sf::Vector2f to = mCurrentHook->getSprite()->getPosition();

	mRope->setTextureRect(sf::IntRect(0, 0, 8, Math::euclideanDistance(from, to)));
	mRope->setRotation(Math::RAD2DEG(Math::angleBetween(to, from)) + 90.f);
	mRope->setOrigin(4, mRope->getGlobalBounds().height / 2.f);

	mCurrentHook->setJoint(mObjectManager->createDistanceJointBetween(mBody, pHook->getBody(), Math::euclideanDistance(to, from)));
	mHooked = true;
}

void Player::updateHook()
{
	sf::Vector2f from = mSprite->getPosition() + sf::Vector2f(0, -32);
	sf::Vector2f to = mCurrentHook->getSprite()->getPosition();

	float centerX = (to.x + from.x) / 2.f;
	float centerY = (to.y + from.y) / 2.f;

	mRope->setOrigin(4, mRope->getLocalBounds().height / 2.f);
	mRope->setPosition(centerX, centerY);
	mRope->setRotation(Math::RAD2DEG(Math::angleBetween(to, from)) + 90.f);
	mRope->setTextureRect(sf::IntRect(0, 0, 8, Math::euclideanDistance(from, to)));
}

sf::Sprite *Player::getRope()
{
	return mRope;
}

void Player::quitHook()
{
	mObjectManager->getB2World()->DestroyJoint(mCurrentHook->getJoint());
	mObjectManager->getB2World()->DestroyBody(mCurrentHook->getBody());
	mCurrentHook->setDead(true);
	mHooked = false;
	mJointHack = false;
}
