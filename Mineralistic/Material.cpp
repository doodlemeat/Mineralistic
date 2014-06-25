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
	breakSound = pMaterialDef->breakSound;
	breakBlockType = pMaterialDef->breakBlockType;
	breakingSound = pMaterialDef->breakingSound;
	stepSounds = pMaterialDef->stepSounds;
}

void MaterialDef::initDefaults()
{
	name = "";
	breakSound = "";
	breakingSound = "";
	resistance = 1;
	textureRect = sf::IntRect(0, 0, 0, 0);
	upperLimitY = 0;
	lumpable = false;
	maxLumpSize = 2;
	minLumpSize = 1;
	collidable = false;
	breakBlockType = PICKAXE;
}

Material::Material(MaterialDef *pMaterialDef)
{
	mName = pMaterialDef->name;
	mBreakSound = pMaterialDef->breakSound;
	mLumpable = pMaterialDef->lumpable;
	mCollidable = pMaterialDef->collidable;
	mTextureRect = pMaterialDef->textureRect;
	mResistance = pMaterialDef->resistance;
	mUpperLimitY = pMaterialDef->upperLimitY;
	mMinLumpSize = pMaterialDef->minLumpSize;
	mMaxLumpSize = pMaterialDef->maxLumpSize;
	mBlockBreakType = pMaterialDef->breakBlockType;
	mBreakingSound = pMaterialDef->breakingSound;
	mStepSounds = pMaterialDef->stepSounds;
}

const bool Material::isCollidable() const
{
	return mCollidable;
}

const std::string &Material::getName() const
{
	return mName;
}

const sf::IntRect &Material::getTextureRect() const
{
	return mTextureRect;
}

const float &Material::getResistance() const
{
	return mResistance;
}

const bool Material::isLumpable() const
{
	return mLumpable;
}

const int &Material::getUpperLimitY() const
{
	return mUpperLimitY;
}

const int &Material::getMinLumpSize() const
{
	return mMinLumpSize;
}

const int &Material::getMaxLumpSize() const
{
	return mMaxLumpSize;
}

const std::string &Material::getBreakSound() const
{
	return mBreakSound;
}

const BreakBlockType &Material::getBlockBreakType() const
{
	return mBlockBreakType;
}

const std::string & Material::getBreakingSound() const
{
	return mBreakingSound;
}

const std::vector<std::string> & Material::getStepSounds() const
{
	return mStepSounds;
}
