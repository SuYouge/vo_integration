cmake_minimum_required(VERSION 2.8)
project(stereomapper)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE  "-std=c++11 -O3 -fopenmp -pthread")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


############### dependencies ######################
# Eigen
include_directories("/usr/include/eigen3")

# OpenCV
find_package(OpenCV 3.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# # pangolin
# find_package(Pangolin REQUIRED)
# include_directories(${Pangolin_INCLUDE_DIRS})

# # Sophus
# find_package(Sophus REQUIRED)
# include_directories(${Sophus_INCLUDE_DIRS})

# # G2O
# find_package(G2O REQUIRED)
# include_directories(${G2O_INCLUDE_DIRS})

# # glog
# find_package(Glog REQUIRED)
# include_directories(${GLOG_INCLUDE_DIRS})

# # gtest
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

# # gflags
# find_package(GFlags REQUIRED)
# include_directories(${GFLAGS_INCLUDE_DIRS})

# # csparse
# find_package(CSparse REQUIRED)
# include_directories(${CSPARSE_INCLUDE_DIR})

set(THIRD_PARTY_LIBS
        ${OpenCV_LIBS}
        # ${Sophus_LIBRARIES}
        # ${Pangolin_LIBRARIES} GL GLU GLEW glut
        # g2o_core g2o_stuff g2o_types_sba g2o_solver_csparse g2o_csparse_extension
        # ${GTEST_BOTH_LIBRARIES}
        # ${GLOG_LIBRARIES}
        # ${GFLAGS_LIBRARIES}
        # pthread
        # ${CSPARSE_LIBRARY}
        )

include_directories("/home/yogurtsuee/stereomapper_module/include/myslam")
include_directories("/home/yogurtsuee/stereomapper_module/include")
# link_directories("/home/yogurtsuee/stereomapper_module")

# add_executable(main main.cpp)
# target_link_libraries(main calibiokitti)

add_subdirectory(src)
# add_subdirectory(test)
add_subdirectory(app)

add_subdirectory(libviso2)