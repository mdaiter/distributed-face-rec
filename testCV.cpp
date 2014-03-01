#include<stdio.h>
#include<sstream>
#include<fstream>
#include<iostream>
#include<tuple>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include"json.h"
#include"testCV.h"
#include<cstdlib>
#include<unistd.h>
#include"PracticalSocket.h"
#define DIST_HEIGHT 10
using namespace cv;
using namespace std;

testCV::testCV(){
    hog.setSVMDetector(gpu::HOGDescriptor::getDefaultPeopleDetector());
    faceDetector = new gpu::CascadeClassifier_GPU();
    bodyDetector = new gpu::CascadeClassifier_GPU();
    faceDetector->load("/usr/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml");
    bodyDetector->load("/usr/share/OpenCV/haarcascades/haarcascade_mcs_upperbody.xml");
    images = new std::vector<cv::Mat>();
    labels = new std::vector<int>();
    faceRecognizer = createLBPHFaceRecognizer();
    images->push_back(imread("./matt.jpg", 0));
    images->push_back(imread("./sang.jpg", 0));
    labels->push_back(1);
    labels->push_back(2);
    faceRecognizer->train(*images, *labels);

}

testCV::~testCV(){

}

int testCV::recognizeFaces(cv::Mat& orig, cv::Mat &gray, cv::Rect rect){
    try{
        Mat roi(gray, rect);
        int label;
        double confidence;
        faceRecognizer->predict(roi, label, confidence);
        if (label == 1){
            putText(orig, "Matt", cvPoint(rect.x, rect.y), FONT_HERSHEY_COMPLEX_SMALL, 0.8,
                    cvScalar(200,200,250), 1, CV_AA);
                   cout << "Matt. Rect x: " << rect.x<<"Rect y: " << rect.y << "Confidence: " << confidence << endl;
        }
        else if (label == 2){
            putText(orig, "Quincy", cvPoint(rect.x, rect.y), FONT_HERSHEY_COMPLEX_SMALL, 0.8,
                    cvScalar(200,200,250), 1, CV_AA);
            cout << "Dan. COnfidence: " << confidence << endl;
        }
        return label;
    }
    catch (cv::Exception& e){
        cout << "Failure!" << endl;
    }
}

void broadcastJsonFromData(int port, vector<tuple<Rect, int>> faces){
    json::Object dataBroadcastRoot;
    dataBroadcastRoot["name"] = "Kinect";
    dataBroadcastRoot["type"] = "camera";
    //Create the json
    json::Array arrayFaces;

    for (int i = 0; i < faces.size(); i++){
        json::Object personObject;

        personObject["positionX"] =  get<0>(faces[i]).x;
        personObject["positionY"] = get<0>(faces[i]).y;
        personObject["name"] = get<1>(faces[i]);

        arrayFaces.push_back(personObject);
    }
    dataBroadcastRoot["faces"] = arrayFaces;
    string serializedString = json::Serialize(dataBroadcastRoot);
    cout << json::Serialize(dataBroadcastRoot) << endl;

    UDPSocket sock;
    sock.sendTo(serializedString.c_str(), serializedString.length(), "127.0.0.1", 8008);
}

vector<tuple<Rect, int>> testCV::detectFaces(Mat& frame){
    Mat frameGray, dst;
    cvtColor(frame, frameGray, CV_BGR2GRAY);
    gpu::GpuMat src(frameGray);
    equalizeHist (frameGray, frameGray);
    gpu::GpuMat objbuf;
    int detections_number = faceDetector->detectMultiScale(src, objbuf, 1.2, 4, Size(20,20));
    Mat obj_host;
    objbuf.colRange(0, detections_number).download(obj_host);
    Rect* faces = obj_host.ptr<Rect>();
    vector<std::tuple<Rect, int>> personVec;
    for (size_t i = 0; i < detections_number; i++){
        rectangle(frame, faces[i], Scalar(255));
        std::tuple<Rect, int> person (faces[i], recognizeFaces(frame, frameGray, faces[i]));
        personVec.push_back(person);
    }
    return personVec;
}

bool inside(Rect x, Rect y){

    //return (x.x > y.x && x.y > y.y && (x.x + x.width) >(y.x + y.width) && (x.y + x.height) && (y.y+y.height) );
    return false;
}

std::vector<Rect> testCV::detectPeople(Mat& frame){
    gpu::HOGDescriptor hog;
    hog.setSVMDetector(gpu::HOGDescriptor::getDefaultPeopleDetector());
    Mat frameGray;
    cvtColor(frame, frameGray, CV_BGR2BGRA);
    gpu::GpuMat src(frameGray);

    std::vector<Rect> found;
    std::vector<Rect> foundFiltered;
    hog.detectMultiScale(src, found, 0.1, Size(8,8), Size(0,0), 1.05, 2);
    printf("Found size:%d\n", found.size());
    for (size_t i = 0; i < found.size(); i++){
        Rect r = found[i];
        for (size_t j = 0; j < found.size(); j++){
            Rect q = found[j];
            if (!inside(r, q)){
                foundFiltered.push_back(r);
            }
        }
    }
    for (size_t k = 0; k < foundFiltered.size(); k++){
        rectangle(frame, foundFiltered[k].tl(), foundFiltered[k].br(), cv::Scalar(0,255,0), 3);
    }
    return found;
}

int main(){
    testCV *cv = new testCV();
    VideoCapture capture(0);
    cvNamedWindow("result", WINDOW_NORMAL);
        while(true){
            Mat frame, frameGray;
            capture >> frame;
            vector<tuple<Rect, int>> faces = cv->detectFaces(frame);
            //run position code
            broadcastJsonFromData(8008, faces);
            //update data in server
            imshow("result", frame);
            waitKey(2);
        }
}
