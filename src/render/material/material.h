#pragma once

#include <glm/vec3.hpp>
#include "utils.h"
#include "geometry/ray.h"

class HitRecord;

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    explicit Lambertian(const glm::vec3 &albedo);

    bool scatter(
        const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 albedo;
};

class Metal : public Material
{
public:
    explicit Metal(const glm::vec3 &albedo);

    bool scatter(const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 albedo;
};
