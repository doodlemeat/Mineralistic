#pragma once
#include <string>
#include <map>
#include <deque>
#include "SFML\Audio\SoundBufferRecorder.hpp"
#include "SFML\Audio\Music.hpp"
#include "SFML\Audio\Sound.hpp"

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
	void playMusic(std::string pName, bool pLoop = false);
	
	void update();

private:
	std::map<std::string, Sound*> mSounds;
	std::map<std::string, Music*> mMusic;
};

class Sound
{
public:
	void update();
	void play(bool pLoop = false);
	void setVolume(int volume);
	void stop();
	int getVolume();
	sf::SoundBuffer &getBuffer();
	bool isPlaying();
	void setBuffer(sf::SoundBuffer pBuffer);
private:
	int mVolume;
	std::deque<sf::Sound> mInstances;
	sf::SoundBuffer mBuffer;
};

class Music: public sf::Music
{
public:
	Music();
	~Music();
};