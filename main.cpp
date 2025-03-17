#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "LibCamera.h"

using namespace cv;

int main() {
    time_t start_time = time(0);
    int frame_count = 0;
    float lens_position = 100;
    float focus_step = 50;
    LibCamera cam1;
    LibCamera cam2;
    // uint32_t width = 1920;
    // uint32_t height = 1080;

    uint32_t width = 480;
    uint32_t height = 360;

    uint32_t stride;
    char key;

    // int window_width = 1920;
    // int window_height = 1080;

    int window_width = 480;
    int window_height = 360;

    if (width > window_width)
    {
        cv::namedWindow("libcamera-demo-cam1", cv::WINDOW_NORMAL);
        cv::resizeWindow("libcamera-demo-cam1", window_width, window_height);
        cv::namedWindow("libcamera-demo-cam2", cv::WINDOW_NORMAL);
        cv::resizeWindow("libcamera-demo-cam2", window_width, window_height);
    } 

    int ret1 = cam1.initCamera(0);
    int ret2 = cam2.initCamera(1);
    cam1.configureStill(width, height, formats::RGB888, 1, libcamera::Orientation::Rotate0);
    cam2.configureStill(width, height, formats::RGB888, 1, libcamera::Orientation::Rotate0);
    ControlList controls_;
    int64_t frame_time = 1000000 / 10;
    // Set frame rate
	controls_.set(controls::FrameDurationLimits, libcamera::Span<const int64_t, 2>({ frame_time, frame_time }));
    // Adjust the brightness of the output images, in the range -1.0 to 1.0
    controls_.set(controls::Brightness, 0.5);
    // Adjust the contrast of the output image, where 1.0 = normal contrast
    controls_.set(controls::Contrast, 1.5);
    // Set the exposure time
    controls_.set(controls::ExposureTime, 20000);
    cam1.set(controls_);
    cam2.set(controls_);
    if (!ret1 & !ret2) {
        bool flag;
        LibcameraOutData frameData1;
        LibcameraOutData frameData2;
        cam1.startCamera();
        cam1.VideoStream(&width, &height, &stride);

        cam2.startCamera();
        cam2.VideoStream(&width, &height, &stride);

        while (true) {
            flag1 = cam1.readFrame(&frameData1);
            flag2 = cam2.readFrame(&frameData2);

            if (!flag1)
                continue;
            Mat im(height, width, CV_8UC3, frameData1.imageData, stride);

            if (!flag2)
                continue;
            Mat im(height, width, CV_8UC3, frameData2.imageData, stride);

            imshow("libcamera-demo-cam1", im);
            imshow("libcamera-demo-cam2", im);
            key = waitKey(1);
            if (key == 'q') {
                break;
            }

            frame_count++;
            if ((time(0) - start_time) >= 1){
                printf("fps: %d\n", frame_count);
                frame_count = 0;
                start_time = time(0);
            }
            cam1.returnFrameBuffer(frameData1);
            cam2.returnFrameBuffer(frameData2);
        }
        destroyAllWindows();
        cam1.stopCamera();
        cam2.stopCamera();
    }
    cam1.closeCamera();
    cam2.closeCamera();
    return 0;
}
