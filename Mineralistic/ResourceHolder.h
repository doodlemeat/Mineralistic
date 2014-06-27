#pragma once

#include <string>
#include <Thor\Resources.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Audio\SoundBuffer.hpp>

class ResourceHolder
{
public:
	ResourceHolder();

	sf::Texture& getTexture(const std::string& pFilename);
	sf::Font& getFont(const std::string& pFilename);
	sf::SoundBuffer& getSound(const std::string& pFilename);
	const std::string & getTexturePath() const;
	const std::string & getFontPath() const;
	const std::string & getSoundPath() const;
private:
	std::string mTexturePath;
	std::string mFontPath;
	std::string mSoundPath;

	thor::MultiResourceCache m_cache;
};


