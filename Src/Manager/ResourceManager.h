#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{

#pragma region �v���C���[�̃A�j���[�V����

		PLAYER,
		PLAYER_IDLE,
		PLAYER_RUN,
		PLAYER_JAB,
		PLAYER_STRAIGHT,
		PLAYER_HOOK,
		PLAYER_LEFT_KICK,
		PLAYER_RIGHT_KICK,
		PLAYER_UPPER,
#pragma endregion

#pragma region �G�̃A�j���[�V����

		ENEMY,
		ENEMY_IDLE,
		ENEMY_RUN,
		ENEMY_PUNCH,
		ENEMY_KICK,
		ENEMY_HIT,
		ENEMY_HIT_FLY,
		ENEMY_KIP_UP,
		ENEMY_JAB,
		ENEMY_STRAIGHT,

#pragma endregion

#pragma region �X�e�[�W

		STAGE

#pragma endregion

	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance();

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance();

	// ������
	void Init();

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release();

	// ���\�[�X�̊��S�j��
	void Destroy();

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};