# Connected-component-labelling

A C++ non-parallel one component at a time implementation of [Connected component labelling](https://en.wikipedia.org/wiki/Connected-component_labeling) for binary image (1 -- foreground, 0 -- background) and comparison it with OpenCV non-parallel [cv::connectedComponentsWithStats()](https://docs.opencv.org/3.1.0/d3/dc0/group__imgproc__shape.html#gae57b028a2b2ca327227c2399a9d53241). 

Image constraints: size -- 1024x576, maximum of components -- 20, maximum of components summary square -- 50% of image.


## Getting Started

Installing OpenCV: https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html#linux-installation.

To get the repository:

```
git clone https://github.com/KlicOgogo/Connected-component-labelling.git
```

To start the program open Terminal and run from .../Connected-component-labelling:

```
bash compile.sh
bash run.sh
```
