#include "TextureManager.h"

using namespace DirectX;
using namespace StringUtility;

TextureManager* TextureManager::instance = nullptr;

// シングルトンインスタンスの取得
TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

// 終了
void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

// 初期化
void TextureManager::Initialize(DirectXBase* dxBase, SrvManager* srvManager)
{
	dxBase_ = dxBase;

	srvManager_ = srvManager;

	spriteBase_ = std::make_unique<SpriteBase>();
	spriteBase_->Initialize(dxBase_);

	// SRVの数と同数
	textureDatas.reserve(SrvManager::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	// 読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		return;
	}

	// テクスチャ枚数上限チェック
	assert(srvManager_->isSecure());

	// Textureを読んで転送する
	ScratchImage mipImages = dxBase_->LoadTexture(filePath);

	// 追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas[filePath];

	// テクスチャデータの書き込み
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxBase_->CreateTextureResource(textureData.metadata);
	textureData.intermediateResource = dxBase_->UploadTextureData(textureData.resource.Get(), mipImages);
	// テクスチャデータの要素数番号をSRVのインデックスとする
	//uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
	//
	//textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(srvIndex);
	//textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(srvIndex);

	// SRV確保
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metadata.format, UINT(textureData.metadata.mipLevels));
}

// SRVインデックスの開始番号
uint32_t TextureManager::GetSrvIndex(const std::string& filePath)
{
	// 読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		// 読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), textureDatas.end()));
		return textureIndex;
	}
	assert(0);
	return 0;
}

// テクスチャ番号からGPUハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath)
{
	// 範囲外指定違反チェック
	assert(srvManager_->isSecure());
	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}

// メタデータを取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath)
{
	// 範囲外指定違反チェック
	assert(srvManager_->isSecure());
	TextureData& textureData = textureDatas[filePath];
	return textureData.metadata;
}

// ブレンドモードのセット
void TextureManager::SetBlendMode(SpriteBase::BlendMode blendMode)
{
	spriteBase_->SetBlendMode(blendMode);
	spriteBase_->CreateGraphicsPipeline();
}
