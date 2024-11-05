#include "ActorManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "../Utility/Utility.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"

ActorManager::ActorManager()
	:
	minEnemy_(nullptr),
	minDistance_(10000.0f)
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

	//// �G�����b�N�I�����Ă�����̂����邩�m�F���鏈��
	//for (const auto& data : activeActorData_)
	//{
	//	for (const auto& actor : data.second)
	//	{

	//		// �G
	//		const auto& enemys = activeActorData_.find(ActorType::ENEMY);

	//		// ���g�������Ă��邩�m�F
	//		if (enemys == activeActorData_.end())continue;

	//		for (const std::shared_ptr<ActorBase>& enemy : enemys->second)
	//		{
	//			// �G�̒���1�ł�true�������珈�����Ȃ�
	//			if (enemy->GetIsLockOn())return;
	//		}
	//	}
	//}

	//// �G�����b�N�I�����邽�߂̏���
	//for (const auto& data : activeActorData_)
	//{
	//	for (const auto& actor : data.second)
	//	{

	//		// �v���C���[
	//		const auto& players = activeActorData_.find(ActorType::PLAYER);

	//		// �G
	//		const auto& enemys = activeActorData_.find(ActorType::ENEMY);

	//		// ���g�������Ă��邩�m�F
	//		if (players == activeActorData_.end())continue;
	//		if (enemys == activeActorData_.end())continue;

	//		for (const std::shared_ptr<ActorBase>& player : players->second)
	//		{
	//			for (const std::shared_ptr<ActorBase>& enemy : enemys->second)
	//			{

	//				// �|�C���^�������Ă��邩�m�F
	//				if (!player)return;
	//				if (!enemy)return;

	//				// �v���C���[����G�Ɍ����Ẵx�N�g��
	//				const VECTOR& vec = VSub(enemy->GetPos(), player->GetPos());

	//				// �x�N�g���̋���
	//				const float distance = Utility::MagnitudeF(vec);

	//				// ��ԏ����������Ɣ�ׂč���r���Ă���ق��������������獡�̂���ԏ������l�ɐݒ肷��
	//				if (distance > minDistance_)continue;

	//				minDistance_ = distance;
	//				minEnemy_ = enemy;
	//				

	//				// ���̓G�����b�N�I������
	//				minEnemy_->SetIsLockOn(true);

	//				// �J�����ɂ��̓G��Ǐ]�����邽��Transform��n��
	//				SceneManager::GetInstance().GetCamera().lock()->SetEnemy(&minEnemy_->GetTransform());

	//				// �J�����Ƀ��b�N�I���@�\��ON�ɂ���
 //					SceneManager::GetInstance().GetCamera().lock()->SetLockOn(true);

	//				// ���܂����狗��������������
	//				minDistance_ = 10000.0f;
	//				return;

	//			}
	//		}
	//	}
	//}

}

void ActorManager::Draw()
{

	// activeActorData_�̕`��
	for (auto& data : activeActorData_)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			actor->Draw();
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

	// deactiveActorData_�̐擪�������폜
	deactiveActorData_[type].erase(deactiveActorData_[type].begin());

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