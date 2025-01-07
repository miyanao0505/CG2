#include "MyGame.h"
#include"SceneFactory.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#pragma comment(lib, "dxcompiler.lib")

// 初期化
void MyGame::Initialize()
{
	// 基底クラスの初期化処理
	MNFramework::Initialize();

	// シーンファクトリーを生成し、マネージャにセット
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	// シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

// 終了
void MyGame::Finalize()
{
	// COMの終了処理
	CoUninitialize();

	// 基底クラスの終了処理
	MNFramework::Finalize();
}

// 毎フレーム更新
void MyGame::Update()
{
	// 基底クラスの更新処理
	MNFramework::Update();
}

// 描画
void MyGame::Draw()
{
	// DirectXの描画前処理。全ての描画に共通のグラフィックスコマンドを積む
	dxBase_->PreDraw();
	srvManager_->PreDraw();

	// カメラの更新
	cameraManager_->GetCamera()->Update();

	// シーンマネージャの描画
	sceneManager_->Draw();

	// 実際のcommandListのImGuiの描画コマンドを積む
#ifdef _DEBUG
	imGuiManager_->Draw();
#endif // _DEBUG

	// 描画後処理
	dxBase_->PostDraw();

}
