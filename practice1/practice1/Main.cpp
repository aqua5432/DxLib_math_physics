#include "DxLib.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    ChangeWindowMode(TRUE);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // --------------------
    // 円のパラメータ
    // --------------------

    // プレイヤー
    float playerX = 400.0f;
    float playerY = 300.0f;
    const float playerRadius = 20.0f;
    const float playerSpeed = 5.0f;

    // 敵（今は動かない）
    float enemyX = 200.0f;
    float enemyY = 200.0f;
    const float enemyRadius = 30.0f;

    // --------------------
    // メインループ
    // --------------------
    while (ProcessMessage() == 0)
    {
        // 入力処理（WASD）
        if (CheckHitKey(KEY_INPUT_W))
        {
            playerY -= playerSpeed;
        }
        if (CheckHitKey(KEY_INPUT_S))
        {
            playerY += playerSpeed;
        }
        if (CheckHitKey(KEY_INPUT_A))
        {
            playerX -= playerSpeed;
        }
        if (CheckHitKey(KEY_INPUT_D))
        {
            playerX += playerSpeed;
        }

        // 画面クリア
        ClearDrawScreen();

        // 敵の円（赤）
        DrawCircle(
            static_cast<int>(enemyX),
            static_cast<int>(enemyY),
            static_cast<int>(enemyRadius),
            GetColor(255, 0, 0),
            TRUE
        );

        // プレイヤーの円（白）
        DrawCircle(
            static_cast<int>(playerX),
            static_cast<int>(playerY),
            static_cast<int>(playerRadius),
            GetColor(255, 255, 255),
            TRUE
        );

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
