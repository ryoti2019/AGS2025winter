#pragma once
#include "CreateBase.h"

class BossAppearanceCreate : public CreateBase
{

public:

	BossAppearanceCreate();

	~BossAppearanceCreate() = default;

	void Update()override;

	void Draw()override;

private:


};

