#include "camera/onb.h"

class PDF
{
public:
    virtual ~PDF() = default;

    virtual double value(const glm::vec3 &direction) const = 0;
    virtual glm::vec3 generate() const = 0;
};

class SpherePDF : public PDF
{
public:
    SpherePDF() = default;

    double value(const glm::vec3 &direction) const override
    {
        return 1 / (4 * std::numbers::pi);
    }

    glm::vec3 generate() const override
    {
        return Utils::Sampling::sampleUnitSphere();
    }
};

class CosinePDF : public PDF
{
public:
    CosinePDF(const glm::vec3 &w) : uvw(w) {}

    double value(const glm::vec3 &direction) const override
    {
        auto cosine_theta = glm::dot(glm::normalize(direction), uvw.w());
        return std::fmax(0, cosine_theta / std::numbers::pi);
    }

    glm::vec3 generate() const override
    {
        return uvw.transform(Utils::Random::randomCosineDirection());
    }

private:
    ONB uvw;
};

class HittablePDF : public PDF
{
public:
    HittablePDF(const Hittable &objects, const glm::vec3 &origin)
        : objects(objects), origin(origin)
    {
    }

    double value(const glm::vec3 &direction) const override
    {
        return objects.pdfValue(origin, direction);
    }

    glm::vec3 generate() const override
    {
        return objects.random(origin);
    }

private:
    const Hittable &objects;
    glm::vec3 origin;
};