#include <string>
#include <format>
#include <wrl.h>
#include <cassert>
#include <sstream>
#include <vector>
#include "Input.h"
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "D3DResourceLeakChecker.h"
#include "SpriteBase.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "MyTools.h"
#include "Matrix.h"
#include "MyBase.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
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

#pragma region シーン初期化
	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxBase);

	// テクスチャファイルパス
	std::string filePath1 = { "resources/uvChecker.png" };
	std::string filePath2 = { "resources/monsterBall.png" };
	//std::string filePath3 = { "resources/fence.png" };

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(filePath1);
	TextureManager::GetInstance()->LoadTexture(filePath2);
	//TextureManager::GetInstance()->LoadTexture(filePath3);

	// スプライト
	std::vector<Sprite*> sprites;
	for (uint32_t i = 0; i < 5; ++i)
	{
		// スプライトの初期化
		Sprite* sprite = new Sprite();
		//sprite->Initialize(spriteBase, "resources/uvChecker.png");
		sprite->Initialize(spriteBase, filePath1);
		sprite->SetPosition({ 200.0f * float(i), 0.0f });
		sprite->SetSize({ 100.f, 100.f });
		sprite->SetAnchorPoint({ 0.0f, 0.0f });
		sprite->SetIsFlipX(false);
		sprite->SetIsFlipY(false);
		sprites.push_back(sprite);
	}

	sprites[1]->SetTexture(filePath2);
	//sprites[1]->SetTexture("resources/monsterBall.png");
	sprites[1]->SetSize({ 100.0f, 100.0f });
	sprites[3]->SetTexture(filePath2);
	//sprites[3]->SetTexture("resources/monsterBall.png");
	sprites[3]->SetSize({ 100.0f, 100.0f });

	// 3Dモデルマネージャの初期化
	ModelManager::GetInstance()->Initialize(dxBase);

	// モデルファイルパス
	MyBase::ModelFilePath modelFilePath1 = { {"resources/plane"}, {"plane.obj"} };
	//MyBase::ModelFilePath modelFilePath2 = { {"resources/axis"}, {"axis.obj"} };
	//MyBase::ModelFilePath modelFilePath3 = { {"resources/fence"}, {"fence.obj"} };

	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel(modelFilePath1.directoryPath, modelFilePath1.filename);
	//ModelManager::GetInstance()->LoadModel(modelFilePath2.directoryPath, modelFilePath2.filename);
	//ModelManager::GetInstance()->LoadModel(modelFilePath3.directoryPath, modelFilePath3.filename);

	// 3Dオブジェクト
	std::vector<Object3d*> objects;
	for (uint32_t i = 0; i < 1; ++i) {
		// 3Dオブジェクトの初期化
		Object3d* object = new Object3d;
		object->Initislize(object3dBase);
		object->SetTranslate({ -2.f + i * 4.f, 0.0f, 0.0f });
		object->SetModel(modelFilePath1.filename);
		objects.push_back(object);
	}
	//objects[1]->SetModel(modelFilePath3.filename);
	
#pragma endregion シーン初期化

	// ブレンドモード
	//enum BlendMode {
	//	kBlendModeNone,			// ブレンドなし
	//	kBlendModeNormal,		// 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	//	kBlendModeAdd,			// 加算。 Src * SrcA + Dest * 1
	//	kBlendModeSubtract,		// 減算。 Dest * 1 - Src * SrcA
	//	kBlendModeMultiply,		// 乗算。 Src * 0 + Dest * Src
	//	kBlendModeScreen,		// スクリーン。 Src * (1 - Dest) + Dest * 1
	//	kCountOfBlendMode,		// 利用してはいけない
	//};

	// ブレンドモード切替用
	//BlendMode blendMode = kBlendModeNone;
	//int blendIndex = 0;

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
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
#endif // _DEBUG

		// Transformの更新(Y軸回転)
		//transform.rotate.y += 1.f / 360.f * float(M_PI);

