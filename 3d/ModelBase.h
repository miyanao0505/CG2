#pragma once
#include "../base/DirectXBase.h"

// 3Dモデル共通部
class ModelBase
{
public:	// メンバ関数
	// 初期化
	void Initislize(DirectXBase* dxBase);

public:	// getter
	DirectXBase* GetDxBase() const { return dxBase_; }

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

};

