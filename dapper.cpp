#include "raylib.h"

//Custom Data type
struct AnimData{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float runningTime;
    float updateTime;
};

bool IsOnGround(AnimData data, int windowHeight){
    return data.pos.y >= windowHeight - data.rect.height;
}

AnimData UpdateAnimData(AnimData data, float deltaTime, int maxFrame){
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime){
        data.runningTime = 0;
        //update animation fram
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if(data.frame > maxFrame){
            data.frame = 0;

        }

    }
    return data;
}
int main()
{
    const int windowWidth = 512;
    const int windowHeight = 418;
    InitWindow(windowWidth, windowHeight, "Dapper Dash");

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX=0;
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX = 0;
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX = 0;

    //nebula texture variable
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    int nebVelocity = -200;  

    const int sizeOfNebula = 6;
    AnimData nebulaData[sizeOfNebula];
    for(int i=0;i<sizeOfNebula;i++){
        nebulaData[i].rect.x = 0;
        nebulaData[i].rect.y = 0;
        nebulaData[i].rect.width = nebula.width/ 8;
        nebulaData[i].rect.height = nebula.height / 8;

        nebulaData[i].pos.y = windowHeight - nebula.height/8;
        nebulaData[i].pos.x = windowWidth + i * 300;

        nebulaData[i].frame = 0;
        nebulaData[i].runningTime = 0;
        nebulaData[i].updateTime = 1/12;
    }

    float finishedLine{nebulaData[sizeOfNebula - 1].pos.x};
    
    //scarfy texture variable
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rect.width = scarfy.width/6;
    scarfyData.rect.height = scarfy.height;
    scarfyData.rect.x = 0.0;
    scarfyData.rect.y = 0.0;
    scarfyData.pos.x = windowWidth/2 + scarfyData.rect.width/6;
    scarfyData.pos.y = windowHeight - scarfyData.rect.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1/12;
    scarfyData.runningTime = 0;

    int velocity = 0;
    const int gravity = 1000;  // pixel / sec / sec
    bool isOnAir = false;
    const int jumpVel = -600; // pixels / sec

    bool collision = false;
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float dt = GetFrameTime();

        //Scrolling background 
        bgX -= 20 * dt;
         if(bgX <= -background.width*2){
            bgX = 0;
        }
        mgX -= 40 * dt;
         if(mgX <= -midground.width*2){
            mgX = 0;
        }
        fgX -= 80 * dt;
        if(fgX <= -foreground.width*2){
            fgX = 0;
        }
       
        Vector2 bgPos{bgX , 0};
        DrawTextureEx(background, bgPos, 0, 2, WHITE);
        Vector2 bg2Pos{background.width * 2 + bgX,0};
        DrawTextureEx(background, bg2Pos, 0, 2, WHITE);

        Vector2 mgPos{mgX , 0};
        DrawTextureEx(midground, mgPos, 0, 2, WHITE);
        Vector2 mg2Pos{midground.width * 2 + mgX,0};
        DrawTextureEx(midground, mg2Pos, 0, 2, WHITE);

        Vector2 fgPos{fgX , 0};
        DrawTextureEx(foreground, fgPos, 0, 2, WHITE); 
        Vector2 fg2Pos{foreground.width * 2 + fgX,0};
        DrawTextureEx(foreground, fg2Pos, 0, 2, WHITE);

        if(IsOnGround(scarfyData, windowHeight))
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
        //update nebulas positions
        for(int i = 0; i < sizeOfNebula; i++){
            nebulaData[i].pos.x += nebVelocity * dt;
        }
        
        //update Scarfy position
        scarfyData.pos.y += velocity * dt;

        //Update scarfy's running animation
        
        if(!isOnAir){
            scarfyData = UpdateAnimData(scarfyData, dt, 5);
        }
        
        //update finishline
        finishedLine += nebVelocity * dt;
        //Update nebula's running animation
        for(int i = 0; i < sizeOfNebula ; i++){

            nebulaData[i] = UpdateAnimData(nebulaData[i], dt, 7);

        }
        
        //Cheking Game over Condition
        for(AnimData nebula : nebulaData){
            float pad = 50; // the size to get proper nebula shape istead of colliding the outer part of nebula
            Rectangle nebRect{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rect.width - 2*pad,
                nebula.rect.height - 2*pad,
            };
            Rectangle scarfyRect{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rect.width,
                scarfyData.rect.height
            };

            if(CheckCollisionRecs(nebRect, scarfyRect)){
                collision = true;
            }
       }
       if(collision){
            // Game over
            DrawText("Game Over!", windowWidth/4, windowHeight/2, 50, RED);
       }
       else if(scarfyData.pos.x >= finishedLine){
            DrawText("You win!", windowWidth/4,windowHeight/2,50,BLUE);
       }
       else
       {
             //Draw texture of nebula
            for(int i =0; i< sizeOfNebula; i++){
                DrawTextureRec(nebula, nebulaData[i].rect, nebulaData[i].pos, WHITE);
            }

            // draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);

       }
       
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
    
}