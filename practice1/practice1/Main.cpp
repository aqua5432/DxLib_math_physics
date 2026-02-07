#include "DxLib.h"
#include "Math.h"
#include "Collision.h"
#include <vector>

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

    playerPos = Vec2(50.0f, 300.0f);
    enemyPos = Vec2(200.0f, 200.0f);

    startTime = GetNowCount();
    elapsedTime = 0;
}

std::vector<Circle> obstacles;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
    goal.radius = 40.0f;

    const float playerSpeed = 5.0f;

    GameState gameState = GameState::Title;

    int startTime = 0;
    int elapsedTime = 0;
    startTime = GetNowCount();

    for (int i = 0; i < 6; i++)
    {
        float x = (i % 2 == 0) ? 350.0f : 450.0f;
        obstacles.push_back({
            Vec2(x, 150.0f + i * 70.0f),
            18.0f
            });
    }


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
            gameState = GameState::Title;
        }

        if (gameState == GameState::Title && CheckHitKey(KEY_INPUT_SPACE)) {
            ResetGame(
                gameState,
                player.pos,
                enemy.pos,
                startTime,
                elapsedTime
            );
        }

        if (gameState == GameState::Playing)
        {
            if (IsOutOfScreen(player,SCREEN_WIDTH,SCREEN_HEIGHT))
            {
                gameState = GameState::GameOver;
            }
        }

        switch (gameState)
        {
        case GameState::Title:
            // 表示のみ
            DrawFormatString(270, 100, GetColor(255, 255, 255), "ルール");
            DrawFormatString(150, 150, GetColor(255, 255, 255), "1. WASDでプレイヤー(白い円)を操作");
            DrawFormatString(150, 200, GetColor(255, 255, 255), "2. 緑の円まで移動するタイムを競う");
            DrawFormatString(150, 250, GetColor(255, 255, 255), "3. 赤の円とぶつかる、画面外にいってしまうとゲームオーバー");
            DrawFormatString(150, 300, GetColor(255, 0, 0), "4. プレイヤーは2秒後に見えなくなります");
            DrawFormatString(200, 400, GetColor(255, 255, 255), "PRESS SPACE TO START!");
            break;

        case GameState::Playing:
            // 更新・入力・判定
            // 入力（Playing中のみ）
            if (CheckHitKey(KEY_INPUT_W)) player.pos.y -= playerSpeed;
            if (CheckHitKey(KEY_INPUT_S)) player.pos.y += playerSpeed;
            if (CheckHitKey(KEY_INPUT_A)) player.pos.x -= playerSpeed;
            if (CheckHitKey(KEY_INPUT_D)) player.pos.x += playerSpeed;

            // 衝突判定
            if (gameState == GameState::Playing)
            {
                for (const Circle& obs : obstacles)
                {
                    if (IsHitCircle(player, obs))
                    {
                        gameState = GameState::GameOver;
                        break;
                    }
                }
            }
            
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
            for (const Circle& obs : obstacles)
            {
                DrawCircle(
                    (int)obs.pos.x,
                    (int)obs.pos.y,
                    (int)obs.radius,
                    GetColor(255, 0, 0),
                    TRUE
                );
            }


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
