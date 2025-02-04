#pragma once
#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include <Windows.h>
#include <wrl.h>
#include <array>
#include "WindowsAPI.h"

// 入力
class Input
{
public:	// メンバ関数
	// シングルトンインスタンスの取得
	static Input* GetInstance();

	// 初期化
	void Initialize(WindowsAPI* winApi);
	// 終了
	void Finalize();
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

private:	// シングルトンインスタンス
	static Input* instance;

	Input() = default;
	~Input() = default;
	Input(Input&) = default;
	Input& operator=(Input&) = delete;

private: // メンバ変数
	// キーボードのデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	// DirectInputのインスタンス
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;

	// 前回の全キーの状態
	std::array<BYTE, 256> keyPre_;
	// 全キーの状態
	std::array<BYTE, 256> key_;
};

