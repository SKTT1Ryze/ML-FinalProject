/*
 * test cpp file for SeetaFace2
 * 2020/5/28
 * hustccc
 * Manjaro
 */
#pragma warning(disable : 4819)
#include <stdio.h>
#include <stdlib.h>
#include <seeta/FaceDetector.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/FaceDatabase.h>
#include <seeta/FaceRecognizer.h>
#include <seeta/FaceEngine.h>
#include <seeta/Struct_cv.h>
#include <seeta/Struct.h>
#include <seeta/QualityAssessor.h>
//#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <array>
#include <map>
#include <iostream>
#include <ctime>
using namespace std;

int main(int argc, char *argv[])
{
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model("./model/fd_2_00.dat", device, id);
    seeta::ModelSetting PD_model("./model/pd_2_00_pts5.dat", device, id);
    seeta::ModelSetting FR_model("./model/fr_2_10.dat", device, id);
    seeta::FaceEngine engine(FD_model, PD_model, FR_model, 2, 16);

    seeta::QualityAssessor QA;

    // recognization threshold
    float threshold = 0.7f;

    //set face detector's min face size
    engine.FD.set(seeta::FaceDetector::PROPERTY_MIN_FACE_SIZE, 80);

    printf("test camera module...\n");
    cv::VideoCapture capture;
    capture.open(0);
    cv::Mat frame;

    while (capture.isOpened())
    {
        capture >> frame;
        if (frame.empty())
            continue;
        seeta::cv::ImageData image = frame;

        vector<SeetaFaceInfo> faces = engine.DetectFaces(image);
        for (SeetaFaceInfo &face : faces)
        {
            int64_t index = -1;
            float similarity = 0;
            auto points = engine.DetectPoints(image, face);
            cv::rectangle(frame, cv::Rect(face.pos.x, face.pos.y, face.pos.width, face.pos.height), CV_RGB(128, 128, 255), 3);
            for (int i = 0; i < 5; i++)
            {
                auto &point = points[i];
                cv::circle(frame, cv::Point(int(point.x), int(point.y)), 2, CV_RGB(128, 255, 128), -1);
            }
            string name = "test";
            cv::putText(frame, name, cv::Point(face.pos.x, face.pos.y - 5), 3, 1, CV_RGB(255, 128, 128));
        }
        cv::imshow("Frame", frame);
        auto key = cv::waitKey(20);
        if (key == 27)
        {
            break;
        }
    }
}
