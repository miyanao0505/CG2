#pragma once
#include "Collider.h"
#include<vector>

// 衝突判定の管理
class CollisionManager
{
public:	// メンバ関数
	CollisionManager();
	~CollisionManager();

	// 衝突判定と応答
	void CheckAllCollisions();

	/// <summary>
	/// コライダー2つの衝突判定
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	// コライダーリストをクリア
	void Clear();

public:	// setter
	// コライダーをリストに登録する
	void SetColliderList(Collider* collider) { colliders_.push_back(collider); }

private:	// メンバ変数
	// コライダーリスト
	std::vector<Collider*> colliders_;
};

