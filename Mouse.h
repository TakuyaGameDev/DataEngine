#pragma once
#include <map>
#include <array>

#include "Geometory.h"

// ����ID
enum class INPUT_ID
{
	// ����
	START,
	// ؾ��
	RESET,
	MAX
};

// INPUT_ID��for���ŉ񂷂��߂̏���
// INPUT_ID��begin
static INPUT_ID begin(INPUT_ID)
{
	return INPUT_ID::START;
}
// INPUT_ID��end
static INPUT_ID end(INPUT_ID)
{
	return INPUT_ID::MAX;
}
// INPUT_ID�̲ݸ����
static INPUT_ID operator++(INPUT_ID& id)
{
	return (id =
		INPUT_ID(std::underlying_type<INPUT_ID>::type(id) + 1));
}
// INPUT_ID���߲���Ԃ�
static INPUT_ID& operator*(INPUT_ID& id)
{
	return id;
}

// �د������ضް�ɂĎg�p
// ���د������̂�1�ڰёO�ɸد������̂�
enum class TRG
{
	// �y���z��\��
	NOW,
	// �y1�ڰёO�z��\��
	OLD,
	MAX
};
// �y���z�Ɓy1�ڰёO�z�Ŕz��m�ۂ��A
// �y���z�د����Ă����׸ށA�y1�ڰёO�z�ɸد������׸ނ�ێ�
using TrgBool = std::array<bool, static_cast<int>(TRG::MAX)>;
// �y���z�د����Ă�����̂��E�د��Ȃ̂����د��Ȃ̂�
// ���ʂ����邽�߂�std::map�ŘA�z�z��ɂ��Ă���
using TrgMap = std::map<INPUT_ID, TrgBool>;

// �������޲�(ϳ�)�׽
class Mouse
{
public:
	Mouse();
	~Mouse();
	// �X�V
	void Update(void);
	// ϳ�����ނ̾�ı���
	void SetUp(void);
	// �����ް��̎擾
	const TrgMap& GetClick(void)
	{
		return inputData_;
	}
	// ϳ��߲�����߼޼�ݎ擾
	const Vector2I& GetPos(void)
	{
		return pos_;
	}

private:
	// �����ް�
	TrgMap inputData_;
	// �����ð���
	std::array<int, static_cast<int>(INPUT_ID::MAX)> commandTable_;
	// ϳ��߲�����߼޼��
	Vector2I pos_;
};

