#pragma once
#include "BaseScene.h"
#include "Input.h"

// シーン管理
class SceneManager
{
public:	// メンバ関数
	// シングルトンインスタンスの取得
	static SceneManager* GetInstance();

	// 終了
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 次のシーン予約
	void SetNextScene(BaseScene* nexxtScene) { nextScene_ = nexxtScene; }

private:	// シングルトンインスタンス
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = default;
	SceneManager& operator=(SceneManager&) = delete;

private:	// メンバ変数
	// 今のシーン(実行中シーン)
	BaseScene* scene_ = nullptr;

	// 次のシーン
	BaseScene* nextScene_ = nullptr;
};

