#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/AnimationController.h"

AnimationController::AnimationController(int modelId)
	:
	modelId_(modelId),
	state_(""),
	preState_(""),
	AttachNum_(0),
	isStop_(false),
	isSlow_(false),
	gameSpeed_(DEFAULT_SPEED)
{
}

AnimationController::~AnimationController(void)
{
	animData_.clear();
}

void AnimationController::Add(const std::string state,float startStep,
	float speed, int animHandle, bool isLoop, int animIndex, bool isReverse)
{
	// 1つのアニメーションデータの情報を入れていく
	AnimationData anim;

	// アニメーションスピード
	anim.speedAnim = speed;

	// アニメーションが始まる時間
	anim.startTime = startStep;

	// アニメーションハンドル
	anim.animHandle = animHandle;

	// ブレンドタイム
	anim.blendTime = BLEND_TIME;

	// ブレンドレート
	anim.blendRate = 0.0f;

	// ループ再生するかしないか
	anim.isLoop = isLoop;

	// アニメーション番号
	anim.animIndex = animIndex;

	// 逆再生するかしないか
	anim.isReverse = isReverse;

	// アニメーションの状態
	anim.state = state;

	// アニメーション情報を格納
	animData_.emplace(state, anim);

}

void AnimationController::Update(const float deltaTime)
{

	// 経過時間の取得
	float slowTime = deltaTime / SLOW_RATE;

	// レートの計算
	float rate = 1.0f;
	for (auto& animData : animData_)
	{

		// アタッチされていないか優先されていたら処理されない
		if (animData.second.attachNo == -1 || animData.second.isPriority)
		{
			continue;
		}

		// ブレンドレートを下げていく
		if (!isSlow_)
		{
			animData.second.blendRate -= deltaTime / animData.second.blendTime* gameSpeed_;
		}
		else
		{
			animData.second.blendRate -= slowTime / animData.second.blendTime* gameSpeed_;
		}

		// ブレンドレートが0になったらデタッチ処理をする
		if (animData.second.blendRate <= 0.0f)
		{

			// デタッチ
			Dettach(animData.second.attachNo);

			// 値の初期化
			animData.second.blendRate = 0.0f;
			animData.second.isPriority = false;
			animData.second.attachNo = -1;

			// 逆再生用
			if (!animData.second.isReverse)
			{
				animData.second.stepAnim = animData.second.startTime;
			}
			// ノーマル再生用
			else
			{
				animData.second.stepAnim = animData.second.animTotalTime;
			}

		}

		// 新しいアニメーションに渡すブレンドレートを作るために1から今のアニメーションのブレンドレートを引く
		rate -= animData.second.blendRate;

	}

	if (animData_.contains(state_))
	{
		// 上で作ったブレンドレ−トを新しいアニメーションのブレンドレートに入れる
		animData_.at(state_).blendRate = rate;
	}


	// アニメーション再生
	for (auto& animData : animData_)
	{

		// アタッチされていなければ処理しない
		if (animData.second.attachNo == -1)
		{
			continue;
		}

		// 再生
		if (animData.second.stepAnim <= animData.second.animTotalTime && !animData.second.isReverse)
		{

			if (!isSlow_)
			{
				animData.second.stepAnim += (deltaTime * animData.second.speedAnim * gameSpeed_);
			}
			else
			{
				animData.second.stepAnim += (slowTime * animData.second.speedAnim) * gameSpeed_;
			}

		}
		// 逆再生
		else if (animData.second.stepAnim >= animData.second.startTime && animData.second.isReverse)
		{

			if (!isSlow_)
			{
				animData.second.stepAnim -= deltaTime * animData.second.speedAnim* gameSpeed_;
			}
			else
			{
				animData.second.stepAnim -= slowTime * animData.second.speedAnim * gameSpeed_;
			}

		}

		if (animData.second.stepAnim > animData.second.animTotalTime && animData.second.isLoop)
		{
			// ループ再生
			animData.second.stepAnim = animData.second.startTime;
		}
		else if (animData.second.stepAnim <= animData.second.startTime && !animData.second.isLoop && animData.second.isReverse)
		{
			// 逆再生
			animData.second.stepAnim = animData.second.startTime;
		}
		else if (animData.second.stepAnim > animData.second.animTotalTime && !animData.second.isLoop && !animData.second.isReverse)
		{
			// どちらでもない再生
			animData.second.stepAnim = animData.second.animTotalTime;
		}

		// 再生するアニメーション時間の設定
		MV1SetAttachAnimTime(modelId_, animData.second.attachNo, animData.second.stepAnim);

		// 優先されているものだけレートを入れる
		if (animData.second.isPriority)
		{
			animData.second.blendRate = rate;
		}

		// アニメーションのレートをモデルに適用する
		MV1SetAttachAnimBlendRate(
			modelId_, animData.second.attachNo, animData.second.blendRate);

	}
}

