PROJECT(app)

cmake_minimum_required(VERSION 2.8)

# Common include / link directories



# collect source files
FILE( GLOB SRC *.cpp )
FILE( GLOB HDR *.h )

SET(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3 -lpng -lm")
ENDIF()

ADD_EXECUTABLE(app ${SRC} ${HDR}) 

#target_include_directories(app)
target_link_libraries(app)

