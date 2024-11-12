#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/AnimationController.h"

#pragma region �萔�錾

// �A�j���[�V�������X���[�ɂ��郌�[�g
static constexpr float SLOW_RATE = 5.0f;

// �f�t�H���g�̃Q�[���X�s�[�h
static constexpr float DEFAULT_SPEED = 1.0f;

// �u�����h���[�g
static constexpr float BLEND_TIME = 0.3f;


#pragma endregion


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

	MV1DeleteModel(modelId_);
}

void AnimationController::Add(const std::string state, const std::string& path, float startStep,
	float speed, int animHandle, bool isLoop, int animIndex, bool isReverse)
{
	// 1�̃A�j���[�V�����f�[�^�̏������Ă���
	AnimationData anim;

	// �A�j���[�V�����X�s�[�h
	anim.speedAnim = speed;

	// �A�j���[�V�������n�܂鎞��
	anim.startTime = startStep;

	// �A�j���[�V�����n���h��
	anim.animHandle = animHandle;

	// �u�����h�^�C��
	anim.blendTime = BLEND_TIME;

	// �u�����h���[�g
	anim.blendRate = 0.0f;

	// ���[�v�Đ����邩���Ȃ���
	anim.isLoop = isLoop;

	// �A�j���[�V�����ԍ�
	anim.animIndex = animIndex;

	// �t�Đ����邩���Ȃ���
	anim.isReverse = isReverse;

	// �A�j���[�V�����̏��
	anim.state = state;

	// �A�j���[�V���������i�[
	animData_.emplace(state, anim);

}

void AnimationController::Update(const float deltaTime)
{

	// �o�ߎ��Ԃ̎擾
	float slowTime = deltaTime / SLOW_RATE;

	// ���[�g�̌v�Z
	float rate = 1.0f;
	for (auto& animData : animData_)
	{

		// �A�^�b�`����Ă��Ȃ����D�悳��Ă����珈������Ȃ�
		if (animData.second.attachNo == -1 || animData.second.isPriority)
		{
			continue;
		}

		// �u�����h���[�g�������Ă���
		if (!isSlow_)
		{
			animData.second.blendRate -= deltaTime / animData.second.blendTime* gameSpeed_;
		}
		else
		{
			animData.second.blendRate -= slowTime / animData.second.blendTime* gameSpeed_;
		}

		// �u�����h���[�g��0�ɂȂ�����f�^�b�`����������
		if (animData.second.blendRate <= 0.0f)
		{

			// �f�^�b�`
			Dettach(animData.second.attachNo);

			// �l�̏�����
			animData.second.blendRate = 0.0f;
			animData.second.isPriority = false;
			animData.second.attachNo = -1;

			// �t�Đ��p
			if (!animData.second.isReverse)
			{
				animData.second.stepAnim = animData.second.startTime;
			}
			// �m�[�}���Đ��p
			else
			{
				animData.second.stepAnim = animData.second.animTotalTime;
			}

		}

		// �V�����A�j���[�V�����ɓn���u�����h���[�g����邽�߂�1���獡�̃A�j���[�V�����̃u�����h���[�g������
		rate -= animData.second.blendRate;

	}

	if (animData_.contains(state_))
	{
		// ��ō�����u�����h���|�g��V�����A�j���[�V�����̃u�����h���[�g�ɓ����
		animData_.at(state_).blendRate = rate;
	}


	// �A�j���[�V�����Đ�
	for (auto& animData : animData_)
	{

		// �A�^�b�`����Ă��Ȃ���Ώ������Ȃ�
		if (animData.second.attachNo == -1)
		{
			continue;
		}

		// �Đ�
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
		// �t�Đ�
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
			// ���[�v�Đ�
			animData.second.stepAnim = animData.second.startTime;
		}
		else if (animData.second.stepAnim <= animData.second.startTime && !animData.second.isLoop && animData.second.isReverse)
		{
			// �t�Đ�
			animData.second.stepAnim = animData.second.startTime;
		}
		else if (animData.second.stepAnim > animData.second.animTotalTime && !animData.second.isLoop && !animData.second.isReverse)
		{
			// �ǂ���ł��Ȃ��Đ�
			animData.second.stepAnim = animData.second.animTotalTime;
		}

		// �Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(modelId_, animData.second.attachNo, animData.second.stepAnim);

		// �D�悳��Ă�����̂������[�g������
		if (animData.second.isPriority)
		{
			animData.second.blendRate = rate;
		}

		// �A�j���[�V�����̃��[�g�����f���ɓK�p����
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

	// �m�[�}���Đ��̏�����
	if (!animData_[state].isReverse)
	{
		animData_[state].stepAnim = animData_[state].startTime;
	}

	// �A�^�b�`����Ă��郂�m������������
	if (animData_[state].attachNo != -1)
	{
		animData_[state].isPriority = true;
		return;
	}

	AttachNum_++;

	// �A�j���[�V�����ԍ����擾
	animData_[state].attachNo = MV1AttachAnim(modelId_, animData_[state].animIndex, animData_[state].animHandle);

	// �D�悳����
	animData_[state].isPriority = true;

	// �A�j���[�V���������Ԃ̎擾
	animData_[state].animTotalTime = MV1GetAttachAnimTotalTime(modelId_, animData_[state].attachNo);

	// �t�Đ��̏�����
	if (animData_[state].isReverse)
	{
		animData_[state].stepAnim = animData_[state].animTotalTime;
	}

}

void AnimationController::Dettach(int attachNo)
{
	AttachNum_--;

	// �f�^�b�`����
	MV1DetachAnim(modelId_, attachNo);
}

void AnimationController::ChangeAnimation(std::string state)
{

	// ������Ԃ����������Ȃ�
	if (state == preState_) return;

	state_ = state;

	// �O�̏�Ԃ�
	if (preState_ != "")
	{
		animData_[preState_].isPriority = false;
	}

	animData_[state_].blendRate = 0.0f;

	// �Đ�����A�j���[�V�����̐ݒ�
	Attatch(state);

	// ��O�̏�Ԃ�ۑ�
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

	// ���̃A�j���[�V�����̍Đ����Ԃ��擾
	auto& data = animData_[state_];
	return data.stepAnim;

}

bool AnimationController::GetIsPriority(void)
{
	return animData_[preState_].isPriority;
}

bool AnimationController::IsPlayNowAnimation(void)
{

	// ���̃A�j���[�V�������Đ����Ă��邩�ǂ������肷��
	auto& data = animData_[state_];

	// stepAnim��0.0f�ȏゾ�ƍĐ�����
	if (data.stepAnim > 0.0f)
	{
		return true;
	}

	return false;

}

bool AnimationController::IsEndPlayAnimation(void)
{

	const auto& data = animData_[state_];

	// ���̃A�j���[�V�����̍Đ����I�����Ă��邩����
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

	// ���̃A�j���[�V�����̍Đ����I�����Ă��邩����
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

	// �Đ����Ă���A�j���[�V�������u�����h����Ă��邩�ǂ���
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

	// �u�����h���Ă���A�j���[�V�������Đ��I�����Ă��邩����
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
