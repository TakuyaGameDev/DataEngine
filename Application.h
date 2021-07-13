#pragma once
#include <memory>
#include "Geometory.h"
#include "BaseScene.h"

#define lpApplication Application::getInstance()

class Mouse;

// ��ʻ���
struct Viewport
{
	// �S���
	Vector2I screen;
	// �ҏW���
	Vector2I editScreen;
};


class Application
{
public:
	// �ݸ���ݲݽ�ݽ
	static Application& getInstance(void)
	{
		static Application instance_;
		return instance_;
	}

	// �ް�ٰ��
	void Run(void);

	// DxLib�̏I��
	void Exit_DxLib(void);
	// �e��ʻ��ގ擾
	const Viewport& GetViewport(void)
	{
		return viewport_;
	}

private:
	Application() = default;
	~Application();
	// ��߰�֎~
	void operator=(const Application&) = delete;
	// DxLib�̏������֌W
	bool SysInit(void);
	// �e��ʻ��ސݒ�
	void SetViewport(const Viewport& viewport);
	// ��ʻ��ނ��i�[����ϐ�
	Viewport viewport_;
	// ϳ����޲��ݽ�ݽ
	std::shared_ptr<Mouse> mouse_;
	// ��ݲݽ�ݽ
	UniqueScene scene_;
};

