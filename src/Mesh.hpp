#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool loadFromOBJ(const std::string& objPath, const std::string& texturePath);
    void draw() const;

    void createQuad(const std::string& texturePath);

private:
    unsigned int VAO, VBO, EBO, textureID;
    unsigned int indexCount;

    void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    unsigned int loadTexture(const std::string& path);
    unsigned int loadSprite(const std::string& path);
};
