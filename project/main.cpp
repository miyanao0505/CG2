#include <string>
#include <vector>
#include "Input.h"
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "D3DResourceLeakChecker.h"
#include "ImGuiManager.h"
#include "SrvManager.h"
#include "CameraManager.h"
#include "SpriteBase.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "MyTools.h"
#include "Matrix.h"
#include "MyBase.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#pragma comment(lib, "dxcompiler.lib")

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	D3DResourceLeakChecker leakCheck;

#pragma region ポインタ置き場
	// ポインタ
	WindowsAPI* winApi = nullptr;
	DirectXBase* dxBase = nullptr;
	Input* input = nullptr;
#pragma endregion ポインタ置き場

#pragma region ゲームウィンドウ作成
	// WindowsAPIの初期化
	winApi = new WindowsAPI();
	winApi->Initialize();
#pragma endregion ゲームウィンドウ作成

#pragma region DirectX初期化
	// DirectXBaseの初期化
	dxBase = new DirectXBase();
	dxBase->Initialize(winApi);
#pragma endregion DirectX初期化

#pragma region 汎用機能初期化
	// 入力の初期化
	input = new Input();
	input->Initialize(winApi);
#pragma endregion 汎用機能初期化

#pragma region 基盤システム初期化
	// 共通部の宣言
	SpriteBase* spriteBase = nullptr;
	ModelBase* modelBase = nullptr;
	Object3dBase* object3dBase = nullptr;
	SrvManager* srvManager = nullptr;

	// SRVマネージャーの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(dxBase);

	// スプライト共通部の初期化
	spriteBase = new SpriteBase;
	spriteBase->Initialize(dxBase);

	// モデル共通部の初期化
	modelBase = new ModelBase;
	modelBase->Initislize(dxBase);

	// 3Dオブジェクト共通部の初期化
	object3dBase = new Object3dBase;
	object3dBase->Initislize(dxBase);
#pragma endregion 基盤システム初期化

#ifdef _DEBUG
#pragma region ImGui初期化
	// ImGuiManagerの初期化
	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApi, dxBase, srvManager);
#pragma endregion
#endif // _DEBUG

#pragma region シーン初期化
	// マネージャー(インスタンス)の初期化
	CameraManager::GetInstance()->Initialize();						// カメラマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxBase, srvManager);	// テクスチャマネージャの初期化
	ModelManager::GetInstance()->Initialize(dxBase);				// 3Dモデルマネージャの初期化
	ParticleManager::GetInstance()->Initialize(dxBase, srvManager);	// パーティクルマネージャの初期化

	// カメラの設定
	CameraManager::GetInstance()->SetCamera("default");
	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetRotate({0.3f,0.0f,0.0f});
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 0.0f,4.0f,-10.0f });
	CameraManager::GetInstance()->SetCamera("sub");
	CameraManager::GetInstance()->FindCamera("sub");
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.3f, 3.1f, 0.0f });
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 0.0f, 4.0f, 10.0f });

	CameraManager::GetInstance()->FindCamera("default");

	// テクスチャファイルパス
	std::string filePath1 = { "resources/uvChecker.png" };
	//std::string filePath2 = { "resources/monsterBall.png" };
	//std::string filePath3 = { "resources/fence.png" };
	std::string filePath4 = { "resources/circle.png" };

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(filePath1);
	//TextureManager::GetInstance()->LoadTexture(filePath2);
	//TextureManager::GetInstance()->LoadTexture(filePath3);
	TextureManager::GetInstance()->LoadTexture(filePath4);

	// スプライト
	std::vector<Sprite*> sprites;
	for (uint32_t i = 0; i < 5; ++i)
	{
		// スプライトの初期化
		Sprite* sprite = new Sprite();
		//sprite->Initialize(spriteBase, "resources/uvChecker.png");
		sprite->Initialize(spriteBase, filePath1);
		sprite->SetPosition({ 200.0f * float(i), 100.0f });
		sprite->SetSize({ 100.f, 100.f });
		sprite->SetAnchorPoint({ 0.0f, 0.0f });
		sprite->SetIsFlipX(false);
		sprite->SetIsFlipY(false);
		sprites.push_back(sprite);
	}

	sprites[1]->SetTexture(filePath4);
	//sprites[1]->SetTexture("resources/monsterBall.png");
	sprites[1]->SetSize({ 100.0f, 100.0f });
	sprites[3]->SetTexture(filePath4);
	//sprites[3]->SetTexture("resources/monsterBall.png");
	sprites[3]->SetSize({ 100.0f, 100.0f });
	
	// モデルファイルパス
	MyBase::ModelFilePath modelFilePath1 = { {"resources/plane"}, {"plane.obj"} };
	MyBase::ModelFilePath modelFilePath2 = { {"resources/axis"}, {"axis.obj"} };
	MyBase::ModelFilePath modelFilePath3 = { {"resources/fence"}, {"fence.obj"} };

	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel(modelFilePath1.directoryPath, modelFilePath1.filename);
	ModelManager::GetInstance()->LoadModel(modelFilePath2.directoryPath, modelFilePath2.filename);
	ModelManager::GetInstance()->LoadModel(modelFilePath3.directoryPath, modelFilePath3.filename);

	// 3Dオブジェクト
	std::vector<Object3d*> objects;
	for (uint32_t i = 0; i < 3; ++i) {
		// 3Dオブジェクトの初期化
		Object3d* object = new Object3d;
		object->Initislize(object3dBase);
		object->SetTranslate({ -2.5f + i * 2.5f, 0.0f, 0.0f });
		object->SetModel(modelFilePath1.filename);
		objects.push_back(object);
	}
	objects[1]->SetModel(modelFilePath2.filename);
	objects[2]->SetModel(modelFilePath3.filename);
	
	// パーティクル
	ParticleEmitter* particleEmitter = nullptr;
	particleEmitter = new ParticleEmitter;
	particleEmitter->Initialize("circle", "resources/circle.png");
