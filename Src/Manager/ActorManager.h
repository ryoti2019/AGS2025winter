#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "CollisionManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/BossBattleScene.h"
#include "../Scene/BossAppearanceScene.h"
#include "../Scene/GameClearScene.h"
#include "../Object/ActorBase.h"
#include "../Common/Vector2F.h"

class ActorBase;

enum class ActorType;

class ActorManager
{

public:

	ActorManager();
	//~ActorManager() = default;
	~ActorManager()
	{
		for (const auto& m : deactiveActorData_)
		{
			for (const auto& v : m.second)
			{
				int cnt = v.use_count();
			}
		}
		deactiveActorData_.clear();
		for (const auto& m : activeActorData_)
		{
			for (const auto& v : m.second)
			{
				int cnt = v.use_count();
				int q = 1;
				q = 2;
			}
		}
		activeActorData_.clear();
	}

	void Init();
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	// Actor�̔h���N���X�̎��̂̍쐬
	template <typename actor>
	void CreateActor(const json& data, const VECTOR& pos);

	// �A�N�e�B�u�ɂȂ������̂��i�[
	std::shared_ptr<ActorBase> ActiveData(const ActorType type, const VECTOR& pos);

	// ��A�N�e�B�u�ɂȂ������̂��i�[
	void DeactiveData(const std::shared_ptr<ActorBase>& actor);

	const std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>>& const GetDeActiveActorData() { return deactiveActorData_; };

	const std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>>& const GetActiveActorData() { return activeActorData_; };

private:

	// ���ׂẴA�N�^�[���܂Ƃ߂��f�[�^
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> activeActorData_;

	// ���ׂẴA�N�^�[���܂Ƃ߂��f�[�^
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> deactiveActorData_;

	// CreateActor�ŃV�[�������邩�m�F���郊�X�g
	const std::vector<std::shared_ptr<SceneBase>> scenes_ =
	{
		{
			{std::shared_ptr<GameScene>()},
			{std::shared_ptr<BossAppearanceScene>()}
		}
	};

	// ��������ԒZ���G�̃|�C���^
	std::shared_ptr<ActorBase> minEnemy_;

	// ��ԒZ��������ێ�����|�C���^
	float minDistance_;

};

template<typename T>
inline void ActorManager::CreateActor(const json& data, const VECTOR& pos)
{

	std::shared_ptr<ActorBase> actor = std::make_shared<T>(pos, data);

	// �|�C���^���g���Ƃ��̓N���b�V�����Ȃ��悤��NULL�`�F�b�N���s���悤�ɂ���
	if (!actor) return;


	auto base = SceneManager::GetInstance().GetNowScene();

	switch (SceneManager::GetInstance().GetSceneID())
	{
	case SCENE_ID::GAME:

		std::shared_ptr<CollisionManager> collisionManager = base->GetCollisionManager();
		// �Փ˔���̊Ǘ��N���X�ɓo�^
		collisionManager->Register(actor);
		break;
	default:
		break;
	}

	// �^�C�g���V�[�������邩�`�F�b�N
	std::shared_ptr<TitleScene> titleScene =
		std::dynamic_pointer_cast<TitleScene>(SceneManager::GetInstance().GetNowScene());

	// �Q�[���V�[�������邩�`�F�b�N
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// �{�X�̓o��V�[�������邩�`�F�b�N
	std::shared_ptr<BossAppearanceScene> bossAppearanceScene =
		std::dynamic_pointer_cast<BossAppearanceScene>(SceneManager::GetInstance().GetNowScene());

	// �{�X�o�g���V�[�������邩�`�F�b�N
	std::shared_ptr<BossBattleScene> bossBattleScene =
		std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

	// �Q�[���N���A�V�[�������邩�`�F�b�N
	std::shared_ptr<GameClearScene> gameClearScene =
		std::dynamic_pointer_cast<GameClearScene>(SceneManager::GetInstance().GetNowScene());

	if (!titleScene && !gameScene && !bossAppearanceScene && !bossBattleScene && !gameClearScene)return;

	if (gameScene)
	{
		std::shared_ptr<CollisionManager> collisionManager = gameScene->GetCollisionManager();
		// �Փ˔���̊Ǘ��N���X�ɓo�^
		collisionManager->Register(actor);
	}

	if (bossAppearanceScene)
	{
		std::shared_ptr<CollisionManager> collisionManager = bossAppearanceScene->GetCollisionManager();
		// �Փ˔���̊Ǘ��N���X�ɓo�^
		collisionManager->Register(actor);
	}

	if (bossBattleScene)
	{
		std::shared_ptr<CollisionManager> collisionManager = bossBattleScene->GetCollisionManager();
		// �Փ˔���̊Ǘ��N���X�ɓo�^
		collisionManager->Register(actor);
	}

	// deactiveActorData_�̒��ɂ��łɓ����^����������Ă��邩�`�F�b�N����
	auto deactorElem = deactiveActorData_.find(actor->GetActorType());

	// ��������Ă��Ȃ��ꍇ�́A�V����vector�z��̔�����肻�̒��ɗv�f�����Ă���
	if (deactorElem == deactiveActorData_.end())
	{
		int cnt = actor.use_count();
		std::vector<std::shared_ptr<ActorBase>> data;
		data.emplace_back(actor);
		deactiveActorData_.emplace(actor->GetActorType(), data);

		cnt = actor.use_count();
		int b = 2;
		b = 3;
	}
	// ��������Ă���ꍇ�͂��łɂ��锠�̒��ɗv�f�����Ă���
	else
	{
		deactorElem->second.emplace_back(actor);
	}

}