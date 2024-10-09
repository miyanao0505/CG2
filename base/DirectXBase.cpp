#include "DirectXBase.h"
#include <cassert>
#include <format>
#include "Logger.h"
#include "StringUtility.h"
#include "../externals/DirectXTex/DirectXTex.h"
#include "../externals/imgui/imgui_impl_dx12.h"
#include "../externals/imgui/imgui_impl_win32.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;
using namespace Logger;
using namespace StringUtility;

// 初期化
void DirectXBase::Initialize(WindowsAPI* winApi)
{
	// NULL検出
	assert(winApi);

	// メンバ変数に記録
	winApi_ = winApi;

	// デバイスの生成
	CreateDevice();
	// コマンド関連の生成
	CreateCommand();
	// スワップチェーンの生成
	CreateSwapChain();
	// 深度バッファの生成
	CreateDepthStencil();
	// 各種デスクリプタヒープの生成
	CreateDescriptorHeapAllKinds();
	// レンダーターゲットビューの初期化
	InitializeRenderTargetView();
	// 深度ステンシルビューの初期化
	InitializeDepthStencilView();
	// フェンスの生成
	CreateFence();
	// ビューポート矩形の生成
	CreateViewPort();
	// シザリング矩形の生成
	CreateScissorRect();
	// DCXコンパイラの生成
	CreateDxcCompiler();
	// ImGuiの初期化
	InitializeImGui();
}

// SRVの指定番号のCPUデスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetSRVCPUDescriptorHandle(uint32_t index)
{
	return GetCPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, index);
}

// SRVの指定番号のGPUデスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetSRVGPUDescriptorHandle(uint32_t index)
{
	return GetGPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, index);
}

// RTVの指定番号のCPUデスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetRTVCPUDescriptorHandle(uint32_t index)
{
	return GetCPUDescriptorHandle(rtvDescriptorHeap_, descriptorSizeRTV_, index);
}

// RTVの指定番号のGPUデスクリプタハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetRTVGPUDescriptorHandle(uint32_t index)
{
	return GetGPUDescriptorHandle(rtvDescriptorHeap_, descriptorSizeRTV_, index);
}

// DSVの指定番号のCPUデスクリプタハンドルを取得
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetDSVCPUDescriptorHandle(uint32_t index)
{
	return GetCPUDescriptorHandle(dsvDescriptorHeap_, descriptorSizeDSV_, index);
}

// DSVの指定番号のGPUデスクリプタハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetDSVGPUDescriptorHandle(uint32_t index)
{
	return GetGPUDescriptorHandle(dsvDescriptorHeap_, descriptorSizeDSV_, index);
}

// デバイスの生成
void DirectXBase::CreateDevice()
{
	HRESULT hr;

#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		// デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG

	// DXGIファクトリーの生成
	dxgiFactory_ = nullptr;
	// HRESULTはWindows系のエラーコードであり、
	// 関数が成功したかどうかをSUCCEEDEDマクロで判定できる
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	// 初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));

	// 使用するアダプタ用の変数。最初にnullptrを入れておく
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter = nullptr;
	// 良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {
		// アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));		// 取得できないのは一大事
		// ソフトウェアアダプタでなければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// 採用したアダプタの情報をログに出力。wstringの方なので注意
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;		// ソフトウェアアダプタの場合は見なかったことにする
	}
	// 適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);

	device_ = nullptr;
	// 機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2", "12.1", "12.0" };
	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		// 採用したアダプターでデバイスを生成
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		// 指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(hr)) {
			// 生成できたのでログ出力を行って抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	// デバイスの生成がうまくいかなかったので起動できない
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!!\n");	// 初期化完了のログを出す

#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// ヤバイエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// 警告時に止まる
		// 警告が出た際は、コメントアウトする -> 修正後は元に戻して確認する
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		// 抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			// Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			// https://stackoverfloe.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		// 抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		// 解放
		infoQueue->Release();
	}
#endif // _DEBUG

}

// コマンド関連の生成
void DirectXBase::CreateCommand()
{
	HRESULT hr;

	// コマンドアロケータの生成
	commandAllocator_ = nullptr;
	hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	// コマンドリストの生成
	commandList_ = nullptr;
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	// コマンドキューの生成
	commandQueue_ = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	// コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
}

