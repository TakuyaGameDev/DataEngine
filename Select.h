#pragma once
#include <array>
#include "BaseScene.h"
#include "Geometory.h"

// �e��Ӱ��
enum class MODE
{
    // �摜�ް��̕ҏWӰ��
    IMAGERESOURCE_EDITOR,
    // �����蔻���ޯ���̕ҏWӰ��
    COLLIDERBOX_EDITOR,
    MAX
};
// Ӱ�ޖ����߼޼�݂⻲�ނ��i�[���������̂�array�Ŏ���Ă���
using ModeVec = std::array<Vector2I, static_cast<int>(MODE::MAX)>;

// Ӱ�޾ڸļ��
class Select :
    public BaseScene
{
public:
    Select();
    ~Select();
    // �X�V
    UniqueScene Update(UniqueScene own,const std::shared_ptr<Mouse>& mouse);
    // ������
    void Initialize(void);
    // ̪��ޖ��̏����̏�����
    void InitializeUpdater(void);
    // ̪��ޖ��̕`�揈���̏�����
    void InitializeDrawExcecuter(void);
    // �`��
    void Draw(void);

private:
    // �ҏWӰ�ޕ\�����߼޼��
    ModeVec modePos_;
    // �ҏWӰ�ޕ\������Ă���Ƃ����ϳ��߲�����������ɕ`�悷�铧�߂��ꂽ��
    ModeVec boxSize_;
    // ���ݑI��ł���Ӱ��
    MODE nowMode_;
    // �د��������ɑI��ł���Ӱ��
    MODE decideMode_;
};

