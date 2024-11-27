#include "CameraManager.h"

CameraManager* CameraManager::instance = nullptr;

// シングルトンインスタンスの取得
CameraManager* CameraManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new CameraManager;
	}
	return instance;
}

// 終了
void CameraManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

// 初期化
void CameraManager::Initialize()
{

}

// カメラのセット
void CameraManager::SetCamera(const std::string& cameraName)
{
	// 読み込み済みカメラを検索
	if (cameras_.contains(cameraName)) {
		// 読み込み済みなら早期return
		return;
	}

	// カメラの生成と初期化
	std::unique_ptr<Camera> camera = std::make_unique<Camera>();
	
	// カメラをmapコンテナに格納する
	cameras_.insert(std::make_pair(cameraName, std::move(camera)));
}

// カメラの検索
void CameraManager::FindCamera(const std::string& cameraName)
{
	// 読み込み済みカメラを検索
	if (cameras_.contains(cameraName)) {
		// 読み込みカメラを現在のカメラとしてセット
		camera_ = cameras_.at(cameraName).get();
	}
}
