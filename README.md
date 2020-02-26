# stereomappter后台测试程序

[stereomapper](https://github.com/willSapgreen/stereo-vision)的qt程序似乎有一些问题，先在后台测试一下各个模块的运行情况。
熟悉一下整个程序。各个模块的信息整理在[这里](http://mecha-su.cn/2020/02/24/vision-slam-2/)。

原来的代码依赖很少，现在想要给它退化一下。

## readfromfiles线程

[readfile_test](app/readfile_test.cpp)

读取校正信息和图片并输出。

## VisualOdometryStereo线程



## stereothread线程

