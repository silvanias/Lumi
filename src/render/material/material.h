#pragma once

#include <glm/vec3.hpp>
#include "utils.h"
#include "geometry/ray.h"

class HitRecord;

// Abstract base class for material properties and behaviors.
class Material
{
public:
    virtual ~Material() = default;

    // Determines if a ray is scattered by the material.
    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const = 0;

    // Returns emitted color from the material (default is no emission).
    virtual glm::vec3 emitted() const;
};

// Lambertian material for diffuse surfaces.
class Lambertian : public Material
{
public:
    explicit Lambertian(const glm::vec3 &albedo);

    bool scatter(
        const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 albedo; // Reflectance color
};

// Metal material for reflective surfaces.
class Metal : public Material
{
public:
    explicit Metal(const glm::vec3 &albedo);

    bool scatter(const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 albedo;
};

// Material for light-emitting surfaces.
class DiffuseLight : public Material
{
public:
    DiffuseLight(const glm::vec3 &emit);

    glm::vec3 emitted() const override;
    bool scatter(const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 emit; // Emission color
};
