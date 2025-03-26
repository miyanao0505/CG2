#include "MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// リークチェック
	D3DResourceLeakChecker leakCheck;
	// 宣言
	std::unique_ptr<MNFramework> game(new MyGame);

	// ゲームループ
	game->Run();

	return 0;
}