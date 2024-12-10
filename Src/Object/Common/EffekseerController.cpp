#include <EffekseerForDXLib.h>
#include "../../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "EffekseerController.h"

EffekseerController::EffekseerController()
{
}

EffekseerController::~EffekseerController()
{
}

void EffekseerController::Add(const std::string& name, const int handleId, const float scale)
{

	EFFECT_DATA data;

	// �G�t�F�N�g�^�C�v
	data.name = name;

	// �G�t�F�N�g�n���h��
	data.resId = handleId;

	// ���W
	data.pos = Utility::VECTOR_ZERO;

	// �傫��
	data.scale = scale;

	// �G�t�F�N�g�f�[�^���i�[
	effectData_.emplace(name, data);

}

void EffekseerController::Draw(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = quaRot;
	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis(relativePos);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectData_[handle].pos = VAdd(followPos, effectPos);

	// �Đ�
	effectData_[handle].playId = PlayEffekseer3DEffect(effectData_[handle].resId);

	// �傫��
	SetScalePlayingEffekseer3DEffect(effectData_[handle].playId,
		effectData_[handle].scale, effectData_[handle].scale, effectData_[handle].scale);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectData_[handle].playId,
		effectData_[handle].pos.x, effectData_[handle].pos.y, effectData_[handle].pos.z);

	// ��]�̐ݒ�
	SetRotationPlayingEffekseer3DEffect(effectData_[handle].playId, rot.x, rot.y, rot.z);

}

void EffekseerController::DrawStop(const std::string& handle)
{
	// �G�t�F�N�g���~�߂�
	StopEffekseer3DEffect(effectData_[handle].playId);
}

void EffekseerController::FollowPos(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = quaRot;
	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis(relativePos);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectData_[handle].pos = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectData_[handle].playId,
		effectData_[handle].pos.x, effectData_[handle].pos.y, effectData_[handle].pos.z);

}

bool EffekseerController::IsDraw(const std::string& handle)
{
	return IsEffekseer3DEffectPlaying(effectData_[handle].playId) == 0;
}
