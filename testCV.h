#ifndef TESTCV_H
#define TESTCV_H
#include<opencv2/core/core.hpp>
#include<opencv2/contrib/contrib.hpp>
#include<tuple>
#include<opencv2/gpu/gpu.hpp>
#include<opencv2/imgproc/imgproc.hpp>
class testCV{
    public:
        std::vector<cv::Rect> detectPeople(cv::Mat&);
        std::vector<std::tuple<cv::Rect, int>> detectFaces(cv::Mat&);
        testCV();
        ~testCV();
    private:
        cv::gpu::CascadeClassifier_GPU *faceDetector;
        cv::gpu::CascadeClassifier_GPU *bodyDetector;
        cv::gpu::HOGDescriptor hog;
        std::vector<int> *labels;
        std::vector<cv::Mat> *images;
        cv::Ptr<cv::FaceRecognizer> faceRecognizer;

        int recognizeFaces(cv::Mat&, cv::Mat&, cv::Rect);
};
#endif
