#include "Object3d.h"
#include <fstream>
#include "Object3dBase.h"
#include "../Script/Matrix.h"
#include "../2d/TextureManager.h"

// 初期化
void Object3d::Initislize(Object3dBase* object3dBase)
{
	// 引数で受け取ってメンバ変数に記録する
	object3dBase_ = object3dBase;

	// モデル読み込み
	LoadObjFile("resources/plane", "plane.obj");

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	// 読み込んだテクスチャの番号を取得
	modelData_.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData_.material.textureFilePath);
	
	// 頂点データの作成
	CreateVertexData();

	// マテリアルデータの作成
	CreateMaterialData();

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// 平行光源データの作成
	CreateDirectionalLightdata();

	// Transform変数を作る
	transform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
	cameraTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.3f, 0.0f, 0.0f }, { 0.0f, 4.0f, -10.0f } };

}

// 更新処理
void Object3d::Update()
{
	// Transformの更新(Y軸回転)
	transform_.rotate.y += 1.f / 360.f * float(3.14f);

	// WorldMatrixの作成
	MyBase::Matrix4x4 worldMatrix = Matrix::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	// cameraMatrixの作成
	MyBase::Matrix4x4 cameraMatrix = Matrix::MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	// viewmatrixの作成
	MyBase::Matrix4x4 viewMatrix = Matrix::Inverse(cameraMatrix);
	// ProjectionMatrixを作って透視投影行列を書き込む
	MyBase::Matrix4x4 projectionMatrix = Matrix::MakePerspectiveFovMatrix(0.45f, float(WindowsAPI::kClientWidth) / float(WindowsAPI::kClientHeight), 0.1f, 500.0f);
	// WVP行列の作成
	MyBase::Matrix4x4 worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, Matrix::Multiply(viewMatrix, projectionMatrix));
	// CBufferの中身を更新
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;
}

// 描画処理
void Object3d::Draw()
{
	object3dBase_->GetDxBase()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	// VBVを設定

	// マテリアルCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	// WVP用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(2, object3dBase_->GetDxBase()->GetSRVGPUDescriptorHandle(modelData_.material.textureIndex));
	// 平行光源用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_.Get()->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	object3dBase_->GetDxBase()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

// .mtlファイルの読み取り
MyBase::MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	/// 1. 中で必要となる変数の宣言
	MyBase::MaterialData materialData;	// 構築するMateriallData
	std::string line;					// ファイルが読んだ1行を格納するもの

	/// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);		// ファイルを開く
	assert(file.is_open());									// 取り敢えず開けなかったら止める

	/// 3. 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/../" + textureFilename;
		}
	}

	/// 4. MaterialDataを返す
	return materialData;
}

// .objファイルの読み取り
void Object3d::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	// 1. 中で必要となる変数の宣言
	std::vector<MyBase::Vector4> positions;		// 位置
	std::vector<MyBase::Vector3> normals;		// 法線
	std::vector<MyBase::Vector2> texcoords;		// テクスチャ座標
	std::string line;							// ファイルから呼んだ1行を格納するもの

	// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);		// ファイルを開く
	assert(file.is_open());									// とりあえず開けなかったら止める

	// 3. 実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;			// 先頭の識別子を読む

		// identifierに応じた処理
		if (identifier == "v")
		{
			MyBase::Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			MyBase::Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			MyBase::Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			//normal.z *= -1.f;
			//normal.y *= -1.f;
			normals.push_back(normal);
		}
		else if (identifier == "mtllib")
		{
			// materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData_.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
		else if (identifier == "f") {
			MyBase::VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndeces[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');		// 区切りでインデックスを読んでいく
					elementIndeces[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				MyBase::Vector4 position = positions[elementIndeces[0] - 1];
				MyBase::Vector2 texcoord = texcoords[elementIndeces[1] - 1];
				MyBase::Vector3 normal = normals[elementIndeces[2] - 1];
				/*MyBase::VertexData vertex = { position, texcoord, normal };
				modelData.vertices.push_back(vertex);*/
				triangle[faceVertex] = { position, texcoord, normal };
			}
			// 頂点を逆順で登録することで、周り順を逆にする
			modelData_.vertices.push_back(triangle[2]);
			modelData_.vertices.push_back(triangle[1]);
			modelData_.vertices.push_back(triangle[0]);
		}
	}
}

// 頂点データ作成
void Object3d::CreateVertexData()
{
	// 頂点リソースを作る
	vertexResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::VertexData) * modelData_.vertices.size());
	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();					// リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(MyBase::VertexData) * modelData_.vertices.size());		// 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(MyBase::VertexData);										// 頂点あたりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));									// 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(MyBase::VertexData)* modelData_.vertices.size());	// 頂点データをリソースにコピー

}

// マテリアルデータ作成
void Object3d::CreateMaterialData()
{
	// マテリアル用のリソースを作る
	materialResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::Material));
	// 書き込むためのアドレスを取得
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 白で読み込む
	materialData_->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// Lightingを有効にする
	materialData_->enableLighting = true;
	// 単位行列で初期化
	materialData_->uvTransform = Matrix::MakeIdentity4x4();
}

// 座標変換行列データ作成
void Object3d::CreateTransformationMatrixData()
{
	// TransformationMatrix用のリソースを作る
	transformationMatrixResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::TransformationMatrix));
	// 書き込むためのアドレスを取得
	transformationMatrixResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = Matrix::MakeIdentity4x4();
	transformationMatrixData_->World = Matrix::MakeIdentity4x4();
}

// 平行光源データ作成
void Object3d::CreateDirectionalLightdata()
{
	// 平行光源用のリソースを作る
	directionalLightResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::DirectionalLight));
	// 書き込むためのアドレス取得
	directionalLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}
