#include <DxLib.h>

#include "Select.h"
#include "Mouse.h"
#include "Application.h"

#include "ImageManager.h"
#include "ImageResourceDataEdit.h"
#include "ColliderBoxEdit.h"

#include "HitCheck.h"

Select::Select()
{
	// ���ع���ݏI���׸ނ̏�����
	exitAppFlag_ = false;
	// �e��ϐ��̏�����
	Initialize();
}

Select::~Select()
{
}

UniqueScene Select::Update(UniqueScene own,const std::shared_ptr<Mouse>& mouse)
{
	// �����ް���Mouse�׽���擾
	auto clickData = mouse->GetClick();
	// ���ݑI�𒆂�Ӱ�ނ�MAX�ɂ���
	// ϳ��߲��������̏ꏊ�ɍs���Ƃ��̕ϐ����ύX�����
	nowMode_ = MODE::MAX;
	// ϳ��߲�����߼޼��
	auto mPoint = mouse->GetPos();

	// �����蔻��-------------------------------------------------------------------
	// �摜�̒��S����̌v�Z
	if (HitCheck_FromCenter()(modePos_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)],
		mPoint, boxSize_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)]))
	{
		// ���݂�Ӱ�ނ��摜�ް��ҏWӰ�ނɕύX
		nowMode_ = MODE::IMAGERESOURCE_EDITOR;
	}

	if (HitCheck_FromCenter()(modePos_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)],
		mPoint, boxSize_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)]))
	{
		// ���݂�Ӱ�ނ𓖂��蔻���ޯ���ҏWӰ�ނɕύX
		nowMode_ = MODE::COLLIDERBOX_EDITOR;
	}
	// -----------------------------------------------------------------------------

	for (auto click : clickData)
	{
		// �د��ضް�L��
		if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
		{
			// �J�ڂ�����Ӱ�ނ����肷��
			decideMode_ = nowMode_;
		}
		// �ضް����
		if (click.second[static_cast<int>(TRG::NOW)])
		{
			// �J�ڂ�����Ӱ�ނ����肷��
			decideMode_ = nowMode_;
		}
	}
	// �摜�ް��ҏW�Ɉڍs
	if (decideMode_ == MODE::IMAGERESOURCE_EDITOR)
	{
		nowMode_ = MODE::MAX;
		decideMode_ = nowMode_;
		return std::make_unique<ImageResourceDataEdit>();
	}
	// �����蔻���ޯ���̕ҏW�Ɉڍs
	if (decideMode_ == MODE::COLLIDERBOX_EDITOR)
	{
		nowMode_ = MODE::MAX;
		decideMode_ = nowMode_;
		return std::make_unique<ColliderBoxEdit>();
	}

	ClsDrawScreen();
	// �`��
	Draw();
	ScreenFlip();
	return move(own);
}

void Select::Initialize(void)
{
	// �e��ʻ��ނ̎擾
	auto tmpPort = lpApplication.GetViewport();
	// �ڍs������Ӱ�����݂̕�����
	modePos_ = { Vector2I{tmpPort.screen.x / 2,tmpPort.screen.y / 2},
				 Vector2I{tmpPort.screen.x / 2,tmpPort.screen.y / 2 + 80} };
	// ���݂̻���
	boxSize_ = { Vector2I{430,60},Vector2I{520,60} };
	// ���݂�Ӱ��(ϳ��߲�������Ă���Ӱ��)
	nowMode_ = MODE::MAX;
	// ���莞(���د���)��Ӱ��
	decideMode_ = nowMode_;
	lpImageManager.Load("Images/modeTitle_ImageData.png");
	lpImageManager.Load("Images/modeTitle_Collider.png");
}

void Select::InitializeUpdater(void)
{
	// ���ɏ������邱�Ɩ���
}

void Select::InitializeDrawExcecuter(void)
{
	// ���ɏ������邱�ƂȂ�
}

void Select::Draw(void)
{
	// �ڍs�������eӰ�ނ����݂̕`��--------------------------------------------
	DrawRotaGraph(modePos_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)].x,
		modePos_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)].y, 1.0f, 0.0f,
		lpImageManager.GetHandle("Images/modeTitle_ImageData.png")[0],true);
	DrawRotaGraph(modePos_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)].x,
		modePos_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)].y, 1.0f, 0.0f,
		lpImageManager.GetHandle("Images/modeTitle_Collider.png")[0],true);
	// ------------------------------------------------------------------------

	// ϳ��߲��������̏ꏊ�ɗ����瓧�߂��ꂽ����`��
	if (nowMode_ != MODE::MAX)
	{
		// ���߂����邽�߂ɱ�̧�����ނ���(����������������`�悳����������150�Ƃ����l)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(modePos_[static_cast<int>(nowMode_)].x - (boxSize_[static_cast<int>(nowMode_)].x / 2),
			modePos_[static_cast<int>(nowMode_)].y - (boxSize_[static_cast<int>(nowMode_)].y / 2),
			modePos_[static_cast<int>(nowMode_)].x + (boxSize_[static_cast<int>(nowMode_)].x / 2),
			modePos_[static_cast<int>(nowMode_)].y + (boxSize_[static_cast<int>(nowMode_)].y / 2), 0xff0000, true);
		// ��̧�����ނ𔠂����ɓK�����������̂ŁA���̕`��̍Ō��0��ؾ�Ă�������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
