#include "Object3d.h"
#include <fstream>
#include "Object3dBase.h"
#include "ModelManager.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "CameraManager.h"

// 初期化
void Object3d::Initislize()
{
	// 引数で受け取ってメンバ変数に記録する
	object3dBase_ = ModelManager::GetInstance()->GetObject3dBase();

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// 平行光源データの作成
	CreateDirectionalLightData();

	// カメラデータの作成
	CreateCameraData();

	// 点光源データの作成
	CreatePointLightData();

	// Transform変数を作る
	transform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
}

// 更新処理
void Object3d::Update()
{
	// WorldMatrixの作成
	MyBase::Matrix4x4 worldMatrix = Matrix::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	MyBase::Matrix4x4 worldViewProjectionMatrix;
	if (CameraManager::GetInstance()->GetCamera()) {
		const MyBase::Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, viewProjectionMatrix);
		cameraData_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	} else {
		worldViewProjectionMatrix = worldMatrix;
	}
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;
	transformationMatrixData_->WorldInverseTranspose = Matrix::Transpose(Matrix::Inverse(worldMatrix));
}

// 描画処理
void Object3d::Draw()
{
	// WVP用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());
	// 平行光源用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_.Get()->GetGPUVirtualAddress());
	// カメラ用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource_.Get()->GetGPUVirtualAddress());
	// 点光源用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLightResource_.Get()->GetGPUVirtualAddress());

	// 3Dモデルが割り当てられていれば描画する
	if (model_) {
		model_->Draw();
	}
}

void Object3d::SetModel(const std::string& filePath)
{
	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

// 座標変換行列データ作成
void Object3d::CreateTransformationMatrixData()
{
	// TransformationMatrix用のリソースを作る
	transformationMatrixResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::TransformationMatrix));
	// 書き込むためのアドレスを取得
	transformationMatrixResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = Matrix::MakeIdentity4x4();
	transformationMatrixData_->World = Matrix::MakeIdentity4x4();
	transformationMatrixData_->WorldInverseTranspose = Matrix::MakeIdentity4x4();
}

// 平行光源データ作成
void Object3d::CreateDirectionalLightData()
{
	// 平行光源用のリソースを作る
	directionalLightResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::DirectionalLight));
	// 書き込むためのアドレス取得
	directionalLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}

// カメラデータ作成
void Object3d::CreateCameraData()
{
	// カメラ用のリソースを作る
	cameraResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::CameraForGPU));
	// 書き込むためのアドレス取得
	cameraResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0.0f, 0.0f, 0.0f };
	if (CameraManager::GetInstance()->GetCamera()) {
		CameraManager::GetInstance()->FindCamera("default");
		cameraData_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	}
}

// 点光源データ作成
void Object3d::CreatePointLightData()
{
	// 点光源データ用のリソースを作る
	pointLightResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::PointLight));
	// 書き込むためのアドレス取得
	pointLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f, 1.0f, 1.0f,1.0f };
	pointLightData_->position = { 0.0f, 0.0f, 0.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->radius = 1.0f;
	pointLightData_->decay = 1.0f;
}
