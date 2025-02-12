#include <DxLib.h>
#include "FpsControl.h"

FpsControl* FpsControl::instance_ = nullptr;

void FpsControl::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new FpsControl();
	}
	instance_->Init();
}

FpsControl& FpsControl::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		FpsControl::CreateInstance();
	}
	return *instance_;
}

FpsControl::FpsControl()
{
	mStartTime = 0;
	mCount = 0;
	mFps = 0;
}

void FpsControl::Init(void)
{
	mStartTime = 0;
	mCount = 0;
	mFps = 0;
}

bool FpsControl::Update()
{

	if (mCount == 0)
	{
		//1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if (mCount == N)
	{
		//60フレーム目なら平均を計算する
		int t = GetNowCount();
		mFps = static_cast<float>(MS_PER_SECOND) / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}

	mCount++;
	return true;

}

void FpsControl::Draw()
{
	DrawFormatString(0, 0, GetColor(COLOR_MAX, COLOR_MAX, COLOR_MAX), "%.1f", mFps);
}

void FpsControl::Wait()
{

	//かかった時間
	int tookTime = GetNowCount() - mStartTime;	

	//待つべき時間
	int waitTime = mCount * MS_PER_SECOND / FPS - tookTime;	
	if (waitTime > 0)
	{
		//待機
		Sleep(waitTime);	
	}

}

