#include "GameEngine.h"
#include "IntroState.h"
#include "vld.h"

int main(int argc, char *argv[])
{
	GameEngine game("Snusk", 1920, 1080);
	game.pushState(new IntroState());
	game.run();
	game.exit();
	return 0;
}