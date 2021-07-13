#include <DxLib.h>
#include "Mouse.h"

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

void Mouse::Update(void)
{
	// Ï³½Îß²İÀ‚ÌÎß¼Ş¼®İXV
	GetMousePoint(&pos_.x, &pos_.y);
	for (auto id : INPUT_ID())
	{
		inputData_[id][static_cast<int>(TRG::OLD)] = 
			inputData_[id][static_cast<int>(TRG::NOW)];
		// “ü—Í‚ª—L‚ê‚Î
		if (commandTable_[static_cast<int>(id)] & GetMouseInput())
		{
			inputData_[id][static_cast<int>(TRG::NOW)] = true;
		}
		// “ü—Í‚ª–³‚¯‚ê‚Î
		else
		{
			inputData_[id][static_cast<int>(TRG::NOW)] = false;
		}
	}
}

void Mouse::SetUp(void)
{
	commandTable_ = {
		MOUSE_INPUT_LEFT,
		MOUSE_INPUT_RIGHT
	};
}
