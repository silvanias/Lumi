#include "camera.h"
#include "pdf.h"
#include <iostream>

Camera::Camera(unsigned int imageWidth, unsigned int imageHeight, const glm::vec3 &pos, const glm::vec3 &focalPoint, const glm::vec3 &up, float vertFov, unsigned int samplesPerPixel, unsigned int maxDepth)
    : imageWidth(imageWidth), imageHeight(imageHeight), center(pos), lookAt(focalPoint),
      vup(up), vertFOV(vertFov), samplePerPixelPerFrame(samplesPerPixel), maxDepth(maxDepth)
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
    // Forces the grid to be square, in the future there is a potential to divide in rectangles
    sqrtSamplePerPixelPerFrame = int(std::sqrt(samplePerPixelPerFrame));
    recipSqrtSPPPF = 1 / sqrtSamplePerPixelPerFrame;

    auto focalLength = glm::length(center - lookAt);

    // Calculate viewport dimensions based on field of view and aspect ratio
    auto h = std::tan(vertFOV / 2);
    auto viewportHeight = 2 * h * focalLength;
    auto viewportWidth = viewportHeight * static_cast<float>(imageWidth) / static_cast<float>(imageHeight);

    // Camera frame basis vectors
    w = glm::normalize(center - lookAt);
    u = glm::normalize(glm::cross(vup, w));
    v = glm::cross(w, u);

    // Vectors across the horizontal and up the vertical viewport edges.
    auto viewportU = viewportWidth * u;
    auto viewportV = viewportHeight * v;

    // Horizontal and vertical delta vectors from pixel to pixel.
    pixelDeltaU = viewportU / static_cast<float>(imageWidth);
    pixelDeltaV = viewportV / static_cast<float>(imageHeight);

    auto viewportUpperLeft = center - (focalLength * w) - viewportU / 2.0f - viewportV / 2.0f;
    pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
}

// Get a random ray for a given pixel on the viewport
Ray Camera::getRandomRay(int x, int y) const
{
    auto offset = Utils::Sampling::sampleUnitSquare();
    auto pixelCenter = pixel00Loc + ((float(x) + offset.x) * pixelDeltaU) + ((float(y) + offset.y) * pixelDeltaV);
    auto rayDirection = pixelCenter - center;
    Ray r(center, rayDirection);
    return r;
}

// Get a random ray for a stratified square for a given pixel on the viewport
Ray Camera::getRandomStratifiedRay(glm::vec3 pixelCenter, int gridX, int gridY) const
{
    // -0.5 as the pixel spans -0.5f to +0.5f and grid top left is 0,0 in indices
    glm::vec3 subPixelX = (gridX * recipSqrtSPPPF - 0.5f) * pixelDeltaU;
    glm::vec3 subPixelY = (gridY * recipSqrtSPPPF - 0.5f) * pixelDeltaV;

    auto subPixel = pixelCenter + subPixelX + subPixelY;

    auto offset = Utils::Sampling::sampleUnitSquare() * recipSqrtSPPPF;
    auto offsetSubPixel = subPixel + offset;

    auto rayDirection = offsetSubPixel - center;
    Ray r(center, rayDirection);
    return r;
}

glm::vec3 Camera::rayColor(const Ray &r, const HittableList &world, const Hittable &lights, int depth) const
{
    if (depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;

    if (!world.hit(r, Interval(0.001f, INFINITY), rec))
        return glm::vec3(0, 0, 0);

    Ray scattered;
    glm::vec3 attenuation;
    float pdfValue;
    glm::vec3 colorFromEmission = rec.mat->emitted(rec);

    if (!rec.mat->scatter(r, rec, attenuation, scattered, pdfValue))
        return colorFromEmission;

    auto p0 = std::make_shared<HittablePDF>(lights, rec.point);
    auto p1 = std::make_shared<CosinePDF>(rec.normal);
    MixturePDF mixed_pdf(p0, p1);

    scattered = Ray(rec.point, mixed_pdf.generate());
    pdfValue = mixed_pdf.value(scattered.direction());

    float scattering_pdf = rec.mat->scatteringPDF(r, rec, scattered);

    glm::vec3 colorFromScatter = (attenuation * scattering_pdf * rayColor(scattered, world, lights, depth - 1)) / pdfValue;
    return colorFromScatter + colorFromEmission;
}

// Render the world into the accumulation buffer
void Camera::render(const HittableList &world, const Hittable &lights, std::vector<glm::vec3> &accumulationBuffer, std::vector<int> &sampleCount)
{
    for (int y = 0; y < imageHeight; ++y)
    {
        for (int x = 0; x < imageWidth; ++x)
        {
            int index = y * imageWidth + x;
            auto pixelCenter = pixel00Loc + (float(x) * pixelDeltaU) + (float(y) * pixelDeltaV);
            // Stratify samples in grid to reduce variance in the image
            for (int gridY = 0; gridY < sqrtSamplePerPixelPerFrame; gridY++)
            {
                for (int gridX = 0; gridX < sqrtSamplePerPixelPerFrame; gridX++)
                {
                    Ray r = getRandomStratifiedRay(pixelCenter, gridX, gridY);
                    accumulationBuffer[index] += rayColor(r, world, lights, maxDepth);
                    sampleCount[index] += 1;
                }
            }
        }
    }
}