/*
 * @Author: Yogurt Suee
 * @Date: 2020-02-25 14:46:37
 * @LastEditors: Yogurt Suee
 * @LastEditTime: 2020-02-25 17:41:28
 * @Description: file content
 */

#include "readfromfilesthread.h"
#include "string"
#include <iostream>

using namespace std;

std::string dir = "/home/yogurtsuee/Dataset/raw/2011_09_26_0002";

int main(){
    cout<<"Start processing"<<endl;

    StereoImage* _stereo_image;
    _stereo_image = new StereoImage();

    GPSInertialData*      _gps_inertial_data;
    _gps_inertial_data = new GPSInertialData();

    CalibIOKITTI* _calib;
    _calib = new CalibIOKITTI();

    StereoImageIOKITTI* _stereo_image_io;
    _stereo_image_io = new StereoImageIOKITTI();

    GPSInertialDataIOKITTI* _gps_inertial_data_io;
    _gps_inertial_data_io = new GPSInertialDataIOKITTI();

    ReadFromFilesThread* _read_thread;   
    _read_thread = new ReadFromFilesThread(_stereo_image, _gps_inertial_data, _calib, _stereo_image_io, _gps_inertial_data_io);
    
    _read_thread->setInputDir(dir);

    _read_thread->run();

    delete _read_thread;
    delete _stereo_image;
    delete _gps_inertial_data;
    delete _calib;
    delete _stereo_image_io;
    delete _gps_inertial_data_io;
    
    return 0;
}

   

