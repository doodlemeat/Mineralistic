#pragma once

#include "GameObject.h"

class b2Joint;

class Hook: public GameObject
{
public:
	Hook();
	~Hook(); 
	
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void setJoint(b2Joint *pJoint);
	b2Joint *getJoint();
private:
	b2Joint *mJoint;
};

