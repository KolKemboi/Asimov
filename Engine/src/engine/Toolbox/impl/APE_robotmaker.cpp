#include <APE_robotmaker.hpp>
#include <cstdio>
#include <fstream>
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

  for (const auto &[name, link] : model->links_) {
    std::string linkName = name;
    printf("=========%s=======\n", linkName.c_str());

    if (link->visual->geometry) {
      urdf::GeometrySharedPtr geometry = link->visual->geometry;
      auto GEOM_TYPE = geometry->type;

      if (GEOM_TYPE == urdf::Geometry::BOX) {
        std::shared_ptr<urdf::Box> box =
            std::dynamic_pointer_cast<urdf::Box>(geometry);
        urdf::Vector3 dims = box->dim;
        printf("DIM -> %f, %f, %f\n", dims.x, dims.y, dims.z);
      }else if (GEOM_TYPE == urdf::Geometry::SPHERE) {

      
      }
			
    }

  }
}
