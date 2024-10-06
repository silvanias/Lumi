#include "hittable.h"
#include "material/material.h"

void HitRecord::setFaceNormal(const Ray &r, const glm::vec3 &outwardNormal)
{
    frontFace = glm::dot(r.direction(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
}