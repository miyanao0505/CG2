#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

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

	/// <summary>
	/// 次のシーン予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

public:	// setter
	// シーンファクトリーのセット
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

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

	// シーンファクトリー(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

