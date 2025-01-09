#include "CollisionManager.h"
#include "MyTools.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

// 衝突判定と応答
void CollisionManager::CheckAllCollisions()
{
	// リスト内のペアを総当たり
	std::vector<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::vector<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

/// コライダー2つの衝突判定
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 || (colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	// コライダーAとBのワールド座標を取得
	MyBase::Vector3 posA = colliderA->GetWorldPosition();
	MyBase::Vector3 posB = colliderB->GetWorldPosition();

	// 距離を求める
	float length = MyTools::Length(MyTools::Subtract(posB, posA));

	// 球と球の交差判定
	if (length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}

// コライダーリストをクリア
void CollisionManager::Clear()
{
	colliders_.clear();
}
