cmake_minimum_required(VERSION 3.0.2)
project(object_recognition)

find_package(OpenCV REQUIRED)
# find_package(Eigen3 REQUIRED)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
#   ${EIGEN3_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/vision.cpp src/logger.cpp)
target_link_libraries(${PROJECT_NAME}_node ${OpenCV_LIBRARIES} ${EIGEN3_LIBRARIES})
