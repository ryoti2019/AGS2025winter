#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "CollisionManager.h"
#include "../Scene/GameScene.h"
#include "../Object/ActorBase.h"
#include "../Common/Vector2F.h"

class ActorBase;

enum class ActorType;

class ActorManager
{

public:

	ActorManager();
	~ActorManager() = default;

	void Init();
	void Update(const float deltaTime);
	void Draw();

	// Actor�̔h���N���X�̎��̂̍쐬
	template <typename actor>
	void CreateActor(const json& data);

	// �A�N�e�B�u�ɂȂ������̂��i�[
	std::shared_ptr<ActorBase> ActiveData(const ActorType type, const VECTOR& pos);

	// ��A�N�e�B�u�ɂȂ������̂��i�[
	void DeactiveData(const std::shared_ptr<ActorBase>& actor);

	const auto& const GetActiveActorData() { return activeActorData_; };

private:

	// ���ׂẴA�N�^�[���܂Ƃ߂��f�[�^
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> activeActorData_;

	// ���ׂẴA�N�^�[���܂Ƃ߂��f�[�^
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> deactiveActorData_;
	
	// ��������ԒZ���G�̃|�C���^
	std::shared_ptr<ActorBase> minEnemy_;

	// ��ԒZ��������ێ�����|�C���^
	float minDistance_;

};

template<typename T>
inline void ActorManager::CreateActor(const json& data)
{

	const VECTOR pos = { 0.0f,0.0f,0.0f };
	std::shared_ptr<ActorBase> actor = std::make_shared<T>(pos, data);

	// �|�C���^���g���Ƃ��̓N���b�V�����Ȃ��悤��NULL�`�F�b�N���s���悤�ɂ���
	if (!actor) return;

	//actor->Init(pos);

	auto a = SceneManager::GetInstance().GetNowScene();
	// ���N���X����g�������^�փL���X�g����
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(a);

	// NULL�`�F�b�N
	if (!gameScene) return;

	// �R���W�����}�l�[�W���[���擾
	std::shared_ptr<CollisionManager> collisionManager = gameScene->GetCollisionManager();

	// �Փ˔���̊Ǘ��N���X�ɓo�^
	collisionManager->Register(actor);

	// deactiveActorData_�̒��ɂ��łɓ����^����������Ă��邩�`�F�b�N����
	auto deactorElem = deactiveActorData_.find(actor->GetActorType());

	// ��������Ă��Ȃ��ꍇ�́A�V����vector�z��̔�����肻�̒��ɗv�f�����Ă���
	if (deactorElem == deactiveActorData_.end())
	{
		std::vector<std::shared_ptr<ActorBase>> data;
		data.emplace_back(actor);
		deactiveActorData_.emplace(actor->GetActorType(), data);
	}
	// ��������Ă���ꍇ�͂��łɂ��锠�̒��ɗv�f�����Ă���
	else
	{
		deactorElem->second.emplace_back(actor);
	}

}