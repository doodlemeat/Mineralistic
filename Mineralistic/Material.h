#pragma once

#include "SFML/Graphics/Rect.hpp"
#include <string>

struct MaterialDef
{
	MaterialDef();
	MaterialDef(MaterialDef *pMaterialDef);
	void initDefaults();

	// Defaults ""
	std::string name;

	// Defaults false
	bool collidable;

	// Defaults false
	bool lumpable;

	// Defaults sf::IntRect(0, 0, 0, 0)
	sf::IntRect textureRect;

	// Defaults 1
	int resistance;

	// Defaults 0
	int upperLimitY;

	// Defaults 1
	int minLumpSize;

	// Defaults 2
	int maxLumpSize;
};

class Material
{
public:
	Material(MaterialDef *pMaterialDef);

	void setCollidable(bool pValue);
	void setLumpable(bool pValue);
	void setTextureRect(sf::IntRect pTextureRect);
	void setResistance(int pResistance);
	void setUpperLimitY(int pUpperLimitY);
	void setMinLumpSize(int pMinLumpSize);
	void setMaxLumpSize(int pMaxLumpSize);

	std::string getName();
	sf::IntRect getTextureRect();
	int getResistance();
	int getUpperLimitY();
	int getMinLumpSize();
	int getMaxLumpSize();

	bool isCollidable();
	bool isLumpable();
private:
	sf::IntRect mTextureRect;
	std::string mName;

	int mResistance;
	int mUpperLimitY;
	int mMinLumpSize;
	int mMaxLumpSize;
	
	bool mCollidable;
	bool mLumpable;
};