#pragma once

#include "utils.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"

class Camera
{
public:
    Camera(const unsigned int &IMAGE_WIDTH, const unsigned int &IMAGE_HEIGHT, const glm::vec3 &POS, const glm::vec3 &FOCAL_POINT, const glm::vec3 &UP, const float &VERT_FOV, const unsigned int &SAMPLE_PER_PIXEL, const unsigned int &MAX_DEPTH) : look_from(POS), look_at(FOCAL_POINT), vup(UP), vert_fov(VERT_FOV), image_width(IMAGE_WIDTH), image_height(IMAGE_HEIGHT), sample_per_pixel(SAMPLE_PER_PIXEL), max_depth(MAX_DEPTH) {}

    void render(const HittableList &world, std::vector<glm::vec3> &accumulationBuffer, std::vector<int> &sampleCount)
    {
        initialize();
        for (int y = 0; y < image_height; ++y)
        {
            for (int x = 0; x < image_width; ++x)
            {
                int index = y * image_width + x;
                for (int sample = 0; sample < sample_per_pixel; sample++)
                {
                    Ray r = getRandomRay(x, y);
                    accumulationBuffer[index] += rayColor(r, world, max_depth);
                    sampleCount[index] += 1;
                }
            }
        }
    }

    int image_width;  // Rendered image width
    int image_height; // Rendered image height

private:
    glm::vec3 center;           // Camera center
    glm::vec3 look_from;        // Point camera is looking from
    glm::vec3 look_at;          // Point camera is looking at
    glm::vec3 vup;              // Camera-relative "up" direction
    const float vert_fov;       // Vertical view angle
    const int sample_per_pixel; // Samples per pixel
    const int max_depth;        // Max number of ray bounces

    glm::vec3 pixel00_loc;   // Location of pixel 0, 0
    glm::vec3 pixel_delta_u; // Offset to pixel to the right
    glm::vec3 pixel_delta_v; // Offset to pixel below
    glm::vec3 u, v, w;       // Camera frame basis vectors

    void initialize()
    {
        this->center = look_from;
        auto focal_length = glm::length(look_from - look_at);

        // FOV
        auto h = std::tan(vert_fov / 2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * ((float)image_width / (float)image_height);

        // Camera frame basis vectors
        w = (look_from - look_at) / focal_length;
        u = glm::normalize(glm::cross(vup, w));
        v = glm::cross(w, u);

        // Vectors across the horizontal and up the vertical viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * v;

        // Horizontal and vertical delta vectors from pixel to pixel.
        this->pixel_delta_u = viewport_u / (float)image_width;
        this->pixel_delta_v = viewport_v / (float)image_height;

        auto viewport_upper_left = this->center - (focal_length * w) - viewport_u / 2.0f - viewport_v / 2.0f;
        this->pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
    }

    Ray getRandomRay(int x, int y) const
    {
        auto offset = sampleUnitSquare();
        auto pixel_center = pixel00_loc + ((float(x) + offset.x) * pixel_delta_u) + ((float(y) + offset.y) * pixel_delta_v);
        auto ray_direction = pixel_center - center;
        Ray r(center, ray_direction);
        return r;
    }

    glm::vec3 rayColor(const Ray &r, const HittableList &world, int depth) const
    {
        if (depth <= 0)
        {
            return glm::vec3(0.0f);
        }

        HitRecord rec;
        if (world.hit(r, Interval(0.001f, INFINITY), rec))
        {
            Ray scattered;
            if (auto attenuation = glm::vec3(0.0f); rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * rayColor(scattered, world, depth - 1);
            return glm::vec3(0.0f);
        }
        else
        {
            // Background gradient
            glm::vec3 unit_direction = glm::normalize(r.direction());
            auto factor = 0.5f * (unit_direction.y + 1.0f);
            auto lerp = (1.0f - factor) * glm::vec3(1.0f) + factor * glm::vec3(0.5, 0.7, 1.0);
            return lerp;
        }
    }
};
