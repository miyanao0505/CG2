#include "Input.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::instance = nullptr;

// シングルトンインスタンスの取得
Input* Input::GetInstance()
{
	if (instance == nullptr) {
		instance = new Input();
	}
	return instance;
}

/// 初期化
void Input::Initialize(WindowsAPI* winApi)
{
	HRESULT hr;

	// 狩りてきたWinApiのインスタンスを記録
	winApi_ = winApi;

	// DirectInputの初期化
	hr = DirectInput8Create(winApi_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);	// 標準形式
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(winApi_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

// 終了
void Input::Finalize()
{
	delete instance;
	instance = nullptr;
}

/// 更新
void Input::Update()
{
	HRESULT hr;

	// 前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	// キーボード情報の取得開始
	hr = keyboard->Acquire();
	// 全キーの入力状態を取得する
	hr = keyboard->GetDeviceState(sizeof(key), key);
}

/// キーのトリガーをチェック
bool Input::TriggerKey(BYTE keyNumber){
	// 指定キーを押した時にtrueを返す
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}
	// そうでなければfalseを返す
	return false;
}

/// キーの押下をチェック
bool Input::PushKey(BYTE keyNumber) {
	// 指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	// そうでなければfalseを返す
	return false;
}
