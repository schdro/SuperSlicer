#include "CameraUtils.hpp"
#include <igl/project.h> // projecting points

using namespace Slic3r;
using namespace GUI;

Points CameraUtils::project(const Camera &            camera,
                            const std::vector<Vec3d> &points)
{
    Vec4i viewport(camera.get_viewport().data());

    // Convert our std::vector to Eigen dynamic matrix.
    Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::DontAlign>
        pts(points.size(), 3);
    for (size_t i = 0; i < points.size(); ++i)
        pts.block<1, 3>(i, 0) = points[i];

    // Get the projections.
    Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::DontAlign> projections;
    igl::project(pts, camera.get_view_matrix().matrix(),
                    camera.get_projection_matrix().matrix(), viewport, projections);

    Points result;
    result.reserve(points.size());
    int window_height = viewport[3];

    // Iterate over all points and determine whether they're in the rectangle.
    for (int i = 0; i < projections.rows(); ++i) {
        double x = projections(i, 0);
        double y = projections(i, 1);
        // opposit direction o Y
        result.emplace_back(x, window_height - y);
    }
    return result;
}
