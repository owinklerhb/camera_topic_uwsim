cmake_minimum_required(VERSION 2.8.3)
project(camera_topic_uwsim)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  std_msgs
  roscpp
  sensor_msgs
  OpenCV REQUIRED
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES camera_topic_uwsim
  CATKIN_DEPENDS cv_bridge std_msgs
  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
  
  src/CTUWSim.cpp)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
  opencv_core
  opencv_highgui)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(camera_topic_olaf src/CTUWSim.cpp)
target_link_libraries(camera_topic_olaf ${catkin_LIBRARIES})
add_dependencies(camera_topic_olaf camera_topic_uwsim_generate_messages_cpp)

