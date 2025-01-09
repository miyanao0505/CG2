#pragma once
#include <MyBase.h>

// カメラ
class Camera
{
public:	// メンバ関数
	// コンストラクタ
	Camera();

	// 更新
	void Update();

public:	// getter
	const MyBase::Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const MyBase::Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const MyBase::Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const MyBase::Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	const MyBase::Vector3& GetRotate() const { return transform_.rotate; }
	const MyBase::Vector3& GetTranslate() const { return transform_.translate; }
	const float& GetFovY() const { return fovY_; }
	const float& GetAspectRatio() const { return aspectRatio_; }
	const float& GetNearCip() const { return nearClip_; }
	const float& GetFarClip() const { return farClip_; }

public:	// setter
	void SetRotate(const MyBase::Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const MyBase::Vector3& translate) { transform_.translate = translate; };
	void SetFovY(const float& fovY) { fovY_ = fovY; }
	void SetAspectRatio(const float& aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(const float& nearClip) { nearClip_ = nearClip; }
	void SetFarClip(const float& farClip) { farClip_ = farClip; }

private:	// メンバ変数
	// ビュー関連データ
	MyBase::Transform transform_;
	MyBase::Matrix4x4 worldMatrix_;
	MyBase::Matrix4x4 viewMatrix_;

	// プロジェクション行列関連データ
	MyBase::Matrix4x4 projectionMatrix_;
	float fovY_;							// 水平方向視野角
	float aspectRatio_;						// アスペクト比
	float nearClip_;						// ニアクリップ距離
	float farClip_;							// ファークリップ距離

	// 合成行列
	MyBase::Matrix4x4 viewProjectionMatrix_;

};