#ifdef _DEBUG
		// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
		//ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);							// ウィンドウの座標(プログラム起動時のみ読み込み)
		//ImGui::SetNextWindowSize(ImVec2(500, 80), ImGuiCond_Once);							// ウィンドウのサイズ(プログラム起動時のみ読み込み)

		//ImGui::Begin("Camera");
		//ImGui::DragFloat3("translate", &cameraTransform.translate.x, 0.05f);
		//ImGui::DragFloat3("rotate", &cameraTransform.rotate.x, 0.05f);
		//ImGui::End();

		MyBase::Transform transform{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		MyBase::Transform transformSprite{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

		//ImGui::SetNextWindowPos(ImVec2(850, 20), ImGuiCond_Once);							// ウィンドウの座標(プログラム起動時のみ読み込み)
		//ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);							// ウィンドウのサイズ(プログラム起動時のみ読み込み)

		ImGui::Begin("Settings");

		// カメラ
		/*ImGui::DragFloat3("CameraTranslate", &cameraTransform.translate.x, 0.05f);
		ImGui::SliderAngle("CameraRotateX", &cameraTransform.rotate.x);
		ImGui::SliderAngle("CameraRotateY", &cameraTransform.rotate.y);
		ImGui::SliderAngle("CameraRotateZ", &cameraTransform.rotate.z);*/
		
		// 球 / Model
		/*ImGui::DragFloat3("PlaneTranslate", &transform.translate.x, 0.05f);
		ImGui::SliderAngle("PlaneRotateX", &transform.rotate.x);
		ImGui::SliderAngle("PlaneRotateY", &transform.rotate.y);
		ImGui::SliderAngle("PlaneRotateZ", &transform.rotate.z);
		ImGui::ColorEdit4("color", &materialData[0].color.x);
		ImGui::Checkbox("enableLighting", (bool*)&materialData[0].enableLighting);*/

		// スプライト
		uint32_t objectIDIndex = 0;
		for(Sprite* sprite : sprites)
		{
			ImGui::PushID(objectIDIndex);
			if (ImGui::CollapsingHeader("Object"))
			{
				// 移動
				transformSprite.translate = { sprite->GetPosition().x, sprite->GetPosition().y, 0.0f };
				ImGui::SliderFloat3("Translate", &transformSprite.translate.x, 0.0f, 640.0f);
				sprite->SetPosition(MyBase::Vector2(transformSprite.translate.x, transformSprite.translate.y));
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
			++objectIDIndex;
		}

		// 3Dオブジェクト
		for (Object3d* object : objects)
		{
			ImGui::PushID(objectIDIndex);
			if (ImGui::CollapsingHeader("Object"))
			{
				// 移動
				transform.translate = object->GetTranslate();
				ImGui::SliderFloat3("Translate", &transform.translate.x, -640.f, 640.0f);
				object->SetTranslate(transform.translate);
				// 回転
				transform.rotate = object->GetRotate();
				ImGui::SliderFloat3("Rotate", &transform.rotate.x, -3.14f, 3.14f);
				object->SetRotate(transform.rotate);
				// 拡縮
				transform.scale = object->GetScale();
				ImGui::SliderFloat3("Scale", &transform.scale.x, 0.0f, 3.0f);
				object->SetScale(transform.scale);

				if (ImGui::CollapsingHeader("Material"))
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
			ImGui::PopID();
			++objectIDIndex;
		}

		// テクスチャ
		//ImGui::Checkbox("useMonsterBall", &useMonsterBall);

		/*const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
		ImGui::Combo("Blend", &blendIndex, blendModeIndex, IM_ARRAYSIZE(blendModeIndex));
		blendMode = (BlendMode)blendIndex;*/

		// ブレンドモード
		/*switch (blendMode)
		{
		case kBlendModeNone:
			blendDesc.RenderTarget[0].BlendEnable = false;
			break;
		case kBlendModeNormal:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			break;
		case kBlendModeAdd:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		case kBlendModeSubtract:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		case kBlendModeMultiply:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
			break;
		case kBlendModeScreen:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		default:
			break;
		}*/

		// UV
		/*ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);*/

		ImGui::End();
#endif // _DEBUG

		// 3Dオブジェクトの更新処理
		for (Object3d* object : objects)
		{
			object->Update();
		}

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
		ImGui::Render();
#endif // _DEBUG

		// DirectXの描画前処理。全ての描画に共通のグラフィックスコマンドを積む
		dxBase->PreDraw();

#pragma region 3Dオブジェクト

		// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
		object3dBase->SetCommonScreen();

		// 全ての3DObject個々の描画
		for (Object3d* object : objects)
		{
			object->Draw();
		}

#pragma endregion 3Dオブジェクト

#pragma region スプライト

		// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
		spriteBase->SetCommonScreen();

		// 全てのSprite個々の描画
		for (Sprite* sprite : sprites)
		{
			sprite->Draw();
		}

#pragma endregion スプライト

		// 実際のcommandListのImGuiの描画コマンドを積む
#ifdef _DEBUG
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxBase->GetCommandList());
#endif // _DEBUG

		// 描画後処理
		dxBase->PostDraw();

	}

	// COMの終了処理
	CoUninitialize();

	// ImGuiの終了処理
#ifdef _DEBUG
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // _DEBUG

	// 解放処理
	// 3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	// テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
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