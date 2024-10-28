#include "WindowsAPI.h"
#include "../externals/imgui/imgui.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib, "winmm.lib")

// ウィンドウプロシージャ
LRESULT CALLBACK WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// 初期化
void WindowsAPI::Initialize(){
	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	HRESULT hr;

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	// ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;
	// ウィンドウクラス名(なんでも良い)
	wc.lpszClassName = L"GE3WindowClass";
	// インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc);

	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0, 0, kClientWidth, kClientHeight };

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hwnd = CreateWindow(
		wc.lpszClassName,			// 利用するクラス名
		L"GE3",						// タイトルバーの文字(何でも良い)
		WS_OVERLAPPEDWINDOW,		// よく見るウィンドウスタイル
		CW_USEDEFAULT,				// 表示X座標(Windowsに任せる)
		CW_USEDEFAULT,				// 表示Y座標(WindowsOSに任せる)
		wrc.right - wrc.left,		// ウィンドウ横幅
		wrc.bottom - wrc.top,		// ウィンドウ縦幅
		nullptr,					// 親ウィンドウハンドル
		nullptr,					// メニューハンドル
		wc.hInstance,				// インスタンスハンドル
		nullptr						// オプション
	);

	// ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);
}

// 更新
void WindowsAPI::Update()
{
}

// メッセージの処理
bool WindowsAPI::ProcessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

// 終了
void WindowsAPI::Finalize(){
	CloseWindow(hwnd);
	CoUninitialize();
}
