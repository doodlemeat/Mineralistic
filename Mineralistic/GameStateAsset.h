#pragma once

class GameEngine;
class WindowManager;
class ResourceHolder;
class AudioSystem;

struct GameStateAsset
{
	GameEngine *gameEngine;
	WindowManager *windowManager;
	ResourceHolder *resourceHolder;
	AudioSystem *audioSystem;
};
