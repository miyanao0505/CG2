#include "MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// 宣言
	MNFramework* game = new MyGame;

	// ゲームループ
	game->Run();

	// 解放
	delete game;

	return 0;
}