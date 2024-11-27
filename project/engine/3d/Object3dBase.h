#pragma once
#include "DirectXBase.h"
#include "Camera.h"

// 3Dオブジェクト共通部
class Object3dBase
{
public:	// メンバ関数
	// 初期化
	void Initislize(DirectXBase* dxBase);
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();
	// 共通画面設定
	void SetCommonScreen();

public:	// getter
	DirectXBase* GetDxBase() const { return dxBase_; }
	Camera* GetDefaultCamera() const { return defaultCamera_; }

public:	// setter
	void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// DirectXBase
	DirectXBase* dxBase_;

	// カメラ
	Camera* defaultCamera_ = nullptr;

};

