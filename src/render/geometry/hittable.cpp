#include "hittable.h"
#include "material/material.h"

void HitRecord::setFaceNormal(const Ray &r, const glm::vec3 &outward_normal)
{
    front_face = glm::dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}