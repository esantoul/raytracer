#include <cstdio>
#include <cstring>
#include "algebra.hpp"
#include "raytracing.hpp"

constexpr std::size_t kImgWidth = 1920;
constexpr std::size_t kImgHeight = 1080;

constexpr Vector<2, float> kSensorSize = 3 * Vector<2, float>{0.0192, 0.0108};

Camera cam{
    {0, 0, 0},
    {0, 0, 1},
    .055, // 55mm focal length
    0.f,
    kSensorSize,
    {kImgWidth, kImgHeight}};

constexpr char filename[] = "scenes/test.ppm";

std::vector<Object> myObjs{
    {std::make_shared<Sphere>(Vec3f{1.4, 0, 5}, 0.5), {0.5, 0.1, 0.1}},
    {std::make_shared<Sphere>(Vec3f{0.5, 0.5, 4}, 0.5), {0.1, 0.1, 0.5}},
    {std::make_shared<Sphere>(Vec3f{-1, -0.6, 4.5}, 0.5), {0.1, 0.5, 0.5}},
    {std::make_shared<Sphere>(Vec3f{-1, 0.5, 5.5}, 0.5), {0.3, 0.1, 0.5}}};

std::vector<Light> myLights{
    {{-1, 1, 1.5}, {10.f, 10.f, 10.f}},
    {{1.f, -3.f, 3.f}, {12.f, 2.f, 2.f}}};

Vec3f frame[kImgHeight * kImgWidth]{};

void fill_gradient(Vec3f fm[])
{
  for (std::size_t i = 0; i < kImgHeight; ++i)
  {
    for (std::size_t j = 0; j < kImgWidth; ++j)
    {
      fm[i * kImgWidth + j] = Vec3f{i / float(kImgHeight - 1), j / float(kImgWidth - 1), 0};
    }
  }
}

void write_frame_to_file(const char *fname)
{
  if (auto f = fopen(fname, "w"))
  {
    fprintf(f, "P6\n%ld %ld\n255\n", kImgWidth, kImgHeight);
    for (auto vec : frame)
    {
      for (auto comp : vec)
        fprintf(f, "%c", static_cast<int>(255 * std::max(0.f, std::min(comp, 1.f))));
    }
    fclose(f);
  }
  else
  {
    printf("Could not open file %s, Aborting!\n", filename);
  }
}

int main()
{
  std::fill(std::begin(frame), std::end(frame), Vec3f{0.2, 0.2, 0.2});
  cam.render(frame, myObjs, myLights);

  write_frame_to_file(filename);
  return 0;
}