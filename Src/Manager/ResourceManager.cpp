#include <fstream>
#include <DxLib.h>
#include "../Lib/nlohmann/json.hpp"
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

	// �O���t�@�C���̓ǂݍ���
	std::ifstream ifs;

	ifs.open(Application::PATH_JSON + "Resource.json");

	if (!ifs)
	{
		// �O���t�@�C���̓ǂݍ��ݎ��s
		return;
	}

	// �t�@�C���X�g���[������json�I�u�W�F�N�g�ɕϊ�
	nlohmann::json objectJson = nlohmann::json::parse(ifs);

	const auto& objectData = objectJson["ResourceData"];

	// �X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	// ���s�V�ǂ��A�����I�Ƀt�@�C���X�g���[�������
	ifs.close();

	// �v���C���[
	const auto& playerData = objectData[0]["ModelData"]["Player"];

	// �v���C���[�̃��f���ƃA�j���[�V�����̏�����
	InitResource(playerData);

	// �v���C���[
	const auto& playerData = objectData[0]["ModelData"]["Player"];

	// �v���C���[�̃��f���ƃA�j���[�V�����̏�����
	InitResource(playerData);

	// �v���C���[
	const auto& playerData = objectData[0]["ModelData"]["Player"];

	// �v���C���[�̃��f���ƃA�j���[�V�����̏�����
	InitResource(playerData);

	// �v���C���[
	const auto& playerData = objectData[0]["ModelData"]["Player"];

	// �v���C���[�̃��f���ƃA�j���[�V�����̏�����
	InitResource(playerData);

//#pragma region �G�̃A�j���[�V����
//
//	// �G�̃��f��
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Enemy.mv1");
//	resourcesMap_.emplace(SRC::ENEMY , res);
//
//	// �G�̑ҋ@�A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Idle.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_IDLE, res);
//
//	// �G�̑���A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Run.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_RUN, res);
//
//	// �G�̃p���`�A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Punch.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_PUNCH, res);
//
//	// �G�̃L�b�N�A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Kick.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_KICK, res);
//
//	// �G�̍U�������ɓ����������̃A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/HitHead.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_HIT_HEAD, res);
//
//	// �G�̍U�����̂ɓ����������̃A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/HitBody.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_HIT_BODY, res);
//
//	// �G�̍U��������������������ɔ��ł����A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/HitFly.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_HIT_FLY, res);
//
//	// �G�̍U���������������ɋ��ރA�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/FlinchUp.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_HIT_FLINCH, res);
//
//	// �G�̍U���������������ɂ܂��������ł����A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/KnockBack.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_HIT_KNOCK_BACK, res);
//
//	// �G�̋N���オ��A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/KipUp.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_KIP_UP, res);
//
//	// �G�̃m�[�}�����S�A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Death.mv1");
//	resourcesMap_.emplace(SRC::ENEMY_NORMAL_DEATH, res);
//
//#pragma endregion
//
//#pragma region �{�X�̃A�j���[�V����
//
//	// �{�X�̃��f��
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Boss.mv1");
//	resourcesMap_.emplace(SRC::BOSS, res);
//
//	// �{�X�̑ҋ@�A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Idle.mv1");
//	resourcesMap_.emplace(SRC::BOSS_IDLE, res);
//
//	// �{�X�̑���A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Run.mv1");
//	resourcesMap_.emplace(SRC::BOSS_RUN, res);
//
//	// �{�X�̃p���`�A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Punch.mv1");
//	resourcesMap_.emplace(SRC::BOSS_PUNCH, res);
//
//	// �{�X�̃L�b�N�A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Kick.mv1");
//	resourcesMap_.emplace(SRC::BOSS_KICK, res);
//
//	// �{�X�̃\�j�b�N�u�[���A�j���[�V����
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Projectile.mv1");
//	resourcesMap_.emplace(SRC::BOSS_PROJECTILE, res);
//
//#pragma endregion
//
//#pragma region �X�e�[�W
//
//	// �X�e�[�W�̃��f��
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Building.mv1");
//	resourcesMap_.emplace(SRC::STAGE_MODEL, res);
//
//	// �X�e�[�W�̏Փ˔��胂�f��
//	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Collision.mv1");
//	resourcesMap_.emplace(SRC::STAGE_COLLISION_MODEL, res);
//
//#pragma endregion

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

void ResourceManager::InitResource(const nlohmann::json_abi_v3_11_3::json data)
{

	Resource res;

	// �v���C���[
	const auto& playerData = data[0]["ModelData"]["Player"];

	for (const auto& data : playerData)
	{

		// ���f���̖��O
		Resource::TYPE type = data["NAME"];

		// �p�X�̖��O
		std::string path = data["PATH"];

		// �i�[����ԍ�
		SRC src = data["SRC"];

		// �v���C���[�̃��f��
		res = Resource(type, Application::PATH_MODEL + path);
		resourcesMap_.emplace(src, res);

	}

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