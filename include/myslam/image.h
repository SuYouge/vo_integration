#ifndef IMAGE_H
#define IMAGE_H

#include "../libviso2/src/timer.h"
#include "simage.h"

struct image
    {
        // typedef std::shared_ptr<image> Ptr;

        unsigned char* data;
        int            width;
        int            height;
        int            step;
        timeval        time;

        /*
         * Default constructor.
         */
        image ()
        {
            data         = 0;
            width        = 0;
            height       = 0;
            time.tv_sec  = 0;
            time.tv_usec = 0;
        }

        /*
         * Destructor.
         */
        ~image ()
        {
            if (data!=0)
            {
                free(data); data = 0;
            }
        }
    };

#endif