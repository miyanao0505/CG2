#pragma once
#include <map>
#include <memory>
#include <string>
#include "Camera.h"

// カメラマネージャー
class CameraManager
{
public:	// メンバ関数
	// シングルトンインスタンスの取得
	static CameraManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize();

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="cameraName">カメラの名前</param>
	void SetCamera(const std::string& cameraName);

	/// <summary>
	/// カメラの検索
	/// </summary>
	/// <param name="cameraName">カメラの名前</param>
	void FindCamera(const std::string& cameraName);

private:	// シングルトン
	static CameraManager* instance;

	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(CameraManager&) = default;
	CameraManager& operator=(CameraManager&) = delete;

public:	// メンバ関数
	/// <summary>
	/// カメラデータの取得
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetCamera() { return camera_; }

private:	// メンバ変数
	// カメラデータ
	std::map<std::string, std::unique_ptr<Camera>> cameras_;

	// 現在指定しているカメラデータ
	Camera* camera_ = nullptr;

};

