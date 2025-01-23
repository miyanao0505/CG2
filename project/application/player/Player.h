#pragma once
#include "Collider.h"
#include <Input.h>
#include "Object3d.h"


// 自キャラ
class Player : public Collider
{

	void Initialize();

	void Update();

	void Attack();
};

