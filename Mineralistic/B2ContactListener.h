#pragma once
#include "Box2D\Dynamics\b2WorldCallbacks.h"
#include "Box2D\Dynamics\Contacts\b2Contact.h"

class B2ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};