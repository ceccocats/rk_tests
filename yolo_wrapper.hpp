#pragma once
#include <iostream>
#include <string>

#include "yolov5.h"
#include "image_utils.h"
#include "file_utils.h"
#include "image_drawing.h"

class YoloModel {

public:
  bool loaded = false;
  rknn_app_context_t rknn_app_ctx;

  YoloModel(std::string model_path) {
    int ret;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    init_post_process();
    ret = init_yolov5_model(model_path.c_str(), &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov5_model fail! ret=%d model_path=%s\n", ret, model_path);
        return;
    }
    loaded = true;
  }

  bool infer(cv::Mat cvImage) {
      if(!loaded)
        return false;

      int ret;

      image_buffer_t src_image;
      memset(&src_image, 0, sizeof(image_buffer_t));
      // ret = read_image("../yolov5/model/bus.jpg", &src_image);
      src_image.width = cvImage.cols;
      src_image.height = cvImage.rows;
      src_image.format = IMAGE_FORMAT_RGB888;
      src_image.virt_addr = cvImage.data;

      object_detect_result_list od_results;
      ret = inference_yolov5_model(&rknn_app_ctx, &src_image, &od_results);
      if (ret != 0)
      {
          printf("init_yolov5_model fail! ret=%d\n", ret);
          return false;
      }

      // print
      char text[256];
      for (int i = 0; i < od_results.count; i++)
      {
          object_detect_result *det_result = &(od_results.results[i]);
          printf("\t%s @ (%d %d %d %d) %.3f\n", coco_cls_to_name(det_result->cls_id),
                det_result->box.left, det_result->box.top,
                det_result->box.right, det_result->box.bottom,
                det_result->prop);
          int x1 = det_result->box.left;
          int y1 = det_result->box.top;
          int x2 = det_result->box.right;
          int y2 = det_result->box.bottom;

          draw_rectangle(&src_image, x1, y1, x2 - x1, y2 - y1, COLOR_BLUE, 3);
          // sprintf(text, "%s %.1f%%", coco_cls_to_name(det_result->cls_id), det_result->prop * 100);
          // draw_text(&src_image, text, x1, y1 - 20, COLOR_RED, 10);
      }
      return true;
  }

};
