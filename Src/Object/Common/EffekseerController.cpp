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

	// エフェクトタイプ
	data.name = name;

	// エフェクトハンドル
	data.resId = handleId;

	// 座標
	data.pos = Utility::VECTOR_ZERO;

	// 大きさ
	data.scale = scale;

	// エフェクトデータを格納
	effectData_.emplace(name, data);

}

void EffekseerController::Draw(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle)
{

	// 追従対象の位置
	VECTOR followPos = pos;

	// 追従対象の向き
	Quaternion followRot = quaRot;
	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象からエフェクトまでの相対座標
	VECTOR effectPos = followRot.PosAxis(relativePos);

	// エフェクトの位置の更新
	effectData_[handle].pos = VAdd(followPos, effectPos);

	// 再生
	effectData_[handle].playId = PlayEffekseer3DEffect(effectData_[handle].resId);

	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectData_[handle].playId,
		effectData_[handle].scale, effectData_[handle].scale, effectData_[handle].scale);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectData_[handle].playId,
		effectData_[handle].pos.x, effectData_[handle].pos.y, effectData_[handle].pos.z);

	// 回転の設定
	SetRotationPlayingEffekseer3DEffect(effectData_[handle].playId, rot.x, rot.y, rot.z);

}

void EffekseerController::DrawStop(const std::string& handle)
{
	// エフェクトを止める
	StopEffekseer3DEffect(effectData_[handle].playId);
}

void EffekseerController::FollowPos(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle)
{

	// 追従対象の位置
	VECTOR followPos = pos;

	// 追従対象の向き
	Quaternion followRot = quaRot;
	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象からエフェクトまでの相対座標
	VECTOR effectPos = followRot.PosAxis(relativePos);

	// エフェクトの位置の更新
	effectData_[handle].pos = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectData_[handle].playId,
		effectData_[handle].pos.x, effectData_[handle].pos.y, effectData_[handle].pos.z);

}

bool EffekseerController::IsDraw(const std::string& handle)
{
	return IsEffekseer3DEffectPlaying(effectData_[handle].playId) == 0;
}
