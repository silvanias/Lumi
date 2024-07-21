#pragma once

#include "utils.h"
#include "hittable_list.h"

class Camera
{
public:
    Camera(int IMAGE_HEIGHT, int IMAGE_WIDTH, int SAMPLE_PER_PIXEL) : image_height(IMAGE_HEIGHT), image_width(IMAGE_WIDTH), sample_per_pixel(SAMPLE_PER_PIXEL) {}

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
                    accumulationBuffer[index] += rayColor(r, world);
                    sampleCount[index] += 1;
                }
            }
        }
    }

private:
    int image_height;        // Rendered image height
    int image_width;         // Rendered image width
    int sample_per_pixel;    // Samples per pixel
    glm::vec3 center;        // Camera center
    glm::vec3 pixel00_loc;   // Location of pixel 0, 0
    glm::vec3 pixel_delta_u; // Offset to pixel to the right
    glm::vec3 pixel_delta_v; // Offset to pixel below

    void initialize()
    {
        this->center = glm::vec3(0.0f, 0.0f, 0.0f);
        auto focal_length = 1.0f;
        auto viewport_height = 2.0f;
        auto viewport_width = viewport_height * ((float)image_width / (float)image_height);
        auto camera_center = glm::vec3(0, 0, 0);

        // Vectors across the horizontal and up the vertical viewport edges.
        auto viewport_u = glm::vec3(viewport_width, 0, 0);
        auto viewport_v = glm::vec3(0, viewport_height, 0);
        // Horizontal and vertical delta vectors from pixel to pixel.
        this->pixel_delta_u = viewport_u / (float)image_width;
        this->pixel_delta_v = viewport_v / (float)image_height;
        auto viewport_upper_left = camera_center - glm::vec3(0, 0, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
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

    glm::vec3 rayColor(const Ray &r, const HittableList &world) const
    {
        HitRecord rec;
        if (world.hit(r, Interval(0, INFINITY), rec))
        {
            // Colored to normal components
            return 0.5f * (rec.normal + glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else
        {
            // Background gradient
            glm::vec3 unit_direction = glm::normalize(r.direction());
            auto factor = 0.5f * (unit_direction.y + 1.0f);
            auto lerp = (1.0f - factor) * glm::vec3(1.0f, 1.0f, 1.0f) + factor * glm::vec3(0.5, 0.7, 1.0);
            return lerp;
        }
    }
};
