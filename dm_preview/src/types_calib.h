// Copyright 2020 eYs3D Co., Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <ostream>

enum class StreamMode : std::int32_t
{
  STREAM_640x480 = 0,  /**< 480p, vga, left */
  STREAM_1280x480 = 1, /**< 480p, vga, left+right */
  STREAM_1280x720 = 2, /**< 720p, hd, left */
  STREAM_2560x720 = 3, /**< 720p, hd, left+right */
  STREAM_640x400 = 4,  /**< 8059, 480p, hd, left+right */
  STREAM_CUSTOM = 5,  /**< 8059, 480p, hd, left+right */
  STREAM_MODE_LAST
};
/**
 * @ingroup calibration
 * Camera intrinsics: size, coeffs and camera matrix.
 */
struct CameraIntrinsics {
  /** The width of the image in pixels */
  std::uint16_t width;
  /** The height of the image in pixels */
  std::uint16_t height;
  /** The focal length of the image plane, as a multiple of pixel width */
  double fx;
  /** The focal length of the image plane, as a multiple of pixel height */
  double fy;
  /** The horizontal coordinate of the principal point of the image */
  double cx;
  /** The vertical coordinate of the principal point of the image */
  double cy;
  /** The distortion coefficients: k1,k2,p1,p2,k3 */
  double coeffs[5];
  /** 3x4 projection matrix in the (rectified) coordinate systems 
   *  left: fx' cx' fy' cy' 1
   *  right: fx' cx' tx fy' cy' 1
   *  */
  double p[12];

  /**
   * 3x3 rectification transform (rotation matrix) for the left camera.
   */
  double r[9];
};

struct eYs3DPlyParameters
{
  std::uint16_t out_witdh;
  std::uint16_t out_height;
  float ReProjectMat[16];
};

std::ostream &operator<<(std::ostream &os, const CameraIntrinsics &in);

/**
 * @ingroup calibration
 * Camera intrinsics: size, coeffs and camera matrix.
 */
struct StreamIntrinsics {
  CameraIntrinsics left;
  CameraIntrinsics right;
  CameraIntrinsics left_d;
  eYs3DPlyParameters ply_parameters;
};

/**
 * @ingroup calibration
 * IMU intrinsics: scale, drift and variances.
 */
struct ImuIntrinsics {
  /**
   * Scale matrix.
   * \code
   *   Scale X     cross axis  cross axis
   *   cross axis  Scale Y     cross axis
   *   cross axis  cross axis  Scale Z
   * \endcode
   */
  double scale[3][3];
  /** Assembly error [3][3] */
  double assembly[3][3];
  /* Zero-drift: X, Y, Z */
  double drift[3];

  /** Noise density variances */
  double noise[3];
  /** Random walk variances */
  double bias[3];


  // std::uint8_t reserve[100];

  /** Temperature drift
   *  \code
   *    0 - Constant value
   *    1 - Slope
   *  \endcode
   */
  double x[2];
  double y[2];
  double z[2];
};

std::ostream &operator<<(std::ostream &os, const ImuIntrinsics &in);

/**
 * @ingroup calibration
 * Motion intrinsics, including accelerometer and gyroscope.
 */
struct MotionIntrinsics {
  ImuIntrinsics accel; /**< Accelerometer intrinsics */
  ImuIntrinsics gyro;  /**< Gyroscope intrinsics */
};

std::ostream &operator<<(std::ostream &os, const MotionIntrinsics &in);

/**
 * @ingroup calibration
 * Extrinsics, represent how the different datas are connected.
 */
struct Extrinsics {
  double rotation[3][3]; /**< Rotation matrix left camera to right camera*/
  double translation[3]; /**< Translation vector left camera to right camera*/

  /**
   * Inverse this extrinsics.
   * @return the inversed extrinsics.
   */
  Extrinsics Inverse() const {
    return {{{rotation[0][0], rotation[1][0], rotation[2][0]},
             {rotation[0][1], rotation[1][1], rotation[2][1]},
             {rotation[0][2], rotation[1][2], rotation[2][2]}},
            {-rotation[0][0] * translation[0] - rotation[1][0] * translation[1] - rotation[2][0] * translation[2],
             -rotation[0][1] * translation[0] - rotation[1][1] * translation[1] - rotation[2][1] * translation[2],
             -rotation[0][2] * translation[0] - rotation[1][2] * translation[1] - rotation[2][2] * translation[2]}};
  }
};

std::ostream &operator<<(std::ostream &os, const Extrinsics &ex);

/**
 * @ingroup calibration
 * Stream extrinsics.
 */
using StreamExtrinsics = Extrinsics;
/**
 * @ingroup calibration
 * Motion extrinsics.
 */
using MotionExtrinsics = Extrinsics;
