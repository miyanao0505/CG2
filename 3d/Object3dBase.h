#pragma once
#include "../base/DirectXBase.h"

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

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// DirectXBase
	DirectXBase* dxBase_;

};

