PROJECT(app)

cmake_minimum_required(VERSION 2.8)


# collect source files
FILE( GLOB SRC *.cpp )
FILE( GLOB HDR *.h )

SET(CMAKE_CXX_STANDARD 11)

find_package(Eigen3 REQUIRED) 
include_directories(${EIGEN3_INCLUDE_DIR})
find_package(OpenCV REQUIRED)



INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3 -lpng -lm")
ENDIF()



ADD_EXECUTABLE(app ${SRC} ${HDR})

target_link_libraries(app PRIVATE ${LIBMYSQLCLIENT_LIBRARIES} ${OpenCV_LIBS})

