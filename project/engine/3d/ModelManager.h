#pragma once
#include <map>
#include <memory>
#include <string>
#include "Object3dBase.h"
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

public:	// getter
	// Object3dBase取得
	Object3dBase* GetObject3dBase() const { return object3dBase_.get(); }

public:	// setter
	// 共通画面設定
	void SetCommonScreen() { object3dBase_->SetCommonScreen(); }

	// ブレンドモードのセット
	void SetBlendMode(Object3dBase::BlendMode blendMode);

private:	// シングルトン
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = default;
	ModelManager& operator=(ModelManager&) = delete;

private:	// メンバ変数
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models_;

	std::unique_ptr<ModelBase> modelBase_ = nullptr;
	std::unique_ptr<Object3dBase> object3dBase_ = nullptr;
};

