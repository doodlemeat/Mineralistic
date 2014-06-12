#include "Material.h"




Material::Material(std::string pName, bool pCollidable, sf::IntRect pTextureRect)
{
	mName = pName;
	mCollidable = pCollidable;
	mTextureRect = pTextureRect;
}

Material::~Material()
{

}

void Material::setCollidable(bool pValue)
{
	mCollidable = pValue;
}

void Material::setTextureRect(sf::IntRect pTextureRect)
{
	mTextureRect = pTextureRect;
}

bool Material::isCollidable()
{
	return mCollidable;
}

std::string Material::getName()
{
	return mName;
}

sf::IntRect Material::getTextureRect()
{
	return mTextureRect;
}
