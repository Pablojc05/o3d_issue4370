#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <time.h>

#include <open3d/Open3D.h>
#include <open3d_conversions/open3d_conversions.h>

#include <ros/ros.h>

#include <eigen_conversions/eigen_msg.h>
#include <sensor_msgs/PointCloud2.h>


namespace example_o3d {
class Node 
{

  public:
    explicit Node();
    virtual ~Node();
    void spin();

  private:
    void managePointcloud(const sensor_msgs::PointCloud2ConstPtr &msg);
    void publishTopics();
    
    ros::NodeHandle _nh;

    ros::Publisher _pub_remainder_pointcloud;

    sensor_msgs::PointCloud2 _remainder_pointcloud_msg;

    ros::Time _original_stamp;
  
};
} // namespace example_o3d

#endif