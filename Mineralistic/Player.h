#pragma once

#include "GameObject.h"
#include "SFML\Graphics\View.hpp"
#include "Thor\Time\StopWatch.hpp"

class World;
class B2UserData;
class Hook;

class Player : public GameObject
{
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
private:
	bool mHooked;
	unsigned int mRopes;
	sf::View* mView;
	B2UserData *mB2UserData;
	unsigned int mFootContacts;
	World *mWorld;
	thor::StopWatch mStopWatch;
	sf::Sprite *mCurrentTileMark;
	sf::Sprite *mRope;
	Hook *mCurrentHook;
	bool mJointHack;
};

