#pragma once
#include "BaseScene.h"
#include <string>
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "Player.h"
#include"Skydome.h"
#include "MyBase.h"

// クリアシーン
class ClearScene : public BaseScene
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
#pragma region シーン初期化
	// テクスチャファイルパス
	std::string filePath1_ = { "resources/clear.png" };
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
	//std::vector<Object3d*> objects_;
	Player* player_ = nullptr;
	Skydome* skydome_ = nullptr;

	// パーティクル
	//ParticleEmitter* particleEmitter_ = nullptr;
#pragma endregion シーン初期化

	//bool isParticleActive_;
	//bool isAccelerationField_;
	//MyBase::Vector3 acceleration_{};
	//MyBase::AABB area_{};

	//// デルタイム
	//const float kDeltaTime_ = 1.0f / 60.0f;
};

