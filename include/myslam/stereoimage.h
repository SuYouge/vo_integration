#ifndef STEREOIMAGE_H
#define STEREOIMAGE_H

// #include <QObject>
#include <sys/time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <simage.h>
#include <image.h>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// TODO:
// Need mutex for read/write when being used in the multi-thread situation.

class StereoImage //: public QObject
{
    // Q_OBJECT

public:

    StereoImage();
    ~StereoImage();

    void setImage( unsigned char* data,int width,int height,int step,bool cam_left,bool rectified,timeval captured_time );
    bool isRectified() { return _simg->rectified; }
    simage* getStereoImage() { return _simg; }
    void pickedUp() { _picked = true; }
    void histogramNormalization(unsigned char* I,const int &width,const int &height,const int &step);

    float timeDiff(timeval a,timeval b)
    {
        return ((float)(a.tv_sec -b.tv_sec ))*1e+3 +
               ((float)(a.tv_usec-b.tv_usec))*1e-3;
    }

private:
    void showsImage();
    
    image*  _img_left;
    image*  _img_right;
    simage* _simg;
    bool    _picked;
};

#endif // STEREOIMAGE_H