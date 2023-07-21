#include "vision/vision.h"

int main(int argc, char** argv)
{
    // for (size_t i = 0; i < argc; i++) {
    //     std::cout << argv[i] << std::endl;
    // }

    // data.image = cv::imread("../digits.png");
    cam.Init();

    while (true) {
        cam.Convert();

        timer.Start();

        if (!data.image.empty()) {
            if (argc > 1) {
                if (std::strcmp(argv[1], "0") == 0)
                    recognition.CascadeMethod(data.image, lbpcascade);
                else if (std::strcmp(argv[1], "1") == 0)
                    recognition.HogMethod(data.image);
            }
            // Logger(CYAN, "%d %d", data.image.rows, data.image.cols);

            // cv::Mat ROI(data.image, cv::Rect(0, 0, 20, 20));
            // ROI.copyTo(data.crop);

            func.Deskew(data.image);

            timer.Stop();

            if (timer.GetDuration() != 0) {
                data.fps_real = 1000 / timer.GetDuration();
                data.text_img = "FPS : " + std::to_string(data.fps_real);
            } else {
                data.text_img = "FPS : " + std::to_string(data.fps_cam);
            }

            cv::putText(data.image, data.text_img, cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255), 2);
            cv::imshow("image", data.image);

            char key = static_cast<char>(cv::waitKey(1));
            if (key == 'q') {
                break;
            }
        }
    }
}
