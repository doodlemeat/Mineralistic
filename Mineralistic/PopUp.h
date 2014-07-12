#pragma once

/// A 2D segment of the window that's shown on
/// top of everything else.
/// It only gets drawn  when requested and when
/// it activates, takes control of the input.
///
class PopUp
{
public:
	PopUp();
	~PopUp();

	bool isActive();
	void activate();
	void deactivate();
	void toggle();

private:
	bool mActive;
};

