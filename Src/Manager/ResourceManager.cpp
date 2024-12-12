#include <fstream>
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

	// �v���C���[�̃��f���ƃA�j���[�V�����f�[�^
	const auto& playerModelData = objectData[0]["ModelData"]["Player"];

	// �v���C���[�̃��f���ƃA�j���[�V�����̏�����
	InitResource(Application::PATH_MODEL, playerModelData);

	// �G�̃��f���ƃA�j���[�V�����f�[�^
	const auto& enemyModelData = objectData[0]["ModelData"]["Enemy"];

	// �G�̃��f���ƃA�j���[�V�����̏�����
	InitResource(Application::PATH_MODEL, enemyModelData);

	// �{�X�̃��f���ƃA�j���[�V�����f�[�^
	const auto& bossModelData = objectData[0]["ModelData"]["Boss"];

	// �{�X�̃��f���ƃA�j���[�V�����̏�����
	InitResource(Application::PATH_MODEL, bossModelData);

	// �X�e�[�W�̃��f���ƃA�j���[�V�����f�[�^
	const auto& stageModelData = objectData[0]["ModelData"]["Stage"];

	// �X�e�[�W�̃��f���ƃA�j���[�V�����̏�����
	InitResource(Application::PATH_MODEL, stageModelData);

	// �{�X�̃G�t�F�N�g�f�[�^
	const auto& effectModelData = objectData[0]["EffectData"]["Boss"];

	// �X�e�[�W�̃��f���ƃA�j���[�V�����̏�����
	InitResource(Application::PATH_EFFECT, effectModelData);

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

Resource ResourceManager::Load(std::string src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(std::string src)
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

void ResourceManager::InitResource(const std::string& pathName, const nlohmann::json_abi_v3_11_3::json jsonData)
{

	Resource res;

	for (const auto& data : jsonData)
	{

		// ���\�[�X�̃^�C�v
		Resource::TYPE type = data["TYPE"];

		// �p�X�̖��O
		std::string path = data["PATH"];

		// �i�[����ԍ�
		std::string src = data["SRC"];

		// �i�[����
		res = Resource(type, pathName + path);
		resourcesMap_.emplace(src, res);

	}

}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(std::string src)
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