#pragma endregion シーン初期化

#pragma region 変数
	bool isAccelerationField = false;
	MyBase::Vector3 acceleration = { 15.0f, 0.0f, 0.0f };
	MyBase::AABB area{ .min{-1.0f, -1.0f, -1.0f}, .max{1.0f, 1.0f, 1.0f} };

	// デルタイム
	const float kDeltaTime = 1.0f / 60.0f;
#pragma endregion 変数

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {	// ゲームループ
		// Windowsのメッセージ処理
		if (winApi->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		}
		
		// 入力の更新
		input->Update();

		// ImGuiにここからフレームが始まる旨を告げる
#ifdef _DEBUG
		imGuiManager->Begin();
#endif // _DEBUG

#ifdef _DEBUG
		// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

		// デモウィンドウの表示オン
		//ImGui::ShowDemoWindow();

		ImGui::Begin("Settings");

		//// カメラ
		//if (ImGui::CollapsingHeader("Camera"))
		//{
		//	// 変更するための変数
		//	MyBase::Transform transformCamera{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

		//	static ImGuiComboFlags flags = 0;
		//	const char* cameraNames[] = { "default", "sub" };
		//	static int cameraIndex = 0;

		//	const char* cameraNowVlue = cameraNames[cameraIndex];

		//	if (ImGui::BeginCombo("Now Camera", cameraNowVlue, flags))
		//	{
		//		for (int i = 0; i < IM_ARRAYSIZE(cameraNames); i++)
		//		{
		//			const bool isSelected = (cameraIndex == i);
		//			if (ImGui::Selectable(cameraNames[i], isSelected)) {
		//				cameraIndex = i;
		//				CameraManager::GetInstance()->FindCamera(cameraNames[i]);
		//			}

		//			if (isSelected) {
		//				ImGui::SetItemDefaultFocus();
		//			}
		//		}
		//		ImGui::EndCombo();
		//	}

		//	transformCamera.translate = CameraManager::GetInstance()->GetCamera()->GetTranslate();
		//	ImGui::DragFloat3("translate", &transformCamera.translate.x, 0.05f);
		//	CameraManager::GetInstance()->GetCamera()->SetTranslate(transformCamera.translate);
		//	transformCamera.rotate = CameraManager::GetInstance()->GetCamera()->GetRotate();
		//	ImGui::DragFloat3("rotate", &transformCamera.rotate.x, 0.05f);
		//	CameraManager::GetInstance()->GetCamera()->SetRotate(transformCamera.rotate);

		//	ImGui::Text("\n");
		//}
		//
	
		//// スプライト
		if (ImGui::CollapsingHeader("Sprite"))
		{
			// ブレンドモード
			if (ImGui::CollapsingHeader("BlendModeSprite")) {
				static ImGuiComboFlags spriteFlags = 0;
				const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
				static int selectID = 1;

				const char* previewValue = blendModeIndex[selectID];

				if (ImGui::BeginCombo("now Blend", previewValue, spriteFlags))
				{
					for (int n = 0; n < IM_ARRAYSIZE(blendModeIndex); n++)
					{
						const bool isSelected = (selectID == n);
						if (ImGui::Selectable(blendModeIndex[n], isSelected)) {
							selectID = n;
							spriteBase->SetBlendMode(static_cast<SpriteBase::BlendMode>(n));
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}
			for (Sprite* sprite : sprites)
			{
				ImGui::PushID(sprite);
				if (ImGui::CollapsingHeader("Object"))
				{
					// 移動
					MyBase::Vector2 translate = sprite->GetPosition();
					ImGui::SliderFloat2("Translate", &translate.x, 0.0f, 640.0f);
					sprite->SetPosition(translate);
					// 回転
					float rotation = sprite->GetRotation();
					ImGui::SliderAngle("Rotate", &rotation);
					sprite->SetRotation(rotation);
					// 拡縮
					MyBase::Vector2 size = sprite->GetSize();
					ImGui::SliderFloat2("Scale", &size.x, 0.0f, 640.f);
					sprite->SetSize(size);
					// アンカーポイント
					MyBase::Vector2 anchorPoint = sprite->GetAnchorPoint();
					ImGui::DragFloat2("AnchorPoint", &anchorPoint.x, 0.05f, -1.0f, 2.0f);
					sprite->SetAnchorPoint(anchorPoint);
					// フリップ
					bool isFlipX = sprite->GetIsFlipX();
					ImGui::Checkbox("isFlipX", &isFlipX);
					sprite->SetIsFlipX(isFlipX);
					bool isFlipY = sprite->GetIsFlipY();
					ImGui::Checkbox("isFlipY", &isFlipY);
					sprite->SetIsFlipY(isFlipY);
					// テクスチャ範囲指定
					MyBase::Vector2 textureLeftTop = sprite->GetTextureLeftTop();
					ImGui::SliderFloat2("textureLeftTop", &textureLeftTop.x, 0.0f, max(sprite->GetSpriteSize().x, sprite->GetSpriteSize().y));
					sprite->SetTextureLeftTop(textureLeftTop);
					MyBase::Vector2 textureSize = sprite->GetTextureSize();
					ImGui::SliderFloat2("textureSize", &textureSize.x, 0.0f, max(sprite->GetSpriteSize().x, sprite->GetSpriteSize().y) * 2.f);
					sprite->SetTextureSize(textureSize);

					if (ImGui::CollapsingHeader("Material"))
					{
						// 色
						MyBase::Vector4 color = sprite->GetColor();
						ImGui::ColorEdit4("color", &color.x);
						sprite->SetColor(color);
					}
				}
				ImGui::PopID();
			}
		}

		// 3Dオブジェクト
		MyBase::Vector3 rotate = objects[0]->GetRotate();
		rotate.y += 0.02f;
		objects[0]->SetRotate(rotate);
		rotate = objects[1]->GetRotate();
		rotate.z += 0.02f;
		objects[1]->SetRotate(rotate);
		if (ImGui::CollapsingHeader("3dObject"))
		{
			// ブレンドモード
			if (ImGui::CollapsingHeader("BlendMode3dObject")) {
				static ImGuiComboFlags spriteFlags = 0;
				const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
				static int selectID = 1;

				const char* previewValue = blendModeIndex[selectID];

				if (ImGui::BeginCombo("now Blend", previewValue, spriteFlags))
				{
					for (int n = 0; n < IM_ARRAYSIZE(blendModeIndex); n++)
					{
						const bool isSelected = (selectID == n);
						if (ImGui::Selectable(blendModeIndex[n], isSelected)) {
							selectID = n;
							object3dBase->SetBlendMode(static_cast<Object3dBase::BlendMode>(n));
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}

			for (Object3d* object : objects)
			{
				ImGui::PushID(object);
				if (ImGui::CollapsingHeader("Object"))
				{
					MyBase::Transform transform{ object->GetScale(), object->GetRotate(), object->GetTranslate() };

					// 移動
					ImGui::SliderFloat3("Translate", &transform.translate.x, -5.0f, 5.0f);
					object->SetTranslate(transform.translate);
					// 回転
					ImGui::SliderFloat3("Rotate", &transform.rotate.x, -3.14f, 3.14f);
					object->SetRotate(transform.rotate);
					// 拡縮
					ImGui::SliderFloat3("Scale", &transform.scale.x, 0.0f, 3.0f);
					object->SetScale(transform.scale);

					if (ImGui::CollapsingHeader("Material"))
					{
						// 平行光源フラグ
						bool isEnableLighting = true;
						//isEnableLighting = object->GetEnableLighting();

						if (isEnableLighting)
						{
							// 平行光源
							MyBase::DirectionalLight directionalLight{};
							// 色
							directionalLight.color = object->GetDirectionalLightColor();
							ImGui::ColorEdit4("LightColor", &directionalLight.color.x);
							object->SetDirectionalLightColor(directionalLight.color);
							// 方向
							directionalLight.direction = object->GetDirectionalLightDirection();
							ImGui::SliderFloat3("LightDirection", &directionalLight.direction.x, -1, 1);
							object->SetDirectionalLightDirection(directionalLight.direction);
							// 輝度
							directionalLight.intensity = object->GetDirectionalLightIntensity();
							ImGui::DragFloat("Intensity", &directionalLight.intensity, 0.01f);
							object->SetDirectionalLightIntensity(directionalLight.intensity);
						}
					}
				}
				ImGui::PopID();
			}
		}
		if (ImGui::CollapsingHeader("particle")) {
			static ImGuiComboFlags particleFlags = 0;
			const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
			static int selectID = 2;

			const char* previewValue = blendModeIndex[selectID];

			if (ImGui::BeginCombo("Now Blend", previewValue, particleFlags))
			{
				for (int n = 0; n < IM_ARRAYSIZE(blendModeIndex); n++)
				{
					const bool isSelected = (selectID == n);
					if (ImGui::Selectable(blendModeIndex[n], isSelected)) {
						selectID = n;
						ParticleManager::GetInstance()->ChangeBlendMode(static_cast<ParticleBase::BlendMode>(n));
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			/*size_t spriteCount = 0;
			for (ParticleEmitter* particle : sprites) {*/
			MyBase::Vector3 position = particleEmitter->GetPosition();
			ImGui::DragFloat2("particleEmitter_.Translate", &position.x, 0.1f);
			/*if (position.y > 640.0f) {
				position.y = 640.0f;
			}*/
			particleEmitter->SetPosition(position);

			/*Vector3 rotation = particleEmitter_->GetRotation();
			ImGui::SliderAngle("particleEmitter_.Rotate", &rotation.x);
			particleEmitter_->SetRotation(rotation);

			Vector3 size = particleEmitter_->GetSize();
			ImGui::DragFloat2("particleEmitter_.Scale", &size.x, 0.1f);
			if (size.y > 360.0f) {
				size.y = 360.0f;
			}
			particleEmitter_->SetSize(size);*/

			int count = particleEmitter->GetCount();
			ImGui::DragInt("particleEmitter_.count", &count, 1, 0, 1000);
			particleEmitter->SetCount(count);

			float frequency = particleEmitter->GetFrequency();
			ImGui::DragFloat("particleEmitter_.frequency", &frequency, 0.1f);
			particleEmitter->SetFrequency(frequency);

			if (ImGui::Button("ParticleEmit", { 100,50 })) {
				particleEmitter->Emit();
			}

			bool isEmitUpdate = particleEmitter->GetIsEmitUpdate();
			ImGui::Checkbox("IsEmitUpdate", &isEmitUpdate);
			particleEmitter->SetIsEmitUpdate(isEmitUpdate);

			ImGui::Checkbox("IsAccelerationField", &isAccelerationField);

			//ImGui::Text("\n");

			//}
		}

		//// テクスチャ
		////ImGui::Checkbox("useMonsterBall", &useMonsterBall);

		//// UV
		///*ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		//ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		//ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);*/

		ImGui::End();
#endif // _DEBUG

		// カメラ
		CameraManager::GetInstance()->GetCamera()->Update();

		// 3Dオブジェクトの更新処理
		for (Object3d* object : objects)
		{
			object->Update();
		}

		if (isAccelerationField) {
			for (std::pair<const std::string, std::unique_ptr<ParticleManager::ParticleGroup>>& pair : ParticleManager::GetInstance()->GetParticleGroups()) {
				ParticleManager::ParticleGroup& group = *pair.second;
				int index = 0;
				for (std::list<MyBase::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
					MyBase::Particle& particle = *it;

					if (MyTools::IsCollision(area, particle.transform.translate)) {
						particle.velocity = MyTools::Add(particle.velocity, MyTools::Multiply(kDeltaTime, acceleration));
					}

					++it;
					++index;
				}
			}
		}

		// パーティクルの更新処理
		particleEmitter->Update();
		ParticleManager::GetInstance()->Update();

		// スプライトの更新処理
		for (Sprite* sprite : sprites)
		{
			sprite->Update();
		}

		// UVTransform用
		/*Matrix::Matrix4x4 uvTransformMatrix = Matrix::MakeScaleMatrix(uvTransformSprite.scale);
		uvTransformMatrix = Matrix::Multiply(uvTransformMatrix, Matrix::MakeRotateZMatrix4x4(uvTransformSprite.rotate.z));
		uvTransformMatrix = Matrix::Multiply(uvTransformMatrix, Matrix::MakeTranslateMatrix(uvTransformSprite.translate));*/
		//materialDataSprite->uvTransform = uvTransformMatrix;

		// ImGuiの内部コマンドを生成する
#ifdef _DEBUG
		imGuiManager->End();
#endif // _DEBUG

		// DirectXの描画前処理。全ての描画に共通のグラフィックスコマンドを積む
		dxBase->PreDraw();
		srvManager->PreDraw();

#pragma region 3Dオブジェクト

		// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
		object3dBase->SetCommonScreen();

		// 全ての3DObject個々の描画
		/*for (Object3d* object : objects)
		{
			object->Draw();
		}*/

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

		// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
		ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

		// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
		spriteBase->SetCommonScreen();

		// 全てのSprite個々の描画
		/*for (Sprite* sprite : sprites)
		{
			sprite->Draw();
		}*/

#pragma endregion スプライト

		// 実際のcommandListのImGuiの描画コマンドを積む
#ifdef _DEBUG
		imGuiManager->Draw();
#endif // _DEBUG

		// 描画後処理
		dxBase->PostDraw();
	}

	// COMの終了処理
	CoUninitialize();

	// 解放処理
	// パーティクルマネージャの終了
	ParticleManager::GetInstance()->Finalize();
	// 3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	// テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	// カメラマネージャの終了
	CameraManager::GetInstance()->Finalize();
	// ImGuiの終了処理
#ifdef _DEBUG
	imGuiManager->Finalize();
	// ImGui解放
	delete imGuiManager;
#endif // _DEBUG
	// パーティクル
	delete particleEmitter;
	// 3Dオブジェクト
	for (Object3d* object : objects) 
	{
		delete object;
	}
	// スプライト
	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}
	// srvマネージャー
	delete srvManager;
	// 3Dオブジェクト共通部
	delete object3dBase;
	// モデル共通部
	delete modelBase;
	// スプライト共通部
	delete spriteBase;
	// 入力解放
	delete input;
	// DirectX解放
	delete dxBase;

	// WindowsAPIの終了処理
	winApi->Finalize();

	// WindowsAPIの解放
	delete winApi;

	return 0;
}