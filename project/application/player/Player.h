#pragma once
#include "Collider.h"
#include <Input.h>
#include <list>
#include "Object3d.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

// 自キャラ
class Player : public Collider
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filename">ファイル名　フォーマット除く</param>
	void Initialize(const std::string& filename);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:	// メンバ変数
	// オブジェクト
	Object3d* player_ = nullptr;	// 自キャラ
	// キーボード入力
	Input* input_ = nullptr;

	// ファイルパス
	MyBase::ModelFilePath path_{};

	// 自キャラの速さ
	const float kCharacterSpeed_ = 0.2f;

	// その他ステータス
	int hp_;
	bool isDead_ = false;
};

