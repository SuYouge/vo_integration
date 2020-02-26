add_library(myslam SHARED
        calibiokitti.cpp
        gpsinertialdata.cpp
        gpsinertialdataiokitti.cpp
        readfromfilesthread.cpp
        stereoimage.cpp
        stereoimageiokitti.cpp

        visualodometrythread.cpp

        planeestimation.cpp
        stereothread.cpp
)

target_link_libraries(myslam viso2 elas 
${THIRD_PARTY_LIBS}
)
