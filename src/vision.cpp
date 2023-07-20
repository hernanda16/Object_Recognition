#include "vision/vision.h"

boost::mutex mutex_img;
int fps_cam, fps_real;
std::string text_img;

int main()
{
    // cv::Mat image = cv::imread("../test_image.png");
    cv::VideoCapture video("/dev/video2");
    cv::Mat image;

    if (!video.isOpened()) {
        std::cerr << "Error: Could not open the camera." << std::endl;
    } else {
        fps_cam = video.get(cv::CAP_PROP_FPS);
    }

    while (true) {

        mutex_img.lock();
        video >> image;
        mutex_img.unlock();

        timer.Start();

        if (!image.empty()) {
            recognition.CascadeMethod(image, lbpcascade);
            recognition.HogMethod(image);

            timer.Stop();

            if (timer.GetDuration() != 0) {
                fps_real = 1000 / timer.GetDuration();
                text_img = "FPS : " + std::to_string(fps_real);
            } else {
                text_img = "FPS : " + std::to_string(fps_cam);
            }

            cv::putText(image, text_img, cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255), 2);
            cv::imshow("image", image);

            char key = static_cast<char>(cv::waitKey(1));
            if (key == 'q') {
                break;
            }
        }
    }
}
