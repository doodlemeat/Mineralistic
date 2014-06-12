#include "B2UserData.h"
#include "Player.h"

void B2UserData::setType(int pType)
{
	mType = pType;
}

int B2UserData::getType()
{
	return mType;
}

bool B2UserData::isType(int pType)
{
	return mType == pType;
}


void UDPlayer::setData(Player* pPlayer)
{
	mPlayer = pPlayer;
}
Player* UDPlayer::getData()
{
	return mPlayer;
}