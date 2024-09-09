#include "camera.h"

Camera::Camera(unsigned int imageWidth, unsigned int imageHeight, const glm::vec3 &pos, const glm::vec3 &focalPoint, const glm::vec3 &up, float vertFov, unsigned int samplesPerPixel, unsigned int maxDepth)
    : image_width(imageWidth), image_height(imageHeight), center(pos), look_at(focalPoint),
      vup(up), vert_fov(vertFov), sample_per_pixel(samplesPerPixel), max_depth(maxDepth)
{
    initialize();
}

Camera::Camera(unsigned int imageWidth, unsigned int imageHeight, const CamPos &camPos, unsigned int samplesPerPixel, unsigned int maxDepth)
    : Camera(imageWidth, imageHeight, camPos.CAMERA_POS, camPos.CAMERA_FOCAL_POINT, camPos.CAMERA_UP_VEC, camPos.VERT_FOV, samplesPerPixel, maxDepth)
{
    // Delegate to main constructor
}

void Camera::initialize()
{
    auto focal_length = glm::length(center - look_at);

    // Calculate viewport dimensions based on field of view and aspect ratio
    auto h = std::tan(vert_fov / 2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * static_cast<float>(image_width) / static_cast<float>(image_height);

    // Camera frame basis vectors
    w = glm::normalize(center - look_at);
    u = glm::normalize(glm::cross(vup, w));
    v = glm::cross(w, u);

    // Vectors across the horizontal and up the vertical viewport edges.
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * v;

    // Horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / static_cast<float>(image_width);
    pixel_delta_v = viewport_v / static_cast<float>(image_height);

    auto viewport_upper_left = center - (focal_length * w) - viewport_u / 2.0f - viewport_v / 2.0f;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

// Get a random ray for a given pixel on the viewport
Ray Camera::getRandomRay(int x, int y) const
{
    auto offset = Utils::Sampling::sampleUnitSquare();
    auto pixel_center = pixel00_loc + ((float(x) + offset.x) * pixel_delta_u) + ((float(y) + offset.y) * pixel_delta_v);
    auto ray_direction = pixel_center - center;
    Ray r(center, ray_direction);
    return r;
}

glm::vec3 Camera::rayColor(const Ray &r, const HittableList &world, int depth) const
{
    if (depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;

    if (!world.hit(r, Interval(0.001f, INFINITY), rec))
        return glm::vec3(0, 0, 0);

    Ray scattered;
    glm::vec3 attenuation;
    glm::vec3 color_from_emission = rec.mat->emitted();

    if (!rec.mat->scatter(r, rec, attenuation, scattered))
        return color_from_emission;

    glm::vec3 color_from_scatter = attenuation * rayColor(scattered, world, depth - 1);
    return color_from_scatter + color_from_emission;
}

// Render the world into the accumulation buffer
void Camera::render(const HittableList &world, std::vector<glm::vec3> &accumulationBuffer, std::vector<int> &sampleCount)
{
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