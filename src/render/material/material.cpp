#include "material.h"
#include "geometry/hittable/hittable.h"

glm::vec3 Material::emitted() const
{
    return glm::vec3(0, 0, 0);
}

Lambertian::Lambertian(const glm::vec3 &albedo) : albedo(albedo) {}

bool Lambertian::scatter(
    const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered, float &pdf) const
{
    ONB uvw(rec.normal);
    auto scatter_direction = uvw.transform(Utils::Random::randomCosineDirection());
    scattered = Ray(rec.point, glm::normalize(scatter_direction));
    pdf = glm::dot(uvw.w(), scattered.direction()) / std::numbers::pi;
    // auto scatter_direction = rec.normal + Utils::Sampling::sampleUnitSphere();
    // scattered = Ray(rec.point, scatter_direction);
    // TODO: is it necessary to check if scatter_direction near zero?

    attenuation = albedo;
    return true;
}

double Lambertian::scatteringPDF(
    const Ray &r_in, const HitRecord &rec, const Ray &scattered) const
{
    auto cos_theta = glm::dot(rec.normal, glm::normalize(scattered.direction()));
    return cos_theta < 0 ? 0 : cos_theta / std::numbers::pi;
}

Metal::Metal(const glm::vec3 &albedo) : albedo(albedo) {}

bool Metal::scatter(
    const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered, float &pdf) const
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
    const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered, float &pdf) const
{
    return false;
}
