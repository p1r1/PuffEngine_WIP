#pragma once

#include <glm/glm.hpp>
#include <PuffEngine/Vertex.h>

struct Cell;
///SRP(single responsibilty principle) each class have one singe responsibility


// POD(plain old data) just variable not have functions
struct Ball {
    Ball(float radius, float mass, const glm::vec2& pos,
         const glm::vec2& vel, unsigned int textureId,
         const PuffEngine::ColorRGBA8& color);

    float radius; 
    float mass; 
    glm::vec2 velocity;
    glm::vec2 position;
    unsigned int textureId = 0;
    PuffEngine::ColorRGBA8 color;
    Cell* ownerCell = nullptr;
    int cellVectorIndex = -1;
};
