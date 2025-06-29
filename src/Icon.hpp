#pragma once

#include <string>
#include <glad/gl.h>

GLuint loadPNG(const std::string& filePath, bool flipVertically = true);
