#pragma once

#include "SFML/Graphics/Rect.hpp"
#include <string>
#include <vector>

enum BreakBlockType
{
	PICKAXE,
	SHOVEL
};

struct MaterialDef
{
	MaterialDef();
	MaterialDef(MaterialDef *pMaterialDef);
	void initDefaults();

	std::string name;
	std::string breakSound;
	std::string breakingSound;
	bool collidable;
	bool lumpable;
	sf::IntRect textureRect;
	float resistance;
	int upperLimitY;
	int minLumpSize;
	int maxLumpSize;
	BreakBlockType breakBlockType;
	std::vector<std::string> stepSounds;
};

class Material
{
public:
	Material(MaterialDef *pMaterialDef);

	void setParticleRectIndex(const int &pRectIndex);

	const std::vector<std::string> &getStepSounds() const;
	const std::string &getName() const;
	const std::string &getBreakSound() const;
	const std::string &getBreakingSound() const;
	const sf::IntRect &getTextureRect() const;

	const BreakBlockType &getBlockBreakType() const;

	const float &getResistance() const;
	const int &getUpperLimitY() const;
	const int &getMinLumpSize() const;
	const int &getMaxLumpSize() const;
	const int &getParticleRectIndex() const;

	const bool isCollidable() const;
	const bool isLumpable() const;
private:
	sf::IntRect mTextureRect;
	std::string mName;
	std::string mBreakSound;
	std::string mBreakingSound;
	std::vector<std::string> mStepSounds;

	BreakBlockType mBlockBreakType;

	float mResistance;
	int mUpperLimitY;
	int mMinLumpSize;
	int mMaxLumpSize;
	int mParticleRectIndex;
	
	bool mCollidable;
	bool mLumpable;
};