#pragma once
#include <map>
#include <array>
#include <string>
#include "../Lib/nlohmann/json.hpp"
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{

#pragma region �摜

#pragma region �v���C���[

		IMAGE_PLAYER_ICON,
		IMAGE_PLAYER_HP_BAR,

#pragma endregion

#pragma region �G

#pragma endregion

#pragma region �{�X

#pragma endregion

#pragma region �X�e�[�W

		IMAGE_NOISE_TEXTURE,

#pragma endregion

#pragma region �^�C�g���V�[��

		IMAGE_TITLE_LOGO,
		IMAGE_PRESS_A_BUTTON_OR_SPACE,

#pragma endregion

#pragma region �Q�[���V�[��

		IMAGE_GAME_PAD_USER_GUIDE,
		IMAGE_GAME_PAD_USER_GUIDE_MOVE,
		IMAGE_GAME_PAD_USER_GUIDE_ATTACK,
		IMAGE_GAME_PAD_USER_GUIDE_SPECIAL_ATTACK,
		IMAGE_KEYBOARD_USER_GUIDE,
		IMAGE_KEYBOARD_USER_GUIDE_MOVE,
		IMAGE_KEYBOARD_USER_GUIDE_ATTACK,
		IMAGE_KEYBOARD_USER_GUIDE_SPECIAL_ATTACK,

#pragma endregion

#pragma region �{�X�̓o��V�[��

		IMAGE_BOSS_APPEARANCE,

#pragma endregion

#pragma region �Q�[���N���A�V�[��

		IMAGE_GAME_CLEAR,

#pragma endregion

#pragma endregion

#pragma region ���f���ƃA�j���[�V����

#pragma region �v���C���[

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
		ANIM_PLAYER_DEATH,
		ANIM_PLAYER_TITLE_IDLE,
		ANIM_PLAYER_VICTORY,

#pragma endregion

#pragma region �G

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

#pragma region �{�X

		MODEL_BOSS,
		ANIM_BOSS_IDLE,
		ANIM_BOSS_RUN,
		ANIM_BOSS_PUNCH,
		ANIM_BOSS_KICK,
		ANIM_BOSS_PROJECTILE,
		ANIM_BOSS_HIT_HEAD,
		ANIM_BOSS_HIT_BODY,
		ANIM_BOSS_HIT_FLY,
		ANIM_BOSS_HIT_FLINCH_UP,
		ANIM_BOSS_HIT_KNOCK_BACK,
		ANIM_BOSS_KIP_UP,
		ANIM_BOSS_JUMP_IDLE,
		ANIM_BOSS_LAND,
		ANIM_BOSS_CALL,
		ANIM_BOSS_DEATH,

#pragma endregion

#pragma region �X�e�[�W

		STAGE_MODEL,
		STAGE_COLLISION_MODEL,
		STAGE_AREA1_COLLISION_MODEL,
		STAGE_AREA2_COLLISION_MODEL,
		STAGE_AREA3_COLLISION_MODEL,
		STAGE_AREA4_COLLISION_MODEL,
		STAGE_AREA5_COLLISION_MODEL,

#pragma endregion

#pragma endregion

#pragma region �G�t�F�N�g

#pragma region �v���C���[



#pragma endregion

#pragma region �G



#pragma endregion

#pragma region �{�X

		EFFECT_BOSS_PROJECTILE,

#pragma endregion

#pragma endregion

#pragma region BGM�ESE

#pragma region �v���C���[

		SOUND_PLAYER_FOOT_STEPS_SE,
		SOUND_PLAYER_JAB_SE,
		SOUND_PLAYER_STRAIGHT_SE,
		SOUND_PLAYER_HOOK_SE,
		SOUND_PLAYER_UPPER_SE,
		SOUND_PLAYER_CHARGE_PUNCH_SE,
		SOUND_PLAYER_LEFT_KICK_SE,
		SOUND_PLAYER_RIGHT_KICK_SE,
		SOUND_PLAYER_SPECIAL_ATTACK_SE,

#pragma endregion

#pragma region �^�C�g���V�[��

		SOUND_TITLE_SCENE_BGM,
		SOUND_TITLE_SCENE_DECIDE_SE,

#pragma endregion

#pragma region �Q�[���V�[��

		SOUND_GAME_SCENE_BGM,

#pragma endregion

#pragma region �{�X�̓o��V�[��

		SOUND_BOSS_APPEARANCE_SCENE_BGM,

#pragma endregion

#pragma region �{�X�o�g���V�[��

		SOUND_BOSS_BATTLE_SCENE_BGM,

#pragma endregion

#pragma region �Q�[���N���A�V�[��

		SOUND_GAME_CLEAR_SCENE_BGM,

#pragma endregion

#pragma endregion

		MAX

	};

	const std::string RESOURCE_KEY[static_cast<int>(SRC::MAX)] =
	{

#pragma region �摜

#pragma region �v���C���[

		"IMAGE_PLAYER_ICON",
		"IMAGE_PLAYER_HP_BAR",

#pragma endregion

#pragma region �G

#pragma endregion

#pragma region �{�X

#pragma endregion

#pragma region �X�e�[�W

		"IMAGE_NOISE_TEXTURE",

#pragma endregion

#pragma region �^�C�g���V�[��

		"IMAGE_TITLE_LOGO",
		"IMAGE_PRESS_A_BUTTON_OR_SPACE",

#pragma endregion

#pragma region �Q�[���V�[��

		"IMAGE_GAME_PAD_USER_GUIDE",
		"IMAGE_GAME_PAD_USER_GUIDE_MOVE",
		"IMAGE_GAME_PAD_USER_GUIDE_ATTACK",
		"IMAGE_GAME_PAD_USER_GUIDE_SPECIAL_ATTACK",
		"IMAGE_KEYBOARD_USER_GUIDE",
		"IMAGE_KEYBOARD_USER_GUIDE_MOVE",
		"IMAGE_KEYBOARD_USER_GUIDE_ATTACK",
		"IMAGE_KEYBOARD_USER_GUIDE_SPECIAL_ATTACK",

#pragma endregion

#pragma region �{�X�̓o��V�[��

		"IMAGE_BOSS_APPEARANCE",

#pragma endregion

#pragma region �Q�[���N���A�V�[��

		"IMAGE_GAME_CLEAR",

#pragma endregion

#pragma endregion

#pragma region ���f���ƃA�j���[�V����

#pragma region �v���C���[

		"MODEL_PLAYER",
		"ANIM_PLAYER_IDLE",
		"ANIM_PLAYER_RUN",
		"ANIM_PLAYER_JAB",
		"ANIM_PLAYER_STRAIGHT",
		"ANIM_PLAYER_HOOK",
		"ANIM_PLAYER_LEFT_KICK",
		"ANIM_PLAYER_RIGHT_KICK",
		"ANIM_PLAYER_UPPER",
		"ANIM_PLAYER_CHARGE_PUNCH",
		"ANIM_PLAYER_SPECIAL_ATTACK",
		"ANIM_PLAYER_POWER_CHARGE",
		"ANIM_PLAYER_HIT_HEAD",
		"ANIM_PLAYER_HIT_BODY",
		"ANIM_PLAYER_DEATH",
		"ANIM_PLAYER_TITLE_IDLE",
		"ANIM_PLAYER_VICTORY",

#pragma endregion

#pragma region �G

		"MODEL_ENEMY",
		"ANIM_ENEMY_IDLE",
		"ANIM_ENEMY_RUN",
		"ANIM_ENEMY_PUNCH",
		"ANIM_ENEMY_KICK",
		"ANIM_ENEMY_HIT_HEAD",
		"ANIM_ENEMY_HIT_BODY",
		"ANIM_ENEMY_HIT_FLY",
		"ANIM_ENEMY_HIT_FLINCH",
		"ANIM_ENEMY_HIT_KNOCK_BACK",
		"ANIM_ENEMY_KIP_UP",
		"ANIM_ENEMY_NORMAL_DEATH",

#pragma endregion

#pragma region �{�X

		"MODEL_BOSS",
		"ANIM_BOSS_IDLE",
		"ANIM_BOSS_RUN",
		"ANIM_BOSS_PUNCH",
		"ANIM_BOSS_KICK",
		"ANIM_BOSS_PROJECTILE",
		"ANIM_BOSS_HIT_HEAD",
		"ANIM_BOSS_HIT_BODY",
		"ANIM_BOSS_HIT_FLY",
		"ANIM_BOSS_HIT_FLINCH_UP",
		"ANIM_BOSS_HIT_KNOCK_BACK",
		"ANIM_BOSS_KIP_UP",
		"ANIM_BOSS_JUMP_IDLE",
		"ANIM_BOSS_LAND",
		"ANIM_BOSS_CALL",
		"ANIM_BOSS_DEATH",

#pragma endregion

#pragma region �X�e�[�W

		"STAGE_MODEL",
		"STAGE_COLLISION_MODEL",
		"STAGE_AREA1_COLLISION_MODEL",
		"STAGE_AREA2_COLLISION_MODEL",
		"STAGE_AREA3_COLLISION_MODEL",
		"STAGE_AREA4_COLLISION_MODEL",
		"STAGE_AREA5_COLLISION_MODEL",

#pragma endregion

#pragma endregion

#pragma region �G�t�F�N�g

#pragma region �v���C���[

#pragma endregion

#pragma region �G

#pragma endregion

#pragma region �{�X

		"EFFECT_BOSS_PROJECTILE",

#pragma endregion

#pragma endregion

#pragma region BGM�ESE

#pragma region �v���C���[

		"SOUND_PLAYER_FOOT_STEPS_SE",
		"SOUND_PLAYER_JAB_SE",
		"SOUND_PLAYER_STRAIGHT_SE",
		"SOUND_PLAYER_HOOK_SE",
		"SOUND_PLAYER_UPPER_SE",
		"SOUND_PLAYER_CHARGE_PUNCH_SE",
		"SOUND_PLAYER_LEFT_KICK_SE",
		"SOUND_PLAYER_RIGHT_KICK_SE",
		"SOUND_PLAYER_SPECIAL_ATTACK_SE",

#pragma endregion
		
#pragma region �^�C�g���V�[��

		"SOUND_TITLE_SCENE_BGM",
		"SOUND_TITLE_SCENE_DECIDE_SE",

#pragma endregion

#pragma region �Q�[���V�[��

		"SOUND_GAME_SCENE_BGM",

#pragma endregion

#pragma region �{�X�̓o��V�[��

		"SOUND_BOSS_APPEARANCE_SCENE_BGM",

#pragma endregion

#pragma region �{�X�o�g���V�[��

		"SOUND_BOSS_BATTLE_SCENE_BGM",

#pragma endregion

#pragma region �Q�[���N���A�V�[��

		"SOUND_GAME_CLEAR_SCENE_BGM"

#pragma endregion

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
	Resource Load(std::string src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(std::string src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<std::string, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<std::string, Resource*> loadedMap_;

	// ���\�[�X�̏�����
	void InitResource(const std::string& pathName, const nlohmann::json_abi_v3_11_3::json jsonData);

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager();

	// �f�X�g���N�^�����l
	~ResourceManager() = default;

	// �������[�h
	Resource* _Load(std::string src);

	// �摜�̃��[�h
	void ImgLoad(const nlohmann::json_abi_v3_11_3::json& objectData);

	// ���f���ƃA�j���[�V�����̃��[�h
	void ModelAndAnimationLoad(const nlohmann::json_abi_v3_11_3::json& objectData);

	// �G�t�F�N�g�̃��[�h
	void EffectLoad(const nlohmann::json_abi_v3_11_3::json& objectData);

	// BGM��SE�̃��[�h
	void BGMAndSELoad(const nlohmann::json_abi_v3_11_3::json& objectData);

};