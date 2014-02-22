#ifndef TESTCV_H
#define TESTCV_H
<<<<<<< HEAD
#include<opencv2/core/core.hpp>
#include<opencv2/contrib/contrib.hpp>
#include<opencv2/ocl/ocl.hpp>
#include<tuple>
class testCV{
    public:
        std::vector<cv::Rect> detectPeople(cv::Mat&);
        std::vector<std::tuple<cv::Rect, int>> detectFaces(cv::Mat&);
=======
#include<opencv2/ocl/ocl.hpp>
#include<opencv2/imgproc/imgproc.hpp>
class testCV{
    public:
        std::vector<cv::Rect> detectPeople(cv::Mat&);
        std::vector<cv::Rect> detectFaces(cv::Mat&);
>>>>>>> 9108755a8713f59559ecee239372909223f1e047
        testCV();
        ~testCV();
    private:
        cv::ocl::OclCascadeClassifier *faceDetector;
        cv::ocl::OclCascadeClassifier *bodyDetector;
        cv::ocl::HOGDescriptor hog;
<<<<<<< HEAD
        std::vector<int> *labels;
        std::vector<cv::Mat> *images;
        cv::Ptr<cv::FaceRecognizer> faceRecognizer;

        int recognizeFaces(cv::Mat&, cv::Mat&, cv::Rect);
=======
>>>>>>> 9108755a8713f59559ecee239372909223f1e047
};
#endif
