#include <camera_topic_uwsim/CTUWSim.h>
// System
#include <iostream>
#include <cstdlib>

#include "ros/ros.h"

int main(int argc, char** argv) {
  int nReturnvalue = EXIT_FAILURE;
  
  ros::init(argc, argv, "the_awesome_node");
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

void CTUWSim::imageCallback(const sensor_msgs::Image& imgData) {
  cv_bridge::CvImagePtr cv_ptr = nullptr;
  
  if(sensor_msgs::image_encodings::isColor(imgData.encoding)) {
    cv_ptr = cv_bridge::toCvCopy(imgData, sensor_msgs::image_encodings::BGR8);
  } else {
    std::cerr << "No color image" << std::endl;
  }
  
  if(cv_ptr) {
    cv::Mat imgMat = cv_ptr->image;
    
    std::cout << "Got an image!" << std::endl;
    
    // TODO: Add CV-related code to process image here.

	ros::Publisher imgPub_pub = n.advertise<sensor_msgs::Image>("Image_Pub", 1000);
	ros::Rate loop_rate(10);

		int count = 0;
	  while (ros::ok())
	  {
	    /**
	     * This is a message object. You stuff it with data, and then publish it.
	     */
	    sensor_msgs::Image msg;
		std::cout << "one more!" << std::endl;

	    //std::stringstream ss;
	    //ss << "hello world " << count;
	    //msg.data = ss.str();

	    //ROS_INFO("%s", msg.data.c_str());

	    /**
	     * The publish() function is how you send messages. The parameter
	     * is the message object. The type of this object must agree with the type
	     * given as a template parameter to the advertise<>() call, as was done
	     * in the constructor above.
	     */
	    imgPub_pub.publish(msg);

	    ros::spinOnce();

	    loop_rate.sleep();
	    ++count;
	  }
	

    // Stop CV-related code here


  } else {
    std::cerr << "Got no image." << std::endl;
  }
}
