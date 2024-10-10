// https://github.com/airockchip/rknn_model_zoo/tree/main/examples/yolov5/cpp

#include "gst_wrapper.hpp"
#include "yolo_wrapper.hpp"

int main(int argc, char *argv[]) {

  bool viz = false;
  std::string model_path = "../yolov5/model/yolov5s_relu_3588.rknn";
  std::string video_path = "../test.h264";

  YoloModel yolo(model_path);
  if(!yolo.loaded) {
    return 1;
  }

  GStreamerPipeline gst("filesrc location=" + video_path + " ! h264parse ! "
                        "mppvideodec format=RGB ! queue max-size-buffers=0 leaky=0");

  cv::Mat image;
  while (gst.read(image)) {
    if(!yolo.infer(image))
      break;

    if(viz) {
      cv::namedWindow("image", cv::WINDOW_NORMAL);
      cv::imshow("image", image);
      cv::resizeWindow("image", 640, 480);
      cv::waitKey(1);
    }
  }

  return 0;
}
