#include "TimedCall.h"

// 更新
void TimedCall::Update() {
	if (IsFinished()) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished_ = true;
		// コールバック関数呼び出し
		f_();
	}
}