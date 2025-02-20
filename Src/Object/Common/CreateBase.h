#pragma once
class CreateBase
{

public:

	CreateBase();

	virtual ~CreateBase() = default;

	virtual void Update();

	virtual void Draw();

protected:

	// ��������
	float GenerateRandNumber();

private:

	// �G���o������͈�
	static constexpr int ENEMY_SPAWN_AREA = 1000;

};

