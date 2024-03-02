// raylibCppThing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"
#include "game.h"
#include "colors.h"

double lastUpdateTime = 0;

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    const int screenWidth = 1020;
    const int screenHeight = 1020;

    InitWindow(screenWidth, screenHeight, "raylib window title ig fuck you");
    SetTargetFPS(60000);

    Font font = LoadFontEx("font/arial.ttf", 64, 0, 0);

    Game game = Game();

    int frame = 0;
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(darkBlue);

        game.Draw();

        char successStr[20];
        sprintf_s(successStr, "successes: %d", game.grid.successCount);
        DrawTextEx(font, successStr, { 10, 10 }, 38, 2, BLACK);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}