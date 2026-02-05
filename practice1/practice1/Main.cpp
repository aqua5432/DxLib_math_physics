#include "DxLib.h"
#include <cmath>

// --------------------
// Vec2（2Dベクトル）
// --------------------
struct Vec2
{
    float x;
    float y;

    Vec2() : x(0), y(0) {}
    Vec2(float _x, float _y) : x(_x), y(_y) {}

    Vec2 operator+(const Vec2& other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    float LengthSq() const
    {
        return x * x + y * y;
    }

    float Length() const
    {
        return sqrtf(LengthSq());
    }
};

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
    // 円の設定
    // --------------------
    Vec2 playerPos(400.0f, 300.0f);
    const float playerRadius = 20.0f;
    const float playerSpeed = 5.0f;

    Vec2 enemyPos(200.0f, 200.0f);
    const float enemyRadius = 30.0f;

    // --------------------
    // メインループ
    // --------------------
    while (ProcessMessage() == 0)
    {
        // 入力（WASD）
        if (CheckHitKey(KEY_INPUT_W)) playerPos.y -= playerSpeed;
        if (CheckHitKey(KEY_INPUT_S)) playerPos.y += playerSpeed;
        if (CheckHitKey(KEY_INPUT_A)) playerPos.x -= playerSpeed;
        if (CheckHitKey(KEY_INPUT_D)) playerPos.x += playerSpeed;

        // --------------------
        // 距離計算
        // --------------------
        Vec2 diff = enemyPos - playerPos;
        float distance = diff.Length();
        float distanceSq = diff.LengthSq();

        // --------------------
        // 描画
        // --------------------
        ClearDrawScreen();

        // 敵（赤）
        DrawCircle(
            static_cast<int>(enemyPos.x),
            static_cast<int>(enemyPos.y),
            static_cast<int>(enemyRadius),
            GetColor(255, 0, 0),
            TRUE
        );

        // プレイヤー（白）
        DrawCircle(
            static_cast<int>(playerPos.x),
            static_cast<int>(playerPos.y),
            static_cast<int>(playerRadius),
            GetColor(255, 255, 255),
            TRUE
        );

        // 距離表示
        DrawFormatString(
            10, 10,
            GetColor(255, 255, 255),
            "Distance     : %.2f", distance
        );

        DrawFormatString(
            10, 30,
            GetColor(255, 255, 255),
            "Distance^2 : %.2f", distanceSq
        );

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
