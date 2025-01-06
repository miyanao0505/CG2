#pragma once
#include "MyBase.h"

// パーティクルエミッター
class ParticleEmitter
{
public:	// メンバ関数
	// コンストラクタ
	ParticleEmitter();

	// 初期化
	void Initialize(const std::string name, const std::string textureFilePath);

	// 更新
	void Update();

	// パーティクルの発生
	void Emit();

public:	// getter
	const MyBase::Vector3& GetPosition() { return transform_.translate; }
	const MyBase::Vector3& GetRotation() { return transform_.rotate; }
	const MyBase::Vector3& GetSize() { return transform_.scale; }
	uint32_t GetCount() { return count_; }
	float GetFrequency() { return frequency_; }
	bool GetIsEmitUpdate() { return isEmitUpdate_; }

public:	// setter
	void SetPosition(const MyBase::Vector3& position) { transform_.translate = position; }
	void SetRotation(const MyBase::Vector3& rotation) { transform_.rotate = rotation; }
	void SetSize(const MyBase::Vector3& size) { transform_.scale = size; }
	void SetCount(uint32_t count) { count_ = count; }
	void SetFrequency(float frequency) { frequency_ = frequency; }
	void SetIsEmitUpdate(bool isEmitUpdate) { isEmitUpdate_ = isEmitUpdate; }

private:	// メンバ変数
	std::string name_;
	std::string textureFilePath_;

	// 発生中心
	MyBase::Transform transform_{};
	// 発生数
	uint32_t count_ = 20;
	// 発生頻度
	float frequency_ = 0.5f;
	// 頻度用時刻
	float frequencyTime_ = 0.0f;
	// 連続発生するか
	bool isEmitUpdate_ = false;

	// デルタタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

