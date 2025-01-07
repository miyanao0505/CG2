#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>
#include <array>
#include <chrono>
#include "Logger.h"
#include "StringUtility.h"
#include "WindowsAPI.h"
#include "DirectXTex.h"

// DirectX基盤
class DirectXBase
{
public:	// メンバ関数
	// 初期化
	void Initialize(WindowsAPI* winApi);
	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();

	/// <summary>
	/// ShaderをCompileをする関数
	/// </summary>
	/// <param name="filePath">CompilerするShaderファイルへのパス</param>
	/// <param name="profile">Compilerに使用するProfile</param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

	/// <summary>
	/// バッファーリソースの生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	// TextureResource作成の関数
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	// データを転送する関数
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	// デスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャファイルのパス</param>
	/// <returns>画像イメージデータ</returns>
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);


public:	// getter

	// RTVの指定番号のCPUデスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	// RTVの指定番号のGPUデスクリプタハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のCPUデスクリプタハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のGPUデスクリプタハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

	ID3D12Device* GetDevice() const { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	// バックバッファの数を取得
	size_t GetBackBufferCount() const { return swapChainResources_.size(); }

private:
	// デバイスの生成
	void CreateDevice();
	// コマンド関連の生成
	void CreateCommand();
	// スワップチェーンの生成
	void CreateSwapChain();
	// 深度バッファの生成
	void CreateDepthStencil();
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

	// FPS固定初期化
	void InitializeFixFPS();
	// FPS固定更新
	void UpdateFixFPS();

private:	// メンバ変数
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	// DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	// SwapChainからResourceを引っ張ってきたリソース(バックバッファ)
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources_;
	// 取得したRTVハンドル
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2> rtvHandles_ ;
	// Fence
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	// FenceのSignal
	uint64_t fenceValue_;
	HANDLE fenceEvent_;
	// ビューポート矩形
	D3D12_VIEWPORT viewport_;
	// シザー矩形
	D3D12_RECT scissorRect_;
	// DXCユーティリティ
	IDxcUtils* dxcUtils_ = nullptr;
	// DXCコンパイラ
	IDxcCompiler3* dxcCompiler_ = nullptr;
	// デフォルトインクルードハンドラ
	IDxcIncludeHandler* includeHandler_ = nullptr;

	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;
	
	// RTV用のDescriptorSize
	uint32_t descriptorSizeRTV_;
	// DSV用のDescriptorSIze
	uint32_t descriptorSizeDSV_;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

};

