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

    virtual glm::vec3 emitted() const;
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

class DiffuseLight : public Material
{
public:
    DiffuseLight(const glm::vec3 &emit);

    glm::vec3 emitted() const override;
    bool scatter(const Ray &r_in, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 emit;
};
