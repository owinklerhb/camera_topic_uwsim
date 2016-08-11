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
  
  //std::cout << "Using OpenCV version: " << CV_VERSION << std::endl;
  
  
  ros::init(argc, argv, "image_publisher_node");
  ros::NodeHandle nh("~");
  
  CTUWSim::Ptr ctuwsimMain = CTUWSim::create();
  
 if(ctuwsimMain->run("/uwsim/camera1")) { 
 //if(ctuwsimMain->run("/uwsim/rangecamera")) {
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

	 /* Gray scale image */
            cv::Mat filtered_image;
            cv::cvtColor(imgMat,filtered_image,CV_BGR2GRAY);

	// threshold image 
	    int global_min_threshold=50; //ggf. löschen
	    cv::Mat threshold_image;
	    cv::threshold(filtered_image,imgMat,global_min_threshold,215,cv::THRESH_BINARY_INV);
	    //cv::namedWindow("Threshold Image");
	    //cv::imshow("Threshold Image",threshold_image);
	    
	    cv::cvtColor(imgMat, cv_ptr->image, CV_GRAY2BGR);
	
   
    //OPENCV TO ROS
    ros::NodeHandle n;
    ros::Publisher imgPub_pub = n.advertise<sensor_msgs::Image>("Image_Pub", 1000);
    //  sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg() //von Fereshta
    imgPub_pub.publish(*(cv_ptr->toImageMsg()));//imgData); cv_prt->toImageMsg()
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

