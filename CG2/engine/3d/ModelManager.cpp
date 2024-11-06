#include "ModelManager.h"

ModelManager* ModelManager::instance = nullptr;

// シングルトンインスタンスの取得
ModelManager* ModelManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}

// 終了
void ModelManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

// 初期化
void ModelManager::Initialize(DirectXBase* dxBase)
{
	// モデル共通部の初期化
	modelBase_ = new ModelBase;
	modelBase_->Initislize(dxBase);
}

/// モデルファイルの読み込み
void ModelManager::LoadModel(const std::string& directoryPath, const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (models_.contains(filePath)) {
		// 読み込み済みなら早期return
		return;
	}

	// モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(modelBase_, directoryPath, filePath);

	// モデルをmapコンテナに格納する
	models_.insert(std::make_pair(filePath, std::move(model)));
}

/// モデルの検索
Model* ModelManager::FindModel(const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (models_.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return models_.at(filePath).get();
	}

	// ファイル名一致なし
	return nullptr;
}
