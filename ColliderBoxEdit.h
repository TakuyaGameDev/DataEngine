#pragma once
#include <map>
#include <array>
#include <string>
#include <functional>
#include <vector>
#include "BaseScene.h"
#include "Geometory.h"

class ColliderBoxManager;

enum class CLICK
{
    RIGHT,
    LEFT,
    MAX
};

// �����蔻���`�̕ҏW���
class ColliderBoxEdit :
    public BaseScene
{
public:
    ColliderBoxEdit();
    ~ColliderBoxEdit();
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

    // ��޼ު�Ė�����͂��鏈��
    void InputObjectName(void);
    // ��Ұ��ݖ�����͂��鏈��
    void InputAnimationName(void);
    // ��Ұ��݂̺ς��w�肷�鏈��
    void SelectFrame(const CLICK& click,const Vector2I& mPoint);
    // �����蔻���`�̕ҏW
    void BoxEdit(const CLICK& click, const Vector2I& mPoint);

private:
    // ���͵�޼ު�Ė�
    char inputObjectName_[20];
    // ���ͱ�Ұ��ݖ�
    char inputAnimationName_[20];
    // ��޼ު�Ė�����͂����ޯ�����߼޼��
    Vector2I objectNameBoxPos_;
    // ��޼ު�Ė�����͂����ޯ���̻���
    Vector2I objectNameBoxSize_;
    // ��Ұ��ݖ�����͂����ޯ�����߼޼��
    Vector2I animationNameBoxPos_;
    // ��Ұ��ݖ�����͂����ޯ���̻���
    Vector2I animationNameBoxSize_;
    // �ڰѐ���\�������߼޼��
    // �ڰѐ�����size������̂ŉςɂ��Ă���
    std::vector<std::pair<Vector2I,int>> frameNumBox_;
    // �ڰт�\�������ޯ���̻���
    Vector2I frameNumBoxSize_;
    // ��`���ߕύX���s�����݂��߼޼��
    std::array<Vector2I, 2> typeChangerPos_;
    // ��`���ߕύX���s�����݂̻���
    Vector2I typeChangerBoxSize_;
    // �������݂��߼޼��
    Vector2I saveButtonPos_;
    // �������݂̻���
    Vector2I saveButtonSize_;
    // �ް��������������̉摜�ް��ҏW�Ɉڍs�������݂��߼޼��
    Vector2I goImageDataButtonPos_;
    // �摜�ް��ҏW�Ɉڍs�������݂̻���
    Vector2I goImageDataButtonSize_;
    // ���ع���݂��I�����������݂��߼޼��
    Vector2I exitAppButtonPos_;
    // ���ع���݂��I�����������݂̻���
    Vector2I exitAppButtonSize_;
    // �ڸĉ�ʂɖ߂����݂̏��ϳ��߲���������׸�
    bool isOnReturn_;
    // ���ع���݂��I�����������݂̏��ϳ��߲���������׸�
    bool isOnExitApp_;

    // �ҏW�Ώۉ摜���߼޼��
    Vector2I objImagePos_;

    // ϳ��߲��������Ă����ڰ�
    int onPointFrame_;
    // ����(���د�)����ڰѐ�
    int selectedFrame_;
    // ���݂̏����i�K
    Phase nowPhase_;
    // ���E�د��ϐ�
    CLICK clickDir_;
    // ϳ��߲��
    Vector2I mPoint_;
    // ������݂�\���֐��߲��
    void(ColliderBoxEdit::* inputFunc_)(void);
    // �ҏW������\���֐��߲��
    void(ColliderBoxEdit::* editFunc_)(const CLICK& click, const Vector2I& mPoint);
    // ��݂̒��ł̏�ԑJ�ڊ֐���std::function�ł܂Ƃ߂�
    std::map<Phase, std::function<void(const std::shared_ptr<Mouse>& mouse)>> updater_;
    // �`��J�ڊ֐���std::function�ł܂Ƃ߂�
    std::map<Phase, std::function<void(void)>> drawExcecuter_;
    // ̧�ق����������ۂɂ�true
    bool isFindFile_;

    // �����蔻���ޯ���̐�������S��Manager
    ColliderBoxManager* boxManager_;

    // �ޯ�����ߖ��̐F
    unsigned int typeColor_;
    // �ޯ������
    bool typeFlag_;
    // ���ނ����׸�
    std::vector<bool> saveFlag_;
    // �ēxؽ��Ă���ۂ��׸ޓ���ؾ��
    void ResetData(void);
};

