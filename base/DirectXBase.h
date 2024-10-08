#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WindowsAPI.h"

// DirectX基盤
class DirectXBase
{
public:	// メンバ関数
	// 初期化
	void Initialize(WindowsAPI* winApi);
private:
	// デバイスの生成
	void CreateDevice();
	// コマンド関連の生成
	void CreateCommand();
	// スワップチェーンの生成
	void CreateSwapChain();
	// 深度バッファの生成
	void CreateDepthStencil();
	// デスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	// 各種デスクリプタヒープの生成
	void CreateDescriptorHeapAllKinds();
	// レンダーターゲットビューの初期化
	void InitializeRenderTargetView();


private:	// メンバ変数
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;
	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;
	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_;
	// DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_;
	// SwapChainからResourceを引っ張ってきたリソース(バックバッファ)
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2] = { nullptr };
	// RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	
	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;
	
	// RTV用のDescriptorSize
	uint32_t descriptorSizeSRV_;
	// SRV用のDescriptorSize
	uint32_t descriptorSizeRTV_;
	// DSV用のDescriptorSIze
	uint32_t descriptorSizeDSV_;
};

