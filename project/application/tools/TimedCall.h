#pragma once
#include <functional>

// 時限発動
class TimedCall
{
public:	// メンバ関数
	// コンストラクタ
	TimedCall(std::function<void(void)> f, uint32_t time) : f_(f), time_(time){};
	// 更新
	void Update();
	// 完了ならtrueを返す
	bool IsFinished() { return isFinished_; }

private:	// メンバ変数
	// コールバック
	std::function<void(void)> f_;
	// 残り時間
	uint32_t time_;
	// 完了フラグ
	bool isFinished_ = false;
};

