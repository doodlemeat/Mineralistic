#include "AudioSystem.h"
#include <iostream>
#include "SFML\Audio\Sound.hpp"

AudioSystem::AudioSystem()
{
}

AudioSystem::~AudioSystem()
{
	auto soundIt = mSounds.begin();
	while (soundIt != mSounds.end())
	{
		delete soundIt->second;
		soundIt->second = nullptr;
		++soundIt;
	}
	mSounds.clear();


	auto musicIt = mMusic.begin();
	while (musicIt != mMusic.end())
	{
		delete musicIt->second;
		musicIt->second = nullptr;
		++musicIt;
	}
	mMusic.clear();
}

void AudioSystem::createSound(std::string pName, sf::SoundBuffer pBuffer)
{
	auto it = mSounds.find(pName);
	if (it != mSounds.end())
	{
		std::cout << "Failed to create sound. Sound '" << pName << "' does already exist." << std::endl;
		return;
	}

	Sound* sound = new Sound();
	sound->setVolume(100);
	sound->setBuffer(pBuffer);
	mSounds.insert(std::make_pair(pName, sound));
}

void AudioSystem::createMusic(std::string pName, std::string pFilePath)
{
	auto it = mMusic.find(pName);
	if (it != mMusic.end())
	{
		std::cout << "Failed to create music. Music '" << pName << "' does already exist." << std::endl;
		return;
	}

	Music* music = new Music();
	music->setVolume(100);
	if (!music->openFromFile(pFilePath))
	{
		delete music;
		music = nullptr;
		return;
	}

	mMusic.insert(std::make_pair(pName, music));
}

Sound* AudioSystem::getSound(std::string pName)
{
	auto it = mSounds.find(pName);
	if (it != mSounds.end())
	{
		return it->second;
	}
	return nullptr;
}

Music* AudioSystem::getMusic(std::string pName)
{
	auto it = mMusic.find(pName);

	if (it != mMusic.end())
	{
		return it->second;
	}
	return nullptr;
}

void AudioSystem::playSound(std::string pName, bool pLoop)
{
	auto it = mSounds.find(pName);
	if (it == mSounds.end())
	{
		std::cout << "Failed to play sound. Sound '" << pName << "' was not found." << std::endl;
		return;
	}

	it->second->play(pLoop);
}

void AudioSystem::playMusic(std::string pName, bool pLoop)
{
	auto it = mMusic.find(pName);
	if (it == mMusic.end())
	{
		std::cout << "Failed to play music. Music '" << pName << "' was not found." << std::endl;
		return;
	}

	it->second->setLoop(pLoop);
	it->second->play();
}

void AudioSystem::update()
{
	auto it = mSounds.begin();
	while (it != mSounds.end())
	{
		it->second->update();
		++it;
	}
}

void Sound::update()
{
	for (std::size_t i = 0; i < mInstances.size(); ++i)
	{
		if (mInstances[i].getStatus() == sf::Sound::Stopped)
		{
			mInstances.erase(mInstances.begin() + i);
			--i;
		}
	}
}

void Sound::setVolume(int volume)
{
	mVolume = volume;
}

int Sound::getVolume()
{
	return mVolume;
}

void Sound::play(bool pLoop)
{
	sf::Sound sound(mBuffer);
	sound.setVolume(mVolume);
	sound.setLoop(pLoop);
	mInstances.push_back(sound);
	mInstances.back().play();
}

sf::SoundBuffer &Sound::getBuffer()
{
	return mBuffer;
}

bool Sound::isPlaying()
{
	bool returnValue = false;
	for (std::size_t i = 0; i < mInstances.size(); i++)
	{
		if (mInstances[i].getStatus() == sf::Sound::Status::Playing)
		{
			returnValue = true;
			break;
		}
	}
	return returnValue;
}

void Sound::setBuffer(sf::SoundBuffer pBuffer)
{
	mBuffer = pBuffer;
}

Music::Music()
{

}

Music::~Music()
{

}
