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
	void DeviceCreate();
	// コマンド関連の生成
	void CommandCreate();
	// スワップチェーンの生成
	void SwapChainCreate();

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
	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;
};

