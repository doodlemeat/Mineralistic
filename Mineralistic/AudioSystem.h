#pragma once
#include <string>
#include <map>
#include <deque>
#include "SFML\Audio\SoundBufferRecorder.hpp"
#include "SFML\Audio\Music.hpp"
#include "SFML\Audio\Sound.hpp"
#include "Thor\Time\Timer.hpp"

class Sound;
class Music;

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	void createSound(std::string pName, sf::SoundBuffer pBuffer);
	void createMusic(std::string pName, std::string pFilePath);

	Sound* getSound(std::string pName);
	Music* getMusic(std::string pName);

	void playSound(std::string pName, bool pLoop);
	void playSound(std::string pName, int pTimeBetween);
	void playMusic(std::string pName, bool pLoop = false);
	
	void update();

private:
	std::map<std::string, Sound*> mSounds;
	std::map<std::string, Music*> mMusic;
};

class Sound
{
public:
	Sound();
	~Sound();
	void update();
	void play(bool pLoop = false);
	void setVolume(int volume);
	void stop();
	int getVolume();
	int getTimeBetween();
	sf::SoundBuffer &getBuffer();
	bool isPlaying();
	bool isTimed();
	void setBuffer(sf::SoundBuffer pBuffer);
	void setTimed(bool pValue);
	void setTimeBetween(int pTimeBetween);
	thor::Timer *getTimer();
private:
	int mVolume;
	int mTimeBetween;
	bool mTimed;
	std::deque<sf::Sound> mInstances;
	sf::SoundBuffer mBuffer;
	thor::Timer mTimer;
};

class Music: public sf::Music
{
public:
	Music();
	~Music();
};