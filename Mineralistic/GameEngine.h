#pragma once
#include <string>
#include <vector>

#include <SFML/Window/Event.hpp>

class GameState;
class WindowManager;
class ResourceHolder;
class AudioSystem;
struct GameStateAsset;

namespace thor
{
	template <typename T> class ActionMap;
}

class GameEngine
{
public:
	GameEngine(std::string pTitle, int pWindowWidth, int pWindowHeight);
	~GameEngine();

	void changeState(GameState* pGameState);
	void popState();
	void pushState(GameState* pGameState);
	void notifyRevealedStates();
	void notifyObscuredStates();
	void draw();
	void clearEvents();
	void run();
	void exit();
	void updateEvents();
	void pushEvents(sf::Event pEvent);
	bool updateActiveStates(float dt);

	GameStateAsset* getStateAsset();
private:
	GameStateAsset* mStateAssets;
	WindowManager *mWindowManager;
	ResourceHolder *mResourceHolder;
	AudioSystem *mAudioSystem;
	std::vector<GameState*> mActiveStates;
	thor::ActionMap<std::string>* mActionMap;
	bool mRunning;
};


