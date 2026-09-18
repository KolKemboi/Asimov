#include <APE_robotmaker.hpp>
#include <cstdio>
#include <fstream>
#include <glm/mat3x3.hpp>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <urdf_model/link.h>
#include <urdf_model/model.h>
#include <urdf_model/pose.h>
#include <urdf_model/types.h>
#include <urdfdom/urdf_parser/urdf_parser.h>

void RobotMaker::loadRobot(const std::string &path) {
  // read file
  std::ifstream file(path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  urdf::ModelInterfaceSharedPtr model = urdf::parseURDF(buffer.str());
  if (!model) {
    throw std::runtime_error("Failed");
  }
  std::string robotName = model->getName();
  printf("%s \n", robotName.c_str());

  // parse links

  for (const auto &[name, link] : model->links_) {
    std::string linkName = name;
    printf("=========%s=======\n", linkName.c_str());

    if (link->visual) {
      if (link->visual->geometry) {
        urdf::GeometrySharedPtr geometry = link->visual->geometry;
        auto GEOM_TYPE = geometry->type;

        if (GEOM_TYPE == urdf::Geometry::BOX) {
          std::shared_ptr<urdf::Box> box =
              std::dynamic_pointer_cast<urdf::Box>(geometry);
          urdf::Vector3 dims = box->dim;
          printf("DIM -> %f, %f, %f\n", dims.x, dims.y, dims.z);
        } else if (GEOM_TYPE == urdf::Geometry::SPHERE) {
          std::shared_ptr<urdf::Sphere> sphere =
              std::dynamic_pointer_cast<urdf::Sphere>(geometry);
          double radius = sphere->radius;
        } else if (GEOM_TYPE == urdf::Geometry::MESH) {
          std::shared_ptr<urdf::Mesh> mesh =
              std::dynamic_pointer_cast<urdf::Mesh>(geometry);
          urdf::Vector3 dims = mesh->scale;
        }
      }

      urdf::Pose pose = link->visual->origin;
      urdf::Vector3 position = pose.position;
      urdf::Rotation rotation_quat = pose.rotation;

      if (link->visual->material) {
        urdf::Color color = link->visual->material->color;
        printf("COLOR -> %f, %f, %f\n", color.r, color.g, color.b);
      }
    }

    if (link->collision) {
      if (link->collision->geometry) {
        urdf::GeometrySharedPtr geometry = link->collision->geometry;
        auto GEOM_TYPE = geometry->type;

        if (GEOM_TYPE == urdf::Geometry::BOX) {
          std::shared_ptr<urdf::Box> box =
              std::dynamic_pointer_cast<urdf::Box>(geometry);
          urdf::Vector3 dims = box->dim;
          printf("DIM -> %f, %f, %f\n", dims.x, dims.y, dims.z);
        } else if (GEOM_TYPE == urdf::Geometry::SPHERE) {
          std::shared_ptr<urdf::Sphere> sphere =
              std::dynamic_pointer_cast<urdf::Sphere>(geometry);
          double radius = sphere->radius;
        } else if (GEOM_TYPE == urdf::Geometry::MESH) {
          std::shared_ptr<urdf::Mesh> mesh =
              std::dynamic_pointer_cast<urdf::Mesh>(geometry);
          urdf::Vector3 dims = mesh->scale;
        }
      }
    }
    if (link->inertial) {
      double mass = link->inertial->mass;

      glm::mat3x3 inertial_vars; // glm is collumn major
      inertial_vars[0][0] = link->inertial->ixx;
      inertial_vars[1][1] = link->inertial->iyy;
      inertial_vars[2][2] = link->inertial->izz;
      inertial_vars[1][0] = link->inertial->ixy;
      inertial_vars[2][0] = link->inertial->ixz;
      inertial_vars[2][1] = link->inertial->iyz;
    }
  }
}
