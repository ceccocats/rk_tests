
#include "gst_wrapper.hpp"

int main(int argc, char *argv[]) {

  GStreamerPipeline gst("filesrc location=../test.h264 ! h264parse ! "
                        "nvh264dec ! videoconvert ! video/x-raw,format=BGR");

  cv::Mat image;
  while (gst.read(image)) {
    cv::imshow("image", image);
    cv::waitKey(1);
  }

  return 0;
}
