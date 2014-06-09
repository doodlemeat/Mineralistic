#pragma once

#include <string>

struct GameStateAsset;

namespace thor
{
	template <typename T> class ActionMap;
}

class GameState
{
public:
	GameState();
	virtual ~GameState();
	virtual void entering() = 0; // entered
	virtual void leaving() = 0; // leaved
	virtual void obscuring() = 0; // obscured
	virtual void releaving() = 0; // revealed
	virtual void setupActions() = 0;
	virtual void draw() = 0;

	virtual bool update(float dt) = 0;

	void setStateAssets(GameStateAsset* pGameStateAsset);
	thor::ActionMap<std::string> *getActionMap();
	void createActionMap();
protected:
	GameStateAsset *mAssets;
	thor::ActionMap<std::string> *mActionMap;
	bool m_pausedWhenObscured;
};

