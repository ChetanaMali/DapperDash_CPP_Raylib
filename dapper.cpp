#include "raylib.h"

//Custom Data type
struct AnimData{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float runningTime;
    float updateTime;
};

int main()
{
    const int windowWidth = 512;
    const int windowHeight = 418;
    InitWindow(windowWidth, windowHeight, "Dapper Dash");

  
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

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float dt = GetFrameTime();

        if(scarfyData.pos.y >= windowHeight - scarfyData.rect.height)
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
        scarfyData.runningTime += dt;
        if(!isOnAir){
            if(scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.runningTime = 0;
                //update animation frame
                
                scarfyData.frame++;
                if(scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }
                 
                scarfyData.rect.x = scarfyData.frame * scarfyData.rect.width;
                
            }
        }
        

        //Update nebula's running animation
        for(int i = 0; i < sizeOfNebula ; i++){
            nebulaData[i].runningTime += dt;
            if(nebulaData[i].runningTime >= nebulaData[i].updateTime){
                nebulaData[i].runningTime = 0;
                //update anim frame
                nebulaData[i].rect.x = nebulaData[i].frame * nebulaData[i].rect.width;
                nebulaData[i].frame++;
                if(nebulaData[i].frame > 7){
                    nebulaData[i].frame = 0;
                }
        }

            nebulaData[2].runningTime = 0;
            //update anim frame
            nebulaData[2].rect.x = nebulaData[2].frame * nebulaData[2].rect.width;
            nebulaData[2].frame++;
            if(nebulaData[2].frame > 7){
                nebulaData[2].frame = 0;
            }
        }

       
        //Draw texture of nebula
        for(int i =0; i< sizeOfNebula; i++){
            DrawTextureRec(nebula, nebulaData[i].rect, nebulaData[i].pos, WHITE);
        }
       
        // draw Scarfy
        DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
    
}