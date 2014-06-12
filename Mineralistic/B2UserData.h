#pragma once

class Player;

class B2UserData
{
public:
	B2UserData() {}
	virtual ~B2UserData() {}
	
	void setType(int pType);
	int getType();
	bool isType(int pType);

private:
	int mType;
};


enum B2UDType
{
	PLAYER
};

// Define all user data classes below.

class UDPlayer: public B2UserData
{
public:
	UDPlayer() {}
	~UDPlayer() {}
	void setData(Player* pPlayer);
	Player* getData();

private:
	Player* mPlayer;
};