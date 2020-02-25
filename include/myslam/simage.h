/*
 * @Author: Yogurt Suee
 * @Date: 2020-02-25 14:13:17
 * @LastEditors: Yogurt Suee
 * @LastEditTime: 2020-02-25 16:20:57
 * @Description: seperated from stereoimage.h
 */
#ifndef SIMAGE_H
#define SIMAGE_H

#include <cstring>
#include <stdlib.h>
#include "../libviso2/src/timer.h"

struct simage
    {
        // I1, I2 are the input image.
        unsigned char* I1;
        unsigned char* I2;

        // D1, D2 are the disparity map.
        float*         D1;
        float*         D2;

        int            width;
        int            height;
        int            step;
        timeval        time;
        bool           rectified;

        /*
         * Default constructor.
         */
        simage ()
        {
            I1           = 0;
            I2           = 0;
            D1           = 0;
            D2           = 0;
            width        = 0;
            height       = 0;
            step         = 0;
            time.tv_sec  = 0;
            time.tv_usec = 0;
        }

        /*
         * Copy constructor.
         */
        simage (const simage &src)
        {
            I1 = 0;
            I2 = 0;
            D1 = 0;
            D2 = 0;

            if (src.I1!=0)
            {
                I1 = (unsigned char*)malloc(src.step*src.height*sizeof(unsigned char));
                memcpy(I1, src.I1, src.step*src.height*sizeof(unsigned char));
            }

            if (src.I2!=0)
            {
                I2 = (unsigned char*)malloc(src.step*src.height*sizeof(unsigned char));
                memcpy(I2, src.I2, src.step*src.height*sizeof(unsigned char));
            }

            if (src.D1!=0)
            {
                D1 = (float*)malloc(src.step*src.height*sizeof(float));
                memcpy(D1, src.D1, src.step*src.height*sizeof(float));
            }

            if (src.D2!=0)
            {
                D2 = (float*)malloc(src.step * src.height * sizeof(float));
                memcpy(D2, src.D2, src.step * src.height * sizeof(float));
            }

            width     = src.width;
            height    = src.height;
            step      = src.step;
            time      = src.time;
            rectified = src.rectified;
        }

        /*
         * Destructor.
         */
        ~simage ()
        {
            if (I1!=0) { free(I1); I1 = 0; }
            if (I2!=0) { free(I2); I2 = 0; }
            if (D1!=0) { free(D1); D1 = 0; }
            if (D2!=0) { free(D2); D2 = 0; }
        }
    };
#endif