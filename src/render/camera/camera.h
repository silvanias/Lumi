#pragma once

#include <vector>
#include "utils.h"
#include "geometry/hittable/hittable.h"
#include "geometry/hittable/hittable_list.h"
#include "material/material.h"
#include "init/world.h"

// Camera class handles ray generation and rendering for the scene.
class Camera
{
public:
    Camera(unsigned int imageWidth, unsigned int imageHeight, const glm::vec3 &pos, const glm::vec3 &focalPoint, const glm::vec3 &up, float vertFov, unsigned int samplesPerPixel, unsigned int maxDepth);
    Camera(unsigned int imageWidth, unsigned int imageHeight, const CamPos &camPos, unsigned int samplesPerPixel, unsigned int maxDepth);

    // Renders the world into the accumulation buffer.
    void render(const HittableList &world, std::vector<glm::vec3> &accumulationBuffer, std::vector<int> &sampleCount);

    int imageWidth;
    int imageHeight;

private:
    // Initializes camera settings
    void initialize();

    // Generates a random ray for a given pixel.
    Ray getRandomRay(int x, int y) const;

    // Generates a random ray for a stratified square for a given pixel.
    Ray getRandomStratifiedRay(glm::vec3 pixelCenter, int gridX, int gridY) const;

    // Computes the color of a ray intersecting with the world.
    glm::vec3 rayColor(const Ray &r, const HittableList &world, int depth) const;

    glm::vec3 center;               // Camera center
    glm::vec3 lookAt;               // Point camera is looking at
    glm::vec3 vup;                  // Camera-relative "up" direction
    float vertFOV;                  // Vertical view angle
    int samplePerPixelPerFrame;     // Must be a square number
    int sqrtSamplePerPixelPerFrame; // To subdivide pixel into a grid
    float recipSqrtSPPPF;
    int maxDepth; // Max number of ray bounces

    glm::vec3 pixel00Loc;
    glm::vec3 pixelDeltaU; // Offset to pixel to the right
    glm::vec3 pixelDeltaV; // Offset to pixel below
    glm::vec3 u;           // Camera frame basis vectors
    glm::vec3 v;
    glm::vec3 w;
};