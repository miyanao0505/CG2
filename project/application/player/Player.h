#pragma once
#include "Collider.h"
#include <Input.h>
#include <list>
#include "Object3d.h"
#include "Sprite.h"
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
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 衝突したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // getter
	MyBase::Vector3 GetWorldPosition() override;
	MyBase::Vector3 Get3DReticleWorldPosition();
	

public:	// setter
	void Set3DReticleFromMouseCursor();

private:	// メンバ変数
	// オブジェクト
	Object3d* player_ = nullptr;	// 自キャラ
	// キーボード入力
	Input* input_ = nullptr;

	// ファイルパス
	MyBase::ModelFilePath path_{};

	// 自キャラの速さ
	const float kCharacterSpeed_ = 0.2f;

	// 3Dレティクル用ワールドトランスフォーム


	// その他ステータス
	int hp_;
	bool isDead_ = false;
};

