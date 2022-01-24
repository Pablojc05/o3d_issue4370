#include "Node.h"
#include "My_O3D_GPU.h"

namespace example_o3d {
Node::Node() {
   ROS_DEBUG("[%s]|[%s]", ros::this_node::getName().data(), __PRETTY_FUNCTION__);
}

Node::~Node() {
   ROS_DEBUG("[%s]|[%s]", ros::this_node::getName().data(), __PRETTY_FUNCTION__);
}

void Node::spin() 
{
   ROS_DEBUG("[%s]|[%s]", ros::this_node::getName().data(), __PRETTY_FUNCTION__);
   // ros::Subscriber sub_pointcloud = _nh.subscribe(
   //    "/os_node/points", 1, &Node::managePointcloud, this);
   
   // Replace poincloud from ros topic for a downloaded pc
   open3d::geometry::PointCloud example_cloud;
   sensor_msgs::PointCloud2 temp_pointcloud_msg;
   sensor_msgs::PointCloud2Ptr temp_PcPtr_msg (new sensor_msgs::PointCloud2);
   open3d::io::ReadPointCloudFromPCD("/home/jborrego/catkin_ws3/src/example_o3d/pcd/cloud.pcd", example_cloud, {});
   open3d_conversions::open3dToRos(example_cloud, *temp_PcPtr_msg);

   _pub_remainder_pointcloud    = _nh.advertise<sensor_msgs::PointCloud2>("/remainder_points", 1);

   managePointcloud(temp_PcPtr_msg); // Comment this if subscriber is uncommented
   // ros::spin(); // Uncomment this if subscriber is uncommented
}

void Node::publishTopics()
{
   // Publish data

   _remainder_pointcloud_msg.header.frame_id = "os_lidar";
   _remainder_pointcloud_msg.header.stamp    = _original_stamp;
   _pub_remainder_pointcloud.publish(_remainder_pointcloud_msg);
}

void Node::managePointcloud(const sensor_msgs::PointCloud2ConstPtr &msg) {

   // Conversion from ros msg to open3d cloud and removing NaN points
   open3d::geometry::PointCloud original_cloud;
   open3d_conversions::rosToOpen3d(msg, original_cloud);
   original_cloud.RemoveNonFinitePoints(true, true);

   // Create a PointCloud at the CUDA device
   const open3d::core::Device gpu = open3d::core::Device("CUDA:0");
   open3d::t::geometry::PointCloud original_cuda_cloud = open3d::t::geometry::PointCloud::FromLegacy(original_cloud,open3d::core::Float32,gpu);

   _original_stamp = ros::Time::now();

   // Transfer the PC to CPU
   open3d::geometry::PointCloud remainder_cloud;
   

   open3d::core::SizeVector shape {15};
   open3d::core::Tensor tensor_out = open3d::core::Tensor(shape,open3d::core::Float32,gpu);
   My_O3D_GPU::Example example = My_O3D_GPU::Example(original_cuda_cloud);
   example.filter(tensor_out);

   remainder_cloud = original_cuda_cloud.ToLegacy();

   // Conversion from open3d pc to ros msg
   open3d_conversions::open3dToRos(remainder_cloud, _remainder_pointcloud_msg);

   publishTopics();
}
} // namespace vodafone_detection