#include "Win.h"

#define WIDTH 18
#define HEIGHT 17

unsigned int win_data[] =
{
          1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
          13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14,
           1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
};

void Win::Initialize() {
    
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("picnic.png");
    state.map = new Map(WIDTH, HEIGHT, win_data, mapTextureID, 1.0f, 12, 12);

   // Initialize and Load Player
   state.player = new Entity();
   state.player->entityType = PLAYER;

    state.player->position = glm::vec3(6.3, -13, 0);
   state.player->movement = glm::vec3(0);
   state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 5.0f;
   state.player->textureID = Util::LoadTexture("ninja_m.png");
   
   // Animate Player
   state.player->animRight = new int[3] {3, 4, 5};
   state.player->animLeft = new int[3] {9, 10, 11};
   state.player->animUp = new int[3] {0, 1, 2};
   state.player->animDown = new int[3] {6, 7, 8};

   state.player->animIndices = state.player->animRight;
   state.player->animFrames = 3;
   state.player->animIndex = 0;
   state.player->animTime = 0;
   state.player->animCols = 3;
   state.player->animRows = 4;

   state.player->height = 0.8f;
   state.player->width = 0.35f;

}

void Win::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, 0, state.map);

}

void Win::Render(ShaderProgram *program) {
        
    state.map->Render(program);
    state.player->Render(program);
    
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID,"You did it!", 0.4f, -0.2f, glm::vec3(3.5, 5, 0));
    Util::DrawText(program, fontTextureID, "Winner!!", 0.8f, -0.1f, glm::vec3(4, -10, 0));
    Util::DrawText(program, fontTextureID, "Press Enter to Play Again!", 0.4f, -0.2f, glm::vec3(4, -11, 0));
}

