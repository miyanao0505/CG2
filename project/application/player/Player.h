#pragma once
#include "Collider.h"
#include <Input.h>
#include "Object3d.h"


// 自キャラ
class Player : public Collider
{
public:	// メンバ関数
	~Player();

	void Initialize(const std::string& filename);

	void Update();

	void Draw();

	void OnCollision() override;

	MyBase::Vector3 GetWorldPosition() override;

private:	// メンバ変数
	MyBase::ModelFilePath path_ = { {"resources/player"}, {"player.obj"} };

	// モデル
	Object3d* model_ = nullptr;

	Input* input_ = nullptr;

};

