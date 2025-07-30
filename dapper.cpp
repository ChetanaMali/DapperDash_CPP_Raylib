#include "raylib.h"
int main()
{
    const int windowWidth = 512;
    const int windowHeight = 418;
    InitWindow(windowWidth, windowHeight, "Dapper Dash");

   
    int velocity = 0;
    const int gravity = 1000;  // pixel / sec / sec
    bool isOnAir = false;
    const int jumpVel = -600; // pixels / sec

    //nebula texture variable
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebullaRec;
    nebullaRec.width = nebula.width / 8;
    nebullaRec.height = nebula.height / 8;
    nebullaRec.x = 0;
    nebullaRec.y = 0;
    Vector2 nebulaPos;
    nebulaPos.x = windowWidth;
    nebulaPos.y = windowHeight - nebula.height;

    //scarfy texture variable
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = windowWidth/2 + scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;

    //scarfy animation
    int frame = 0;
    // 12 FPS animation
    const float updateTime = 1.0f / 12.0f; // amount of to before updating the frame
    float runningTime = 0;

    SetTargetFPS(30);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        float dt = GetFrameTime();

        if(scarfyPos.y >= windowHeight - scarfyRec.height)
        {
            //check that player is on ground
            velocity = 0;
            isOnAir = false;
        }
        else
        {
            //player is on air
            velocity += gravity * dt;
            isOnAir = true;
        }
        if(IsKeyPressed(KEY_SPACE) && !isOnAir){
            //jump the player
            velocity += jumpVel ;
        }

       
        scarfyPos.y += velocity * dt;

       
        runningTime += dt;
        if(runningTime >= updateTime)
        {
            runningTime = 0.0f;
            //update animation frame
            
            frame++;
            if(frame > 5)
            {
                frame = 0;
            }
            scarfyRec.x = frame * scarfyRec.width;
        }
       
        
       
        
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
    
}