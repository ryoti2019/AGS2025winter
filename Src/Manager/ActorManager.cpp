#include "ActorManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "../Utility/Utility.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"

ActorManager::ActorManager()
{
}

void ActorManager::Init()
{
}

void ActorManager::Update(const float deltaTime)
{

	// activeActorData_�ɔ�A�N�e�B�u��Ԃ̂��̂������
	// deactiveActorData_�Ɋi�[����
	int num = 0;
	for (auto& data : activeActorData_)
	{
		num = 0;
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			if (actor && !actor->GetIsActive())
			{

				// ��A�N�e�B�u�ɂȂ������̂��i�[
				DeactiveData(actor);

				// ��A�N�e�B�u�ɂȂ������̂��폜
				data.second.erase(data.second.begin() += num);

			}
			num++;
		}
	}

	// activeActorData_�̍X�V
	for (const auto& data : activeActorData_)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			actor->Update(deltaTime);
		}
	}

}

void ActorManager::Draw(const float deltaTime)
{

	// activeActorData_�̕`��
	for (auto& data : activeActorData_)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			actor->Draw(deltaTime);
		}
	}

}

std::shared_ptr<ActorBase> ActorManager::ActiveData(const ActorType type, const VECTOR& pos)
{

	// ���g��0�̏ꍇ�́ANULL��Ԃ�
	if (deactiveActorData_[type].size() == 0)return nullptr;

	// deactiveActorData_�̒��ɂ��łɓ����^����������Ă��邩�`�F�b�N����
	auto deactorElem = deactiveActorData_.find(type);

	// ��������Ă��Ȃ��ꍇ�́ANULL��Ԃ�
	if (deactorElem == deactiveActorData_.end()) return nullptr;

	// deactiveActorData_�̐擪�������폜����activeActorData_�Ɋi�[����

	// deactiveActorData_�̐擪����
	std::shared_ptr<ActorBase> active = deactiveActorData_[type].front();

	auto base = SceneManager::GetInstance().GetNowScene();

	// �Փ˔���̊Ǘ��N���X
	std::shared_ptr<CollisionManager> collisionManager;

	switch (SceneManager::GetInstance().GetSceneID())
	{
	case SCENE_ID::GAME:
		collisionManager = base->GetCollisionManager();
		// �Փ˔���̊Ǘ��N���X�ɓo�^
		collisionManager->Register(active);
		break;
	case SCENE_ID::BOSS_APPEARANCE:
		collisionManager = base->GetCollisionManager();
		// �Փ˔���̊Ǘ��N���X�ɓo�^
		collisionManager->Register(active);
		break;
	case SCENE_ID::BOSS_BATTLE:
		collisionManager = base->GetCollisionManager();
		// �Փ˔���̊Ǘ��N���X�ɓo�^
		collisionManager->Register(active);
		break;
	}
	
	// �A�N�e�B�u��Ԃɂ���
	active->Init(pos);
	active->SetIsActive(true);
	// activeActorData_�Ɋi�[
	auto actorElem = activeActorData_.find(type);

	if (actorElem == activeActorData_.end())
	{
		std::vector<std::shared_ptr<ActorBase>> data;
		data.emplace_back(active);
		activeActorData_.emplace(type, data);
	}
	// ��������Ă���ꍇ�͂��łɂ��锠�̒��ɗv�f�����Ă���
	else
	{
		actorElem->second.emplace_back(active);
	}

	// deactiveActorData_�̐擪�������폜
	deactiveActorData_[type].erase(deactiveActorData_[type].begin());

	// �A�N�e�B�u��ԂɂȂ������̂�Ԃ�
	return active;

}

void ActorManager::DeactiveData(const std::shared_ptr<ActorBase>& actor)
{

	ActorType type = actor->GetActorType();

	// activeActorData_�̒��ɂ��łɓ����^����������Ă��邩�`�F�b�N����
	auto actorElem = activeActorData_.find(type);

	// ��������Ă��Ȃ��ꍇ�́ANULL��Ԃ�
	if (actorElem == activeActorData_.end()) return;

	// activeActorData_�̐擪�������폜����deactiveActorData_�Ɋi�[����

	// activeActorData_�Ɋi�[
	deactiveActorData_[type].emplace_back(actor);

}