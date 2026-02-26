#include <iostream>
#include <raylib.h>
#include "SpriteAnimator.hpp"

int main()
{
    InitWindow(800, 600, "");
    SetTargetFPS(60);

    SpriteAnimator player("resources/image.png", 250, 256);
    
    player.AddAnimation("p", 0, 4, 0.1f, true);
    player.AddAnimation("attack", 2, 6, 0.07f, false);

    player.AddAnimation("up", 3, 4, 0.1f, true);
    player.AddAnimation("left", 1, 4, 0.1f, true);
    player.AddAnimation("right", 2, 4, 0.1f, true);
    player.AddAnimation("idle", 1, 6, 0.5, true);
    player.SetIdleAnimation("idle");

    player.SetScale(0.5f);
    player.SetPosition(200, 200);

    Camera2D camera = {0};
    camera.target = (Vector2){player.GetPosition().x + 20, player.GetPosition().y + 20};
    camera.offset = (Vector2){800 / 2.0f, 600 / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        bool attackFinished = player.UpdateAction(deltaTime, KEY_SPACE, "attack");
        player.Update(deltaTime);
        if (attackFinished)
        {
            std::cout << "Attack finished" << std::endl;
        }
        camera.target = (Vector2){player.GetPosition().x + 20, player.GetPosition().y + 20};
        Vector2 pos = player.GetPosition();

        if (IsKeyDown(KEY_LEFT))
        {
            player.SetPosition(pos.x - 500 * deltaTime, player.GetPosition().y);
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            player.SetPosition(pos.x + 500 * deltaTime, player.GetPosition().y);
        }

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(LIGHTGRAY);

        player.Draw();
        DrawRectangle(100, 325, 600, 100, BLUE);
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

/*
    g++ test/*.cpp src/SpriteAnimator.cpp -o prog -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
*/
