#pragma once

#include "algebra.hpp"
#include <cstdio>
#include <vector>
#include <utility>
#include <memory>

struct Ray
{
  constexpr Ray(Vec3f origin, Vec3f direction, Vec3f strength = {1.f, 1.f, 1.f})
      : mOrigin{origin}, mDirection{direction}, mStrength{strength}
  {
  }

  Vec3f mOrigin;
  Vec3f mDirection;
  Vec3f mStrength;
};

struct Intersect
{
  float distance_from_origin;
  Vec3f intersection_coordinates;
  Vec3f incidence_vector;
};

class Object;
class Light;

class Shape
{
public:
  virtual std::optional<Intersect> ray_intersect(const Ray &ray) const = 0;
  virtual Vec3f get_diffuse_light(const Vec3f &coordinates,
                                  const std::vector<Object> &inObj,
                                  const std::vector<Light> &inLight) const = 0;
  virtual ~Shape() {}
};

struct Object
{
  std::shared_ptr<Shape> shape;
  Vec3f color;
};

struct Light
{
  Vec3f position;
  Vec3f power;
};

class Sphere : public Shape
{
public:
  constexpr Sphere(Vec3f center, float radius) : mCenter{center}, mRadius{radius} {}

  std::optional<Intersect> ray_intersect(const Ray &ray) const override
  {
    Vec3f L = ray.mOrigin - mCenter;
    float a = ray.mDirection.dot(ray.mDirection);
    float b = 2 * L.dot(ray.mDirection);
    float c = L.dot(L) - mRadius * mRadius;
    float delta = b * b - 4 * a * c;
    if (delta < 0)
      return {};
    float t1 = (-b - std::sqrt(delta)) / (2 * a);
    float t2 = (-b + std::sqrt(delta)) / (2 * a);
    Intersect isect;
    if (t1 < 0 && t2 < 0)
      return {};
    else if (t1 < 0)
      t1 = t2;
    else if (t1 > 0 && t2 > 0)
      t1 = std::min(t1, t2);
    isect.distance_from_origin = (t1 * ray.mDirection).norm();
    isect.intersection_coordinates = ray.mOrigin + t1 * ray.mDirection;
    isect.incidence_vector = ray.mDirection;
    return isect;
  }

#include <cstdio>

  Vec3f get_diffuse_light(const Vec3f &coordinates,
                          const std::vector<Object> &inObj,
                          const std::vector<Light> &inLight) const override
  {
    Vec3f ret_light{0, 0, 0};
    (void)inObj; // for shadows
    for (const auto &el : inLight)
    {
      Vec3f normal = (mCenter - coordinates).normalize();
      Vec3f incident = (coordinates - el.position);
      // printf("n%f,%f,%f\n", normal[0], normal[1], normal[2]);
      // printf("i%f,%f,%f\n", incident[0], incident[1], incident[2]);
      float coeff = std::pow(normal.dot(incident / incident.norm()), 2.f);
      if (coeff > 0)
        ret_light += el.power / (incident.norm() * incident.norm()) * coeff;
    }
    return ret_light;
  }

private:
  Vec3f mCenter;
  float mRadius;
};

class Camera
{
public:
  constexpr Camera(const Vec3f &position,
                   const Vec3f &direction,
                   const float &focal_length,
                   const float &rotation,
                   const Vec<2, float> &sensor_size,
                   const Vec<2, std::size_t> &sensor_resolution)
      : mPosition{position},
        mDirection{direction / direction.norm()},
        mFocal{focal_length},
        mRotation{rotation},
        mSensorSize{sensor_size},
        mSensorRes{sensor_resolution} {};

  void render(Vec3f *outFrame, const std::vector<Object> &inObj, const std::vector<Light> &inLight)
  {
    (void)inLight;
    std::vector<std::pair<Intersect, const Object *>> opt;
    for (std::size_t i = 0; i < mSensorRes[1]; ++i)
    {
      for (std::size_t j = 0; j < mSensorRes[0]; ++j)
      {
        Vec3f px_pos{mSensorSize[0] * (j - mSensorRes[0] / 2.f) / mSensorRes[0], mSensorSize[1] * (i - mSensorRes[1] / 2.f) / mSensorRes[1], mFocal};
        Ray r{mPosition, px_pos - mPosition};
        for (const auto &el : inObj)
          if (auto val = el.shape->ray_intersect(r))
            opt.push_back(std::pair<Intersect, const Object *>{val.value(), &el});
        if (opt.size())
        {
          const auto &target = *std::min_element(opt.begin(), opt.end(), [](auto lhs, auto rhs) {
            return lhs.first.distance_from_origin < rhs.first.distance_from_origin;
          });
          const auto &specular_light = target.second->shape->get_diffuse_light(target.first.intersection_coordinates, inObj, inLight);
          outFrame[j + i * mSensorRes[0]] = target.second->color.hadamard(specular_light);
        }
        opt.clear();
      }
    }
  }

private:
  Vec3f mPosition;
  Vec3f mDirection;
  float mFocal;
  float mRotation;
  Vec<2, float> mSensorSize;
  Vec<2, std::size_t> mSensorRes;
};