#include "MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// 宣言
	std::unique_ptr<MNFramework> game = std::make_unique<MyGame>();

	// ゲームループ
	game->Run();

	return 0;
}