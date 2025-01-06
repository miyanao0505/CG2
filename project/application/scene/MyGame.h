#pragma once
#include <MNFramework.h>
#include <SpriteBase.h>
#include <ModelBase.h>
#include <Object3dBase.h>
#include <Sprite.h>
#include <Object3d.h>
#include <ParticleEmitter.h>

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
	// 共通部の宣言
	SpriteBase* spriteBase_ = nullptr;
	ModelBase* modelBase_ = nullptr;
	Object3dBase* object3dBase_ = nullptr;

#pragma region シーン初期化
	// テクスチャファイルパス
	std::string filePath1_ = { "resources/uvChecker.png" };
	std::string filePath2_ = { "resources/monsterBall.png" };
	//std::string filePath3_ = { "resources/fence.png" };
	//std::string filePath4_ = { "resources/circle.png" };

	// スプライト
	std::vector<Sprite*> sprites_;

	// モデルファイルパス
	MyBase::ModelFilePath modelFilePath1_ = { {"resources/plane"}, {"plane.obj"} };
	MyBase::ModelFilePath modelFilePath2_ = { {"resources/axis"}, {"axis.obj"} };
	MyBase::ModelFilePath modelFilePath3_ = { {"resources/fence"}, {"fence.obj"} };

	// 3Dオブジェクト
	std::vector<Object3d*> objects_;

	// パーティクル
	ParticleEmitter* particleEmitter_ = nullptr;
#pragma endregion シーン初期化

#pragma region 変数
	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
#pragma endregion 変数
};

