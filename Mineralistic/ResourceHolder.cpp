#include "ResourceHolder.h"

ResourceHolder::ResourceHolder()
{
	m_cache.setReleaseStrategy(thor::Resources::ExplicitRelease);
	m_cache.setLoadingFailureStrategy(thor::Resources::ReturnNullPointer);

	mTexturePath = "../assets/textures/";
	mFontPath = "../assets/fonts/";
	mSoundPath = "../assets/sounds/";
}

sf::Texture &ResourceHolder::getTexture(const std::string& pFilename)
{
	return *m_cache.acquire(thor::Resources::fromFile<sf::Texture>(mTexturePath + pFilename));
}
sf::Font &ResourceHolder::getFont(const std::string& pFilename)
{
	return *m_cache.acquire(thor::Resources::fromFile<sf::Font>(mFontPath + pFilename));
}
sf::SoundBuffer &ResourceHolder::getSound(const std::string& pFilename)
{
	return *m_cache.acquire(thor::Resources::fromFile<sf::SoundBuffer>(mSoundPath + pFilename));
}

