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

#pragma region �v���C���[�̃��f���ƃA�j���[�V����

		MODEL_PLAYER,
		ANIM_PLAYER_IDLE,
		ANIM_PLAYER_RUN,
		ANIM_PLAYER_JAB,
		ANIM_PLAYER_STRAIGHT,
		ANIM_PLAYER_HOOK,
		ANIM_PLAYER_LEFT_KICK,
		ANIM_PLAYER_RIGHT_KICK,
		ANIM_PLAYER_UPPER,
		ANIM_PLAYER_CHARGE_PUNCH,
		ANIM_PLAYER_SPECIAL_ATTACK,
		ANIM_PLAYER_POWER_CHARGE,
		ANIM_PLAYER_HIT_HEAD,
		ANIM_PLAYER_HIT_BODY,
#pragma endregion

#pragma region �G�̃��f���ƃA�j���[�V����

		MODEL_ENEMY,
		ANIM_ENEMY_IDLE,
		ANIM_ENEMY_RUN,
		ANIM_ENEMY_PUNCH,
		ANIM_ENEMY_KICK,
		ANIM_ENEMY_HIT_HEAD,
		ANIM_ENEMY_HIT_BODY,
		ANIM_ENEMY_HIT_FLY,
		ANIM_ENEMY_HIT_FLINCH,
		ANIM_ENEMY_HIT_KNOCK_BACK,
		ANIM_ENEMY_KIP_UP,
		ANIM_ENEMY_NORMAL_DEATH,

#pragma endregion

#pragma region �{�X�̃��f���ƃA�j���[�V����

		MODEL_BOSS,
		ANIM_BOSS_IDLE,
		ANIM_BOSS_RUN,
		ANIM_BOSS_PUNCH,
		ANIM_BOSS_KICK,
		ANIM_BOSS_PROJECTILE,

#pragma endregion

#pragma region �X�e�[�W�̃��f��

		STAGE_MODEL,
		STAGE_COLLISION_MODEL,

#pragma endregion

#pragma region �v���C���[�̃G�t�F�N�g



#pragma endregion

#pragma region �G�̃G�t�F�N�g



#pragma endregion

#pragma region �{�X�̃G�t�F�N�g

		EFFECT_BOSS_PROJECTILE

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

	// ���\�[�X�̏�����
	void InitResource(const nlohmann::json_abi_v3_11_3::json data);

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager();

	// �f�X�g���N�^�����l
	~ResourceManager() = default;

	// �������[�h
	Resource* _Load(SRC src);

};