# # project
# cmake_minimum_required (VERSION 2.6)
# project (libviso2)

# # directories
# set (LIBVISO2_SRC_DIR src)

# # include directory
# include_directories("${LIBVISO2_SRC_DIR}")

# # use sse3 instruction set
# SET(CMAKE_CXX_FLAGS "-msse3")

# # sources
# FILE(GLOB LIBVISO2_SRC_FILES "src/*.cpp")

# # make release version
# set(CMAKE_BUILD_TYPE Release)

# # demo program
# # add_executable(viso2 ${LIBVISO2_SRC_FILES})
# # target_link_libraries (viso2 png)

# # add_library(matrix SHARED
# #         src/matrix.cpp
# # )

add_library(viso2 SHARED
        src/matrix.cpp
        src/filter.cpp
        src/matcher.cpp
        # src/reconstruction.cpp
        src/triangle.cpp
        src/viso_stereo.cpp
        src/viso.cpp
)