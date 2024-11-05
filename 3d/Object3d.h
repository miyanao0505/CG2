#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "../Script/MyBase.h"

// 前方宣言
class Object3dBase;

// 3Dオブジェクト
class Object3d
{
public:	// メンバ関数
	// 初期化
	void Initislize(Object3dBase* object3dBase);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// .mtlファイルの読み取り
	static MyBase::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// .objファイルの読み取り
	void LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:	// メンバ関数
	// 頂点データ作成
	void CreateVertexData();
	// マテリアルデータ作成
	void CreateMaterialData();
	// 座標変換行列データ作成
	void CreateTransformationMatrixData();
	// 平行光源データ作成
	void CreateDirectionalLightdata();

private:	// メンバ変数
	Object3dBase* object3dBase_ = nullptr;

	// objファイルのデータ
	MyBase::ModelData modelData_;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;	// 座標変換行列
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;		// 平行光源
	// バッファリソース内のデータを指すポインタ
	MyBase::VertexData* vertexData_ = nullptr;										// vertex
	MyBase::Material* materialData_ = nullptr;										// マテリアル
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	MyBase::DirectionalLight* directionalLightData_ = nullptr;						// 平行光源
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};									// vertex


	// Transform
	MyBase::Transform transform_;			// 3Dオブジェクト
	MyBase::Transform cameraTransform_;		// カメラ
};

