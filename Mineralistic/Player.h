#pragma once

#include "GameObject.h"
#include "SFML\Graphics\View.hpp"
#include "Thor\Time\StopWatch.hpp"
#include "Material.h"

class World;
class B2UserData;
class Hook;
class Tile;

class Player : public GameObject
{
	enum AnimationType
	{
		IDLE,
		WALK_LEFT,
		WALK_RIGHT,
		MINE_LEFT,
		MINE_RIGHT,
		MINE_DOWN,
		SHOVEL_LEFT,
		SHOVEL_RIGHT,
		SHOVEL_DOWN,
		SWING
	};
public:
	Player();
	~Player();

	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::View *getView();
	sf::Sprite *getRope();
	void centerOrigin();

	void setWorld(World *pWorld);
	void setB2UserData(B2UserData *pUserData);
	World *getWorld();
	void increaseFootContacts();
	void decreaseFootContacts();
	bool canJump();
	void setCurrentTileMark(sf::Sprite *pCurrentTileMark);
	void hookTo(Hook *pHook);
	void updateHook();
	void quitHook();

	bool IsMining() const { return mIsMining; }
	void IsMining(bool val) { mIsMining = val; }

	bool Hooked() const { return mHooked; }
	void Hooked(bool val) { mHooked = val; }

	bool Standing() const { return mStanding; }
	void Standing(bool val) { mStanding = val; }

	bool FacingLeft() const { return mFacingLeft; }
	void FacingLeft(bool val) { mFacingLeft = val; }
private:
	bool mHooked;
	bool mJointHack;
	bool mIsMining;
	bool mFacingLeft;
	bool mStanding;
	int lastX;
	unsigned int mFootContacts;
	AnimationType mCurrentAnimation;
	BreakBlockType mMineType;

	sf::View* mView;
	B2UserData *mB2UserData;
	World *mWorld;

	thor::StopWatch mJumpTimer;
	thor::StopWatch mBreakTileTimer;

	thor::StopWatch mFootStepAudio;

	sf::Sprite *mCurrentTileMark;
	sf::Sprite *mRope;
	Hook *mCurrentHook;
	Tile *mLastTile;
};

