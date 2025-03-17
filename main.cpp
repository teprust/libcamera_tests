#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include "LibCamera.h"

using namespace cv;


#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include "LibCamera.h"

using namespace cv;

void captureLoop(LibCamera &cam) {
    uint32_t width = 640, height = 480, stride;
    cam.configureStill(width, height, formats::RGB888, 1, libcamera::Orientation::Rotate0);
    cam.startCamera();

    while (true) {
        LibcameraOutData frameData;
        if (!cam.readFrame(&frameData))
            continue;

        Mat im(height, width, CV_8UC3, frameData.imageData, stride);
        imshow("libcamera-demo", im);

        char key = waitKey(1);
        if (key == 'q') break;

        cam.returnFrameBuffer(frameData);
    }

    cam.stopCamera();
    cam.closeCamera();
}

int main() {
    LibCamera cam;

    if (cam.initCamera(0)) { // 0 — первая камера
        std::cerr << "Error initializing camera" << std::endl;
        return -1;
    }

    // Запускаем поток для захвата кадров
    std::thread cameraThread(captureLoop, std::ref(cam));
    
    // Ждем завершения потока
    cameraThread.join();

    return 0;
}


/*
Рабочий вариант для одной камеры
int main() {
    time_t start_time = time(0);
    int frame_count = 0;
    float lens_position = 100;
    float focus_step = 50;
    LibCamera cam;
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
        cv::namedWindow("libcamera-demo", cv::WINDOW_NORMAL);
        cv::resizeWindow("libcamera-demo", window_width, window_height);
    } 
    // Если одна камера, то ret = 0
    int ret = cam.initCamera();
    cam.configureStill(width, height, formats::RGB888, 1, libcamera::Orientation::Rotate0);
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
    cam.set(controls_);
    if (!ret) {
        bool flag;
        LibcameraOutData frameData;
        cam.startCamera();
        cam.VideoStream(&width, &height, &stride);
        while (true) {
            flag = cam.readFrame(&frameData);
            if (!flag)
                continue;
            Mat im(height, width, CV_8UC3, frameData.imageData, stride);

            imshow("libcamera-demo", im);
            key = waitKey(1);
            if (key == 'q') {
                break;
            } else if (key == 'f') {
                ControlList controls;
                controls.set(controls::AfMode, controls::AfModeAuto);
                controls.set(controls::AfTrigger, 0);
                cam.set(controls);
            } else if (key == 'a' || key == 'A') {
                lens_position += focus_step;
            } else if (key == 'd' || key == 'D') {
                lens_position -= focus_step;
            }

            // To use the manual focus function, libcamera-dev needs to be updated to version 0.0.10 and above.
            if (key == 'a' || key == 'A' || key == 'd' || key == 'D') {
                ControlList controls;
                controls.set(controls::AfMode, controls::AfModeManual);
				controls.set(controls::LensPosition, lens_position);
                cam.set(controls);
            }

            frame_count++;
            if ((time(0) - start_time) >= 1){
                printf("fps: %d\n", frame_count);
                //printf("ret: %d\n", ret);
                frame_count = 0;
                start_time = time(0);
            }
            cam.returnFrameBuffer(frameData);
        }
        destroyAllWindows();
        cam.stopCamera();
    }
    cam.closeCamera();
    return 0;
}

*/