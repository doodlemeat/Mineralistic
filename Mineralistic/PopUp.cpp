#include "PopUp.h"


PopUp::PopUp():
mActive(false)
{

}

PopUp::~PopUp()
{

}

void PopUp::activate()
{
	mActive = true;
}
void PopUp::deactivate()
{
	mActive = false;
}
void PopUp::toggle()
{
	mActive = !mActive;
}

bool PopUp::isActive()
{
	return mActive;
}
