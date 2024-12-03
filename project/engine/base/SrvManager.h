#pragma once
#include <DirectXBase.h>

// SRV管理
class SrvManager
{
public:	// メンバ関数
	// 初期化
	void Initialize(DirectXBase* dxBase);

	// Allocationの確保
	uint32_t Allocate();

	// SRVの指定番号のCPUデスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	// SRVの指定番号のGPUデスクリプタハンドルを取得する
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// SRV生成(テクスチャ用)
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	// SRV生成(Structured Buffer用)
	//void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	// 描画前処理
	void PreDraw();

	// SRVセットコマンド
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	// テクスチャ枚数上限チェック
	bool isSecure();

private:	// メンバ変数
	DirectXBase* dxBase_ = nullptr;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// 次に使用するSRVインデックス
	uint32_t useIndex_ = 0;

public: // メンバ定数
	// 最大SRV数(最大テクスチャ枚数)
	static const uint32_t kMaxSRVCount;


};

