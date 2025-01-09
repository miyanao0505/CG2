#pragma once
#include "BaseScene.h"
#include <string>

// シーン工場(概念)
class AbstractSceneFactory
{
public:	// メンバ関数
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーンの生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

