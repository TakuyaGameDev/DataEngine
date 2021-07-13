#pragma once
#include <memory>
// �e��݂̐e�׽�ƂȂ�

class BaseScene;
class Mouse;

using UniqueScene = std::unique_ptr<BaseScene>;

enum class Phase
{
	// 1�i�K��
    FIRST,
	// 2�i�K��
    SECOND,
	// 3�i�K��
    THIRD,
	// 4�i�K��
    FOURTH,
    MAX
};

class BaseScene
{
public:
	BaseScene();
	~BaseScene();
	// �e��݂̍X�V
	virtual UniqueScene Update(UniqueScene own,const std::shared_ptr<Mouse>& mouse) = 0;
	// �e��ϐ��̏�����
	virtual void Initialize(void) = 0;
	// ̪��ޖ��̏����̏�����
	virtual void InitializeUpdater(void) = 0;
	// ̪��ޖ��̕`�揈���̏�����
	virtual void InitializeDrawExcecuter(void) = 0;
	// �e��݂̕`��
	virtual void Draw(void) = 0;

	// ���ع���ݏI����m�点���׸ގ擾
	const bool& ExitApp(void)const
	{
		return exitAppFlag_;
	}
private:
	
protected:
	// ���ع���ݏI���׸�
	bool exitAppFlag_;
};