// スワップチェーンの生成
void DirectXBase::CreateSwapChain()
{
	HRESULT hr;

	swapChainDesc_.Width = WindowsAPI::kClientWidth;					// 画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Height = WindowsAPI::kClientHeight;				// 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 色の形式
	swapChainDesc_.SampleDesc.Count = 1;								// マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2;									// ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// モニタにうつしたら、中身を破棄
	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), winApi_->GetHwnd(), &swapChainDesc_, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

// 深度バッファの生成
void DirectXBase::CreateDepthStencil()
{
	HRESULT hr;

	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = winApi_->kClientWidth;						// Textureの幅
	resourceDesc.Height = winApi_->kClientHeight;					// Textureの高さ
	resourceDesc.MipLevels = 1;										// mipmapの数
	resourceDesc.DepthOrArraySize = 1;								// 奥行き or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;								// サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	// 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	// DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;					// VRAM上に作る

	// 深度のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;						// 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// フォーマット。Resourceと合わせる

	// Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	hr = device_->CreateCommittedResource(
		&heapProperties,					// Heapの設定
		D3D12_HEAP_FLAG_NONE,				// Heapの特殊な設定。特になし
		&resourceDesc,						// Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	// 深度値を書き込む状態にしておく
		&depthClearValue,					// Clear最適値
		IID_PPV_ARGS(&resource)				// 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));
	depthStencilResource_ = resource;
}

// デスクリプタヒープの生成
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXBase::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));

	assert(SUCCEEDED(hr));

	return descriptorHeap;
}

// 各種デスクリプタヒープの生成
void DirectXBase::CreateDescriptorHeapAllKinds()
{
	// RTV用のDescriptorSizeを取得しておく
	descriptorSizeSRV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// SRV用のDescriptorSizeを取得しておく
	descriptorSizeRTV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// DSV用のDescriptorSizeを取得しておく
	descriptorSizeDSV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// RTV用のでスクリプタヒープの生成
	rtvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	// SRV用のでスクリプタヒープの生成
	srvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
	// DSV用のでスクリプタヒープの生成
	dsvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
}

// レンダーターゲットビューの初期化
void DirectXBase::InitializeRenderTargetView()
{
	HRESULT hr;

	// SwapChainからResourceを引っ張ってくる
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	// うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr));

	// RTVの設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;		// 出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2dテクスチャとして書き込む
	
	// ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	// 要素の2つ分
	for (uint32_t i = 0; i < 2; ++i)
	{
		// RTVハンドルを取得
		rtvHandles_[i].ptr = rtvStartHandle.ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(swapChainResources_[i].Get(), &rtvDesc_, rtvHandles_[i]);
	}
}

// CPUDescriptorHandle取得関数
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

// GPUDescriptorHandle取得関数
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

// 深度ステンシルビューの初期化
void DirectXBase::InitializeDepthStencilView()
{
	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	// 2dTexture

	// DSVHeapの先頭にDSVをつくる
	device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

// フェンスの生成
void DirectXBase::CreateFence()
{
	HRESULT hr;

	// 初期値0でFenceを作る
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	uint64_t fenceValue = 0;
	hr = device_->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
}

// ビューポート矩形の生成
void DirectXBase::CreateViewPort()
{
	// クライアント領域のサイズと一緒にして画面企画に表示
	viewport_.Width = WindowsAPI::kClientWidth;
	viewport_.Height = WindowsAPI::kClientHeight;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

// シザリング矩形の生成
void DirectXBase::CreateScissorRect()
{
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = WindowsAPI::kClientWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = WindowsAPI::kClientHeight;
}

// DXCコンパイラの生成
void DirectXBase::CreateDxcCompiler()
{
	HRESULT hr;

	// dxcCompilerを初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	// 現時点でincludeはしないが、includeに対応するための設定を行っておく
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

// ImGuiの初期化
void DirectXBase::InitializeImGui()
{
#ifdef _DEBUG
	// ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApi_->GetHwnd());
	ImGui_ImplDX12_Init(device_.Get(), swapChainDesc_.BufferCount, rtvDesc_.Format, srvDescriptorHeap_.Get(), srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart());
#endif // _DEBUG
}



