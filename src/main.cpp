// System
#include <iostream>
#include <cstdlib>

// Private
#include <camera_topic_uwsim/CTUWSim.h>


int main(int argc, char** argv) {
  int nReturnvalue = EXIT_FAILURE;
  
  ros::init(argc, argv, "the_awesome_node");
  ros::NodeHandle nh("~");
  
  CTUWSim::Ptr ctuwsimMain = CTUWSim::create();
  
  if(ctuwsimMain->run("/camera_topic")) {
    nReturnvalue = EXIT_SUCCESS;
  }
  
  return nReturnvalue;
}
