#include "boost/thread.hpp"
#include "chrono"
#include "iostream"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "string.h"
#include "utils/logger.h"

#define haarcascade "../resources/haarcascades/haarcascade_frontalcatface_extended.xml"
#define lbpcascade "../resources/lbpcascades/lbpcascade_frontalface_improved.xml"

struct _recognition {
    /**
     * * Cascade Method
     * @param Mat frame
     * @param string pretrained data
     * @note Not the efficient method (false detect might be occured)
     * @note haarcascade 10% not being false detect
     * @note lbpcascade 65% not being false detect
    */
    void CascadeMethod(cv::Mat& frame, std::string pretrained_data)
    {
        cv::CascadeClassifier obj_data;
        obj_data.load(pretrained_data);

        if (obj_data.empty()) {
            static bool init = true;
            if (init) {
                Logger(RED, "Cascade Trained Data Could'nt be Openned");
            }
        } else {
            static bool init = true;
            if (init) {
                Logger(BLUE, "Cascade Loaded");
                init = false;
            }
            std::vector<cv::Rect> face_box;
            obj_data.detectMultiScale(frame, face_box, 1.1, 2);

            for (size_t i = 0; i < face_box.size(); i++) {
                cv::rectangle(frame, face_box[i].tl(), face_box[i].br(), cv::Scalar(255), 2);
            }
        }
    }

    /**
     * * Histogram of Oriented Gradient
     * @param Mat frame
    */
    void HogMethod(cv::Mat& frame)
    {
        frame.convertTo(frame, CV_32F, 1 / 255.0);

        cv::Mat gx, gy;
        cv::Sobel(frame, gx, CV_32F, 1, 0, 1);
        cv::Sobel(frame, gy, CV_32F, 0, 1, 1);

        cv::Mat mag, angle;
        cv::cartToPolar(gx, gy, mag, angle, 1);

        cv::imshow("gx", gx);
        cv::imshow("gy", gy);
        cv::imshow("mag", mag);
        // cv::imshow("angle", angle);
    }
} recognition;

struct _timer {
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point stop;

    void Start()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    void Stop()
    {
        stop = std::chrono::high_resolution_clock::now();
    }
    float GetDuration() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    }

} timer;

struct _func {
    cv::Mat Deskew(cv::Mat& frame)
    {
        cv::Moments moment = cv::Moments(frame);
        if (abs(moment.mu02) < 1e-2) {
            return frame.clone();
        }
    }
} func;
