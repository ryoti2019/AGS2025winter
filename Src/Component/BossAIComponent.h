#pragma once
#include "Component.h"

class Boss;

class BossAIComponent : public Component
{

public:

	// �s�����
	enum class ACTION_TYPE
	{
		MOVE,
		CLOSE_RANGE,
		LONG_RANGE,
		CALL,
		MAX
	};

	// �ߐڍU�����
	enum class CLOSE_RANGE_TYPE
	{
		PUNCH,
		KICK,
		MAX
	};

	// ��]�̋��e�덷
	static constexpr float ANGLE_TOLERANCE = 5.0f;

	BossAIComponent(const std::shared_ptr<Boss> boss);

	void Update(const float deltaTime);

private:

	// �G�̃|�C���^�[
	std::shared_ptr<Boss> boss_;

	// �A�N�V������I��
	void SelectAction(const float deltaTime);

	// �ړ�����
	void Move(const float deltaTime);

	// �ߋ����U������
	void CloseRangeAttack(const float deltaTime);

	// �������U������
	void LongRangeAttack(const float deltaTime);

};

