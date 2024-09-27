#pragma once

#include <vector>
#include "utils.h"
#include "geometry/hittable.h"
#include "geometry/hittable_list.h"
#include "material/material.h"
#include "init/world.h"

class Camera
{
public:
    Camera(unsigned int imageWidth, unsigned int imageHeight, const glm::vec3 &pos, const glm::vec3 &focalPoint, const glm::vec3 &up, float vertFov, unsigned int samplesPerPixel, unsigned int maxDepth);
    Camera(unsigned int imageWidth, unsigned int imageHeight, const CamPos &camPos, unsigned int samplesPerPixel, unsigned int maxDepth);

    void render(const HittableList &world, std::vector<glm::vec3> &accumulationBuffer, std::vector<int> &sampleCount);

    int image_width;
    int image_height;

private:
    glm::vec3 center;                    // Camera center
    glm::vec3 look_at;                   // Point camera is looking at
    glm::vec3 vup;                       // Camera-relative "up" direction
    float vert_fov;                      // Vertical view angle
    int sample_per_pixel_per_frame;      // Must be a square number
    int sqrt_sample_per_pixel_per_frame; // To subdivide pixel into a grid
    float recip_sqrt_sppf;
    int max_depth; // Max number of ray bounces

    glm::vec3 pixel00_loc;
    glm::vec3 pixel_delta_u; // Offset to pixel to the right
    glm::vec3 pixel_delta_v; // Offset to pixel below
    glm::vec3 u;             // Camera frame basis vectors
    glm::vec3 v;
    glm::vec3 w;

    void initialize();
    Ray getRandomRay(int x, int y) const;
    Ray getRandomStratifiedRay(glm::vec3 pixelCenter, int gridX, int gridY) const;
    glm::vec3 rayColor(const Ray &r, const HittableList &world, int depth) const;
};