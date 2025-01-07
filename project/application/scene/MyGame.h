#pragma once
#include "MNFramework.h"

// ゲーム全体
class MyGame : public MNFramework
{
public:	// メンバ関数

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

private:	// メンバ変数

};

