#include <stdio.h>
#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 500;

int score_1 = 0;
int score_2 = 0;



char result_text[10] = "0:0";

struct Paddle {
    float pos_x, pos_y;
    float width, height;
    float speed;
};

struct Ball {
    float pos_x, pos_y;
    float radius;
    float speed_x, speed_y;
};

void updatePlayerPaddle1(Paddle *player) {
    if (IsKeyDown(KEY_W)) 
        player->pos_y -= player->speed * GetFrameTime();

    if (player->pos_y < 0)
        player->pos_y = 0;
        

    if (IsKeyDown(KEY_S))
        player->pos_y += player->speed * GetFrameTime();

    if (player->pos_y + player->height > screenHeight)
        player->pos_y = screenHeight - player->height;
        
}

void updatePlayerPaddle2(Paddle *player) {
    if (IsKeyDown(KEY_UP)) 
        player->pos_y -= player->speed * GetFrameTime();

    if (player->pos_y < 0)
        player->pos_y = 0;
        

    if (IsKeyDown(KEY_DOWN))
        player->pos_y += player->speed * GetFrameTime();

    if (player->pos_y + player->height > screenHeight)
        player->pos_y = screenHeight - player->height;
        
}

void updateBall(Ball* ball, Paddle* p1, Paddle* p2, Sound blip) {
    //change position based on speed
    ball->pos_x += ball->speed_x * GetFrameTime();
    ball->pos_y += ball->speed_y * GetFrameTime();

    //check for collision with pad1
    if (CheckCollisionCircleRec(Vector2{ ball->pos_x, ball->pos_y }, ball->radius, Rectangle{p1->pos_x, p1->pos_y, p1->width, p1->height})
        && ball->speed_x < 0) {
        ball->speed_x *= -1.1f;
        PlaySound(blip);
    }

    //check for collision with pad2
    if (CheckCollisionCircleRec(Vector2{ ball->pos_x, ball->pos_y }, ball->radius, Rectangle{p2->pos_x, p2->pos_y, p2->width, p2->height})
        && ball->speed_x > 0) {
        ball->speed_x *= -1.1f;
        PlaySound(blip);
    }

    //bounce ball on the y axis
    if (ball->pos_y + ball->radius >= screenHeight || ball->pos_y - ball->radius <= 0) {
        ball->speed_y = - ball->speed_y;
    }

    //second player scores a point
    if (ball->pos_x < 0) {
        score_2++;
        ball->pos_x = screenWidth / 2;
        ball->pos_y = screenHeight / 2;
        ball->speed_x = 100.0f;
        sprintf(result_text, "%d:%d", score_1, score_2);
    }

    //first player scores a point
    if (ball->pos_x > screenWidth) {
        score_1++;
        ball->pos_x = screenWidth / 2;
        ball->pos_y = screenHeight / 2;
        ball->speed_x = 100.0f;
        sprintf(result_text, "%d:%d", score_1, score_2);
    }
}

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib");

    InitAudioDevice();

    Sound blip = LoadSound("res/blip.mp3");

    sprintf(result_text, "%d:%d", score_1, score_2);

    Paddle player = {};
    player.pos_x =  (float)screenWidth / 10;
    player.pos_y =  (float)screenHeight / 2;
    player.width =  (float)screenWidth / 40;
    player.height = (float)screenHeight / 5;
    player.speed =  200.0f;

    Paddle enemy = {};
    enemy.pos_x =  (float)screenWidth * 9 / 10;
    enemy.pos_y =  (float)screenHeight / 2;
    enemy.width =  (float)screenWidth / 40;
    enemy.height = (float)screenHeight / 5;
    enemy.speed =  200.0f;

    Ball ball = {};
    ball.pos_x =    (float)screenWidth / 2;
    ball.pos_y =    (float)screenHeight / 2;
    ball.radius =   (float)screenWidth / 80;
    ball.speed_x =  100.0f;
    ball.speed_y =  100.0f;

    //SetTargetFPS(60); 

     while (!WindowShouldClose()) {   // Detect window close button or ESC key

        updatePlayerPaddle1(&player);
        updatePlayerPaddle2(&enemy);

        updateBall(&ball, &player, &enemy, blip);

        BeginDrawing();
            {
                ClearBackground(BLACK);
                DrawRectangle(player.pos_x, player.pos_y, player.width, player.height, GOLD);
                DrawRectangle(enemy.pos_x,  enemy.pos_y,  enemy.width,  enemy.height,  LIME);
                DrawCircle(ball.pos_x, ball.pos_y, ball.radius, RAYWHITE);
                int textWidth = MeasureText(result_text, 40);
                DrawText(result_text, screenWidth/2 - textWidth/2, 40, 40, RAYWHITE);
                DrawFPS(10, 10);
            }
        EndDrawing();

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadSound(blip);

    CloseAudioDevice(); 
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}