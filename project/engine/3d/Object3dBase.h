#pragma once
#include "DirectXBase.h"

// 3Dオブジェクト共通部
class Object3dBase
{
public:	// 列挙型
	// namespace省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// ブレンドモード
	enum class BlendMode {
		kBlendModeNone,			//!< ブレンドなし
		kBlendModeNormal,		//!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
		kBlendModeAdd,			//!< 加算。 Src * SrcA + Dest * 1
		kBlendModeSubtract,		//!< 減算。 Dest * 1 - Src * SrcA
		kBlendModeMultiply,		//!< 乗算。 Src * 0 + Dest * Src
		kBlendModeScreen,		//!< スクリーン。 Src * (1 - Dest) + Dest * 1
		kBlendModeExclusion,	//!< 除外。(1 - Dest) * Src + (1 - Src) * Dest

		kCountOfBlendMode,	//!< 利用してはいけない
	};
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

public:	// setter
	void SetBlendMode(BlendMode blendMode);

private:	// メンバ関数
	// ブレンドモード設計
	D3D12_BLEND_DESC SetBlendModeNone();
	D3D12_BLEND_DESC SetBlendModeNormal();
	D3D12_BLEND_DESC SetBlendModeAdd();
	D3D12_BLEND_DESC SetBlendModeSubtract();
	D3D12_BLEND_DESC SetBlendModeMultiply();
	D3D12_BLEND_DESC SetBlendModeScreen();
	static D3D12_BLEND_DESC(Object3dBase::* spFuncTable[])();

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::kBlendModeNormal;

	// DirectXBase
	DirectXBase* dxBase_;

};

