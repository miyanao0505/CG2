#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Model.h"
#include "MyBase.h"

// 前方宣言
class Object3dBase;

// 3Dオブジェクト
class Object3d
{
public:	// メンバ関数
	// 初期化
	void Initislize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

public:	// getter
	const MyBase::Vector3& GetScale() const { return transform_.scale; }
	const MyBase::Vector3& GetRotate() const { return transform_.rotate; }
	const MyBase::Vector3& GetTranslate() const { return transform_.translate; }
	const MyBase::DirectionalLight& GetDirectionalLight() const { return *directionalLightData_; }
	const MyBase::PointLight& GetPointLight() const { return *pointLightData_; }
	const MyBase::SpotLight& GetSpotLight() const { return *spotLightData_; }
	const MyBase::Vector4& GetDirectionalLightColor() const { return directionalLightData_->color; }
	const MyBase::Vector3& GetDirectionalLightDirection() const { return directionalLightData_->direction; }
	const float& GetDirectionalLightIntensity() const { return directionalLightData_->intensity; }
	const bool& GetEnableLighting() const { return model_->GetEnableLighting(); }
	const MyBase::Vector4& GetPointLightColor() const { return pointLightData_->color; }
	const MyBase::Vector3& GetPointLightPosition() const { return pointLightData_->position; }
	const float& GetPointLightIntensity() const { return pointLightData_->intensity; }
	const float& GetPointLightRadius() const { return pointLightData_->radius; }
	const float& GetPointLightDecay() const { return pointLightData_->decay; }
	const MyBase::Vector4& GetSpotLightColor() const { return spotLightData_->color; }
	const MyBase::Vector3& GetSpotLightPosition() const { return spotLightData_->position; }
	const float& GetSpotLightIntensity() const { return spotLightData_->intensity; }
	const MyBase::Vector3& GetSpotLightDirection() const { return spotLightData_->direction; }
	const float& GetSpotLightDistance() const { return spotLightData_->distance; }
	const float& GetSpotLightDecay() const { return spotLightData_->decay; }
	const float& GetSpotLightCosAngle() const { return spotLightData_->cosAngle; }

public:	// setter
	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	void SetScale(const MyBase::Vector3& scale) { transform_.scale = scale; }
	void SetRotate(const MyBase::Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const MyBase::Vector3& translate) { transform_.translate = translate; }
	void SetDirectionalLight(const MyBase::DirectionalLight& directionalLight) { *directionalLightData_ = directionalLight; }
	void SetPointLight(const MyBase::PointLight& pointLight) { *pointLightData_ = pointLight; }
	void SetSpotLight(const MyBase::SpotLight& spotLight) { *spotLightData_ = spotLight; }
	void SetDirectionalLightColor(const MyBase::Vector4& lightColor) { directionalLightData_->color = lightColor; }
	void SetDirectionalLightDirection(const MyBase::Vector3& lightDirection) { directionalLightData_->direction = lightDirection; }
	void SetDirectionalLightIntensity(const float& lightIntensity) { directionalLightData_->intensity = lightIntensity; }
	void SetEnableLighting(const bool& enableLighting) { model_->SetEnableLighting(enableLighting); }
	void SetPointLightColor(const MyBase::Vector4& lightColor) { pointLightData_->color = lightColor; }
	void SetPointLightPosition(const MyBase::Vector3& lightPosition) { pointLightData_->position = lightPosition; }
	void SetPointLightIntensity(const float& lightIntensity) { pointLightData_->intensity = lightIntensity; }
	void SetPointLightRadius(const float& lightRadius) { pointLightData_->radius = lightRadius; }
	void SetPointLightDecay(const float& lightDecay) { pointLightData_->decay = lightDecay; }
	void SetSpotLightColor(const MyBase::Vector4& lightColor) { spotLightData_->color = lightColor; }
	void SetSpotLightPosition(const MyBase::Vector3& lightPosition) { spotLightData_->position = lightPosition; }
	void SetSpotLightIntensity(const float& lightIntensity) { spotLightData_->intensity = lightIntensity; }
	void SetSpotLightDirection(const MyBase::Vector3& lightDirection) { spotLightData_->direction = lightDirection; }
	void SetSpotLightDistance(const float& lightDistance) { spotLightData_->distance = lightDistance; }
	void SetSpotLightDecay(const float& lightDecay) { spotLightData_->decay = lightDecay; }
	void SetSpotLightCosAngle(const float& lightCosAngle) { spotLightData_->cosAngle = lightCosAngle; }

private:	// メンバ関数
	// 座標変換行列データ作成
	void CreateTransformationMatrixData();
	// 平行光源データ作成
	void CreateDirectionalLightData();
	// カメラデータ作成
	void CreateCameraData();
	// 点光源データ作成
	void CreatePointLightData();
	// スポットライトデータ作成
	void CreateSpotLightData();

private:	// メンバ変数
	Object3dBase* object3dBase_ = nullptr;

	Model* model_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;	// 座標変換行列
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;		// 平行光源
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_ = nullptr;				// カメラ
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_ = nullptr;			// 点光源
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_ = nullptr;			// スポットライト
	// バッファリソース内のデータを指すポインタ
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	MyBase::DirectionalLight* directionalLightData_ = nullptr;						// 平行光源
	MyBase::CameraForGPU* cameraData_ = nullptr;									// カメラ
	MyBase::PointLight* pointLightData_ = nullptr;									// 点光源
	MyBase::SpotLight* spotLightData_ = nullptr;									// スポットライト

	// Transform
	MyBase::Transform transform_;			// 3Dオブジェクト
};

