#include <math.h>
#include "DxLib.h"

class FpsControl {

public:

	//測定開始時刻
	int mStartTime;

	//カウンタ
	int mCount;

	//fps
	float mFps;

	//平均を取るサンプル数
	static constexpr int N = 60;

	//設定したFPS
	static constexpr int FPS = 60;	

	// 1秒あたりのミリ秒
	static constexpr int MS_PER_SECOND = 1000;

	// 色の最大値
	static constexpr int COLOR_MAX = 255;

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

	// シングルトン用インスタンス
	static FpsControl* instance_;
	
};
