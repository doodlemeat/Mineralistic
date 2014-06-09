#pragma once

class GameEngine;
class WindowManager;
class ResourceHolder;

struct GameStateAsset
{
	GameEngine *gameEngine;
	WindowManager *windowManager;
	ResourceHolder *resourceHolder;
};
