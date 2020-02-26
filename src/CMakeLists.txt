add_library(myslam SHARED
        calibiokitti.cpp
        gpsinertialdata.cpp
        gpsinertialdataiokitti.cpp
        readfromfilesthread.cpp
        stereoimage.cpp
        stereoimageiokitti.cpp

        visualodometrythread.cpp
)

target_link_libraries(myslam viso2
${THIRD_PARTY_LIBS}
)
