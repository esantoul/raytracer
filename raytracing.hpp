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

class Object;
class Light;
class Intersect;
class Shape
{
public:
  virtual std::optional<Intersect> ray_intersect(const Ray &ray) const = 0;
  virtual Vec3f get_diffuse_light(const Vec3f &coordinates,
                                  const std::vector<Object> &inObj,
                                  const std::vector<Light> &inLight) const = 0;
  virtual ~Shape() {}
};

struct Intersect
{
  float distance_from_origin;
  Vec3f intersection_coordinates;
  Vec3f incidence_vector;
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

std::optional<std::pair<Intersect, const Object *>> cast_ray(const Ray &r, const std::vector<Object> &inObj)
{
  std::optional<std::pair<Intersect, const Object *>> first_intersect;
  for (const auto &obj : inObj)
  {
    if (auto val = obj.shape->ray_intersect(r))
    {
      if (!first_intersect || first_intersect->first.distance_from_origin > val->distance_from_origin)
        first_intersect = {*val, &obj};
    }
  }
  return first_intersect;
}

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

  static constexpr float epsilon = 0.001f;

  Vec3f get_diffuse_light(const Vec3f &obj_isect_coord,
                          const std::vector<Object> &inObj,
                          const std::vector<Light> &inLight) const override
  {
    Vec3f ret_light{0, 0, 0};
    for (const auto &light : inLight)
    {
      Vec3f incident = (obj_isect_coord - light.position);
      Ray light_ray{light.position, incident / incident.norm()};

      auto first_intersect = cast_ray(light_ray, inObj);
      if ((first_intersect->first.intersection_coordinates - obj_isect_coord).norm_inf() > epsilon)
        continue;

      Vec3f normal = (mCenter - obj_isect_coord).normalize();

      float coeff = std::pow(normal.dot(incident / incident.norm()), 2.f);
      if (coeff > 0)
        ret_light += light.power / (incident.norm() * incident.norm()) * coeff;
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
                   const Vector<2, float> &sensor_size,
                   const Vector<2, std::size_t> &sensor_resolution)
      : mPosition{position},
        mDirection{direction / direction.norm()},
        mFocal{focal_length},
        mRotation{rotation},
        mSensorSize{sensor_size},
        mSensorRes{sensor_resolution} {};

  void render(Vec3f *outFrame, const std::vector<Object> &inObj, const std::vector<Light> &inLight)
  {
    std::vector<std::pair<Intersect, const Object *>> opt;
    for (std::size_t i = 0; i < mSensorRes[1]; ++i)
    {
      for (std::size_t j = 0; j < mSensorRes[0]; ++j)
      {
        Vec3f px_pos{mSensorSize[0] * (j - mSensorRes[0] / 2.f) / mSensorRes[0], mSensorSize[1] * (i - mSensorRes[1] / 2.f) / mSensorRes[1], mFocal};
        px_pos += mPosition;
        Ray r{mPosition, px_pos - mPosition};
        auto first_intersect = cast_ray(r, inObj);
        if (first_intersect)
        {
          const auto &diffuse_light = first_intersect->second->shape->get_diffuse_light(first_intersect->first.intersection_coordinates, inObj, inLight);
          outFrame[j + i * mSensorRes[0]] = first_intersect->second->color.hadamard(diffuse_light);
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
  Vector<2, float> mSensorSize;
  Vector<2, std::size_t> mSensorRes;
};