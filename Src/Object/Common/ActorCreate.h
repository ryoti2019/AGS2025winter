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

	// エリア2と衝突しているか
	bool isCollisionArea2_;

};

