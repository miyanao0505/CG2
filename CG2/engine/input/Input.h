#pragma once
#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include <Windows.h>
#include <wrl.h>
#include "WindowsAPI.h"

// 入力
class Input
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:	// メンバ関数
	// 初期化
	void Initialize(WindowsAPI* winApi);
	// 更新
	void Update();

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

private: // メンバ変数
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;
	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;

	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;

	// 前回の全キーの状態
	BYTE keyPre[256] = {};
	// 全キーの状態
	BYTE key[256] = {};
};

