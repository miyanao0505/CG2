#include "ClearScene.h"
#include <imgui.h>
#include "CameraManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include"SceneManager.h"
#include "MyTools.h"

void ClearScene::Initialize()
{
	BaseScene::Initialize();

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(filePath1_);

	// スプライト
	for (uint32_t i = 0; i < 1; ++i)
	{
		// スプライトの初期化
		Sprite* sprite = new Sprite();
		sprite->Initialize(filePath1_);
		sprite->SetAnchorPoint({ 0.0f, 0.0f });
		sprite->SetIsFlipX(false);
		sprite->SetIsFlipY(false);
		sprites_.push_back(sprite);
	}

	sprites_[0]->SetPosition({ 0.0f, 0.0f });
	sprites_[0]->SetSize({ 1280.0f, 720.f });
}

void ClearScene::Finalize()
{
	BaseScene::Finalize();

	// スプライト
	for (Sprite* sprite : sprites_)
	{
		delete sprite;
	}
}

void ClearScene::Update()
{
	BaseScene::Update();

	// ENTERキーを押したら
	if (input_->TriggerKey(DIK_RETURN)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	// スプライトの更新処理
	for (Sprite* sprite : sprites_)
	{
		sprite->Update();
	}
}

void ClearScene::Draw()
{
#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	for (Sprite* sprite : sprites_)
	{
		sprite->Draw();
	}

#pragma endregion スプライト
}
