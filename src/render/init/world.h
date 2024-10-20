#pragma once

#include "utils.h"
#include "geometry/hittable/hittable_list.h"
#include "geometry/bounding/bvh.h"
#include "material/material.h"
#include "geometry/objects/sphere.h"
#include "geometry/objects/quad.h"

// Structure representing camera parameters for different scenes.
struct CamPos
{
    const glm::vec3 CAMERA_POS;
    const glm::vec3 CAMERA_FOCAL_POINT;
    const glm::vec3 CAMERA_UP_VEC;
    const float VERT_FOV;
};

struct World
{
    World(const CamPos &cam_pos, const HittableList &objs, const HittableList &lights) : camPos(cam_pos), objects(objs), lights(lights) {}

    CamPos camPos;
    HittableList objects;
    HittableList lights;
};

HittableList cornellBoxObjs();
extern const World litWorld;
extern const World cornellBox;