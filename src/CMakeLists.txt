add_library(myslam SHARED
        calibiokitti.cpp
        gpsinertialdata.cpp
        gpsinertialdataiokitti.cpp
        readfromfilesthread.cpp
        stereoimage.cpp
        stereoimageiokitti.cpp
)

target_link_libraries(myslam matrix 
${THIRD_PARTY_LIBS}
)
