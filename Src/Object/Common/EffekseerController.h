#pragma once
#include <DxLib.h>
#include <string>
#include <map>
#include "../Common/Transform.h"
class ResourceManager;

class EffekseerController
{

public:

	struct EFFECT_DATA
	{

		// エフェクトの名前
		std::string name;

		// エフェクトハンドル
		int resId;

		// 再生ハンドル
		int playId;

		// 座標
		VECTOR pos;

		// 大きさ
		float scale;
	};

	// コンストラクタ
	EffekseerController();

	// デストラクタ
	~EffekseerController();

	/// <summary>
	/// エフェクトデータ追加
	/// </summary>
	/// <param name="name">エフェクトの名前</param>
	/// <param name="handleId">エフェクトハンドル</param>
	/// <param name="scale">エフェクトのスケール</param>
	virtual void Add(const std::string& name, const int handleId, const float scale);

	// 描画
	virtual void Draw(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle);

	// 描画を止める
	virtual void DrawStop(const std::string& handle);

	// エフェクト追従
	virtual void FollowPos(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle);

protected:

	// エフェクトデータ
	std::map <std::string, EFFECT_DATA> effectData_;

};