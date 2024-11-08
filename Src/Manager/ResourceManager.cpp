#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;

#pragma region �v���C���[�̃A�j���[�V����

	// �v���C���[�̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	// �v���C���[�̑ҋ@�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE, res);

	// �v���C���[�̑���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN, res);

	// �v���C���[�̃W���u�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Jab.mv1");
	resourcesMap_.emplace(SRC::PLAYER_JAB, res);

	// �v���C���[�̃X�g���[�g�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Straight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_STRAIGHT, res);

	// �v���C���[�̃t�b�N�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Hook.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HOOK, res);

	// �v���C���[�̍��L�b�N�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/LeftKick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_LEFT_KICK, res);

	// �v���C���[�̉E�L�b�N�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/RightKick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RIGHT_KICK, res);

	// �v���C���[�̃A�b�p�[�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Upper.mv1");
	resourcesMap_.emplace(SRC::PLAYER_UPPER, res);

	// �v���C���[�̃q�b�g�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Hit.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HIT, res);

#pragma endregion

#pragma region �G�̃A�j���[�V����

	// �G�̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Enemy.mv1");
	resourcesMap_.emplace(SRC::ENEMY , res);

	// �G�̑ҋ@�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_IDLE, res);

	// �G�̑���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Run.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RUN, res);

	// �G�̃p���`�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Punch.mv1");
	resourcesMap_.emplace(SRC::ENEMY_PUNCH, res);

	// �G�̃L�b�N�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Kick.mv1");
	resourcesMap_.emplace(SRC::ENEMY_KICK, res);

	// �G�̍U���q�b�g�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Hit.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT, res);

	// �G�̍U���q�b�g�Ŕ��ł����A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/HitFly.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT_FLY, res);

	// �G�̋N���オ��A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/KipUp.mv1");
	resourcesMap_.emplace(SRC::ENEMY_KIP_UP, res);

#pragma endregion

#pragma region �X�e�[�W

	// �G�̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, res);

#pragma endregion

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}