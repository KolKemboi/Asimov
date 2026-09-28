#include <APE_robotmaker.hpp>
#include <cstdio>
#include <fstream>
#include <glm/mat3x3.hpp>
#include <memory>
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
    Links.push_back(linkName);
    // printf("=========%s=======\n", linkName.c_str());

    if (link->visual) {
      if (link->visual->geometry) {
        printf("VISUIAL\n");
        urdf::GeometrySharedPtr geometry = link->visual->geometry;
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
          // grab the name
        }
      }

      urdf::Pose pose = link->visual->origin;
      urdf::Vector3 position = pose.position;
      urdf::Rotation rotation_quat = pose.rotation;

      if (link->visual->material) {
        urdf::Color color = link->visual->material->color;
        // printf("COLOR -> %f, %f, %f\n", color.r, color.g, color.b);
      }
    }

    if (link->collision) {
      // printf("======================COLLISSION======================\n");

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

#ifdef DEBUG__

    printf("=====> %s \n", jointName.c_str());

    if (jointType == urdf::Joint::CONTINUOUS) {
      printf("CONTINUOUS\n");
    }
    if (jointType == urdf::Joint::FIXED) {
      printf("FIXED\n");
    }
    if (jointType == urdf::Joint::FLOATING) {
      printf("FLOATING\n");
    }
    if (jointType == urdf::Joint::PLANAR) {
      printf("PLANAR\n");
    }
    if (jointType == urdf::Joint::PRISMATIC) {
      printf("PRISMATIC\n");
    }
    if (jointType == urdf::Joint::REVOLUTE) {
      printf("REVOLUTE\n");
    }
    if (jointType == urdf::Joint::UNKNOWN) {
      printf("UNKNOWN\n");
    }

    if (_checkValidity(jointParentName)) {
      printf("PARENT PRESENT\n");
    } else {
      printf("PARENT ABSENT\n");
    }

    if (_checkValidity(jointParentName)) {
      printf("CHILD PRESENT\n");
    } else {
      printf("CHILD ABSENT\n");
    }

    printf("Parent Name   %s \n", jointParentName.c_str());
    printf("Child Name   %s \n", jointChildName.c_str());

    printf("POS -> %f %f %f\n", position.x, position.y, position.z);
    printf("ROT(QUAT) -> %f %f %f %f\n", rotation_quat.x, rotation_quat.y,
           rotation_quat.z, rotation_quat.w);

    urdf::Vector3 axisOfAction = joint->axis;
    printf("AXIS OF ACTION -> %f %f %f\n", axisOfAction.x, axisOfAction.y,
           axisOfAction.z);
#endif

    if (joint->limits) {
      float lowerLim = (float)joint->limits->lower;
      float upperLim = (float)joint->limits->upper;
      float effort = (float)joint->limits->effort;
      float velocity = (float)joint->limits->velocity;

#ifdef DEBUG__
      printf("LIMITS -> lower  %f upper %f effort %f velocity %f\n", lowerLim,
             upperLim, effort, velocity);
#endif
    }
  }
}
