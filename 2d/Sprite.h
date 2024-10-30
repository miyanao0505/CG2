#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "../Script/MyBase.h"

// 前方宣言
class SpriteBase;

// スプライト
class Sprite
{
public:	// メンバ関数
	// 初期化
	void Initialize(SpriteBase* spriteBase);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

private:	// メンバ関数
	// 頂点データ作成
	void CreateVertexData();
	// マテリアルデータ作成
	void CreateMaterialData();
	// 座標変換行列データ作成
	void CreateTransformationMatrixData();

private:	// メンバ変数
	SpriteBase* spriteBase_ = nullptr;
	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;				// index
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;	// 座標変換行列
	// バッファリソース内のデータを指すポインタ
	MyBase::VertexData* vertexData_ = nullptr;										// vertex
	uint32_t* indexData_ = nullptr;													// index
	MyBase::Material* materialData_ = nullptr;										// マテリアル
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;										// vertex
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;										// index
};
