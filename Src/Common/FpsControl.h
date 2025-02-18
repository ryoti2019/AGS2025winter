#include <math.h>
#include "DxLib.h"

class FpsControl {

public:

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static FpsControl& GetInstance(void);
	FpsControl();
	void Init(void);
	bool Update();
	void Draw();
	void Wait();

private:

	//平均を取るサンプル数
	static constexpr int FPS_AVERAGE_FRAME_COUNT = 60;

	//設定したFPS
	static constexpr int FPS = 60;

	// 1秒あたりのミリ秒
	static constexpr int MS_PER_SECOND = 1000;

	// 色の最大値
	static constexpr int COLOR_MAX = 255;

	// シングルトン用インスタンス
	static FpsControl* instance_;
	
	//測定開始時刻
	int mStartTime;

	//カウンタ
	int mCount;

	//fps
	float mFps;

};
