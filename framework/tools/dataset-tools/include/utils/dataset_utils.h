/*

 Copyright (c) 2019 University of Edinburgh, Imperial College, University of Manchester.
 Developed in the PAMELA project, EPSRC Programme Grant EP/K008730/1

 This code is licensed under the MIT License.

 */

#ifndef FRAMEWORK_TOOLS_DATASET_TOOLS_INCLUDE_DATASET_UTILS_H_
#define FRAMEWORK_TOOLS_DATASET_TOOLS_INCLUDE_DATASET_UTILS_H_

#include <boost/filesystem.hpp>

/**
 * Check for files and folders in a given directory and return true iff all exist
 *
 * @param directory_name directory to check for requirements
 * @param required vector of names of required files / folders
 */
inline bool checkRequirements(const std::string& directory_name, const std::vector<std::string>& requirements) {

  try {
    if (!boost::filesystem::exists(directory_name)) return false;

    boost::filesystem::directory_iterator end_itr;  // default construction yields past-the-end

    for (auto const &requirement : requirements) {
      bool seen = false;

      for (boost::filesystem::directory_iterator itr(directory_name); itr != end_itr; ++itr) {
        if (requirement == itr->path().filename()) seen = true;
      }

      if (!seen) {
        std::cout << "File not found: <dataset_dir>/" << requirement << std::endl;
        return false;
      }
    }
  } catch (boost::filesystem::filesystem_error &e) {
    std::cerr << "I/O Error with directory " << directory_name << std::endl;
    std::cerr << e.what() << std::endl;
    return false;
  }

  return true;
}

/// Load a generalised grey sensor
inline slambench::io::CameraSensor* makeGreySensor(const slambench::io::Sensor::pose_t &pose,
                                                   const slambench::io::CameraSensor::intrinsics_t &intrinsics,
                                                   const slambench::io::CameraSensor::distortion_coefficients_t &distortion) {

  using namespace slambench::io;

  auto grey_sensor = new CameraSensor("Grey", CameraSensor::kCameraType);

  grey_sensor->Index = 0;

  grey_sensor->Rate = 30.0;
  grey_sensor->Width = 640;
  grey_sensor->Height = 480;

  grey_sensor->FrameFormat = frameformat::Raster;
  grey_sensor->PixelFormat = pixelformat::G_I_8;
  grey_sensor->Description = "Grey";

  grey_sensor->CopyPose(pose);
  grey_sensor->CopyIntrinsics(intrinsics);
  grey_sensor->CopyRadialTangentialDistortion(distortion);
  grey_sensor->DistortionType = slambench::io::CameraSensor::RadialTangential;

  return grey_sensor;
}

/// Load a generalised gt sensor
inline slambench::io::GroundTruthSensor* makeGTSensor() {

  using namespace slambench::io;

  auto gt_sensor = new GroundTruthSensor("GroundTruth");
  gt_sensor->Index = 0;
  gt_sensor->Description = "GroundTruthSensor";

  return gt_sensor;
}

/// Load a generalised rgb sensor
inline slambench::io::CameraSensor* makeRGBSensor(const slambench::io::Sensor::pose_t &pose,
                                                  const slambench::io::CameraSensor::intrinsics_t &intrinsics,
                                                  const slambench::io::CameraSensor::distortion_coefficients_t &distortion) {

  using namespace slambench::io;

  auto rgb_sensor = new CameraSensor("RGB", CameraSensor::kCameraType);

  rgb_sensor->Index = 0;

  rgb_sensor->Rate = 30.0;
  rgb_sensor->Width = 640;
  rgb_sensor->Height = 480;
  rgb_sensor->FrameFormat = frameformat::Raster;
  rgb_sensor->PixelFormat = pixelformat::RGB_III_888;
  rgb_sensor->Description = "RGB";
  rgb_sensor->CopyPose(pose);
  rgb_sensor->CopyIntrinsics(intrinsics);
  rgb_sensor->DistortionType = slambench::io::CameraSensor::RadialTangential;
  rgb_sensor->CopyRadialTangentialDistortion(distortion);

  return rgb_sensor;
}

#endif  // FRAMEWORK_TOOLS_DATASET_TOOLS_INCLUDE_DATASET_UTILS_H_
