#include "DxLib.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    // ウィンドウモードに設定
    ChangeWindowMode(TRUE);

    // DxLib初期化
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // 描画開始
    while (ProcessMessage() == 0)
    {
        // 画面クリア
        ClearDrawScreen();

        // 円を描画（x, y, 半径, 色）
        DrawCircle(400, 300, 50, GetColor(255, 255, 255), TRUE);

        // 裏画面を表画面に反映
        ScreenFlip();
    }

    // DxLib終了処理
    DxLib_End();
    return 0;
}
