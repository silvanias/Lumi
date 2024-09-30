#include "material.h"
#include "geometry/hittable/hittable.h"

glm::vec3 Material::emitted() const
{
    return glm::vec3(0, 0, 0);
}

Lambertian::Lambertian(const glm::vec3 &albedo) : albedo(albedo) {}

bool Lambertian::scatter(
    const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    auto scatter_direction = rec.normal + Utils::Sampling::sampleUnitSphere();
    // TODO: is it necessary to check if scatter_direction near zero?
    scattered = Ray(rec.point, scatter_direction);
    attenuation = albedo;
    return true;
}

Metal::Metal(const glm::vec3 &albedo) : albedo(albedo) {}

bool Metal::scatter(
    const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 reflected = reflect(r_in.direction(), rec.normal);
    scattered = Ray(rec.point, reflected);
    attenuation = albedo;
    return true;
}

DiffuseLight::DiffuseLight(const glm::vec3 &emit) : emit(emit) {}

glm::vec3 DiffuseLight::emitted() const
{
    return emit;
}

bool DiffuseLight::scatter(
    const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    return false;
}
