#pragma once

#include "Classification.h"
#include "../Asset/AssetLoader.h"

struct Entity {
    ENTITY_NAME name;
    ENTITY_TYPE type;
    float x;
    float y;
    // This holds collision_box and quad data which is useful for drawing in ECS.
    Asset_Ase* asset;
};