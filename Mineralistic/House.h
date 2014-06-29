#pragma once
#include "GameObject.h"

class Chimney;

class House : public GameObject
{
public:
	House();
	~House();

	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void setChimney(Chimney *pChimney);

private:
	bool mActive;
	Chimney *mChimney;
};

