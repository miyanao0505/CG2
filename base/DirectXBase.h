#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>
#include <array>
#include "WindowsAPI.h"

// DirectX基盤
class DirectXBase
{
public:	// メンバ関数
	// 初期化
	void Initialize(WindowsAPI* winApi);
	// SRVの指定番号のCPUデスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	// SRVの指定番号のGPUデスクリプタハンドルを取得する
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);
	// RTVの指定番号のCPUデスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	// RTVの指定番号のGPUデスクリプタハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のCPUデスクリプタハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のGPUデスクリプタハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

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
	// CPUデスクリプタハンドルを取得する
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	// GPUデスクリプタハンドルを取得する
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	// 深度ステンシルビューの初期化
	void InitializeDepthStencilView();
	// フェンスの生成
	void CreateFence();
	// ビューポート矩形の生成
	void CreateViewPort();
	// シザリング矩形の生成
	void CreateScissorRect();
	// DXCコンパイラの生成
	void CreateDxcCompiler();
	// ImGuiの初期化
	void InitializeImGui();

private:	// メンバ変数
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
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
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources_;
	// 取得したRTVハンドル
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2> rtvHandles_;
	// ビューポート矩形
	D3D12_VIEWPORT viewport_;
	// シザー矩形
	D3D12_RECT scissorRect_;
	// DXCユーティリティ
	IDxcUtils* dxcUtils_;
	// DXCコンパイラ
	IDxcCompiler3* dxcCompiler_;
	// デフォルトインクルードハンドラ
	IDxcIncludeHandler* includeHandler_;

	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;
	
	// RTV用のDescriptorSize
	uint32_t descriptorSizeSRV_;
	// SRV用のDescriptorSize
	uint32_t descriptorSizeRTV_;
	// DSV用のDescriptorSIze
	uint32_t descriptorSizeDSV_;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
};
