/*
Copyright 2011. All rights reserved.
Institute of Measurement and Control Systems
Karlsruhe Institute of Technology, Germany

This file is part of libviso2.
Authors: Andreas Geiger

libviso2 is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or any later version.

libviso2 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
libviso2; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA 
*/

#include "viso.h"

#include <math.h>

using namespace std;

VisualOdometry::VisualOdometry(parameters param) : _param(param)
{
    _J         = 0;
    _p_observe = 0;
    _p_predict = 0;
    _matcher   = new Matcher(param.match);
    _Tr_delta  = Matrix::eye(4);
    _Tr_valid  = false;
    srand(0);
}

//==============================================================================//

VisualOdometry::~VisualOdometry()
{
    delete _matcher;
}

//==============================================================================//

bool VisualOdometry::updateMotion()
{
    // estimate motion
    vector<double> tr_delta = estimateMotion(_p_matched);

    // on failure
    if (tr_delta.size()!=6)
    {
        return false;
    }

    // set transformation matrix (previous to current frame)
    _Tr_delta = transformationVectorToMatrix(tr_delta);
    _Tr_valid = true;

    // success
    return true;
}

//==============================================================================//

Matrix VisualOdometry::transformationVectorToMatrix(std::vector<double> tr)
{
    // extract parameters
    double rx = tr[0]; // pitch
    double ry = tr[1]; // yaw
    double rz = tr[2]; // roll
    double tx = tr[3]; // right
    double ty = tr[4]; // up
    double tz = tr[5]; // forward

    // precompute sine/cosine
    double sx = sin(rx);
    double cx = cos(rx);
    double sy = sin(ry);
    double cy = cos(ry);
    double sz = sin(rz);
    double cz = cos(rz);

    // compute transformation
    // Rx(pitch) * Ry(yaw) * Rz(roll)
    Matrix Tr(4,4);
    Tr._val[0][0] = +cy*cz;          Tr._val[0][1] = -cy*sz;          Tr._val[0][2] = +sy;    Tr._val[0][3] = tx;
    Tr._val[1][0] = +sx*sy*cz+cx*sz; Tr._val[1][1] = -sx*sy*sz+cx*cz; Tr._val[1][2] = -sx*cy; Tr._val[1][3] = ty;
    Tr._val[2][0] = -cx*sy*cz+sx*sz; Tr._val[2][1] = +cx*sy*sz+sx*cz; Tr._val[2][2] = +cx*cy; Tr._val[2][3] = tz;
    Tr._val[3][0] = 0;               Tr._val[3][1] = 0;               Tr._val[3][2] = 0;      Tr._val[3][3] = 1;
    return Tr;
}

//==============================================================================//

bool VisualOdometry::inverseTransformationVector(const std::vector<double> &tr, std::vector<double> &tr_inv)
{
    Matrix tr_matrix = transformationVectorToMatrix( tr );
    Matrix tr_matrix_inv = Matrix::eye(4);
    bool is_inverse_work = true;

    if( !tr_matrix_inv.solve( tr_matrix ) )
    {
        is_inverse_work = false;
    }
    else
    {
        // roll
        tr_inv[2] = atan2( -tr_matrix_inv._val[0][1], tr_matrix_inv._val[0][0] );

        // pitch
        tr_inv[0] = atan2( -tr_matrix_inv._val[1][2], tr_matrix_inv._val[2][2] );

        // yaw
        tr_inv[1] = atan2( tr_matrix_inv._val[0][2],\
                    sqrt( tr_matrix_inv._val[0][0] * tr_matrix_inv._val[0][0] + tr_matrix_inv._val[0][1] * tr_matrix_inv._val[0][1] ) );

        tr_inv[3] = tr_matrix_inv._val[0][3];
        tr_inv[4] = tr_matrix_inv._val[1][3];
        tr_inv[5] = tr_matrix_inv._val[2][3];
    }
    return is_inverse_work;
}

//==============================================================================//

vector<int32_t> VisualOdometry::getRandomSample(int32_t N,int32_t num)
{
    // init sample and totalset
    vector<int32_t> sample;
    vector<int32_t> totalset;

    // create vector containing all indices
    for (int32_t i=0; i<N; i++)
    {
        totalset.push_back(i);
    }

    // add num indices to current sample
    sample.clear();
    for (int32_t i=0; i<num; i++)
    {
        int32_t j = rand()%totalset.size();
        sample.push_back(totalset[j]);
        totalset.erase(totalset.begin()+j);
    }

    // return sample
    return sample;
}

//==============================================================================//

void VisualOdometry::calculateRollPitchYawFromTransformation( double& roll, double& pitch, double& yaw ) const
{
    // Reference:
    // 1. http://planning.cs.uiuc.edu/node103.html
    // 2. Matrix VisualOdometry::transformationVectorToMatrix(std::vector<double> tr)
    roll = atan2( -_Tr_delta._val[0][1], _Tr_delta._val[0][0] );
    pitch = atan2( -_Tr_delta._val[1][2], _Tr_delta._val[2][2] );
    yaw = atan2( _Tr_delta._val[0][2],\
                 sqrt( _Tr_delta._val[0][0] * _Tr_delta._val[0][0] + _Tr_delta._val[0][1] * _Tr_delta._val[0][1] ) );

}

//==============================================================================//

void VisualOdometry::calculateVelocityFromTransformation( double& velocity ) const
{
    // http://www.cvlibs.net/software/libviso/
    // Get delta x and delta z
    velocity = sqrt( _Tr_delta._val[0][3] * _Tr_delta._val[0][3] + _Tr_delta._val[2][3] * _Tr_delta._val[2][3]  );
}

//==============================================================================//

void VisualOdometry::calculateAltitudeFromTransformation( double& altitude ) const
{
    altitude = _Tr_delta._val[1][3];
}
