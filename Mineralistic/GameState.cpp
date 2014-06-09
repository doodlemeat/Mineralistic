#include "GameState.h"
#include "GameStateAsset.h"
#include <Thor/Input/ActionMap.hpp>

GameState::GameState()
{
	this->m_pausedWhenObscured = true;
}


GameState::~GameState()
{
}

void GameState::setStateAssets(GameStateAsset* pGameStateAsset)
{
	this->mAssets = pGameStateAsset;
}

thor::ActionMap<std::string> *GameState::getActionMap()
{
	return mActionMap;
}

void GameState::createActionMap()
{
	mActionMap = new thor::ActionMap<std::string>();
}
