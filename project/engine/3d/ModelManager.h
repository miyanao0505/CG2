#pragma once
#include <map>
#include <memory>
#include <string>
#include "ModelBase.h"
#include "Model.h"

// モデルマネージャー
class ModelManager
{
public:	// メンバ関数
	// シングルトンインスタンスの取得
	static ModelManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize(DirectXBase* dxBase);

	/// <summary>
	/// モデルファイルの読み込み
	/// </summary>
	/// /// <param name="directoryPath">モデルファイルのダイレクトパス</param>
	/// <param name="filePath">モデルファイルのパス</param>
	void LoadModel(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// モデルの検索
	/// </summary>
	/// <param name="filePath">モデルファイルのパス</param>
	/// <returns>モデル</returns>
	Model* FindModel(const std::string& filePath);

private:	// シングルトン
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = default;
	ModelManager& operator=(ModelManager&) = delete;

private:	// メンバ変数
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models_;

	ModelBase* modelBase_ = nullptr;
};

