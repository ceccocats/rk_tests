// https://github.com/airockchip/rknn_model_zoo/tree/main/examples/yolov5/cpp

#include "gst_wrapper.hpp"

int main(int argc, char *argv[]) {

  GStreamerPipeline gst("filesrc location=../test.h264 ! h264parse ! "
                        "mppvideodec format=RGB");

  cv::Mat image;
  while (gst.read(image)) {
    cv::imshow("image", image);
    cv::waitKey(1);
  }

  return 0;
}
