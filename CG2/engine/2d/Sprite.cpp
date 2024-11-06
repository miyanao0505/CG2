#include "Sprite.h"
#include "SpriteBase.h"
#include "Matrix.h"

// 初期化
void Sprite::Initialize(SpriteBase* spriteBase, std::string textureFilePath)
{
	// 引数を受け取ってメンバ変数に記録する
	spriteBase_ = spriteBase;
	textureIndex_ = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	// 頂点データの作成
	CreateVertexData();

	// マテリアルデータの作成
	CreateMaterialData();

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// テクスチャのサイズセット
	AdjustTextureSize();
}

// 更新処理
void Sprite::Update()
{
	// アンカーポイントの反映処理
	float left = 0.0f - anchroPoint_.x;
	float right = 1.0f - anchroPoint_.x;
	float top = 0.0f - anchroPoint_.y;
	float bottom = 1.0f - anchroPoint_.y;

	// フリップの反映処理
	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// テクスチャ範囲指定の反映処理
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureIndex_);
	float tex_left = textureLeftTop_.x / metadata.width;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / metadata.width;
	float tex_top = textureLeftTop_.y / metadata.height;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

	// 頂点リソースにデータを書き込む(4点分)
	// 左下
	vertexData_[0].position = { left, bottom, 0.0f, 1.0f };
	vertexData_[0].texcoord = { tex_left, tex_bottom };
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
	// 左上
	vertexData_[1].position = { left, top, 0.0f, 1.0f };
	vertexData_[1].texcoord = { tex_left, tex_top };
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
	// 右下
	vertexData_[2].position = { right, bottom, 0.0f, 1.0f };
	vertexData_[2].texcoord = { tex_right, tex_bottom };
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
	// 右上
	vertexData_[3].position = { right, top, 0.0f, 1.0f };
	vertexData_[3].texcoord = { tex_right, tex_top };
	vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };
	// インデックスリソースにデータを書き込む(6個分)
	indexData_[0] = 0;		indexData_[1] = 1;		indexData_[2] = 2;
	indexData_[3] = 1;		indexData_[4] = 3;		indexData_[5] = 2;

	// Transform情報を作る
	MyBase::Transform transform = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	transform.translate = { position_.x, position_.y, 0.0f };
	transform.rotate = { 0.0f, 0.0f, rotation_ };
	transform.scale = { size_.x, size_.y, 1.0f };
	// TransformからWorldMatrixを作る
	Matrix::Matrix4x4 worldMatrix = Matrix::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	// ViewMatrixを作って単位行列を代入
	Matrix::Matrix4x4 viewMatrix = Matrix::MakeIdentity4x4();
	// ProjectionMatrixを作って平行投影行列を書き込む
	Matrix::Matrix4x4 projectionMatrix = Matrix::MakeOrthographicMatrix(0.0f, 0.0f, float(WindowsAPI::kClientWidth), float(WindowsAPI::kClientHeight), 0.0f, 100.0f);
	transformationMatrixData_->WVP = Matrix::Multiply(worldMatrix, Matrix::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData_->World = worldMatrix;
}

// 描画処理
void Sprite::Draw()
{
	// VertexBufferViewを設定
	spriteBase_->GetDxBase()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IndexBufferViewを設定
	spriteBase_->GetDxBase()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	// マテリアルCBufferの場所を設定
	spriteBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	// 座標変換行列CBufferの場所を設定
	spriteBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	spriteBase_->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex_));
	// 描画！(DrawCall/ドローコール)
	spriteBase_->GetDxBase()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

// テクスチャのセット
void Sprite::SetTexture(std::string textureFilePath)
{
	// テクスチャのセット
	textureIndex_ = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	// テクスチャ変更に合わせてサイズも再度セット
	AdjustTextureSize();
}

// 頂点データ作成
void Sprite::CreateVertexData()
{
	// VertexResourceを作る
	vertexResource_ = spriteBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::VertexData) * 6);
	// IndexResourceを作る
	indexResource_ = spriteBase_->GetDxBase()->CreateBufferResource(sizeof(uint32_t) * 6);
	
	// VertexBufferViewを作成する(値を設定するだけ)
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(MyBase::VertexData) * 4;
	// 1頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(MyBase::VertexData);
	
	// IndexBufferViewを作成する(値を設定するだけ)
	// リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_.Get()->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	// VertexResourceにデータを書き込むためのアドレスを取得してvertexDataに割り当てる
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	// IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる
	indexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
}

// マテリアルデータ作成
void Sprite::CreateMaterialData()
{
	// マテリアルリソースを作る
	materialResource_ = spriteBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::Material));

	// マテリアルリソースにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	// マテリアルデータの初期値を書き込む
	materialData_->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = Matrix::MakeIdentity4x4();
}

// 座標変換行列データ作成
void Sprite::CreateTransformationMatrixData()
{
	// 座標変換行列リソースを作る
	transformationMatrixResource_ = spriteBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::TransformationMatrix));

	// 座標変換行列リソースにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	transformationMatrixResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));

	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = Matrix::MakeIdentity4x4();
	transformationMatrixData_->World = Matrix::MakeIdentity4x4();
}

// テクスチャサイズをイメージに合わせる
void Sprite::AdjustTextureSize()
{
	// テクスチャメタデータを取得
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureIndex_);

	textureSize_.x = static_cast<float>(metadata.width);
	textureSize_.y = static_cast<float>(metadata.height);
	// 画像サイズをテクスチャサイズに合わせる
	size_ = textureSize_;
	spriteSize_ = size_;
}
