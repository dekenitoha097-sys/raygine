#include <iostream>
#include <SpriteAnimator.hpp>

int main()
{
    InitWindow(800, 600, "");
    SetTargetFPS(60);

    SpriteAnimator player("resources/img.png", 85, 121);
    player.AddAnimation("right", 0, 8,0.05,true);

    while (!WindowShouldClose())
    {

        float deltatime = GetFrameTime();
        player.Update(deltatime);
        Vector2 pos = player.GetPosition();

        if (IsKeyDown(KEY_RIGHT))
        {
            player.SetPosition(pos.x + 100 * deltatime, player.GetPosition().y);
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            
        }
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        player.Draw(0,0,100,100);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*
g++ *.cpp src/SpriteAnimator.cpp -o prog -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
*/