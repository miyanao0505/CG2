#pragma once
#include "Object3d.h"
#include "MyBase.h"

// 天球
class Skydome
{
public:	// メンバ関数
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filename">ファイル名 フォーマット除く</param>
	void Initialize(const std::string& filename);

	// 更新
	void Update();

	// 描画
	void Draw();

private:	// メンバ変数
	// オブジェクト
	Object3d* skydome_ = nullptr;

	// ファイルパス
	MyBase::ModelFilePath path_{};
};

