#pragma once

#include "utils.h"
#include "geometry/hittable_list.h"
#include "geometry/bvh.h"
#include "material/material.h"
#include "geometry/sphere.h"
#include "geometry/quad.h"

struct CamPos
{
    const glm::vec3 CAMERA_POS;
    const glm::vec3 CAMERA_FOCAL_POINT;
    const glm::vec3 CAMERA_UP_VEC;
    const float VERT_FOV;
};

HittableList sphereWorld();
HittableList complexSphereWorld();
HittableList quadWorld();
HittableList cornellBox();

extern const CamPos CAM_POS_SPHERE;
extern const CamPos CAM_POS_QUAD;
extern const CamPos CAM_POS_CORNELL_BOX;