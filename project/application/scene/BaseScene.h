#pragma once
#include"Input.h"

// 前方宣言
class SceneManager;

// シーン基底クラス
class BaseScene
{
public:	//メンバ関数
	virtual ~BaseScene() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

public:	// setter
	// シーンマネージャのセット
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:	// 継承
	// シーンマネージャ(借りてくる)
	SceneManager* sceneManager_ = nullptr;
	// 入力
	Input* input_ = nullptr;

};

