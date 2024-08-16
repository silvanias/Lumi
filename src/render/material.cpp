#include "material.h"
#include "hittable.h"

Lambertian::Lambertian(const glm::vec3 &albedo) : albedo(albedo) {}

bool Lambertian::scatter(
    const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    auto scatter_direction = rec.normal + sampleUnitSphere();
    scattered = Ray(rec.point, scatter_direction);
    attenuation = albedo;
    return true;
}