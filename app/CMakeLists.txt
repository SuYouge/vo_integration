add_executable(readfile_test readfile_test.cpp)
target_link_libraries(readfile_test myslam ${THIRD_PARTY_LIBS})

add_executable(visualodometry_test visualodometry_test.cpp)
target_link_libraries(visualodometry_test myslam ${THIRD_PARTY_LIBS})