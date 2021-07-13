#pragma once
#include <array>
#include <map>
#include <vector>
#include <functional>
#include "BaseScene.h"
#include "Geometory.h"

// �y����z�Ȃ̂��y�߂�z�Ȃ̂�
enum class OperationProcess
{
    // ����
    DECISION,
    // �߂�
    RETURN,
    MAX
};

using ProcessVec = std::array<Vector2I, static_cast<int>(OperationProcess::MAX)>;

// ImageData��ҏW���鼰� 
class ImageResourceDataEdit :
    public BaseScene
{
public:
    ImageResourceDataEdit();
    ~ImageResourceDataEdit();

    // �X�V
    UniqueScene Update(UniqueScene own, const std::shared_ptr<Mouse>& mouse);
    // ������
    void Initialize(void);
    // ̪��ޖ��̏����̏�����
    void InitializeUpdater(void);
    // ̪��ޖ��̕`�揈���̏�����
    void InitializeDrawExcecuter(void);
    // �`��
    void Draw(void);

    // Phase::INPUTDIRECTORY�Ŏg�p-----------------
    // �ިڸ�ؖ��̓���
    void InputDirectoryName(void);
    // ��޼ު�Ė��̓���
    void InputObjectName(void);
    // --------------------------------------------

    // Phase::DATAEDIT�Ŏg�p-----------------------
    // �ő��ڰт̓���
    void InputMaxFrame(void);
    // ٰ���׸ނ̓���(0��1��true��false�̔��f)
    void InputLoopFlag(void);
    // 1�ςɂ����鎞�Ԃ̓���
    void InputDuration(void);
private:
    // �y����z�Ɓy�߂�z�����݂��߼޼��
    ProcessVec processPos_;
    // �y����z�Ɓy�߂�z�������ޯ������
    ProcessVec boxSize_;
    // �����݂̑I������(�y����z���y�߂�z�̂ǂ��炩)
    OperationProcess nowOperation_;
    // �د����̑I������(�y����z���y�߂�z�̂ǂ��炩)
    OperationProcess excecuteProcess_;
    // ���͗����߼޼��
    Vector2I inputBoxPos_;
    // ���͗��̻���
    Vector2I inputBoxSize_;
    // �y���́z�Ƃ����������߼޼��
    Vector2I inputStringPos_;
    // �y���́z�Ƃ��������̻���
    Vector2I inputStringSize_;
    // ��Ұ��ݖ����\������Ă����߼޼��
    std::vector<Vector2I> animNamePos_;
    // �\������Ă����Ұ��ݖ��S�̻̂���(̧�ٖ���12������������12*40�ɂȂ�)
    std::vector<Vector2I> animNameSize_;
    // ���݂�̪���
    Phase nowPhase_;
    // ̪��ޖ��̏������L�q
    std::map<Phase,std::function<void(const std::shared_ptr<Mouse>& mouse)>> updater_;
    // ̪��ޖ��̕`�揈��
    std::map<Phase, std::function<void()>> drawExcecuter_;
    // �����ިڸ�ؖ�
    char inputDirectoryName_[20];
    // ���͵�޼ު�Ė�
    char inputObjectName_[20];
    // ���͂����ő��ڰѐ�
    char inputMaxFrame_[20];
    // ���͂���loopFlag(��Ұ��݂�ٰ�ߍĐ�����̂�(true)���Ȃ��̂�(false))
    char inputLoopFlag_[20];
    // ���͂���duration(1�ςɂ����鎞��)
    char inputDuration_[20];
    // ���͂����ۂɐ�������������
    int inputHandle_[4];
    // ���͒i�K���̊֐��߲��
    void (ImageResourceDataEdit::* inputFunc_)(void);
    // �y���́z�Ƃ������݂̏��ϳ��߲����������true�ɂȂ��׸�
    bool onInputDirectoryButton_;
    // �ިڸ�ؖ��̓��͉\���׸�
    bool inputDirectoryFlag_;
    // ϳ��߲���������̧�ق����ް
    int animNoOnMouse_;
    // �I������̧�ٖ����ް(�ϒ��z��ō���Ă���̂ł��̔ԍ��őI��̧�قɱ���)
    int selectAnimNo_;
    // �ް����ނ������Ƃ�\���׸�
    bool saveCmpFlag_;
};

