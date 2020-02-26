#include "visualodometrythread.h"

int main(){

    VisualOdometryThread* _visual_odom_thread;

    CalibIOKITTI* _calib;

    _calib = new CalibIOKITTI();

    _visual_odom_thread = new VisualOdometryThread(_calib);

    _visual_odom_thread->run();

    return 0;
}
