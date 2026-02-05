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

bool IsHitCircle(
    const Vec2& aPos, float aRadius,
    const Vec2& bPos, float bRadius)
{
    Vec2 diff = aPos - bPos;
    float distSq = diff.LengthSq();
    float radiusSum = aRadius + bRadius;

    return distSq < radiusSum * radiusSum;
}

enum class GameState
{
    Playing,
    GameOver,
    Clear
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

    GameState gameState = GameState::Playing;

    // 制限時間（ミリ秒）
    const int TIME_LIMIT_MS = 10000; // 10秒

    int startTime = 0;
    startTime = GetNowCount();

    // --------------------
    // メインループ
    // --------------------
    while (ProcessMessage() == 0)
    {
        int currentTime = GetNowCount();
        int elapsedTime = currentTime - startTime;

        // --------------------
        // 描画
        // --------------------
        ClearDrawScreen();

        switch (gameState)
        {
        case GameState::Playing:
            // 更新・入力・判定
            // 入力（Playing中のみ）
            if (CheckHitKey(KEY_INPUT_W)) playerPos.y -= playerSpeed;
            if (CheckHitKey(KEY_INPUT_S)) playerPos.y += playerSpeed;
            if (CheckHitKey(KEY_INPUT_A)) playerPos.x -= playerSpeed;
            if (CheckHitKey(KEY_INPUT_D)) playerPos.x += playerSpeed;

            // 衝突判定
            if (IsHitCircle(playerPos, playerRadius, enemyPos, enemyRadius))
            {
                gameState = GameState::GameOver;
            }

            // 制限時間クリア判定
            if (elapsedTime >= TIME_LIMIT_MS)
            {
                gameState = GameState::Clear;
            }
            break;

        case GameState::GameOver:
            // 表示のみ
            DrawFormatString(300, 20, GetColor(255, 0, 0), "GAME OVER");
            break;

        case GameState::Clear:
            // 表示のみ
            DrawFormatString(300, 20, GetColor(0, 255, 0), "GAME CLEAR");
            break;
        }

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

        // 残り時間表示
        int remainMs = TIME_LIMIT_MS - elapsedTime;
        if (remainMs < 0) remainMs = 0;

        DrawFormatString(
            10, 10,
            GetColor(255, 255, 255),
            "Time : %.2f",
            remainMs / 1000.0f
        );

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
