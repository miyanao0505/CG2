#pragma once
#include <string>
#include <d3d12.h>
#include <unordered_map>
#include "DirectXBase.h"
#include "SrvManager.h"
#include "DirectXTex.h"
#include "SpriteBase.h"

// テクスチャマネージャー
class TextureManager
{
private: // テクスチャデータ構造体
	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

public:	// メンバ関数
	// シングルトンインスタンスの取得
	static TextureManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize(DirectXBase* dxBase, SrvManager* srvManager);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャファイルのパス</param>
	void LoadTexture(const std::string& filePath);

public: // getter
	// SRVインデックスの開始番号
	uint32_t GetSrvIndex(const std::string& filePath);

	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);

	// メタデータを取得
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

	// spriteBaseの取得
	SpriteBase* GetSpriteBase() const { return spriteBase_.get(); }

public:	// setter
	// スプライト共有部のセット
	void SetCommonScreen() { spriteBase_->SetCommonScreen();}

	// ブレンドモードのセット
	void SetBlendMode(SpriteBase::BlendMode blendMode);

private: // シングルトン
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = delete;

private: // メンバ変数
	// テクスチャデータ
	std::unordered_map<std::string, TextureData> textureDatas;

	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// SrvManager
	SrvManager* srvManager_ = nullptr;

	// スプライト共通部
	std::unique_ptr<SpriteBase> spriteBase_ = nullptr;
};

