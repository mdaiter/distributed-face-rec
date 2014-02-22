#ifndef TESTCV_H
#define TESTCV_H
#include<opencv2/core/core.hpp>
#include<opencv2/contrib/contrib.hpp>
#include<opencv2/ocl/ocl.hpp>
#include<tuple>
#include<opencv2/ocl/ocl.hpp>
#include<opencv2/imgproc/imgproc.hpp>
class testCV{
    public:
        std::vector<cv::Rect> detectPeople(cv::Mat&);
        std::vector<std::tuple<cv::Rect, int>> detectFaces(cv::Mat&);
        testCV();
        ~testCV();
    private:
        cv::ocl::OclCascadeClassifier *faceDetector;
        cv::ocl::OclCascadeClassifier *bodyDetector;
        cv::ocl::HOGDescriptor hog;
        std::vector<int> *labels;
        std::vector<cv::Mat> *images;
        cv::Ptr<cv::FaceRecognizer> faceRecognizer;

        int recognizeFaces(cv::Mat&, cv::Mat&, cv::Rect);
};
#endif
