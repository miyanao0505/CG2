#include "ParticleManager.h"
#include "DirectXBase.h"
#include"SrvManager.h"
#include"TextureManager.h"
#include"CameraManager.h"
#include "MyTools.h"
#include "Matrix.h"
#include<numbers>

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ParticleManager::Initialize(DirectXBase* dxBase, SrvManager* srvManager)
{
	dxBase_ = dxBase;
	srvManager_ = srvManager;

	particleBase_ = std::make_unique_for_overwrite<ParticleBase>();
	particleBase_->Initislize(dxBase_);

	// ランダムエンジンの初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// インデックスリソースの生成
	indexResource_ = dxBase_->CreateBufferResource(sizeof(uint32_t) * kParticleIndexNum);

	// インデックスバッファビューの生成
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kParticleIndexNum;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
	indexResource_->Unmap(0, nullptr);
}

void ParticleManager::Update()
{
	MyBase::Matrix4x4 viewProjectionMatrix;
	MyBase::Matrix4x4 billoardMatrix;

	if (CameraManager::GetInstance()->GetCamera()) {
		viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		billoardMatrix = CameraManager::GetInstance()->GetCamera()->GetWorldMatrix();
		billoardMatrix.m[3][0] = 0.0f;
		billoardMatrix.m[3][1] = 0.0f;
		billoardMatrix.m[3][2] = 0.0f;
	}
	for (std::pair<const std::string, std::unique_ptr<ParticleGroup>>& pair : particleGroups) {
		ParticleGroup& group = *pair.second;
		int index = 0;
		for (std::list<MyBase::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
			MyBase::Particle& particle = *it;

			particle.currentTime += kDelTime_;
			particle.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
			particle.transform.translate = MyTools::Add(particle.transform.translate, MyTools::Multiply(kDelTime_, particle.velocity));
			if (particle.lifeTime <= particle.currentTime) {
				it = group.particles.erase(it);
				group.kNumInstance--;
				continue;
			}
			MyBase::Matrix4x4 worldMatrix = Matrix::Multiply(Matrix::Multiply(Matrix::MakeScaleMatrix(particle.transform.scale), billoardMatrix), Matrix::MakeTranslateMatrix(particle.transform.translate));
			MyBase::Matrix4x4 worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, viewProjectionMatrix);
			group.instancingData[index].World = worldMatrix;
			group.instancingData[index].WVP = worldViewProjectionMatrix;
			group.instancingData[index].color = particle.color;

			++it;
			++index;
		}
	}
}

void ParticleManager::Draw()
{
	// ルートシグネチャを設定
	particleBase_->SetCommonScreen();

	// 全てのパーティクルグループについて処理
	for (auto& [name, group] : particleGroups) {
		dxBase_->GetCommandList()->IASetVertexBuffers(0, 1, &group->vertexBufferView);	// VBVを設定
		dxBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);					// IBAを設定
		// インスタンシングデータのSRVのDescriptorTableを設定
		srvManager_->SetGraphicsRootDescriptorTable(0, group->srvIndexForInstancing);
		// SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxBase_->GetCommandList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetSrvHandleGPU(group->materialData.textureFilePath));
		// DrawCall(インスタンシング描画)
		dxBase_->GetCommandList()->DrawIndexedInstanced(6, group->kNumInstance, 0, 0, 0);
	}
}

void ParticleManager::ChangeBlendMode(ParticleBase::BlendMode blendMode)
{
	if (particleBase_->GetBlendMode() == blendMode) {
		return;
	}

	particleBase_->SetBlendMode(blendMode);
	particleBase_->CreateGraphicsPipeline();
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{

}

void ParticleManager::Emit(const std::string name, const MyBase::Vector3& position, uint32_t count)
{
}

MyBase::Particle ParticleManager::CreateParticle(std::mt19937& randomEngine, const MyBase::Vector3& position)
{
	return MyBase::Particle();
}
