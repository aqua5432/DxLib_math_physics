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

struct Circle
{
    Vec2 pos;
    float radius;
};

bool IsHitCircle(const Circle& a, const Circle& b)
{
    Vec2 diff = a.pos - b.pos;
    float distSq = diff.LengthSq();   // 距離²
    float radiusSum = a.radius + b.radius;

    return distSq <= radiusSum * radiusSum;
}


enum class GameState
{
    Title,
    Playing,
    GameOver,
    Clear
};

void ResetGame(
    GameState& gameState,
    Vec2& playerPos,
    Vec2& enemyPos,
    int& startTime,
    int& elapsedTime)
{
    gameState = GameState::Playing;

    playerPos = Vec2(400.0f, 300.0f);
    enemyPos = Vec2(200.0f, 200.0f);

    startTime = GetNowCount();
    elapsedTime = 0;
}


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

    Circle player;
    Circle enemy;
    Circle goal;

    player.pos = Vec2(200.0f, 200.0f);
    player.radius = 16.0f;

    enemy.pos = Vec2(400.0f, 200.0f);
    enemy.radius = 20.0f;

    goal.pos = Vec2(600.0f, 400.0f);
    goal.radius = 24.0f;

    const float playerSpeed = 5.0f;

    GameState gameState = GameState::Title;

    int startTime = 0;
    int elapsedTime = 0;
    startTime = GetNowCount();

    // --------------------
    // メインループ
    // --------------------
    while (ProcessMessage() == 0)
    {

        // --------------------
        // 描画
        // --------------------
        ClearDrawScreen();

        bool isPlayerVisible = true;

        if (elapsedTime > 2000.0f)
        {
            isPlayerVisible = false;
        }

        if (gameState != GameState::Playing &&
            CheckHitKey(KEY_INPUT_RETURN))
        {
            if (gameState == GameState::Title) {
                ResetGame(
                    gameState,
                    player.pos,
                    enemy.pos,
                    startTime,
                    elapsedTime
                );
            }
            else {
                gameState = GameState::Title;
            }
        }

        switch (gameState)
        {
        case GameState::Title:
            // 表示のみ
            DrawFormatString(270, 100, GetColor(255, 255, 255), "ルール");
            DrawFormatString(150, 150, GetColor(255, 255, 255), "1. WASDでプレイヤー(白い円)を操作");
            DrawFormatString(150, 200, GetColor(255, 255, 255), "2. 緑の円まで移動するタイムを競う");
            DrawFormatString(150, 250, GetColor(255, 255, 255), "3. 赤の円とぶつかるとゲームオーバー");
            DrawFormatString(150, 300, GetColor(255, 0, 0), "4. プレイヤーは2秒後に見えなくなります");
            DrawFormatString(200, 400, GetColor(255, 255, 255), "PRESS ENTER TO START!");
            break;

        case GameState::Playing:
            // 更新・入力・判定
            // 入力（Playing中のみ）
            if (CheckHitKey(KEY_INPUT_W)) player.pos.y -= playerSpeed;
            if (CheckHitKey(KEY_INPUT_S)) player.pos.y += playerSpeed;
            if (CheckHitKey(KEY_INPUT_A)) player.pos.x -= playerSpeed;
            if (CheckHitKey(KEY_INPUT_D)) player.pos.x += playerSpeed;

            // 衝突判定
            if (IsHitCircle(player, enemy))
            {
                gameState = GameState::GameOver;
            }

            // 衝突判定
            if (IsHitCircle(player, goal))
            {
                gameState = GameState::Clear;
            }

            elapsedTime = GetNowCount() - startTime;

            break;

        case GameState::GameOver:
            // 表示のみ
            DrawFormatString(300, 20, GetColor(255, 0, 0), "GAME OVER");
            DrawFormatString(300, 40, GetColor(255, 255, 255), "PRESS ENTER TO TITLE");
            isPlayerVisible = true;
            break;

        case GameState::Clear:
            // 表示のみ
            DrawFormatString(300, 20, GetColor(0, 255, 0), "GAME CLEAR");
            DrawFormatString(300, 40, GetColor(255, 255, 255), "PRESS ENTER TO TITLE");
            isPlayerVisible = true;
            break;
        }

        if (gameState != GameState::Title) {
            // 敵（赤）
            DrawCircle(
                static_cast<int>(enemy.pos.x),
                static_cast<int>(enemy.pos.y),
                static_cast<int>(enemy.radius),
                GetColor(255, 0, 0),
                TRUE
            );

            if (isPlayerVisible) {
                // プレイヤー（白）
                DrawCircle(
                    static_cast<int>(player.pos.x),
                    static_cast<int>(player.pos.y),
                    static_cast<int>(player.radius),
                    GetColor(255, 255, 255),
                    TRUE
                );
            }

            // ゴール(緑)
            DrawCircle(
                (int)goal.pos.x,
                (int)goal.pos.y,
                (int)goal.radius,
                GetColor(0, 255, 0),
                TRUE
            );

            // 残り時間表示
            int remainMs = elapsedTime;
            if (remainMs < 0) remainMs = 0;

            DrawFormatString(
                10, 10,
                GetColor(255, 255, 255),
                "Time : %.2f",
                remainMs / 1000.0f
            );
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
