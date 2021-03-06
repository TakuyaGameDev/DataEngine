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
	// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ更新
	GetMousePoint(&pos_.x, &pos_.y);
	for (auto id : INPUT_ID())
	{
		inputData_[id][static_cast<int>(TRG::OLD)] = 
			inputData_[id][static_cast<int>(TRG::NOW)];
		// 入力が有れば
		if (commandTable_[static_cast<int>(id)] & GetMouseInput())
		{
			inputData_[id][static_cast<int>(TRG::NOW)] = true;
		}
		// 入力が無ければ
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
