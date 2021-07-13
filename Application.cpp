#include <DxLib.h>

#include "Application.h"
#include "Mouse.h"
#include "Select.h"

void Application::SetViewport(const Viewport& viewport)
{
	viewport_ = viewport;
}

void Application::Run(void)
{
	if (!SysInit())
	{
		return;
	}
	while (ProcessMessage() == 0 && 
		   CheckHitKey(KEY_INPUT_ESCAPE) == 0 &&
		   !scene_->ExitApp())
	{
		mouse_->Update();

		scene_ = scene_->Update(std::move(scene_), mouse_);
	}
}

void Application::Exit_DxLib(void)
{
	DxLib_End();
}

Application::~Application()
{
}

bool Application::SysInit(void)
{
	Viewport view{ {800,600},{400,300} };
	SetViewport(view);
	SetGraphMode(viewport_.screen.x, viewport_.screen.y, 32);
	ChangeWindowMode(true);
	SetWindowText("RectEngine");
	if (DxLib_Init() == 1)
	{
		return false;
	}
	// ��݂̲ݽ�ݽ
	scene_ = std::make_unique<Select>();
	// ϳ����޲��̲ݽ�ݽ
	mouse_ = std::make_shared<Mouse>();
	// ϳ��̺���޾��
	mouse_->SetUp();

	return true;
}