std::string AnimationController::GetPlayType(void) const
{
	return state_;
}

int AnimationController::GetAttachNum(void) const
{
	return AttachNum_;
}

void AnimationController::Attatch(std::string state)
{

	// ノーマル再生の初期化
	if (!animData_[state].isReverse)
	{
		animData_[state].stepAnim = animData_[state].startTime;
	}

	// アタッチされているモノだけ処理する
	if (animData_[state].attachNo != -1)
	{
		animData_[state].isPriority = true;
		return;
	}

	AttachNum_++;

	// アニメーション番号を取得
	animData_[state].attachNo = MV1AttachAnim(modelId_, animData_[state].animIndex, animData_[state].animHandle);

	// 優先させる
	animData_[state].isPriority = true;

	// アニメーション総時間の取得
	animData_[state].animTotalTime = MV1GetAttachAnimTotalTime(modelId_, animData_[state].attachNo);

	// 逆再生の初期化
	if (animData_[state].isReverse)
	{
		animData_[state].stepAnim = animData_[state].animTotalTime;
	}

}

void AnimationController::Dettach(int attachNo)
{
	AttachNum_--;

	// デタッチする
	MV1DetachAnim(modelId_, attachNo);
}

void AnimationController::ChangeAnimation(std::string state)
{

	// 同じ状態だったら入らない
	if (state == preState_) return;

	state_ = state;

	// 前の状態の
	if (preState_ != "")
	{
		animData_[preState_].isPriority = false;
	}

	animData_[state_].blendRate = 0.0f;

	// 再生するアニメーションの設定
	Attatch(state);

	// 一個前の状態を保存
	preState_ = state;

}

AnimationController::AnimationData AnimationController::GetAnimData(const std::string& state)
{
	return animData_[state];
}

const std::map<std::string, AnimationController::AnimationData>& AnimationController::GetAnimDatas(void) const
{
	return animData_;
}

void AnimationController::ResetStepAnim()
{
	animData_[state_].stepAnim = 0.0f;
}

float AnimationController::GetStepAnim()
{

	// 今のアニメーションの再生時間を取得
	auto& data = animData_[state_];
	return data.stepAnim;

}

bool AnimationController::GetIsPriority(void)
{
	return animData_[preState_].isPriority;
}

bool AnimationController::IsPlayNowAnimation(void)
{

	// 今のアニメーションを再生しているかどうか判定する
	auto& data = animData_[state_];

	// stepAnimが0.0f以上だと再生判定
	if (data.stepAnim > 0.0f)
	{
		return true;
	}

	return false;

}

bool AnimationController::IsEndPlayAnimation(void)
{

	const auto& data = animData_[state_];

	// 今のアニメーションの再生が終了しているか判定
	if (!data.isLoop && data.stepAnim >= data.animTotalTime && !data.isReverse)
	{
		return true;
	}
	else if (!data.isLoop && data.stepAnim <= data.startTime && data.isReverse)
	{
		return true;
	}

	return false;

}

bool AnimationController::IsPreEndPlayAnimation(void)
{

	const auto& data = animData_[state_];

	// 今のアニメーションの再生が終了しているか判定
	if (!data.isLoop && data.stepAnim >= data.animTotalTime - 1.0f && !data.isReverse)
	{
		return true;
	}
	else if (!data.isLoop && data.stepAnim <= data.startTime + 1.0f && data.isReverse)
	{
		return true;
	}
	return false;

}

bool AnimationController::IsBlendPlay(std::string state)
{

	// 再生しているアニメーションがブレンドされているかどうか
	for (auto& data : animData_)
	{
		if (data.second.blendRate > 0.0 && data.second.state == state)
		{
			return true;
		}
	}

	return false;

}

bool AnimationController::IsEndBlendingPlayAnimation(std::string state)
{

	const auto& data = animData_[state];

	// ブレンドしているアニメーションが再生終了しているか判定
	if (data.blendRate > 0.0 && data.state == state)
	{
		if (!data.isLoop && data.stepAnim >= data.animTotalTime)
		{
			return true;
		}
	}

	return false;

}

void AnimationController::Stop(bool stop)
{
	isStop_ = stop;
}

void AnimationController::Slow(bool isSlow)
{
	isSlow_ = isSlow;
}

void AnimationController::SpeedChange(float speed)
{
	gameSpeed_ = speed;
}
