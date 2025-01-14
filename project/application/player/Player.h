#pragma once
#include "Collider.h"
#include <Input.h>
#include <list>
#include "Object3d.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG


// 自キャラ
class Player : public Collider
{
public:	// メンバ関数


private:	// メンバ変数
	// オブジェクト
	Object3d* player_ = nullptr;	// 自キャラ


};

