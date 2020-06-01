/*
 * test cpp file for SeetaFace2
 * 2020/5/28
 * hustccc
 * Manjaro
 */
#pragma warning(disable: 4819)
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

void    printFeature(float*feature,int size)
{
    printf("[]print feature here ...\n");
    int i=0;
    for(i=0;i<size;i++)
    {
        printf("%.10f ",*(feature+i));
        if(i%10==0) printf("\n");
    }
    printf("\n");
}

int main(int argc,char*argv[])
{
    if(argc<3)
    {
        printf("parameter not enough: 3\n");
        return  1;
    }

    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model( "./model/fd_2_00.dat", device, id );
    seeta::ModelSetting PD_model( "./model/pd_2_00_pts5.dat", device, id );
    seeta::ModelSetting FR_model( "./model/fr_2_10.dat", device, id );
    seeta::FaceEngine engine( FD_model, PD_model, FR_model, 2, 16 );
    
    clock_t s,t;
    double time_cost;

    string PictureStorePath("/home/hustccc/Pictures/FaceImage/Train/");

    s=clock();
    printf("[]DetectFaces\n");
    seeta::cv::ImageData image1 = cv::imread(PictureStorePath+argv[1]);
    seeta::cv::ImageData image2 = cv::imread(PictureStorePath+argv[2]);
    printf("image1 width: %d , height: %d , channels: %d\n",image1.width,image1.height,image1.channels);
    printf("image2 width: %d , height: %d , channels: %d\n",image2.width,image2.height,image2.channels);
    std::vector<SeetaFaceInfo> faces1 = engine.DetectFaces(image1);
    printf("vector faces1 size: %d\n",faces1.size());
    std::vector<SeetaFaceInfo> faces2 = engine.DetectFaces(image2);
    printf("vector faces2 size: %d\n",faces2.size());
    t=clock();
    time_cost=(double)(t-s)/CLOCKS_PER_SEC;
    printf("[]DetectFaces Finish , cost: %f\n",time_cost);

    s = clock();
    printf("[]DetectPoints\n");
    auto points1 = engine.DetectPoints(image1, faces1.at(0));
    printf("vector points1 size: %d\n",points1.size());
    auto points2 = engine.DetectPoints(image2, faces2.at(0));
    printf("vector points2 size: %d\n",points2.size());
    t = clock();
    time_cost=(double)(t-s)/CLOCKS_PER_SEC;
    printf("[]DetectPoints Finish, cost: %f\n",time_cost);

    s = clock();
    printf("[]FaceRecognizer\n");
    seeta::FaceRecognizer FR(seeta::ModelSetting("model/fr_2_10.dat"));
    std::shared_ptr<float> features1(new float[FR.GetExtractFeatureSize()],std::default_delete<float[]>());
    std::shared_ptr<float> features2(new float[FR.GetExtractFeatureSize()],std::default_delete<float[]>());
    //get image1 features
    //get image2 features
    FR.Extract(image1,points1.data(),features1.get());
    FR.Extract(image2,points2.data(),features2.get());
    t = clock();
    time_cost=(double)(t-s)/CLOCKS_PER_SEC;
    printf("[]FaceRecognizer Finish, cost: %f\n",time_cost);

    printf("feature1:\n");
    printFeature(features1.get(),FR.GetExtractFeatureSize());

    printf("feature2:\n");
    printFeature(features2.get(),FR.GetExtractFeatureSize());

    /*
    s=clock();
    printf("[]FaceDetetor\n");
    seeta::FaceDetector FD(seeta::ModelSetting("model/fd_2_00.dat"));
    //seeta::cv::ImageData image1 = cv::imread(PictureStorePath+"chechunchi_01.jpg");
    //seeta::cv::ImageData image2 = cv::imread(PictureStorePath+"chechunchi_05.jpg");
    seeta::cv::ImageData image1 = cv::imread(PictureStorePath+argv[1]);
    seeta::cv::ImageData image2 = cv::imread(PictureStorePath+argv[2]);
    printf("image1 width: %d , height: %d , channels: %d\n",image1.width,image1.height,image1.channels);
    printf("image2 width: %d , height: %d , channels: %d\n",image2.width,image2.height,image2.channels);
    SeetaFaceInfoArray faces1 = FD.detect(image1);
    SeetaFaceInfoArray faces2 = FD.detect(image2);
    printf("face1 array length: %d\n",faces1.size);
    printf("face2 array length: %d\n",faces2.size);
    printf("face1 location: %d %d \n",faces1.data[0].pos.x,faces1.data[0].pos.y);
    printf("face2 location: %d %d \n",faces2.data[0].pos.x,faces2.data[0].pos.y);
    t=clock();
    time_cost=(double)(t-s)/CLOCKS_PER_SEC;
    printf("[]FaceDeteor Finish , cost: %f\n",time_cost);

    s = clock();
    printf("[]FaceLandmarker\n");
    //seeta::FaceLandmarker FL(seeta::ModelSetting("model/pd_2_00_pts81.dat"));
    seeta::FaceLandmarker FL(seeta::ModelSetting("model/pd_2_00_pts5.dat"));
    std::vector<SeetaPointF> points1 = FL.mark(image1,faces1.data[0].pos);
    std::vector<SeetaPointF> points2 = FL.mark(image2,faces2.data[0].pos);
    t = clock();
    time_cost=(double)(t-s)/CLOCKS_PER_SEC;
    printf("[]FaceLandmarker Finish, cost: %f\n",time_cost);

    printf("[]FaceDatabase\n");
    seeta::FaceDatabase FDB(seeta::ModelSetting("model/fr_2_10.dat"));
    float similar_1vs1 = FDB.Compare(image1,points1.data(),image2,points2.data());
    printf("similar: %f\n",similar_1vs1);
    printf("[]FaceDatabase Finish\n");

    s = clock();
    printf("[]FaceRecognizer\n");
    seeta::FaceRecognizer FR(seeta::ModelSetting("model/fr_2_10.dat"));
    //get image1 features
    std::shared_ptr<float> features1(new float[FR.GetExtractFeatureSize()],std::default_delete<float[]>());
    FR.Extract(image1,points1.data(),features1.get());
    //get image2 features
    std::shared_ptr<float> features2(new float[FR.GetExtractFeatureSize()],std::default_delete<float[]>());
    FR.Extract(image2,points2.data(),features2.get());
    //auto similar = FR.CalculateSimilarity(features1.get(),features2.get());
    //printf("[]similar: %f\n",similar);
    t = clock();
    time_cost=(double)(t-s)/CLOCKS_PER_SEC;
    printf("[]FaceRecognizer Finish, cost: %f\n",time_cost);
    */

    return  0;
}
