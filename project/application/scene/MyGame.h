#pragma once
#include <D3DResourceLeakChecker.h>

#include <WindowsAPI.h>
#include <DirectXBase.h>
#include <Input.h>
#include <SpriteBase.h>
#include <ModelBase.h>
#include <Object3dBase.h>
#include <SrvManager.h>
#include <ImGuiManager.h>
#include <CameraManager.h>
#include <TextureManager.h>
#include <ParticleManager.h>
#include <ModelManager.h>
#include <Sprite.h>
#include <Object3d.h>
#include <ParticleEmitter.h>

// ゲーム全体
class MyGame
{
public:	// メンバ関数

	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();

	// 終了リクエスト
	bool IsEndRequest();

private:	// メンバ変数
	D3DResourceLeakChecker leakCheck;

	// ポインタ
	WindowsAPI* winApi_ = nullptr;
	DirectXBase* dxBase_ = nullptr;
	Input* input_ = nullptr;

	// 共通部の宣言
	SpriteBase* spriteBase_ = nullptr;
	ModelBase* modelBase_ = nullptr;
	Object3dBase* object3dBase_ = nullptr;
	SrvManager* srvManager_ = nullptr;

#ifdef _DEBUG
	// ImGuiManagerの宣言
	ImGuiManager* imGuiManager_ = nullptr;
#endif // _DEBUG

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

