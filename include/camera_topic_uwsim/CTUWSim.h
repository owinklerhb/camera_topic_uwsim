#ifndef __CTUWSIM_H__
#define __CTUWSIM_H__


// System
#include <memory>
#include <iostream>

// ROS
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core_c.h>

// OpenCV
//#include <cv.h>
//#include <highgui.h>
//#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/NavSatFix.h>


class CTUWSim {
public:
  typedef std::shared_ptr<CTUWSim> Ptr;
  
private:
  ros::NodeHandle m_nhNodeHandle;
  ros::Subscriber m_subImage;
  sensor_msgs::NavSatFix m_gpsPosition;
  bool m_gpsSignalReceived = false;
  
protected:
public:
  CTUWSim();
  ~CTUWSim();
  
  template<class ... Args>
    static CTUWSim::Ptr create(Args ... args) {
    return std::make_shared<CTUWSim>(std::forward<Args>(args)...);
  }
  
  bool run(std::string strTopicName);
  void imageCallback(const sensor_msgs::ImagePtr& imgData);
  void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr& msg);
};


#endif /* __CTUWSIM_H__ */
