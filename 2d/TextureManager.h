#pragma once
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include "../base/DirectXBase.h"
#include "../externals/DirectXTex/DirectXTex.h"

// テクスチャマネージャー
class TextureManager
{
private: // テクスチャデータ構造体
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

public:	// メンバ関数
	// シングルトンインスタンスの取得
	static TextureManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize(DirectXBase* dxBase);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャファイルのパス</param>
	void LoadTexture(const std::string& filePath);

	// SRVインデックスの開始番号
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

	// メタデータを取得
	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);

private: // シングルトン
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = delete;

private: // メンバ変数
	// テクスチャデータ
	std::vector<TextureData> textureDatas;

	// DirectXBase
	DirectXBase* dxBase_;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;

};

