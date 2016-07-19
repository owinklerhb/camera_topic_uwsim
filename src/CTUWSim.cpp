#include <camera_topic_uwsim/CTUWSim.h>


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
  } else {
    std::cerr << "Got no image." << std::endl;
  }
}
