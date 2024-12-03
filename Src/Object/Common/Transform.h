#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Utility/Utility.h"
#include "../Component/Component.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：VECTOR基準
/// 回転　：Quaternion基準
/// 位置　：VECTOR基準
/// </summary>
class Transform
{

public:

	// コンストラクタ
	Transform();
	Transform(const int model);

	// デストラクタ
	~Transform();

	// モデルのハンドルID
	int modelId;

	// 大きさ
	VECTOR scl;

	// 回転
	VECTOR rot;

	// 位置
	VECTOR pos;
	VECTOR localPos;

	// 行列
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// 回転
	Quaternion quaRot;

	// ローカル回転
	Quaternion quaRotLocal;

	/// <summary>
	/// モデル制御の基本情報更新
	/// </summary>
	/// <param name=""></param>
	void Update();

	void Release();

	void SetModel(const int model) { modelId = model; };

	// 前方方向を取得
	const VECTOR& GetForward() const { return GetDir(Utility::DIR_F); };

	// 後方方向を取得
	const VECTOR& GetBack() const { return GetDir(Utility::DIR_B); };

	// 右方向を取得
	const VECTOR& GetRight() const { return GetDir(Utility::DIR_R); };

	// 左方向を取得
	const VECTOR& GetLeft() const { return GetDir(Utility::DIR_L); };

	// 上方向を取得
	const VECTOR& GetUp() const { return GetDir(Utility::DIR_U); };

	// 下方向を取得
	const VECTOR& GetDown() const { return GetDir(Utility::DIR_D); };

	// 対象方向を取得
	const VECTOR& GetDir(const VECTOR& vec) const { return quaRot.PosAxis(vec); };

};