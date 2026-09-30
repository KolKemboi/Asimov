#include <APE_robotmaker.hpp>
#include <cstdio>
#include <fstream>
#include <glm/mat3x3.hpp>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <urdf_model/joint.h>
#include <urdf_model/link.h>
#include <urdf_model/model.h>
#include <urdf_model/pose.h>
#include <urdf_model/types.h>
#include <urdfdom/urdf_parser/urdf_parser.h>

void RobotMaker::loadRobot(const std::string &path) {
#ifdef DEBUG__
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::warn);
#endif

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
  spdlog::debug("Robot Name:: %s", robotName);

  // parse links
  for (const auto &[name, link] : model->links_) {
    std::string linkName = name;
    Links.push_back(linkName);

    if (link->visual) {
      if (link->visual->geometry) {
        urdf::GeometrySharedPtr geometry = link->visual->geometry;
        auto GEOM_TYPE = geometry->type;

        if (GEOM_TYPE == urdf::Geometry::BOX) {
          std::shared_ptr<urdf::Box> box =
              std::dynamic_pointer_cast<urdf::Box>(geometry);
          urdf::Vector3 dims = box->dim;
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

          // printf("DIM -> %f, %f, %f\n", dims.x, dims.y, dims.z);
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

  for (const auto &[name, joint] : model->joints_) {
    std::string jointName = name;

    // auto since it is an unnamed enum type
    auto jointType = joint->type;

    std::string jointParentName = joint->parent_link_name;
    std::string jointChildName = joint->child_link_name;

    urdf::Pose pose = joint->parent_to_joint_origin_transform;
    urdf::Vector3 position = pose.position;
    urdf::Rotation rotation_quat = pose.rotation;

    spdlog::debug("joint name: %s ", jointName.c_str());

    if (jointType == urdf::Joint::CONTINUOUS) {
      spdlog::debug("CONTINUOUS");
    }
    if (jointType == urdf::Joint::FIXED) {
      spdlog::debug("FIXED");
    }
    if (jointType == urdf::Joint::FLOATING) {
      spdlog::debug("FLOATING");
    }
    if (jointType == urdf::Joint::PLANAR) {
      spdlog::debug("PLANAR");
    }
    if (jointType == urdf::Joint::PRISMATIC) {
      spdlog::debug("PRISMATIC");
    }
    if (jointType == urdf::Joint::REVOLUTE) {
      spdlog::debug("REVOLUTE");
    }
    if (jointType == urdf::Joint::UNKNOWN) {
      spdlog::debug("UNKNOWN");
    }
    if (_checkValidity(jointParentName)) {
      spdlog::debug("PARENT PRESENT");
    } else {
      spdlog::debug("PARENT ABSENT");
    }
    if (_checkValidity(jointParentName)) {
      spdlog::debug("CHILD PRESENT");
    } else {
      spdlog::debug("CHILD ABSENT");
    }
    spdlog::debug("Parent Name   %s ", jointParentName.c_str());
    spdlog::debug("Child Name   %s ", jointChildName.c_str());
    spdlog::debug("POS -> %f %f %f", position.x, position.y, position.z);
    spdlog::debug("ROT(QUAT) -> %f %f %f %f", rotation_quat.x, rotation_quat.y,
                  rotation_quat.z, rotation_quat.w);

    urdf::Vector3 axisOfAction = joint->axis;
    spdlog::debug("AXIS OF ACTION -> %f %f %f", axisOfAction.x, axisOfAction.y,
                  axisOfAction.z);

    if (joint->limits) {
      float lowerLim = (float)joint->limits->lower;
      float upperLim = (float)joint->limits->upper;
      float effort = (float)joint->limits->effort;
      float velocity = (float)joint->limits->velocity;

      spdlog::debug("LIMITS -> lower  %f upper %f effort %f velocity %f",
                    lowerLim, upperLim, effort, velocity);
    }
  }
}
