#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "MyBase.h"

// 前方宣言
class SpriteBase;

// スプライト
class Sprite
{
public:	// メンバ関数
	// 初期化
	void Initialize(std::string textureFilePath);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

public:	// getter
	// 座標の取得
	const MyBase::Vector2& GetPosition() const { return position_; }
	// 角度の取得
	float GetRotation() const { return rotation_; }
	// 色の取得
	const MyBase::Vector4& GetColor() const { return materialData_->color; }
	// サイズの取得
	const MyBase::Vector2& GetSize() const { return size_; }
	// アンカーポイントの取得
	const MyBase::Vector2& GetAnchorPoint() const { return anchroPoint_; }
	// フリップの取得
	const bool& GetIsFlipX() const { return isFlipX_; }		// 左右
	const bool& GetIsFlipY() const { return isFlipY_; }		// 上下
	// テクスチャ範囲指定の取得
	const MyBase::Vector2& GetTextureLeftTop() const { return textureLeftTop_; }	// 左上座標
	const MyBase::Vector2& GetTextureSize() const { return textureSize_; }			// 切り出しサイズ
	// スプライトのサイズ
	const MyBase::Vector2& GetSpriteSize() const { return spriteSize_; }

public:	// setter
	// 座標のセット
	void SetPosition(const MyBase::Vector2& position) { position_ = position; }
	// 角度のセット
	void SetRotation(float rotation) { rotation_ = rotation; }
	// 色のセット
	void SetColor(const MyBase::Vector4& color) { materialData_->color = color; }
	// サイズのセット
	void SetSize(const MyBase::Vector2& size) { size_ = size; }
	// テクスチャのセット
	void SetTexture(std::string textureFilePath);
	// アンカーポイントのセット
	void SetAnchorPoint(const MyBase::Vector2& anchorPoint) { anchroPoint_ = anchorPoint; }
	// フリップのセット
	void SetIsFlipX(const bool& isFlipX) { isFlipX_ = isFlipX; }		// 左右
	void SetIsFlipY(const bool& isFlipY) { isFlipY_ = isFlipY; }		// 上下
	// テクスチャ範囲指定のセット
	void SetTextureLeftTop(const MyBase::Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }	// 左上座標
	void SetTextureSize(const MyBase::Vector2& textureSize) { textureSize_ = textureSize; }				// 切り出しサイズ

private:	// メンバ関数
	// 頂点データ作成
	void CreateVertexData();
	// マテリアルデータ作成
	void CreateMaterialData();
	// 座標変換行列データ作成
	void CreateTransformationMatrixData();
	// テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

private:	// メンバ変数
	SpriteBase* spriteBase_ = nullptr;
	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;				// index
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;	// 座標変換行列
	// バッファリソース内のデータを指すポインタ
	MyBase::SpriteVertexData* vertexData_ = nullptr;										// vertex
	uint32_t* indexData_ = nullptr;													// index
	MyBase::SpriteMaterial* materialData_ = nullptr;										// マテリアル
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};									// vertex
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};										// index

	// ファイルパス
	std::string filePath_;
	// スプライトのサイズ
	MyBase::Vector2 spriteSize_ = { 0.0f, 0.0f };

	// 座標
	MyBase::Vector2 position_ = { 0.0f, 0.0f };
	// 回転
	float rotation_ = 0.0f;
	// サイズ
	MyBase::Vector2 size_ = { 640.f, 360.f };

	// アンカーポイント
	MyBase::Vector2 anchroPoint_ = { 0.0f, 0.0f };
	// フリップ
	bool isFlipX_ = false;		// 左右
	bool isFlipY_ = false;		// 上下
	// テクスチャ範囲指定
	MyBase::Vector2 textureLeftTop_ = { 0.0f, 0.0f };	// 左上座標
	MyBase::Vector2 textureSize_ = { 100.0f, 100.0f };	// 切り出しサイズ

};
