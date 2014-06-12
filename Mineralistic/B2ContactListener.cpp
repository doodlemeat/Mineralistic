#include "B2ContactListener.h"
#include "B2UserData.h"
#include "Player.h"
#include <iostream>

void B2ContactListener::BeginContact(b2Contact* contact)
{
	if (contact->GetFixtureA()->GetUserData() != nullptr)
	{
		B2UserData *fixture = static_cast<B2UserData*>(contact->GetFixtureA()->GetUserData());
		if (fixture->isType(B2UDType::PLAYER))
		{
			UDPlayer *player = static_cast<UDPlayer*>(fixture);
			player->getData()->increaseFootContacts();
		}
	}
	
	if (contact->GetFixtureB()->GetUserData() != nullptr)
	{
		B2UserData *fixture = static_cast<B2UserData*>(contact->GetFixtureB()->GetUserData());
		if (fixture->isType(B2UDType::PLAYER))
		{
			UDPlayer *player = static_cast<UDPlayer*>(fixture);
			player->getData()->increaseFootContacts();
		}
	}
}

void B2ContactListener::EndContact(b2Contact* contact)
{
	if (contact->GetFixtureA()->GetUserData() != nullptr)
	{
		B2UserData *fixture = static_cast<B2UserData*>(contact->GetFixtureA()->GetUserData());
		if (fixture->isType(B2UDType::PLAYER))
		{
			UDPlayer *player = static_cast<UDPlayer*>(fixture);
			player->getData()->decreaseFootContacts();
		}
	}

	if (contact->GetFixtureB()->GetUserData() != nullptr)
	{
		B2UserData *fixture = static_cast<B2UserData*>(contact->GetFixtureB()->GetUserData());
		if (fixture->isType(B2UDType::PLAYER))
		{
			UDPlayer *player = static_cast<UDPlayer*>(fixture);
			player->getData()->decreaseFootContacts();
		}
	}
}
