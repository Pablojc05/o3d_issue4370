#include "Node.h"

int main(int argc, char **argv)
{
   ros::init(argc, argv, "example_o3d_node");

   ROS_INFO("[%s] Node initialization.", ros::this_node::getName().data());

   if (!ros::master::check())
   {
      ROS_ERROR("[%s] Roscore is not running.", ros::this_node::getName().data());
      return EXIT_FAILURE;
   }

   example_o3d::Node Node;
   Node.spin();

   ROS_INFO("[%s] Node finished.", ros::this_node::getName().data());

   return EXIT_SUCCESS;
}