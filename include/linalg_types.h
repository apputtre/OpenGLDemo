#ifndef LINALG_TYPES_H
#define LINALG_TYPES_H

#include "tl3d/quat.h"
#include "linalg/vec.h"
#include "linalg/mat.h"

using vec2 = linalg::vec<2, float>;
using vec3 = linalg::vec<3, float>;
using vec4 = linalg::vec<4, float>;
using mat3 = linalg::mat<3, 3, float>;
using mat4 = linalg::mat<4, 4, float>;
using quat = tl3d::quat<float>;

#endif