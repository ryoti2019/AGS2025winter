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

	const nlohmann::json_abi_v3_11_3::json& objectData = objectJson["ResourceData"];

	// �X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	// ���s�V�ǂ��A�����I�Ƀt�@�C���X�g���[�������
	ifs.close();

	// �摜�̃��[�h
	ImgLoad(objectData);

	// ���f���ƃA�j���[�V�����̃��[�h
	ModelAndAnimationLoad(objectData);

	// �G�t�F�N�g�̃��[�h
	EffectLoad(objectData);

	// BGM��SE�̃��[�h
	SoundLoad(objectData);

}

void ResourceManager::ImgLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// �v���C���[�̉摜�f�[�^
	const auto& playerImageData = objectData[0]["ImageData"]["Player"];

	// �X�e�[�W�̉摜�f�[�^�̏�����
	InitResource(Application::PATH_IMAGE, playerImageData);

	// �X�e�[�W�̉摜�f�[�^
	const auto& stageImageData = objectData[0]["ImageData"]["Stage"];

	// �X�e�[�W�̉摜�f�[�^�̏�����
	InitResource(Application::PATH_IMAGE, stageImageData);

	// �^�C�g���V�[���̉摜�f�[�^
	const auto& titleSceneImageData = objectData[0]["ImageData"]["TitleScene"];

	// �^�C�g���V�[���̉摜�f�[�^�̏�����
	InitResource(Application::PATH_IMAGE, titleSceneImageData);

	// �Q�[���V�[���̉摜�f�[�^
	const auto& gameSceneImageData = objectData[0]["ImageData"]["GameScene"];

	// �Q�[���V�[���̉摜�f�[�^�̏�����
	InitResource(Application::PATH_IMAGE, gameSceneImageData);

	// �{�X�̓o��V�[���̉摜�f�[�^
	const auto& bossAppearanceSceneImageData = objectData[0]["ImageData"]["BossAppearanceScene"];

	// �{�X�̓o��V�[���̉摜�f�[�^�̏�����
	InitResource(Application::PATH_IMAGE, bossAppearanceSceneImageData);

	// �Q�[���N���A�V�[���̉摜�f�[�^
	const auto& gameClearSceneImageData = objectData[0]["ImageData"]["GameClearScene"];

	// �Q�[���N���A�V�[���̉摜�f�[�^�̏�����
	InitResource(Application::PATH_IMAGE, gameClearSceneImageData);

	// �Q�[���I�[�o�[�V�[���̉摜�f�[�^
	const auto& gameOverSceneImageData = objectData[0]["ImageData"]["GameOverScene"];

	// �Q�[���I�[�o�[�V�[���̉摜�f�[�^�̏�����
	InitResource(Application::PATH_IMAGE, gameOverSceneImageData);

}

void ResourceManager::ModelAndAnimationLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

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

}

void ResourceManager::EffectLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// �v���C���[�̃G�t�F�N�g�f�[�^
	const auto& playerEffectData = objectData[0]["EffectData"]["Player"];

	// �v���C���[�̃G�t�F�N�g�f�[�^�̏�����
	InitResource(Application::PATH_EFFECT, playerEffectData);

	// �G�̃G�t�F�N�g�f�[�^
	const auto& enemyEffectData = objectData[0]["EffectData"]["Enemy"];

	// �G�̃G�t�F�N�g�f�[�^�̏�����
	InitResource(Application::PATH_EFFECT, enemyEffectData);

	// �{�X�̃G�t�F�N�g�f�[�^
	const auto& bossEffectData = objectData[0]["EffectData"]["Boss"];

	// �{�X�̃G�t�F�N�g�f�[�^�̏�����
	InitResource(Application::PATH_EFFECT, bossEffectData);

}

void ResourceManager::SoundLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// �v���C���[�̃T�E���h�f�[�^
	const auto& playerSoundData = objectData[0]["SoundData"]["Player"];

	// �v���C���[�̃T�E���h�f�[�^�̏�����
	InitResource(Application::PATH_SOUND, playerSoundData);

	// �^�C�g���V�[���̃T�E���h�f�[�^
	const auto& titleSceneSoundData = objectData[0]["SoundData"]["TitleScene"];

	// �^�C�g���V�[���̃T�E���h�f�[�^�̏�����
	InitResource(Application::PATH_SOUND, titleSceneSoundData);

	// �Q�[���V�[���̃T�E���h�f�[�^
	const auto& gameSceneSoundData = objectData[0]["SoundData"]["GameScene"];

	// �Q�[���V�[���̃T�E���h�f�[�^�̏�����
	InitResource(Application::PATH_SOUND, gameSceneSoundData);

	// �{�X�̓o��V�[���̃T�E���h�f�[�^
	const auto& bossAppearanceSceneSoundData = objectData[0]["SoundData"]["BossAppearanceScene"];

	// �{�X�̓o��V�[���̃T�E���h�f�[�^�̏�����
	InitResource(Application::PATH_SOUND, bossAppearanceSceneSoundData);

	// �{�X�o�g���V�[���̃T�E���h�f�[�^
	const auto& bossBattleSceneSoundData = objectData[0]["SoundData"]["BossBattleScene"];

	// �{�X�o�g���V�[���̃T�E���h�f�[�^�̏�����
	InitResource(Application::PATH_SOUND, bossBattleSceneSoundData);

	// �Q�[���N���A�V�[���̃T�E���h�f�[�^
	const auto& gameClearSceneSoundData = objectData[0]["SoundData"]["GameClearScene"];

	// �Q�[���N���A�V�[���̃T�E���h�f�[�^�̏�����
	InitResource(Application::PATH_SOUND, gameClearSceneSoundData);

	// �Q�[���I�[�o�[�V�[���̃T�E���h�f�[�^
	const auto& gameOverSceneSoundData = objectData[0]["SoundData"]["GameOverScene"];

	// �Q�[���I�[�o�[�V�[���̃T�E���h�f�[�^�̏�����
	InitResource(Application::PATH_SOUND, gameOverSceneSoundData);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		if (p.second->resType_ != Resource::TYPE::ANIM_MODEL)
		{
			p.second->Release();
			delete p.second;
		}
	}

	// DxLib�̉�������s��΍�̂��߁A
	// �A�j���[�V�������f���̃�����������Ō�ɍs��
	for (auto& p : loadedMap_)
	{
		if (p.second->resType_ == Resource::TYPE::ANIM_MODEL)
		{
			p.second->Release();
			delete p.second;
		}
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
	//Resource ret = *res;
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

	if (src == "MODEL_ENEMY" || src == "ANIM_ENEMY_IDLE")
	{
		int a = 1;
		a = 2;
	}

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