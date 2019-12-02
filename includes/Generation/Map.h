
#ifndef VOX_MAP_H
#define VOX_MAP_H

#include "vox.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <iostream>


class StoredMapData
{
    public:
        float value;
//      std::vector<Blocks*> vec;
    StoredMapData(float _value) {value = _value;};
};

#endif
