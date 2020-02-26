#include "stereothread.h"
#include "visualodometrythread.h"
#include "readfromfilesthread.h"
#include <iostream>

std::string dir = "/home/yogurtsuee/Dataset/raw/2011_09_26_0002";

void showsImage(simage* _simg){
    // int nr = _simg->height;
    // int nv = _simg->step*sizeof(unsigned char);
    // 得找个机会把这个地方改了
    CvSize size;
    CvSize disp_size;
    size.height = _simg->height;
    size.width = _simg->step*sizeof(unsigned char);

    IplImage* imgptr_1 = cvCreateImageHeader( size, IPL_DEPTH_8U, 1 );
    IplImage* imgptr_2 = cvCreateImageHeader( size, IPL_DEPTH_8U, 1 );

    auto ptr_1 = _simg->I1;
    auto ptr_2 = _simg->I2;

    cvSetData( imgptr_1, ptr_1 , _simg->step );
    cvSetData( imgptr_2, ptr_2 , _simg->step );

    cv::Mat m_1 = cv::cvarrToMat(imgptr_1); 
    cv::Mat m_2 = cv::cvarrToMat(imgptr_2);

    cv::Mat m;
    vconcat(m_1,m_2, m);
    cv::imshow("data_set",m);
    cvWaitKey(1);
}

void showDisparity(float* disp_img, int height, int width){
    CvSize disp_size;
    disp_size.height = height;
    disp_size.width = width;
    IplImage* imgptr = cvCreateImageHeader( disp_size, IPL_DEPTH_32F, 3);

    cvSetData( imgptr, disp_img , width*4*3);
    cv::Mat m = cv::cvarrToMat(imgptr);
    cv::imshow("dispartity",m); 
}

int main(){

    // CalibIOKITTI* _calib;
    // _calib = new CalibIOKITTI();

    std::cout<<"Start processing"<<std::endl;

    /* init read data thread */

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

    _read_thread->init(dir);

    int32_t data_size = _gps_inertial_data_io->getOxTSDataSize();

    std::cout<<"data size of gps/stereo image = "<<data_size<<std::endl;

    VisualOdometryThread* _visual_odom_thread;

    _visual_odom_thread = new VisualOdometryThread(_calib);

    //  new for stereo_thread
    View3D* modelView;
    modelView = new View3D();

    StereoThread* _stereo_thread;
    _stereo_thread = new StereoThread(_calib, modelView);

    // _calib->showCalibrationParameters();

    /* Loop for simage */

    // start output loop
    float fps = 10;
    //bool getOxTSData(int nth, OxSTData& oxts_data);
    OxSTData oxts_data;
    ImageDataCV image_set[IMAGE_INPUT_SOURCE_COUNT];
    unsigned char* left_img_data;
    unsigned char* right_img_data;

    for (int32_t i=0; i < data_size; ++i)
        {
            if(!(_stereo_image_io->getNextImageDataSet(image_set) &&
                 _gps_inertial_data_io->getNextGPSInertialData(oxts_data)))
            {
                continue;
            }
            // Convert OxST data to GPSInertialData.
            GPSInertialDataFormat gi_data;
            oxts_data.toGPSInertialDataFormat(gi_data);
            _gps_inertial_data->setData(gi_data);
            // Get the raw data.
            cvGetRawData(image_set[IMAGE_INPUT_SOURCE_GRAY_LEFT]._image, &left_img_data);
            cvGetRawData(image_set[IMAGE_INPUT_SOURCE_GRAY_RIGHT]._image, &right_img_data);
            _stereo_image->setImage(left_img_data,
                                    image_set[IMAGE_INPUT_SOURCE_GRAY_LEFT]._image->width,
                                    image_set[IMAGE_INPUT_SOURCE_GRAY_LEFT]._image->height,
                                    image_set[IMAGE_INPUT_SOURCE_GRAY_LEFT]._image->widthStep,
                                    true, true, image_set[IMAGE_INPUT_SOURCE_GRAY_RIGHT]._captured_time);
            _stereo_image->setImage(right_img_data,
                                    image_set[IMAGE_INPUT_SOURCE_GRAY_RIGHT]._image->width,
                                    image_set[IMAGE_INPUT_SOURCE_GRAY_RIGHT]._image->height,
                                    image_set[IMAGE_INPUT_SOURCE_GRAY_RIGHT]._image->widthStep,
                                    false, true, image_set[IMAGE_INPUT_SOURCE_GRAY_RIGHT]._captured_time);
                                    
            // Release the memory in IplImage.
            for(int j = 0; j < IMAGE_INPUT_SOURCE_COUNT; ++j)
            {
                cvReleaseImage(&(image_set[j]._image));
            }

            /* Necessary steps for utilize visual_odom_thread */
            // showsImage(_stereo_image->getStereoImage());
            _visual_odom_thread->pushBack(*_stereo_image->getStereoImage(),0);
            _visual_odom_thread->step(_stereo_image->getStereoImage());
            // void StereoThread::pushBack(simage& s,Matrix H,float gain)

            _stereo_thread->pushBack(*_stereo_image->getStereoImage(),_visual_odom_thread->getHomographyTotal(),_visual_odom_thread->getGain());
            _stereo_thread->run();
            showsImage(_stereo_thread->getStereoImage());
            showDisparity(_stereo_thread->getColorDisparityMap(),_stereo_thread->getStereoImage()->height,_stereo_thread->getStereoImage()->width);

            // std::cout<<"total gain = "<<_visual_odom_thread->getGain()<<"in step"<<i+1<<std::endl;
            // usleep(1e6/fps);
        } // for (int32_t i=0; i < data_size; ++i)

    return 0;
}