#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	COOL_TIME(data["COOL_TIME"]),
	ACTIVATION_DISTANCE(data["ACTIVATION_DISTANCE"]),
	TRACKING_MAX_TIME(data["TRACKING_MAX_TIME"]),
	HIT_FLY_UP_VEC_POW(data["TRACKING_MAX_TIME"]),
	HIT_FLY_MOVE_POW(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_UP_VEC_POW(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_UP_VEC_SMALL_POW(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_SPEED(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_GRAVITY_SCALE(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_ANGLE_X(data["TRACKING_MAX_TIME"]),
	KNOCK_BACK_HEIGHT_OFFSET(data["TRACKING_MAX_TIME"]),
	KNOCK_BACK_TIME(data["TRACKING_MAX_TIME"])
{
}

void EnemyBase::Init()
{

	// �N�[���^�C��
	coolTime_ = 0.0f;

	// �s�������߂����ǂ���
	isActionDecided_ = false;

}

std::optional<VECTOR> EnemyBase::GetPlayerPos()
{
	// �Q�[���V�[���̏��������Ă���
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!gameScene)
	{
		return std::nullopt;
	}

	// �A�N�^�[�}�l�[�W���[���擾
	auto actorManager = gameScene->GetActorManager();

	// �Ǐ]�Ώ�
	auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// �v���C���[�̍��W���擾
	for (const auto& player : players->second)
	{
		return player->GetTransform()->pos;
	}
}
