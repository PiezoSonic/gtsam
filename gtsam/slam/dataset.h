/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file dataset.h
 * @date Jan 22, 2010
 * @author nikai, Luca Carlone
 * @brief utility functions for loading datasets
 */

#pragma once

#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/linear/NoiseModel.h>
#include <gtsam/geometry/Point2.h>
#include <gtsam/geometry/Pose3.h>
#include <gtsam/geometry/Cal3Bundler.h>
#include <gtsam/geometry/PinholeCamera.h>

#include <vector>
#include <utility> // for pair
#include <string>

namespace gtsam {

#ifndef MATLAB_MEX_FILE
/**
 * Find the full path to an example dataset distributed with gtsam.  The name
 * may be specified with or without a file extension - if no extension is
 * give, this function first looks for the .graph extension, then .txt.  We
 * first check the gtsam source tree for the file, followed by the installed
 * example dataset location.  Both the source tree and installed locations
 * are obtained from CMake during compilation.
 * @return The full path and filename to the requested dataset.
 * @throw std::invalid_argument if no matching file could be found using the
 * search process described above.
 */
GTSAM_EXPORT std::string findExampleDataFile(const std::string& name);
#endif

/**
 * Load TORO 2D Graph
 * @param dataset/model pair as constructed by [dataset]
 * @param maxID if non-zero cut out vertices >= maxID
 * @param addNoise add noise to the edges
 * @param smart try to reduce complexity of covariance to cheapest model
 */
GTSAM_EXPORT std::pair<NonlinearFactorGraph::shared_ptr, Values::shared_ptr> load2D(
    std::pair<std::string, boost::optional<noiseModel::Diagonal::shared_ptr> > dataset,
    int maxID = 0, bool addNoise = false, bool smart = true);

/**
 * Load TORO 2D Graph
 * @param filename
 * @param model optional noise model to use instead of one specified by file
 * @param maxID if non-zero cut out vertices >= maxID
 * @param addNoise add noise to the edges
 * @param smart try to reduce complexity of covariance to cheapest model
 */
GTSAM_EXPORT std::pair<NonlinearFactorGraph::shared_ptr, Values::shared_ptr> load2D(
    const std::string& filename,
    boost::optional<gtsam::SharedDiagonal> model = boost::optional<
    noiseModel::Diagonal::shared_ptr>(), int maxID = 0, bool addNoise = false,
    bool smart = true);

GTSAM_EXPORT std::pair<NonlinearFactorGraph::shared_ptr, Values::shared_ptr> load2D_robust(
    const std::string& filename,
    gtsam::noiseModel::Base::shared_ptr& model, int maxID = 0);

/** save 2d graph */
GTSAM_EXPORT void save2D(const NonlinearFactorGraph& graph, const Values& config,
    const noiseModel::Diagonal::shared_ptr model, const std::string& filename);

/**
 * Load TORO 3D Graph
 */
GTSAM_EXPORT bool load3D(const std::string& filename);

/// A measurement with its camera index
typedef std::pair<size_t,gtsam::Point2> SfM_Measurement;

/// Define the structure for the 3D points
struct SfM_Track
{
  gtsam::Point3 p; ///< 3D position of the point
  float r,g,b; ///< RGB color of the 3D point
  std::vector<SfM_Measurement> measurements; ///< The 2D image projections (id,(u,v))
  size_t number_measurements() const { return measurements.size();}
};

/// Define the structure for the camera poses
typedef gtsam::PinholeCamera<gtsam::Cal3Bundler> SfM_Camera;

/// Define the structure for SfM data
struct SfM_data
{
  std::vector<SfM_Camera> cameras;    ///< Set of cameras
  std::vector<SfM_Track> tracks; ///< Sparse set of points
  size_t number_cameras() const { return cameras.size();}   ///< The number of camera poses
  size_t number_tracks()  const { return tracks.size();}  ///< The number of reconstructed 3D points
};

/**
 * @brief This function parses a bundler output file and stores the data into a
 * SfM_data structure
 * @param filename The name of the bundler file
 * @param data SfM structure where the data is stored
 * @return true if the parsing was successful, false otherwise
 */
GTSAM_EXPORT bool readBundler(const std::string& filename, SfM_data &data);

/**
 * @brief This function parses a "Bundle Adjustment in the Large" (BAL) file and stores the data into a
 * SfM_data structure
 * @param filename The name of the BAL file
 * @param data SfM structure where the data is stored
 * @return true if the parsing was successful, false otherwise
 */
GTSAM_EXPORT bool readBAL(const std::string& filename, SfM_data &data);

/**
 * @brief This function writes a "Bundle Adjustment in the Large" (BAL) file from a
 * SfM_data structure
 * @param filename The name of the BAL file to write
 * @param data SfM structure where the data is stored
 * @return true if the parsing was successful, false otherwise
 */
GTSAM_EXPORT bool writeBAL(const std::string& filename, SfM_data &data);

/**
 * @brief This function writes a "Bundle Adjustment in the Large" (BAL) file from a
 * SfM_data structure and a value structure (measurements are the same as the SfM input data,
 * while camera poses and values are read from Values)
 * @param filename The name of the BAL file to write
 * @param data SfM structure where the data is stored
 * @param values structure where the graph values are stored
 * @return true if the parsing was successful, false otherwise
 */
GTSAM_EXPORT bool writeBALfromValues(const std::string& filename, SfM_data &data, Values& values);

/**
 * @brief This function converts an openGL camera pose to an GTSAM camera pose
 * @param R rotation in openGL
 * @param tx x component of the translation in openGL
 * @param ty y component of the translation in openGL
 * @param tz z component of the translation in openGL
 * @return Pose3 in GTSAM format
 */
GTSAM_EXPORT Pose3 openGL2gtsam(const Rot3& R, double tx, double ty, double tz);

/**
 * @brief This function converts a GTSAM camera pose to an openGL camera pose
 * @param R rotation in GTSAM
 * @param tx x component of the translation in GTSAM
 * @param ty y component of the translation in GTSAM
 * @param tz z component of the translation in GTSAM
 * @return Pose3 in openGL format
 */
GTSAM_EXPORT Pose3 gtsam2openGL(const Rot3& R, double tx, double ty, double tz);

/**
 * @brief This function converts a GTSAM camera pose to an openGL camera pose
 * @param PoseGTSAM pose in GTSAM format
 * @return Pose3 in openGL format
 */
GTSAM_EXPORT Pose3 gtsam2openGL(const Pose3& PoseGTSAM);

} // namespace gtsam
