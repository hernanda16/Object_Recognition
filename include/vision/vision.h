#include "boost/thread.hpp"
#include "chrono"
#include "iostream"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "string.h"
#include "utils/logger.h"

#define cam_path "/dev/v4l/by-id/usb-046d_C270_HD_WEBCAM_8E1A4E60-video-index0"

#define haarcascade "../resources/haarcascades/haarcascade_frontalcatface_extended.xml"
#define lbpcascade "../resources/lbpcascades/lbpcascade_frontalface_improved.xml"

class _data {
public:
    int fps_cam, fps_real;

    cv::Mat image;
    cv::Mat crop = cv::Mat::zeros(20, 20, CV_8UC1);

    std::string text_img;

    boost::mutex mutex_img;
} data;

struct _cam {
    cv::VideoCapture video;

    void Init()
    {
        video.open(cam_path);
        if (!video.isOpened()) {
            std::cerr << "Error: Could not open the camera." << std::endl;
        } else {
            data.fps_cam = video.get(cv::CAP_PROP_FPS);
        }
    }

    void Convert()
    {
        data.mutex_img.lock();
        video >> data.image;
        data.mutex_img.unlock();
    }
} cam;

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
    cv::Mat Deskew(cv::Mat frame)
    {
        cv::Mat deskew_frame;
        cv::cvtColor(frame, deskew_frame, cv::COLOR_BGR2GRAY);
        cv::Moments moment = cv::moments(deskew_frame);
        if (abs(moment.mu02) < 1e-2) {
            return deskew_frame.clone();
        }
        float skew = moment.mu11 / moment.mu02; //central moments spreaded along xy-axes/central moments spreaded along y-axis
        cv::Mat warp_deskew_matrix = (cv::Mat_<float>(2, 3) << 1, skew, -0.5 * 1 * skew, 0, 1, 0);
        cv::Mat warpMat = (cv::Mat_<float>(2, 3) << 1, 0, 50, 0, 1, 50);
        cv::Mat imgOut = cv::Mat::zeros(deskew_frame.rows, deskew_frame.cols, deskew_frame.type());
        cv::warpAffine(deskew_frame, imgOut, warp_deskew_matrix, imgOut.size(), 1);

        // cv::Mat result_nearest, result_linear, result_cubic;

        // cv::warpAffine(deskew_frame, result_nearest, warpMat, deskew_frame.size(), cv::INTER_NEAREST);
        // cv::warpAffine(deskew_frame, result_linear, warpMat, deskew_frame.size(), cv::INTER_LINEAR);
        // cv::warpAffine(deskew_frame, result_cubic, warpMat, deskew_frame.size(), cv::INTER_CUBIC);

        // cv::imshow("Nearest-Neighbor Interpolation", result_nearest);
        // cv::imshow("Bilinear Interpolation", result_linear);
        // cv::imshow("Bicubic Interpolation", result_cubic);

        cv::imshow("imgOut", imgOut);

        return imgOut;
    }
} func;
