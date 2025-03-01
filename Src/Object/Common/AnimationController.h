#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{

public:

	// アニメーションデータ
	struct AnimationData
	{

		// アニメーションハンドル
		int animHandle = -1;

		// アニメーションの状態
		std::string state = "";

		// アタッチNo
		int attachNo = -1;

		// 優先されるアニメーション
		bool isPriority = false;

		// アニメーションの総再生時間
		float animTotalTime = 0.0f;

		// アニメーションスピード
		float speedAnim = 0.0f;

		// 再生中のアニメーション時間
		float stepAnim = 0.0f;

		// アニメーションが始まる時間
		float startTime = 0.0f;

		// アニメーションが終わる時間
		float endStep = 0.0f;

		// ブレンドレート
		float blendRate = 0.0f;

		// ブレンドタイム
		float blendTime = 0.0f;

		// ループ再生するかどうか
		bool isLoop = false;

		// ストップするかどうか
		bool isStop = false;

		// 逆再生するかどうか
		bool isReverse = false;

		// アニメーション番号
		int animIndex = 0;

	};

	// コンストラクタ
	AnimationController(int modelId);

	// デストラクタ
	~AnimationController(void);

	/// <summary>
	/// アニメーション追加
	/// </summary>
	/// <param name="state">アニメーションの名前</param>
	/// <param name="path">アニメーションのパス</param>
	/// <param name="startStep">アニメーションが始まる時間</param>
	/// <param name="speed">アニメーションスピード</param>
	/// <param name="animHandle">アニメーションハンドル</param>
	/// <param name="isLoop">アニメーションのループ再生</param>
	/// <param name="animIndex">アニメーションの番号</param>
	/// <param name="isReverse">アニメーションの逆再生</param>
	void Add(const std::string state, float startStep,
		 float speed, int animHandle, bool isLoop, int animIndex, bool isReverse);

	// アニメーションの更新
	void Update(const float deltaTime);

	// 再生中のアニメーション
	std::string GetPlayType(void) const;

	// アタッチされている数を取得
	int GetAttachNum(void) const;

	// 状態遷移
	void ChangeAnimation(std::string state);

	// アニメーションデータの取得
	AnimationData GetAnimData(const std::string& state);

	// すべてのアニメーションデータを取得
	const std::map <std::string, AnimationData>& GetAnimDatas(void) const;

	// アニメーションが始まる時間をリセット
	void ResetStepAnim();

	// 今のアニメーション時間
	float GetStepAnim();

	// 優先されているアニメーションタイプを取得
	bool GetIsPriority(void);

	// アニメーションが再生されているかどうか
	bool IsPlayNowAnimation(void);

	// 今のアニメーションの再生が終了しているか判定
	bool IsEndPlayAnimation(void);

	// 今のアニメーションの総再生時間の-1.0前が終了しているか判定
	bool IsPreEndPlayAnimation(void);

	// 再生しているアニメーションがブレンドされているかどうか
	bool IsBlendPlay(std::string state);

	// ブレンドしているアニメーションが再生終了しているか判定
	bool IsEndBlendingPlayAnimation(std::string state);

	// ストップさせる
	void Stop(bool stop);

	// スローさせる
	void Slow(bool isSlow);

	// アニメーションスピードを変える
	void SpeedChange(float speed);

private:

	// アニメーションをスローにするレート
	static constexpr float SLOW_RATE = 5.0f;

	// デフォルトのゲームスピード
	static constexpr float DEFAULT_SPEED = 1.0f;

	// ブレンドレート
	static constexpr float BLEND_TIME = 0.3f;

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map <std::string, AnimationData> animData_;

	// 今の状態
	std::string state_;

	// 一個前の状態
	std::string preState_;

	// アタッチしている数
	int AttachNum_;

	// ヒットストップ
	bool isStop_;

	// スピードコントロールする用
	bool isSlow_;

	// ゲーム全体のスピード
	float gameSpeed_;

	// アタッチ
	void Attatch(std::string state);

	// デタッチ
	void Dettach(int attachNo);

};

