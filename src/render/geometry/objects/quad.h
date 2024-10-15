#pragma once

#include "../hittable/hittable.h"
#include "../hittable/hittable_list.h"
#include "material/material.h"
#include "../bounding/aabb.h"
#include "../interval.h"
#include "../ray.h"

// Quad class represents a rectangular plane in 3D space.
class Quad : public Hittable
{
public:
    Quad(const glm::vec3 &Q, const glm::vec3 &u, const glm::vec3 &v, std::shared_ptr<Material> mat);

    void setBoundingBox();
    AABB boundingBox() const override;
    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override;
    bool inQuad(double alpha, double beta) const;
    double pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const override;
    glm::vec3 random(const glm::vec3 &origin) const override;

private:
    glm::vec3 Q; // A corner
    glm::vec3 u; // Vectors to the other corners
    glm::vec3 v;
    double area;

    glm::vec3 basis_scaling_factor;
    glm::vec3 normal;
    double D; // Offset of the plane from origin

    AABB bbox;
    std::shared_ptr<Material> mat;
};

// Generates a box formed by quads between two points in space.
std::shared_ptr<HittableList> Box(const glm::vec3 &a, const glm::vec3 &b, std::shared_ptr<Material> mat);