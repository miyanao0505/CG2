#include "SrvManager.h"
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

const uint32_t SrvManager::kMaxSRVCount = 512;

// 初期化
void SrvManager::Initialize(DirectXBase* dxBase)
{
	// メンバ変数に記録
	dxBase_ = dxBase;


	// SRV用のでスクリプタヒープの生成
	descriptorHeap_ = dxBase_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	// SRV用のDescriptorSizeを取得して記録
	descriptorSize_ = dxBase_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

// Allocationの確保
uint32_t SrvManager::Allocate()
{
	// 上限に達していないかチェックしてassert
	if (useIndex_ >= kMaxSRVCount) {
		assert(false);
	}

	// return する番号を一旦記録しておく
	uint32_t index = useIndex_;
	// 次回のために番号を1進める
	useIndex_++;
	// 上で記録した番号をreturn
	return index;
}

// SRVの指定番号のCPUデスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

// SRVの指定番号のGPUデスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

// SRV生成(テクスチャ用)
void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;							// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);

	// 設定をもとにSRVを生成
	dxBase_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

// SRV生成(Structured Buffer用)
//void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride)
//{
//
//}

// 描画前処理
void SrvManager::PreDraw()
{
	// 描画用のDescriptorHeapの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { descriptorHeap_.Get()};
	dxBase_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
}

// SRVセットコマンド
void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex)
{
	dxBase_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}

// テクスチャ枚数上限チェック
bool SrvManager::isSecure()
{
	if (useIndex_ < SrvManager::kMaxSRVCount)
	{
		return true;
	}
	return false;
}
