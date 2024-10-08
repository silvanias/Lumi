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

extern const CamPos CAM_POS_SPHERES;
HittableList sphereWorld();
HittableList complexSphereWorld();

extern const CamPos CAM_POS_LIT;
HittableList litWorld();

extern const CamPos CAM_POS_QUAD;
HittableList quadWorld();

extern const CamPos CAM_POS_CORNELL_BOX;
HittableList cornellBox();
