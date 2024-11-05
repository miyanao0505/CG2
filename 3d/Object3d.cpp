#include "Object3d.h"
#include <fstream>
#include "Object3dBase.h"
#include "../Script/Matrix.h"
#include "../2d/TextureManager.h"

// 初期化
void Object3d::Initislize(Object3dBase* object3dBase)
{
	// 引数で受け取ってメンバ変数に記録する
	object3dBase_ = object3dBase;

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// 平行光源データの作成
	CreateDirectionalLightdata();

	// Transform変数を作る
	transform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
	cameraTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.3f, 0.0f, 0.0f }, { 0.0f, 4.0f, -10.0f } };

}

// 更新処理
void Object3d::Update()
{
	// WorldMatrixの作成
	MyBase::Matrix4x4 worldMatrix = Matrix::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	// cameraMatrixの作成
	MyBase::Matrix4x4 cameraMatrix = Matrix::MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	// viewmatrixの作成
	MyBase::Matrix4x4 viewMatrix = Matrix::Inverse(cameraMatrix);
	// ProjectionMatrixを作って透視投影行列を書き込む
	MyBase::Matrix4x4 projectionMatrix = Matrix::MakePerspectiveFovMatrix(0.45f, float(WindowsAPI::kClientWidth) / float(WindowsAPI::kClientHeight), 0.1f, 500.0f);
	// WVP行列の作成
	MyBase::Matrix4x4 worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, Matrix::Multiply(viewMatrix, projectionMatrix));
	// CBufferの中身を更新
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;
}

// 描画処理
void Object3d::Draw()
{
	// WVP用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());
	// 平行光源用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_.Get()->GetGPUVirtualAddress());

	// 3Dモデルが割り当てられていれば描画する
	if (model_) {
		model_->Draw();
	}
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
}

// 平行光源データ作成
void Object3d::CreateDirectionalLightdata()
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
