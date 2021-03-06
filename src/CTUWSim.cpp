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
#include <fstream> //for writing into files
#include <sensor_msgs/NavSatFix.h>
//Seesterne zur Erkennung unterschiedlicher Arten mit einbauen -> auf Farben zurückgreifen

using namespace std; //also for writing into files 



int main(int argc, char** argv) {
  int nReturnvalue = EXIT_FAILURE;
  
  //std::cout << "Using OpenCV version: " << CV_VERSION << std::endl;
  
  
  ros::init(argc, argv, "image_publisher_node");
  ros::NodeHandle nh("~");
  //hier csv erstellen und öffnen
  CTUWSim::Ptr ctuwsimMain = CTUWSim::create();

 if(ctuwsimMain->run("/uwsim/camera1")) { 
 //if(ctuwsimMain->run("/uwsim/rangecamera")) {
    nReturnvalue = EXIT_SUCCESS;
  }
  
  return nReturnvalue;
}


		//subscribe to GPS
void CTUWSim::gpsCallback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
  m_gpsPosition = *msg;
  m_gpsSignalReceived = true;
}


CTUWSim::CTUWSim() {
}

CTUWSim::~CTUWSim() {
}

bool CTUWSim::run(std::string strTopicName) {
  bool bSuccess = false;
  
  m_subImage = m_nhNodeHandle.subscribe(strTopicName, 1, &CTUWSim::imageCallback, this);
  ros::Subscriber sub = m_nhNodeHandle.subscribe("/g500/gps", 1, &CTUWSim::gpsCallback, this);
  ros::spin();
  
  return bSuccess;
}

void CTUWSim::imageCallback(const sensor_msgs::ImagePtr& imgData) { 		//callback bekommt Daten von sensor_msgs im Image Format

   if (!m_gpsSignalReceived){
	return;
  }
  cv_bridge::CvImagePtr cv_ptr = nullptr;					//absichern, dass cv_ptr nirgendwo hinzeigt
  //sensor_msgs::Image msg;
	//hier in csv reinschreiben
        //ofstream myfile;
	ofstream myfile("val2.csv", std::ios_base::app); //ios_base macht den Unterschied ;)
        //myfile.open ("val.csv");


  //msg wird als sensor_msgs im Format Image deklariert
  if(sensor_msgs::image_encodings::isColor(imgData->encoding)) {		//wenn das eingehende Bild BGR8 Format ist, zeigt der cv_ptr darauf
    cv_ptr = cv_bridge::toCvCopy(imgData, sensor_msgs::image_encodings::BGR8); 	//BGR8, RGB8// BGR16 /// RGBA16 // etc.
    //cv_ptr = cv_bridge::toCvShare(imgData, "rgb8")->image;
  } else {
    std::cerr << "No color image" << std::endl;					//Fehlermeldung, dass das Bild kein Farbformat hat
  }

  if(cv_ptr) {
    cv::Mat imgMat = cv_ptr->image; //ROS to OpenCv

       // Add CV-related code to process image here.

	 /* Gray scale image */
           cv::Mat filtered_image;
            cv::cvtColor(imgMat,filtered_image,CV_BGR2GRAY);

	// threshold image 
	    int global_min_threshold=50; //hier die Untergrenze des Threshold
	    cv::Mat threshold_image;
	    cv::threshold(filtered_image,imgMat,global_min_threshold,255,cv::THRESH_BINARY_INV); //Obergrenze auf 255 für echtes Weiß
	    //cv::namedWindow("Threshold Image");
	    //cv::imshow("Threshold Image",threshold_image);
	    
	    cv::cvtColor(imgMat, cv_ptr->image, CV_GRAY2BGR);
	
   
    //OPENCV TO ROS
    ros::NodeHandle n;
    ros::Publisher imgPub_pub = n.advertise<sensor_msgs::Image>("Image_Pub", 100);
    //  sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg() //von Fereshta
    imgPub_pub.publish(*(cv_ptr->toImageMsg()));//imgData); cv_prt->toImageMsg()
    //std::cout << imgData << std::endl;
    ros::Rate loop_rate(2);

    //count white pixels
    
         cv::Mat partROI;
            float count_white = 0;
            float count_black = 0;
            count_white = countNonZero(imgMat);
            count_black = imgMat.cols * imgMat.rows - count_white;
	    float perc =count_white/768;
	    cout << "Percentage of objects covering surface = " << perc << endl;
	    cout << "latitude " << m_gpsPosition.latitude << " longitude " << m_gpsPosition.longitude << endl;
          
            imshow("Image", imgMat); 
  	    cout <<"HIER SEGMENT "<<endl;
	myfile << perc << ", " << "latitude, " << m_gpsPosition.latitude * 3.5 << ", longitude, " << m_gpsPosition.longitude * 3.2 << endl;
	//cout << "Hallo Fereshta" << endl;	
/*				//Publish Percentage on ros topic
    ros::NodeHandle k;
    ros::Publisher percentPub = k.advertise<std::float>("percentage_pub", 1000);
    //percentPub.publish(*(cv_ptr->toImageMsg()));
	percentPub.publish(perc);
    //std::cout << imgData << std::endl;
    //ros::Rate loop_rate(10);
				//Ende Pusblish Percentage on ros topic */


    int count = 0;
    while (ros::ok())
      {
	
	ros::spinOnce();
	
	loop_rate.sleep();
	++count;
      }
    
    
    // Stop CV-related code here
      //hier csv schließen
      myfile.close();

               
    
  } else {
    std::cerr << "Got no image." << std::endl;
  }
}

