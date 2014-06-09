#include "GameEngine.h"
#include "IntroState.h"

int main(int argc, char *argv[])
{
	GameEngine game("Snusk", 1024, 768);
	game.pushState(new IntroState());
	game.run();
	game.exit();
	return 0;
}