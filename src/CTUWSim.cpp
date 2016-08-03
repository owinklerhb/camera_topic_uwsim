#include <camera_topic_uwsim/CTUWSim.h>
// System
#include <iostream>
#include <cstdlib>
#include <sstream>
//#include <ros/ros.h>
//#include <image_transport/image_transport.h>
//#include <cv_bridge/cv_bridge.h>
//#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ros/ros.h"

int main(int argc, char** argv) {
  int nReturnvalue = EXIT_FAILURE;
  
  ros::init(argc, argv, "image_publisher_node");
  ros::NodeHandle nh("~");
  
  CTUWSim::Ptr ctuwsimMain = CTUWSim::create();
  
  if(ctuwsimMain->run("/uwsim/camera1")) {
    nReturnvalue = EXIT_SUCCESS;
  }
  
  return nReturnvalue;
}

CTUWSim::CTUWSim() {
}

CTUWSim::~CTUWSim() {
}

bool CTUWSim::run(std::string strTopicName) {
  bool bSuccess = false;
  
  m_subImage = m_nhNodeHandle.subscribe(strTopicName, 1, &CTUWSim::imageCallback, this);
  
  ros::spin();
  
  return bSuccess;
}

void CTUWSim::imageCallback(const sensor_msgs::ImagePtr& imgData) { 		//callback bekommt Daten von sensor_msgs im Image Format
  cv_bridge::CvImagePtr cv_ptr = nullptr;					//absichern, dass cv_ptr nirgendwo hinzeigt
  //sensor_msgs::Image msg;  	
  //msg wird als sensor_msgs im Format Image deklariert
  if(sensor_msgs::image_encodings::isColor(imgData->encoding)) {		//wenn das eingehende Bild BGR8 Format ist, zeigt der cv_ptr darauf
    cv_ptr = cv_bridge::toCvCopy(imgData, sensor_msgs::image_encodings::BGR8); 	//RGB8// BGR16 /// RGBA16 // etc.
    //cv_ptr = cv_bridge::toCvShare(imgData, "rgb8")->image;
  } else {
    std::cerr << "No color image" << std::endl;					//Fehlermeldung, dass das Bild kein Farbformat hat
  }

  if(cv_ptr) {
    cv::Mat imgMat = cv_ptr->image; //ROS to OpenCv

       // TODO: Add CV-related code to process image here.



    //OPENCV TO ROS
    //cv::imshow("Image", cv_ptr->image);
    //	if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
    //  	cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));    
    
    //std::cout << cv_ptr->image << std::endl;
	//std::cout << imgMat.data << std::endl;
    //ROS_INFO_STREAM( cv_ptr->image );

    
 

    
    
    ros::NodeHandle n;
    ros::Publisher imgPub_pub = n.advertise<sensor_msgs::Image>("Image_Pub", 1000);
    imgPub_pub.publish(cv_ptr->toImageMsg());//imgData);
    //std::cout << imgData << std::endl;
    ros::Rate loop_rate(10);
    
    int count = 0;
    while (ros::ok())
      {
	
	ros::spinOnce();
	
	loop_rate.sleep();
	++count;
      }
    
    
    // Stop CV-related code here
    
    
  } else {
    std::cerr << "Got no image." << std::endl;
  }
}

