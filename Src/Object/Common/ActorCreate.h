#pragma once
class ActorCreate
{

public:

	ActorCreate();

	~ActorCreate() = default;

	void Update();

	void Draw();

private:

	// エリア1と衝突しているか
	bool isCollisionArea1_;

};

