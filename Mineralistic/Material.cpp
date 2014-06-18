#include "Material.h"

MaterialDef::MaterialDef()
{
	initDefaults();
}

MaterialDef::MaterialDef(MaterialDef *pMaterialDef)
{
	name = pMaterialDef->name;
	resistance = pMaterialDef->resistance;
	textureRect = pMaterialDef->textureRect;
	upperLimitY = pMaterialDef->upperLimitY;
	lumpable = pMaterialDef->lumpable;
	maxLumpSize = pMaterialDef->maxLumpSize;
	minLumpSize = pMaterialDef->minLumpSize;
	collidable = pMaterialDef->collidable;
}

void MaterialDef::initDefaults()
{
	name = "";
	resistance = 1;
	textureRect = sf::IntRect(0, 0, 0, 0);
	upperLimitY = 0;
	lumpable = false;
	maxLumpSize = 2;
	minLumpSize = 1;
	collidable = false;
}

Material::Material(MaterialDef *pMaterialDef)
{
	mName = pMaterialDef->name;
	mLumpable = pMaterialDef->lumpable;
	mCollidable = pMaterialDef->collidable;
	mTextureRect = pMaterialDef->textureRect;
	mResistance = pMaterialDef->resistance;
	mUpperLimitY = pMaterialDef->upperLimitY;
	mMinLumpSize = pMaterialDef->minLumpSize;
	mMaxLumpSize = pMaterialDef->maxLumpSize;
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

void Material::setResistance(int pResistance)
{
	mResistance = pResistance;
}

int Material::getResistance()
{
	return mResistance;
}

void Material::setLumpable(bool pValue)
{
	mLumpable = pValue;
}

bool Material::isLumpable()
{
	return mLumpable;
}

void Material::setUpperLimitY(int pUpperLimitY)
{
	mUpperLimitY = pUpperLimitY;
}

void Material::setMinLumpSize(int pMinLumpSize)
{
	mMinLumpSize = pMinLumpSize;
}

void Material::setMaxLumpSize(int pMaxLumpSize)
{
	mMaxLumpSize = pMaxLumpSize;
}

int Material::getUpperLimitY()
{
	return mUpperLimitY;
}

int Material::getMinLumpSize()
{
	return mMinLumpSize;
}

int Material::getMaxLumpSize()
{
	return mMaxLumpSize;
}
