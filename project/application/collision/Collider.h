#pragma once
#include "CollisionConfig.h"
#include "MyBase.h"
#include <cstdint>

// 衝突判定オブジェクト
class Collider
{
public:	// メンバ関数
	// 衝突時に呼ばれる関数
	virtual void OnCollision();
	
public:	// getter
	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() { return collisionMask_; }
	// ワールド座標を取得
	virtual MyBase::Vector3 GetWorldPosition() = 0;
	// 半径を取得
	float GetRadius() { return radius_; }

public: // setter
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private:	// メンバ変数
	// 衝突判定
	float radius_ = 1.0f;

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};

