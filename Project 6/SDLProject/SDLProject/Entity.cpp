#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    lives = 3;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other) {
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0);

    if (xdist < 0 && ydist < 0) return true;

    return false;
}


// Check for overlap X and Y
void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));

            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                topCollision = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                bottomCollision = true;
            }
        }
    }
}


void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                rightCollision = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                leftCollision = true;
            }
        }
    }
}


// player collisions with enemies (aliens)
void Entity::CheckCollisionsEnemy(Entity *enemies, int enemyCount) {
    for (int i = 0; i < enemyCount; i++) {
        Entity *enemy = &enemies[i];

        if (CheckCollision(enemy)) {
            playerDefeated = true; // any enemy collison occurs 
        }
        
    }
}


// player collisions with coin (cake)
void Entity::CheckCollisionsCoin(Entity *coins, int coinCount) {
    std::cout << "inside check coins collision";
    for (int i = 0; i < coinCount; i++) {
        Entity *coin = &coins[i];
        
        if (CheckCollision(coin)) {
            wasCollected = true;
            coin->isActive = false;
        }
    }
}


void Entity::AI(Entity *player) {
    switch(aiType) {
        case WALKER:
            AIWalker();
            break;
            
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
            
        case PATROLX:
            AIPatrolX();
            break;
            
        case PATROLY:
            AIPatrolY();
            break;
    }
}

void Entity::AIWalker() {
    movement = glm::vec3(-1, 0, 0);
}

void Entity::AIWaitAndGo(Entity *player) {
    switch(aiState) {
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f) {
                aiState = ACTIVE;
            }
            break;
            
        case ACTIVE:
            if (glm::distance(position, player->position) > 3.5f) {
                aiState = IDLE;
            }
            
            if (player->position.x < position.x) {
                movement = glm::vec3(-1, 0, 0);
            } else {
                movement = glm::vec3(1, 0, 0);
            }
            
            break;
    }
}

// moves in continuous loop on X axis
void Entity::AIPatrolX() {
    switch(aiState) {
        case IDLE:
            break;
            
        case ACTIVE:
            if (position.x > 1.0f) {
                movement = glm::vec3(-1, 0, 0);
            }
            else {
                position.x = 16.0f;
                movement = glm::vec3(-1, 0, 0);
            }
            movement = glm::vec3(-1, 0, 0);
            break;
    }
}
    
    // moves in continuous loop on Y axis
    void Entity::AIPatrolY() {
        switch(aiState) {
            case IDLE:
                break;
                
            case ACTIVE:
                if (position.y > -15.0f) {
                    movement = glm::vec3(0, -1, 0);
                }
                else {
                    position.y = 0.0f;
                    movement = glm::vec3(0, -1, 0);
                }
                movement = glm::vec3(0, -1, 0);
                break;
        }
}
    
void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Map *map)
{
    if (isActive == false) return;

    topCollision = false;
    bottomCollision = false;
    leftCollision = false;
    rightCollision = false;

    if (entityType == ENEMY) {
        AI(player);
    }
    else {
        CheckCollisionsEnemy(objects, objectCount);
    }

//    if (entityType == ENEMY) {
//        AI(player);
//    }
//    else {
//
//        if ((CheckCollision(&objects[0])) && (objects[0].entityType == ENEMY)){
//            CheckCollisionsEnemy(objects, objectCount);
//        }
//        else ((CheckCollision(&objects[0])) && (objects[0].entityType == COIN)) {
//            CheckCollisionsCoin(objects, objectCount);
//        }
//
//    }

    
//    if (entityType == PLAYER){
//         std::cout << "step 1 to get to coin collison";
//        CheckCollisionsCoin(objects, objectCount);
//    }
    

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    
    position += movement * speed * deltaTime;

    if (entityType == PLAYER) {
        CheckCollisionsY(objects, objectCount);
        CheckCollisionsX(objects, objectCount);
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
