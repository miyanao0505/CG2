#include "Particle.h"
#include <fstream>
#include "ModelManager.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "ParticleBase.h"

// ImGuiで0番を使用するため、1番から使用
//uint32_t Particle::kNumInstance = 10;

// 初期化
void Particle::Initislize(DirectXBase* dxBase, SrvManager* srvManager, ParticleBase* particleBase)
{
	// 引数で受け取ってメンバ変数に記録する
	dxBase_ = dxBase;
	srvManager_ = srvManager;
	particleBase_ = particleBase;

	// 平行光源データの作成
	CreateDirectionalLightdata();

	// indexデータの作成
	CreateIndexData();

	// マテリアルデータの作成
	CreateMaterialData();

	// Instancingデータの作成
	CreateInstancingData();

	// Transform変数を作る
	for (uint32_t index = 0; index < kNumInstance; ++index) {
		transform_[index].scale = {1.0f, 1.0f, 1.0f};
		transform_[index].rotate = { 0.0f, 0.0f, 0.0f };
		transform_[index].translate = { index * 0.1f, index * 0.1f, index * 0.1f };
	}

	srvManager_->Allocate();
}

// 更新処理
void Particle::Update()
{
	for (uint32_t index = 0; index < kNumInstance; ++index) {
		// WorldMatrixの作成
		MyBase::Matrix4x4 worldMatrix = Matrix::MakeAffineMatrix(transform_[index].scale, transform_[index].rotate, transform_[index].translate);
		MyBase::Matrix4x4 worldViewProjectionMatrix;
		if (CameraManager::GetInstance()->GetCamera()) {
			const MyBase::Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
			worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, viewProjectionMatrix);
		}
		else {
			worldViewProjectionMatrix = worldMatrix;
		}

		instancingData_[index].WVP = worldViewProjectionMatrix;
		instancingData_[index].World = worldMatrix;
	}
}

// 描画処理
void Particle::Draw()
{
	// VBVの設定
	particleBase_->GetDxBase()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	particleBase_->GetDxBase()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	// 印す短寝具データのSRVのDescriptorTableを設定
	particleBase_->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(0, );
	// 平行光源用のCBufferの場所を設定
	particleBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_.Get()->GetGPUVirtualAddress());
	// マテリアルCBufferの場所を設定
	particleBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	particleBase_->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	particleBase_->GetDxBase()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), kNumInstance, 0, 0);
}

// 平行光源データ作成
void Particle::CreateDirectionalLightdata()
{
	// 平行光源用のリソースを作る
	directionalLightResource_ = particleBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::DirectionalLight));
	// 書き込むためのアドレス取得
	directionalLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}

void Particle::CreateVertexData()
{
	// 頂点リソースを作る
	vertexResource_ = particleBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::VertexData) * modelData_.vertices.size());
	// 頂点バッファビューを作成する
	//vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();					// リソースの先頭のアドレスから使う
	//vertexBufferView_.SizeInBytes = UINT(sizeof(MyBase::VertexData) * modelData_.vertices.size());		// 使用するリソースのサイズは頂点のサイズ
	//vertexBufferView_.StrideInBytes = sizeof(MyBase::VertexData);										// 頂点あたりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));									// 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(MyBase::VertexData) * modelData_.vertices.size());	// 頂点データをリソースにコピー
}

void Particle::CreateIndexData()
{
	// indexリソースを作る
	indexResource_ = dxBase_->CreateBufferResource(sizeof(uint32_t) * kNumInstance);

	// indexバッファービューを作成する
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kNumInstance;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
	indexResource_->Unmap(0, nullptr);
}

void Particle::CreateMaterialData()
{
	// マテリアル用のリソースを作る
	materialResource_ = particleBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::ModelMaterial));
	// 書き込むためのアドレスを取得
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 白で読み込む
	materialData_->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// Lightingを有効にする
	materialData_->enableLighting = true;
	// 単位行列で初期化
	materialData_->uvTransform = Matrix::MakeIdentity4x4();
}

void Particle::CreateInstancingData()
{
	// Instancing用のTransformationMatrixリソースを作る
	instancingResource_ = particleBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::TransformationMatrix) * kNumInstance);
	// 書き込むためのアドレスを取得
	instancingResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// 単位行列を書き込んでおく
	for (uint32_t index = 0; index < kNumInstance; ++index) {
		instancingData_[index].WVP = Matrix::MakeIdentity4x4();
		instancingData_[index].World = Matrix::MakeIdentity4x4();
	}

}
