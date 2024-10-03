#pragma once
#include <iostream>
#include <string>

#include <gst/app/gstappsink.h>
#include <gst/gst.h>

#include <opencv2/opencv.hpp>

class GStreamerPipeline {

public:
  GstElement *pipeline;
  GstElement *appsink;

  GStreamerPipeline(std::string pipeline_str) {
    // Initialize GStreamer
    int argc = 0;
    char **argv = nullptr;
    gst_init(&argc, &argv);

    // Create the GStreamer pipeline
    pipeline =
        gst_parse_launch((pipeline_str + " ! appsink name=sink").c_str(), NULL);

    if (!pipeline) {
      std::cerr << "Failed to create pipeline." << std::endl;
      return;
    }

    // Get the appsink element from the pipeline
    appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

    // Configure the appsink to emit signals for each frame
    g_object_set(appsink, "emit-signals", TRUE, "sync", FALSE, NULL);

    // Start playing the pipeline
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
  }

  bool read(cv::Mat &image) {
    if (!pipeline)
      return false;

    // Pull a sample (frame) from the appsink
    GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (!sample) {
      std::cerr << "Failed to get sample! Exiting..." << std::endl;
      return false;
    }

    // Get the buffer from the sample
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstMapInfo map;

    bool read_ok = false;

    // Map the buffer to access the raw data
    if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
      // Get the video frame info from the sample's caps
      GstCaps *caps = gst_sample_get_caps(sample);
      GstStructure *s = gst_caps_get_structure(caps, 0);
      const gchar *format = gst_structure_get_string(s, "format");
      int width, height;
      gst_structure_get_int(s, "width", &width);
      gst_structure_get_int(s, "height", &height);

      std::cout << "Frame format: " << format << " | Width: " << width
                << " | Height: " << height << std::endl;

      // Check if the format is BGR (compatible with OpenCV)
      if (strcmp(format, "BGR") == 0) {
        // Create an OpenCV Mat from the raw data
        image = cv::Mat(height, width, CV_8UC3, (void *)map.data);
        read_ok = true;
      } else {
        std::cerr << "Unsupported frame format: " << format << std::endl;
      }

      gst_buffer_unmap(buffer, &map);
    }

    // Free the sample when done
    gst_sample_unref(sample);

    return read_ok;
  }
};