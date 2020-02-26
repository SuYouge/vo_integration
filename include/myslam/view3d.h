#ifndef VIEW3D_H
#define VIEW3D_H

#include "../libviso2/src/matrix.h"

class View3D{

public:

    struct point_3d
        {
            float x,y,z;
            float val;
            point_3d (float x,float y,float z,float val) : x(x),y(y),z(z),val(val) {}
        };




};

#endif