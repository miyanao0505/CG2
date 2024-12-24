#pragma once
#include <d3d12.h>
#include "DirectXBase.h"
#include "SrvManager.h"
#include "MyBase.h"

// 前方宣言
class ParticleBase;

// 3Dオブジェクト
class Particle
{
public:	// メンバ関数
	// 初期化
	void Initislize(DirectXBase* dxBase, SrvManager* srvManager, ParticleBase* particleBase);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

public:	// getter
	const MyBase::Vector3& GetScale(int index) const { return transform_[index].scale; }
	const MyBase::Vector3& GetRotate(int index) const { return transform_[index].rotate; }
	const MyBase::Vector3& GetTranslate(int index) const { return transform_[index].translate; }
	const MyBase::Vector4& GetDirectionalLightColor() const { return directionalLightData_->color; }
	const MyBase::Vector3& GetDirectionalLightDirection() const { return directionalLightData_->direction; }
	const float& GetDirectionalLightIntensity() const { return directionalLightData_->intensity; }
	const bool& GetEnableLighting() const { return materialData_->enableLighting; }

public:	// setter
	void SetScale(const MyBase::Vector3& scale, int index) { transform_[index].scale = scale; }
	void SetRotate(const MyBase::Vector3& rotate, int index) { transform_[index].rotate = rotate; }
	void SetTranslate(const MyBase::Vector3& translate, int index) { transform_[index].translate = translate; }
	void SetDirectionalLightColor(const MyBase::Vector4& lightColor) { directionalLightData_->color = lightColor; }
	void SetDirectionalLightDirection(const MyBase::Vector3& lightDirection) { directionalLightData_->direction = lightDirection; }
	void SetDirectionalLightIntensity(const float& lightIntensity) { directionalLightData_->intensity = lightIntensity; }
	void SetEnableLighting(const bool& enableLighting) { materialData_->enableLighting = enableLighting; }

private:	// メンバ関数
	// 平行光源データ作成
	void CreateDirectionalLightdata();
	// 頂点データ作成
	void CreateVertexData();
	// indexデータ作成
	void CreateIndexData();
	// マテリアルデータ作成
	void CreateMaterialData();
	// Insrancingデータ作成
	void CreateInstancingData();

private:	// メンバ変数
	DirectXBase* dxBase_ = nullptr;
	ParticleBase* particleBase_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_ = nullptr;			// instancing
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;		// 平行光源
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;				// index
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル
	// バッファリソース内のデータを指すポインタ
	MyBase::TransformationMatrix* instancingData_ = nullptr;						// instancing
	MyBase::DirectionalLight* directionalLightData_ = nullptr;						// 平行光源
	MyBase::VertexData* vertexData_ = nullptr;										// vertex
	uint32_t* indexData_ = nullptr;													// index
	MyBase::ModelMaterial* materialData_ = nullptr;									// マテリアル
	
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};									// index
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};								// vertex

	// objファイルのデータ
	MyBase::ModelData modelData_;

	// パーティクルの数
	static const uint32_t kNumInstance = 10;

	// Transform
	MyBase::Transform transform_[kNumInstance];

	
};

