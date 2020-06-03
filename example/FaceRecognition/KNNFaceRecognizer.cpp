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
#include <vector>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <algorithm>
using namespace std;

#define MaxTrainImageNum 200
#define MulRate 20
#define Parameter_A 2.00
#define Parameter_C 3.00
#define Parameter_D 4
#define Parameter_gama 0.00001f
#define Parameter_R 2.00
#define L 40
#define M 100
#define TRAINREADNUM 800
#define TESTREADNUM 100
//function declartion
void HelpMenu();
void printFeature(float *feature, int size);

int WorkMode(int argc, char *argv[]);
int Mode_1();
int Mode_1_x();
int Mode_2(string predictImagePath);
int Mode_3(string option_1, string option_2);
int Mode_4();
int Mode_GUI();
int getAbsoluteFiles(string directory, vector<string> &filesAbsolutePath);
int compareLabel(vector<int> x, vector<int> y);

double KernelDist_1(float *x, float *y, float c, int length);
double KernelDist_2(float *x, float *y, float a, float c, int d, int length);
double KernelDist_3(float *x, float *y, float gama, int length);
double KernelDist_4(float *x, float *y, float gama, float r, int length);
double EuclideanDist(float *x, float *y, int lenght);
double ManhanttenDist(float *x, float *y, int length);
double LinearKernel(float *x, float *y, float c, int length);
double PolynomialKernel(float *x, float *y, float a, float c, int d, int length);
double RadialBasisKernel(float *x, float *y, float gama, int length);
double SigmoidKernel(float *x, float *y, float gama, float r, int length);

vector<string> KNNClassifier(vector<float *> train_data, vector<string> train_label, vector<float *> test_data, int feature_length, int k);
vector<vector<int>> KNNClassifier_x(vector<float *> train_data, vector<vector<int>> train_label, vector<float *> test_data, int feature_length, int k);
vector<int> GetKMinIndex(vector<double> distlist, int k);

int main(int argc, char *argv[])
{
    int mode = WorkMode(argc, argv);
    int return_status;
    printf("Mode ID: [%d]\n", mode);
    switch (mode)
    {
    case 0:
        exit(-1);
        return_status = -1;
        break;
    case -1:
        HelpMenu();
        return_status = -1;
        break;
    case 1:
        printf("search\n");
        return_status = Mode_1();
        break;
    case 2:
        printf("Run bat\n");
        return_status = Mode_2(argv[1]);
        break;
    case 3:
        printf("calculate similarity\n");
        return_status = Mode_3(argv[1], argv[2]);
        break;
    case -2:
        return_status = Mode_GUI();
    default:
        exit(-1);
        break;
    }
    printf("main exit.\n");
    return return_status;
}

int WorkMode(int argc, char *argv[])
{
    string help = "--help";
    string gui = "--GUI";
    switch (argc)
    {
    case 0:
        printf("Error\n");
        exit(-1);
        return 0;
        break;
    case 1:
        printf("Mode [1]\n");
        return 1;
        break;
    case 2:
        if (help.compare(argv[1]) == 0)
        {
            printf("help menu\n");
            return -1;
        }
        else if (gui.compare(argv[1]) == 0)
        {
            printf("GUI Mode\n");
            return -2;
        }
        else
        {
            printf("Mode [2]\n");
            return 2;
        }

        break;
    case 3:
        printf("Mode [3]\n");
        return 3;
        break;
    default:
        printf("too many parameter: %d\n", argc);
        break;
    }
    return 0;
}

void HelpMenu()
{
    printf("=====>help menu\n");
}

int Mode_GUI()
{
    int i, j;
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model("./model/fd_2_00.dat", device, id);
    seeta::ModelSetting PD_model("./model/pd_2_00_pts5.dat", device, id);
    seeta::ModelSetting FR_model("./model/fr_2_10.dat", device, id);
    seeta::FaceEngine engine(FD_model, PD_model, FR_model, 2, 16);

    seeta::QualityAssessor QA;

    clock_t s, t;
    double time_cost;
    // recognization threshold
    float threshold = 0.7f;

    //set face detector's min face size
    engine.FD.set(seeta::FaceDetector::PROPERTY_MIN_FACE_SIZE, 80);

    //prepare image store
    printf("prepare pictures store...\n");

    s = clock();
    string ImageStoreDir = "/home/hustccc/Pictures/FaceImage/Train_01/";
    vector<string> TrainImagePath;
    vector<string> TrainLabels;
    if (getAbsoluteFiles(ImageStoreDir, TrainImagePath) != 0)
    {
        printf("read train image error\n");
        exit(-1);
    }
    //log
    //cout << "size of train image: " << TrainImagePath.size() << endl;
    //cout << "the first train image: " << TrainImagePath.at(0) << endl;

    vector<seeta::cv::ImageData> TrainImageList;
    vector<SeetaFaceInfo> TrainFaceList;
    vector<SeetaFaceInfo> tempface;
    j = 0;
    for (i = 0; i < TrainImagePath.size(); i++)
    {
        //cout << "read image: " << TrainImagePath.at(i) << endl;
        TrainImageList.push_back(cv::imread(TrainImagePath.at(i)));
        tempface = engine.DetectFaces(TrainImageList.at(TrainImageList.size() - 1));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TrainFaceList.push_back(tempface.at(0));
        //TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 7, 3));
        TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 6, 2));
    }
    cout << "train data size: " << TrainFaceList.size() << endl;
    /*
    cout << "Train Image List size: " << TrainImageList.size() << endl;
    cout << "Train Faces List size: " << TrainFaceList.size() << endl;
    cout << "Train Labels size: " << TrainLabels.size() << endl;
    cout << "the first label: " << TrainLabels.at(0) << endl;
    cout << "the second label: " << TrainLabels.at(1) << endl;
    */

    if (TrainImageList.size() != TrainFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TrainPointList;
    for (i = 0; i < TrainImageList.size(); i++)
    {
        TrainPointList.push_back(engine.DetectPoints(TrainImageList.at(i), TrainFaceList.at(i)));
    }
    //cout << "Train Points List size: " << TrainPointList.size() << endl;
    //cout << "the size of first points: " << TrainPointList.at(0).size() << endl;

    //get features of image in store
    printf("get features of image in store...\n");
    seeta::FaceRecognizer FR(seeta::ModelSetting("./model/fr_2_10.dat"));
    vector<float *> TrainFeatureList;
    float *temp_float;
    for (i = 0; i < TrainPointList.size(); i++)
    {
        temp_float = (float *)malloc(sizeof(float) * FR.GetExtractFeatureSize());
        TrainFeatureList.push_back(temp_float);
    }
    for (i = 0; i < TrainPointList.size(); i++)
    {
        FR.Extract(TrainImageList.at(i), TrainPointList.at(i).data(), TrainFeatureList.at(i));
    }

    //cout << "Train Features List size: " << TrainFeatureList.size() << endl;
    /*
    cout << "the first train feature:" << endl;
    printFeature(TrainFeatureList.at(0), FR.GetExtractFeatureSize());
    cout << "the second train feature:" << endl;
    printFeature(TrainFeatureList.at(1), FR.GetExtractFeatureSize());
    */
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare train image finish, cost: %f\n", time_cost);

    printf("open camera...\n");
    cv::VideoCapture capture;
    capture.open(0);
    cv::Mat frame;
    vector<string> PredictResult;
    string predictName;
    while (capture.isOpened())
    {
        capture >> frame;
        if (frame.empty())
            continue;
        seeta::cv::ImageData predictImage = frame;
        vector<SeetaFaceInfo> predictfaces = engine.DetectFaces(predictImage);
        if (predictfaces.size() != 1)
        {
            printf("no face or have more than one faces...continue...\n");
            continue;
        }
        vector<SeetaPointF> predictpoints = engine.DetectPoints(predictImage, predictfaces.at(0));

        int64_t index = -1;
        cv::rectangle(frame, cv::Rect(predictfaces.at(0).pos.x, predictfaces.at(0).pos.y, predictfaces.at(0).pos.width, predictfaces.at(0).pos.height), CV_RGB(128, 128, 255), 3);
        for (i = 0; i < 5; i++)
        {
            auto &point = predictpoints[i];
            cv::circle(frame, cv::Point(int(point.x), int(point.y)), 2, CV_RGB(128, 255, 128), -1);
        }

        //get features fo predictImage
        //seeta::FaceRecognizer FR(seeta::ModelSetting("./model/fr_2_10.dat"));
        vector<float *> predictFeature;
        predictFeature.push_back((float *)malloc(sizeof(float) * FR.GetExtractFeatureSize()));
        FR.Extract(predictImage, predictpoints.data(), predictFeature.at(0));

        int k = 1;
        PredictResult = KNNClassifier(TrainFeatureList, TrainLabels, predictFeature, FR.GetExtractFeatureSize(), k);
        //cout<<"predict result: "<<PredictResult.at(0)<<endl;
        if (PredictResult.at(0) == "00")
        {
            predictName = "chechunchi";
        }
        else if (PredictResult.at(0) == "01")
        {
            predictName = "Messi";
        }
        else if (PredictResult.at(0) == "02")
        {
            predictName = "Faker";
        }
        else if (PredictResult.at(0) == "03")
        {
            predictName = "theshy";
        }
        else if (PredictResult.at(0) == "04")
        {
            predictName = "777";
        }
        else if (PredictResult.at(0) == "05")
        {
            predictName = "knight";
        }
        else if (PredictResult.at(0) == "06")
        {
            predictName = "uzi";
        }
        else if (PredictResult.at(0) == "07")
        {
            predictName = "scout";
        }
        else if (PredictResult.at(0) == "08")
        {
            predictName = "smlz";
        }
        else if (PredictResult.at(0) == "09")
        {
            predictName = "jackelove";
        }
        else if (PredictResult.at(0) == "10")
        {
            predictName = "luweiqin";
        }
        else if (PredictResult.at(0) == "11")
        {
            predictName = "cheqixian";
        }
        else
        {
            predictName = "None";
        }

        cv::putText(frame, predictName, cv::Point(predictfaces.at(0).pos.x, predictfaces.at(0).pos.y - 5), 3, 1, CV_RGB(255, 128, 128));
        cv::imshow("Frame", frame);
        PredictResult.clear();
        auto key = cv::waitKey(20);
        if (key == 27)
        {
            break;
        }
    }

    return 0;
}

int Mode_1()
{
    //Mode 1
    int i = 0;
    int j = 0;
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model("./model/fd_2_00.dat", device, id);
    seeta::ModelSetting PD_model("./model/pd_2_00_pts5.dat", device, id);
    seeta::ModelSetting FR_model("./model/fr_2_10.dat", device, id);
    seeta::FaceEngine engine(FD_model, PD_model, FR_model, 2, 16);

    clock_t s, t;
    double time_cost;

    //prepare image store
    printf("prepare pictures store...\n");

    s = clock();
    string ImageStoreDir = "/home/hustccc/Pictures/FaceImage/Train_01/";
    vector<string> TrainImagePath;
    vector<string> TrainLabels;
    if (getAbsoluteFiles(ImageStoreDir, TrainImagePath) != 0)
    {
        printf("read train image error\n");
        exit(-1);
    }
    //log
    //cout << "size of train image: " << TrainImagePath.size() << endl;
    //cout << "the first train image: " << TrainImagePath.at(0) << endl;

    vector<seeta::cv::ImageData> TrainImageList;
    vector<SeetaFaceInfo> TrainFaceList;
    vector<SeetaFaceInfo> tempface;
    j = 0;
    for (i = 0; i < TrainImagePath.size(); i++)
    {
        //cout << "read image: " << TrainImagePath.at(i) << endl;
        TrainImageList.push_back(cv::imread(TrainImagePath.at(i)));
        tempface = engine.DetectFaces(TrainImageList.at(TrainImageList.size() - 1));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TrainFaceList.push_back(tempface.at(0));
        //TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 7, 3));
        TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 6, 2));
    }
    cout << "train data size: " << TrainFaceList.size() << endl;
    /*
    cout << "Train Image List size: " << TrainImageList.size() << endl;
    cout << "Train Faces List size: " << TrainFaceList.size() << endl;
    cout << "Train Labels size: " << TrainLabels.size() << endl;
    cout << "the first label: " << TrainLabels.at(0) << endl;
    cout << "the second label: " << TrainLabels.at(1) << endl;
    */

    if (TrainImageList.size() != TrainFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TrainPointList;
    for (i = 0; i < TrainImageList.size(); i++)
    {
        TrainPointList.push_back(engine.DetectPoints(TrainImageList.at(i), TrainFaceList.at(i)));
    }
    //cout << "Train Points List size: " << TrainPointList.size() << endl;
    //cout << "the size of first points: " << TrainPointList.at(0).size() << endl;

    //get features of image in store
    printf("get features of image in store...\n");
    seeta::FaceRecognizer FR(seeta::ModelSetting("./model/fr_2_10.dat"));
    vector<float *> TrainFeatureList;
    float *temp_float;
    for (i = 0; i < TrainPointList.size(); i++)
    {
        temp_float = (float *)malloc(sizeof(float) * FR.GetExtractFeatureSize());
        TrainFeatureList.push_back(temp_float);
    }
    for (i = 0; i < TrainPointList.size(); i++)
    {
        FR.Extract(TrainImageList.at(i), TrainPointList.at(i).data(), TrainFeatureList.at(i));
    }

    //cout << "Train Features List size: " << TrainFeatureList.size() << endl;
    /*
    cout << "the first train feature:" << endl;
    printFeature(TrainFeatureList.at(0), FR.GetExtractFeatureSize());
    cout << "the second train feature:" << endl;
    printFeature(TrainFeatureList.at(1), FR.GetExtractFeatureSize());
    */
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare train image finish, cost: %f\n", time_cost);

    s = clock();
    //read test image
    printf("read test image...\n");
    string TestImageDir = "/home/hustccc/Pictures/FaceImage/Test_01/";
    vector<string> TestImagePath;
    vector<string> TestLabels;
    if (getAbsoluteFiles(TestImageDir, TestImagePath) != 0)
    {
        printf("read test image error\n");
        exit(-1);
    }
    //log
    //cout << "size of test image: " << TestImagePath.size() << endl;
    //cout << "the first test image: " << TestImagePath.at(0) << endl;

    vector<seeta::cv::ImageData> TestImageList;
    vector<SeetaFaceInfo> TestFaceList;
    j = 0;
    for (i = 0; i < TestImagePath.size(); i++)
    {
        //cout << "read image: " << TestImagePath.at(i) << endl;
        TestImageList.push_back(cv::imread(TestImagePath.at(i)));
        tempface = engine.DetectFaces(TestImageList.at(j++));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TestImageList.pop_back();
            j--;
            cout << "read image: " << TestImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TestImageList.pop_back();
            j--;
            cout << "read image: " << TestImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TestFaceList.push_back(tempface.at(0));
        //TestLabels.push_back(TestImagePath.at(i).substr(TestImagePath.at(i).size() - 7, 3));
        TestLabels.push_back(TestImagePath.at(i).substr(TestImagePath.at(i).size() - 6, 2));
        //cout<<"["<<j-1<<"] "<<TestImagePath.at(i)<<endl;
    }
    cout << "test data size: " << TestFaceList.size() << endl;
    //cout << "Test Image List size: " << TestImageList.size() << endl;
    //cout << "Test Faces List size: " << TestFaceList.size() << endl;

    if (TestImageList.size() != TestFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TestPointList;
    for (i = 0; i < TestImageList.size(); i++)
    {
        TestPointList.push_back(engine.DetectPoints(TestImageList.at(i), TestFaceList.at(i)));
    }
    //cout << "Train Points List size: " << TestPointList.size() << endl;
    //cout << "the size of first points: " << TestPointList.at(0).size() << endl;

    //get features of test image
    printf("get features of test image...\n");
    vector<float *> TestFeatureList;
    for (i = 0; i < TestPointList.size(); i++)
    {
        temp_float = (float *)malloc(sizeof(float) * FR.GetExtractFeatureSize());
        TestFeatureList.push_back(temp_float);
    }

    for (i = 0; i < TestPointList.size(); i++)
    {
        FR.Extract(TestImageList.at(i), TestPointList.at(i).data(), TestFeatureList.at(i));
    }

    //cout << "Test Features List size: " << TestFeatureList.size() << endl;
    /*  
    cout << "the first test feature:" << endl;
    printFeature(TestFeatureList.at(0), FR.GetExtractFeatureSize());
    printFeature(TestFeatureList.at(1), FR.GetExtractFeatureSize());
    */
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare test image finish, cost: %f\n", time_cost);

    //printFeature(TrainFeatureList.at(0),10);
    //predict with knn
    /*
    printf("train labels:\n");
    for (i = 0; i < TrainLabels.size(); i++)
        cout << TrainLabels.at(i) << " ";
    cout << endl;
    */

    printf("predict with knn...\n");
    s = clock();
    vector<string> PredictResult;
    int k = 3;
    PredictResult = KNNClassifier(TrainFeatureList, TrainLabels, TestFeatureList, FR.GetExtractFeatureSize(), k);
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    cout << "predict finish , cost: " << time_cost << endl;
    cout << "predict result: " << endl;
    for (i = 0; i < PredictResult.size(); i++)
    {
        cout << "[" << i << "] " << PredictResult.at(i) << "  ";
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    printf("\n");
    //print result

    float accuracy = 0;
    j = 0;
    for (i = 0; i < TestLabels.size(); i++)
        if (PredictResult.at(i) == TestLabels.at(i))
            j++;
        else
            cout << "not recoginzed: " << i << " " << TestLabels.at(i) << endl;

    //printf("%d %d\n",j,TestLabels.size());
    accuracy = (float)j / (float)TestLabels.size();
    printf("accuracy: %f\%\n", accuracy * 100);
    return 0;
}

int Mode_1_x()
{
    //Mode 1 x
    int i = 0;
    int j = 0;
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model("./model/fd_2_00.dat", device, id);
    seeta::ModelSetting PD_model("./model/pd_2_00_pts81.dat", device, id);
    seeta::ModelSetting FR_model("./model/fr_2_10.dat", device, id);
    seeta::FaceEngine engine(FD_model, PD_model, FR_model, 2, 16);

    clock_t s, t;
    double time_cost;

    //prepare image store
    printf("prepare pictures store...\n");

    s = clock();
    string ImageStoreDir = "/home/hustccc/Pictures/FaceImage/CelebA/img_align_celeba/";
    vector<string> TrainImagePath;
    //vector<string> TrainLabels;

    //cout << "size of train image: " << TrainImagePath.size() << endl;
    //cout << "the first train image: " << TrainImagePath.at(0) << endl;

    char trainfilename[M] = "/home/hustccc/Pictures/FaceImage/CelebA/celeba_train_labels";
    char temp[M] = {};
    char block;

    vector<vector<int>> TrainLabels;
    vector<int> line;
    FILE *fp = fopen(trainfilename, "r");

    for (i = 0; i < TRAINREADNUM; i++)
    {
        line.clear();
        //cout<<"i: "<<i<<endl;
        for (j = 0; j < 41; j++)
        {
            if (j == 0)
            {
                fread(temp, sizeof(char), 10, fp); //read image name
                TrainImagePath.push_back(temp);
                //fread(&block,sizeof(char),1,fp);    //read block
            }
            else
            {
                fread(temp, sizeof(char), 1, fp);
                if (temp[0] == 49)
                {
                    printf("1 ");
                    line.push_back(1);
                }
                else
                {
                    printf("0 ");
                    line.push_back(0);
                }
                //fread(&block,sizeof(char),1,fp);    //read block
            }
        }
        fread(&block, sizeof(char), 2, fp); //read block
        TrainLabels.push_back(line);
    }
    fclose(fp);
    cout << endl;
    cout << "ImagePathList size: " << TrainImagePath.size() << endl;
    cout << "LabelsList size: " << TrainLabels.size() << endl;
    cout << "line size: " << TrainLabels.at(0).size() << endl;
    for (i = 0; i < TRAINREADNUM; i++)
    {
        cout << TrainImagePath.at(i) << ": ";
        for (j = 0; j < 40; j++)
        {
            cout << TrainLabels.at(i).at(j) << " ";
        }
        printf("\n");
    }
    printf("\n");
    vector<seeta::cv::ImageData> TrainImageList;
    vector<SeetaFaceInfo> TrainFaceList;
    vector<SeetaFaceInfo> tempface;
    j = 0;
    for (i = 0; i < TrainImagePath.size(); i++)
    {
        //cout << "read image: " << TrainImagePath.at(i) << endl;
        TrainImageList.push_back(cv::imread(ImageStoreDir + TrainImagePath.at(i)));
        tempface = engine.DetectFaces(TrainImageList.at(TrainImageList.size() - 1));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TrainFaceList.push_back(tempface.at(0));
        //TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 7, 3));
        //TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 5, 1));
    }
    cout << "train data size: " << TrainFaceList.size() << endl;
    cout << "Train Image List size: " << TrainImageList.size() << endl;
    cout << "Train Faces List size: " << TrainFaceList.size() << endl;

    if (TrainImageList.size() != TrainFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TrainPointList;
    for (i = 0; i < TrainImageList.size(); i++)
    {
        TrainPointList.push_back(engine.DetectPoints(TrainImageList.at(i), TrainFaceList.at(i)));
    }
    //cout << "Train Points List size: " << TrainPointList.size() << endl;
    //cout << "the size of first points: " << TrainPointList.at(0).size() << endl;

    //get features of image in store
    printf("get features of image in store...\n");
    //seeta::FaceRecognizer FR(seeta::ModelSetting("./model/fr_2_10.dat"));
    seeta::FaceRecognizer FR(FR_model);
    vector<float *> TrainFeatureList;
    float *temp_float;
    for (i = 0; i < TrainPointList.size(); i++)
    {
        //temp_float = (float *)malloc(sizeof(float) * FR.GetExtractFeatureSize());
        temp_float = new float[sizeof(float) * FR.GetExtractFeatureSize()];
        if (temp_float == NULL)
        {
            printf("new space error\n");
            exit(-1);
        }
        TrainFeatureList.push_back(temp_float);
    }
    for (i = 0; i < TrainPointList.size(); i++)
    {
        FR.Extract(TrainImageList.at(i), TrainPointList.at(i).data(), TrainFeatureList.at(i));
    }

    cout << "Train Features List size: " << TrainFeatureList.size() << endl;

    //cout << "the first train feature:" << endl;
    //printFeature(TrainFeatureList.at(0), FR.GetExtractFeatureSize());
    //cout << "the second train feature:" << endl;
    //printFeature(TrainFeatureList.at(1), FR.GetExtractFeatureSize());
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare train image finish, cost: %f\n", time_cost);

    //prepare test image
    printf("prepare test image...\n");

    s = clock();
    vector<string> TestImagePath;

    char testfilename[M] = "/home/hustccc/Pictures/FaceImage/CelebA/celeba_test_labels";

    vector<vector<int>> TestLabels;
    FILE *fb = fopen(testfilename, "r");

    for (i = 0; i < TESTREADNUM; i++)
    {
        line.clear();
        //cout<<"i: "<<i<<endl;
        for (j = 0; j < 41; j++)
        {
            if (j == 0)
            {
                fread(temp, sizeof(char), 10, fb); //read image name
                TestImagePath.push_back(temp);
                //fread(&block,sizeof(char),1,fp);    //read block
            }
            else
            {
                fread(temp, sizeof(char), 1, fb);
                if (temp[0] == 49)
                {
                    printf("1 ");
                    line.push_back(1);
                }
                else
                {
                    printf("0 ");
                    line.push_back(0);
                }
                //fread(&block,sizeof(char),1,fp);    //read block
            }
        }
        fread(&block, sizeof(char), 2, fb); //read block
        TestLabels.push_back(line);
    }
    fclose(fb);
    cout << endl;
    cout << "ImagePathList size: " << TestImagePath.size() << endl;
    cout << "LabelsList size: " << TestLabels.size() << endl;
    cout << "line size: " << TestLabels.at(0).size() << endl;
    for (i = 0; i < TESTREADNUM; i++)
    {
        cout << TestImagePath.at(i) << ": ";
        for (j = 0; j < 40; j++)
        {
            cout << TestLabels.at(i).at(j) << " ";
        }
        printf("\n");
    }
    printf("\n");
    vector<seeta::cv::ImageData> TestImageList;
    vector<SeetaFaceInfo> TestFaceList;
    j = 0;
    for (i = 0; i < TestImagePath.size(); i++)
    {
        //cout << "read image: " << TestImagePath.at(i) << endl;
        TestImageList.push_back(cv::imread(ImageStoreDir + TestImagePath.at(i)));
        tempface = engine.DetectFaces(TestImageList.at(TestImageList.size() - 1));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TestImageList.pop_back();
            j--;
            cout << "read image: " << TestImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TestImageList.pop_back();
            j--;
            cout << "read image: " << TestImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TestFaceList.push_back(tempface.at(0));
    }
    cout << "test data size: " << TestFaceList.size() << endl;

    cout << "Test Image List size: " << TestImageList.size() << endl;
    cout << "Test Faces List size: " << TestFaceList.size() << endl;

    if (TestImageList.size() != TestFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TestPointList;
    for (i = 0; i < TestImageList.size(); i++)
    {
        TestPointList.push_back(engine.DetectPoints(TestImageList.at(i), TestFaceList.at(i)));
    }
    cout << "Test Points List size: " << TestPointList.size() << endl;
    cout << "the size of first points: " << TestPointList.at(0).size() << endl;

    //get features of test image
    printf("get features of test image ...\n");
    //seeta::FaceRecognizer FM(seeta::ModelSetting("./model/fr_2_10.dat"));
    seeta::FaceRecognizer FM(FR_model);
    //cout<<FM.GetExtractFeatureSize()<<endl;
    vector<float *> TestFeatureList;
    for (j = 0; j < TestPointList.size(); j++)
    {
        //temp_float = (float *)malloc(sizeof(float) * FM.GetExtractFeatureSize());
        temp_float = new float[FM.GetExtractFeatureSize() * sizeof(float)];
        if (temp_float == NULL)
        {
            printf("new space error\n");
            exit(-1);
        }
        TestFeatureList.push_back(temp_float);
    }
    for (i = 0; i < TestPointList.size(); i++)
    {
        FM.Extract(TestImageList.at(i), TestPointList.at(i).data(), TestFeatureList.at(i));
    }

    cout << "Test Features List size: " << TestFeatureList.size() << endl;
    //cout << "the first test feature:" << endl;
    //printFeature(TestFeatureList.at(0), FM.GetExtractFeatureSize());
    //cout << "the second test feature:" << endl;
    //printFeature(TestFeatureList.at(1), FM.GetExtractFeatureSize());
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare test image finish, cost: %f\n", time_cost);

    printf("predict with knn...\n");
    s = clock();
    vector<vector<int>> PredictResult;
    int k = 3;
    PredictResult = KNNClassifier_x(TrainFeatureList, TrainLabels, TestFeatureList, FM.GetExtractFeatureSize(), k);
    printf("ok here\n");
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    cout << "predict finish , cost: " << time_cost << endl;
    cout << "predict result: " << endl;
    for (i = 0; i < PredictResult.size(); i++)
    {
        cout << "[" << i << "] ";
        for (j = 0; j < PredictResult.at(i).size(); j++)
            cout << PredictResult.at(i).at(j) << " ";
        printf("\n");
    }
    printf("\n");
    //print result

    float accuracy = 0;
    j = 0;
    int num = 0;
    cout << "PredictResult size: " << PredictResult.size() << endl;
    for (i = 0; i < PredictResult.size(); i++)
        num += compareLabel(PredictResult.at(i), TestLabels.at(i));
    printf("num: %d\n", num);
    accuracy = (float)num / (float)(PredictResult.size() * PredictResult.at(0).size());
    printf("accuracy: %f\%\n", accuracy * 100);

    exit(-1);
    return 0;
}
int Mode_2(string predictImagePath)
{
    //Mode 2
    //Mode 1 x
    int i = 0;
    int j = 0;
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model("./model/fd_2_00.dat", device, id);
    seeta::ModelSetting PD_model("./model/pd_2_00_pts81.dat", device, id);
    seeta::ModelSetting FR_model("./model/fr_2_10.dat", device, id);
    seeta::FaceEngine engine(FD_model, PD_model, FR_model, 2, 16);

    clock_t s, t;
    double time_cost;

    //prepare image store
    printf("prepare pictures store...\n");

    s = clock();
    string ImageStoreDir = "/home/hustccc/Pictures/FaceImage/CelebA/img_align_celeba/";
    vector<string> TrainImagePath;
    //vector<string> TrainLabels;

    //cout << "size of train image: " << TrainImagePath.size() << endl;
    //cout << "the first train image: " << TrainImagePath.at(0) << endl;

    char trainfilename[M] = "/home/hustccc/Pictures/FaceImage/CelebA/celeba_train_labels";
    char temp[M] = {};
    char block;

    vector<vector<int>> TrainLabels;
    vector<int> line;
    FILE *fp = fopen(trainfilename, "r");

    for (i = 0; i < TRAINREADNUM; i++)
    {
        line.clear();
        //cout<<"i: "<<i<<endl;
        for (j = 0; j < 41; j++)
        {
            if (j == 0)
            {
                fread(temp, sizeof(char), 10, fp); //read image name
                TrainImagePath.push_back(temp);
                //fread(&block,sizeof(char),1,fp);    //read block
            }
            else
            {
                fread(temp, sizeof(char), 1, fp);
                if (temp[0] == 49)
                {
                    printf("1 ");
                    line.push_back(1);
                }
                else
                {
                    printf("0 ");
                    line.push_back(0);
                }
                //fread(&block,sizeof(char),1,fp);    //read block
            }
        }
        fread(&block, sizeof(char), 2, fp); //read block
        TrainLabels.push_back(line);
    }
    fclose(fp);
    cout << endl;
    cout << "ImagePathList size: " << TrainImagePath.size() << endl;
    cout << "LabelsList size: " << TrainLabels.size() << endl;
    cout << "line size: " << TrainLabels.at(0).size() << endl;
    for (i = 0; i < TRAINREADNUM; i++)
    {
        cout << TrainImagePath.at(i) << ": ";
        for (j = 0; j < 40; j++)
        {
            cout << TrainLabels.at(i).at(j) << " ";
        }
        printf("\n");
    }
    printf("\n");
    vector<seeta::cv::ImageData> TrainImageList;
    vector<SeetaFaceInfo> TrainFaceList;
    vector<SeetaFaceInfo> tempface;
    j = 0;
    for (i = 0; i < TrainImagePath.size(); i++)
    {
        //cout << "read image: " << TrainImagePath.at(i) << endl;
        TrainImageList.push_back(cv::imread(ImageStoreDir + TrainImagePath.at(i)));
        tempface = engine.DetectFaces(TrainImageList.at(TrainImageList.size() - 1));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TrainFaceList.push_back(tempface.at(0));
        //TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 7, 3));
        //TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 5, 1));
    }
    cout << "train data size: " << TrainFaceList.size() << endl;
    cout << "Train Image List size: " << TrainImageList.size() << endl;
    cout << "Train Faces List size: " << TrainFaceList.size() << endl;

    if (TrainImageList.size() != TrainFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TrainPointList;
    for (i = 0; i < TrainImageList.size(); i++)
    {
        TrainPointList.push_back(engine.DetectPoints(TrainImageList.at(i), TrainFaceList.at(i)));
    }
    //cout << "Train Points List size: " << TrainPointList.size() << endl;
    //cout << "the size of first points: " << TrainPointList.at(0).size() << endl;

    //get features of image in store
    printf("get features of image in store...\n");
    //seeta::FaceRecognizer FR(seeta::ModelSetting("./model/fr_2_10.dat"));
    seeta::FaceRecognizer FR(FR_model);
    vector<float *> TrainFeatureList;
    float *temp_float;
    for (i = 0; i < TrainPointList.size(); i++)
    {
        //temp_float = (float *)malloc(sizeof(float) * FR.GetExtractFeatureSize());
        temp_float = new float[sizeof(float) * FR.GetExtractFeatureSize()];
        if (temp_float == NULL)
        {
            printf("new space error\n");
            exit(-1);
        }
        TrainFeatureList.push_back(temp_float);
    }
    for (i = 0; i < TrainPointList.size(); i++)
    {
        FR.Extract(TrainImageList.at(i), TrainPointList.at(i).data(), TrainFeatureList.at(i));
    }

    cout << "Train Features List size: " << TrainFeatureList.size() << endl;

    //cout << "the first train feature:" << endl;
    //printFeature(TrainFeatureList.at(0), FR.GetExtractFeatureSize());
    //cout << "the second train feature:" << endl;
    //printFeature(TrainFeatureList.at(1), FR.GetExtractFeatureSize());
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare train image finish, cost: %f\n", time_cost);

    //prepare test image
    printf("prepare predict image...\n");

    s = clock();

    seeta::cv::ImageData predictImage;
    vector<SeetaFaceInfo> predictFace;
    predictImage = cv::imread(predictImagePath);
    predictFace = engine.DetectFaces(predictImage);
    if (predictFace.size() != 1)
    {
        printf("no face or more than one face...exit...\n");
        exit(-1);
    }

    //vector<vector<SeetaPointF>> predictPoint;
    //predictPoint.push_back(engine.DetectPoints(predictImage,predictFace.at(0)));
    vector<SeetaPointF> predictPoint;
    predictPoint = engine.DetectPoints(predictImage, predictFace.at(0));

    //get features of test image
    printf("get features of predict image ...\n");
    seeta::FaceRecognizer FM(FR_model);
    vector<float *> predictFeatureList;
    temp_float = new float[FM.GetExtractFeatureSize() * sizeof(float)];
    if (temp_float == NULL)
    {
        printf("new space error\n");
        exit(-1);
    }
    predictFeatureList.push_back(temp_float);

    FM.Extract(predictImage, predictPoint.data(), predictFeatureList.at(0));

    cout << "Test Features List size: " << predictFeatureList.size() << endl;
    //cout << "the first test feature:" << endl;
    //printFeature(TestFeatureList.at(0), FM.GetExtractFeatureSize());
    //cout << "the second test feature:" << endl;
    //printFeature(TestFeatureList.at(1), FM.GetExtractFeatureSize());
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare predict image finish, cost: %f\n", time_cost);

    printf("predict with knn...\n");
    s = clock();
    vector<vector<int>> PredictResult;
    int k = 3;
    PredictResult = KNNClassifier_x(TrainFeatureList, TrainLabels, predictFeatureList, FM.GetExtractFeatureSize(), k);
    //printf("ok here\n");
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    cout << "predict finish , cost: " << time_cost << endl;
    cout << "predict result: " << endl;
    for (j = 0; j < PredictResult.at(0).size(); j++)
        cout << PredictResult.at(0).at(j) << " ";
    printf("\n");
    vector<string> LabelsList = {"5_o_Clock_Shadow", "Arched_Eyebrows", "Attractive", "Bags_Under_Eyes", "Bald", "Bangs", "Big_Lips", "Big_Nose", "Black_Hair", "Blond_Hair", "Blurry", "Brown_Hair", " Bushy_Eyebrows", "Chubby", "Double_Chin", "Eyeglasses", "Goatee", "Gray_Hair", "Heavy_Makeup", "High_Cheekbones", "Male", "Mouth_Slightly_Open", "Mustache", "Narrow_Eyes", "No_Beard", "Oval_Face", "Pale_Skin", "Pointy_Nose", "Receding_Hairline", "Rosy_Cheeks", "Sideburns", "Smiling", "Straight_Hair", "Wavy_Hair", "Wearing_Earrings", "Wearing_Hat", "Wearing_Lipstick", "Wearing_Necklace", "Wearing_Necktie", "Young"};
    cout << "LabelsList size: " << LabelsList.size() << endl;
    for (i = 0; i < LabelsList.size(); i++)
    {
        if (PredictResult.at(0).at(i))
            cout << LabelsList.at(i) << "  ";
    }
    printf("\n");

    exit(-1);

    return 0;
}

int Mode_3(string option_1, string option_2)
{
    //Mode 3
    int i = 0;
    int j = 0;
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model("./model/fd_2_00.dat", device, id);
    seeta::ModelSetting PD_model("./model/pd_2_00_pts5.dat", device, id);
    seeta::ModelSetting FR_model("./model/fr_2_10.dat", device, id);
    seeta::FaceEngine engine(FD_model, PD_model, FR_model, 2, 16);

    clock_t s, t;
    double time_cost;

    //prepare image store
    printf("prepare pictures store...\n");

    s = clock();
    string ImageStoreDir = "/home/hustccc/Pictures/FaceImage/" + option_1 + "/";
    vector<string> TrainImagePath;
    vector<string> TrainLabels;
    if (getAbsoluteFiles(ImageStoreDir, TrainImagePath) != 0)
    {
        printf("read train image error\n");
        exit(-1);
    }
    //log
    //cout << "size of train image: " << TrainImagePath.size() << endl;
    //cout << "the first train image: " << TrainImagePath.at(0) << endl;

    vector<seeta::cv::ImageData> TrainImageList;
    vector<SeetaFaceInfo> TrainFaceList;
    vector<SeetaFaceInfo> tempface;
    j = 0;
    for (i = 0; i < TrainImagePath.size(); i++)
    {
        //cout << "read image: " << TrainImagePath.at(i) << endl;
        TrainImageList.push_back(cv::imread(TrainImagePath.at(i)));
        tempface = engine.DetectFaces(TrainImageList.at(TrainImageList.size() - 1));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TrainImageList.pop_back();
            j--;
            cout << "read image: " << TrainImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TrainFaceList.push_back(tempface.at(0));
        //TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 7, 3));
        TrainLabels.push_back(TrainImagePath.at(i).substr(TrainImagePath.at(i).size() - 5, 1));
    }
    cout << "train data size: " << TrainFaceList.size() << endl;
    /*
    cout << "Train Image List size: " << TrainImageList.size() << endl;
    cout << "Train Faces List size: " << TrainFaceList.size() << endl;
    cout << "Train Labels size: " << TrainLabels.size() << endl;
    cout << "the first label: " << TrainLabels.at(0) << endl;
    cout << "the second label: " << TrainLabels.at(1) << endl;
    */

    if (TrainImageList.size() != TrainFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TrainPointList;
    for (i = 0; i < TrainImageList.size(); i++)
    {
        TrainPointList.push_back(engine.DetectPoints(TrainImageList.at(i), TrainFaceList.at(i)));
    }
    //cout << "Train Points List size: " << TrainPointList.size() << endl;
    //cout << "the size of first points: " << TrainPointList.at(0).size() << endl;

    //get features of image in store
    printf("get features of image in store...\n");
    seeta::FaceRecognizer FR(seeta::ModelSetting("./model/fr_2_10.dat"));
    vector<float *> TrainFeatureList;
    float *temp_float;
    for (i = 0; i < TrainPointList.size(); i++)
    {
        temp_float = (float *)malloc(sizeof(float) * FR.GetExtractFeatureSize());
        TrainFeatureList.push_back(temp_float);
    }
    for (i = 0; i < TrainPointList.size(); i++)
    {
        FR.Extract(TrainImageList.at(i), TrainPointList.at(i).data(), TrainFeatureList.at(i));
    }

    //cout << "Train Features List size: " << TrainFeatureList.size() << endl;
    /*
    cout << "the first train feature:" << endl;
    printFeature(TrainFeatureList.at(0), FR.GetExtractFeatureSize());
    cout << "the second train feature:" << endl;
    printFeature(TrainFeatureList.at(1), FR.GetExtractFeatureSize());
    */
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare train image finish, cost: %f\n", time_cost);

    s = clock();
    //read test image
    printf("read test image...\n");
    string TestImageDir = "/home/hustccc/Pictures/FaceImage/" + option_2 + "/";
    vector<string> TestImagePath;
    vector<string> TestLabels;
    if (getAbsoluteFiles(TestImageDir, TestImagePath) != 0)
    {
        printf("read test image error\n");
        exit(-1);
    }
    //log
    //cout << "size of test image: " << TestImagePath.size() << endl;
    //cout << "the first test image: " << TestImagePath.at(0) << endl;

    vector<seeta::cv::ImageData> TestImageList;
    vector<SeetaFaceInfo> TestFaceList;
    j = 0;
    for (i = 0; i < TestImagePath.size(); i++)
    {
        //cout << "read image: " << TestImagePath.at(i) << endl;
        TestImageList.push_back(cv::imread(TestImagePath.at(i)));
        tempface = engine.DetectFaces(TestImageList.at(j++));
        //cout << "tempface size: " << tempface.size() << endl;
        if (tempface.size() > 1)
        {
            TestImageList.pop_back();
            j--;
            cout << "read image: " << TestImagePath.at(i) << "... ";
            printf("more than one face in an image, continue...\n");
            continue;
        }
        else if (tempface.size() == 0)
        {
            TestImageList.pop_back();
            j--;
            cout << "read image: " << TestImagePath.at(i) << "... ";
            printf("no face , continue...\n");
            continue;
        }
        TestFaceList.push_back(tempface.at(0));
        //TestLabels.push_back(TestImagePath.at(i).substr(TestImagePath.at(i).size() - 7, 3));
        TestLabels.push_back(TestImagePath.at(i).substr(TestImagePath.at(i).size() - 5, 1));
        //cout<<"["<<j-1<<"] "<<TestImagePath.at(i)<<endl;
    }
    cout << "test data size: " << TestFaceList.size() << endl;
    //cout << "Test Image List size: " << TestImageList.size() << endl;
    //cout << "Test Faces List size: " << TestFaceList.size() << endl;

    if (TestImageList.size() != TestFaceList.size())
    {
        printf("something wrong...exit\n");
        exit(-1);
    }

    vector<vector<SeetaPointF>> TestPointList;
    for (i = 0; i < TestImageList.size(); i++)
    {
        TestPointList.push_back(engine.DetectPoints(TestImageList.at(i), TestFaceList.at(i)));
    }
    //cout << "Train Points List size: " << TestPointList.size() << endl;
    //cout << "the size of first points: " << TestPointList.at(0).size() << endl;

    //get features of test image
    printf("get features of test image...\n");
    vector<float *> TestFeatureList;
    for (i = 0; i < TestPointList.size(); i++)
    {
        temp_float = (float *)malloc(sizeof(float) * FR.GetExtractFeatureSize());
        TestFeatureList.push_back(temp_float);
    }

    for (i = 0; i < TestPointList.size(); i++)
    {
        FR.Extract(TestImageList.at(i), TestPointList.at(i).data(), TestFeatureList.at(i));
    }

    //cout << "Test Features List size: " << TestFeatureList.size() << endl;
    /*  
    cout << "the first test feature:" << endl;
    printFeature(TestFeatureList.at(0), FR.GetExtractFeatureSize());
    cout << "the second test feature:" << endl;
    printFeature(TestFeatureList.at(1), FR.GetExtractFeatureSize());
    */
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    printf("prepare test image finish, cost: %f\n", time_cost);

    //printFeature(TrainFeatureList.at(0),10);
    //predict with knn
    /*
    printf("train labels:\n");
    for (i = 0; i < TrainLabels.size(); i++)
        cout << TrainLabels.at(i) << " ";
    cout << endl;
    */

    printf("predict with knn...\n");
    s = clock();
    vector<string> PredictResult;
    int k = 3;
    PredictResult = KNNClassifier(TrainFeatureList, TrainLabels, TestFeatureList, FR.GetExtractFeatureSize(), k);
    t = clock();
    time_cost = (double)(t - s) / CLOCKS_PER_SEC;
    cout << "predict finish , cost: " << time_cost << endl;
    cout << "predict result: " << endl;
    for (i = 0; i < PredictResult.size(); i++)
        cout << "[" << i << "] " << PredictResult.at(i) << endl;
    //print result

    float accuracy = 0;
    j = 0;
    for (i = 0; i < TestLabels.size(); i++)
        if (PredictResult.at(i) == TestLabels.at(i))
            j++;
        else
            cout << "not recoginzed: " << i << " " << TestLabels.at(i) << endl;

    //printf("%d %d\n",j,TestLabels.size());
    accuracy = (float)j / (float)TestLabels.size();
    printf("accuracy: %f\%\n", accuracy * 100);
    return 0;
}

int Mode_4()
{
    //Mode 4
    return 0;
}

vector<string> KNNClassifier(vector<float *> train_data, vector<string> train_label, vector<float *> test_data, int feature_length, int k)
{
    //printf("Run KNN Classifier..\n");
    int i, j, m;
    int predictLabelIndex;
    float a = Parameter_A;
    float c = Parameter_C;
    int d = Parameter_D;
    float gama = Parameter_gama;
    float r = Parameter_R;
    vector<int> LabelCount;
    for (i = 0; i < k; i++)
        LabelCount.push_back(0);
    vector<int> kMinIndex;
    //feature*MulRate
    for (i = 0; i < train_data.size(); i++)
        for (j = 0; j < feature_length; j++)
            *(train_data.at(i) + j) *= MulRate;

    for (i = 0; i < test_data.size(); i++)
        for (j = 0; j < feature_length; j++)
            *(test_data.at(i) + j) *= MulRate;

    vector<string> predict_result;
    //printFeature(train_data.at(0),10);
    vector<vector<double>> DistList;
    vector<double> zerodist;
    //init DistList
    for (i = 0; i < train_data.size(); i++)
        zerodist.push_back(0);
    for (i = 0; i < test_data.size(); i++)
        DistList.push_back(zerodist);

    //cout<<"zerodist size: "<<zerodist.size()<<endl;
    //cout<<"DistList size: "<<DistList.size()<<endl;

    for (i = 0; i < test_data.size(); i++)
        for (j = 0; j < train_data.size(); j++)
            //DistList.at(i).at(j) = EuclideanDist(test_data.at(i), train_data.at(j), feature_length);
            //DistList.at(i).at(j) = ManhanttenDist(test_data.at(i), train_data.at(j), feature_length);
            DistList.at(i).at(j) = KernelDist_1(test_data.at(i), train_data.at(j),c, feature_length);
            //DistList.at(i).at(j) = KernelDist_2(test_data.at(i), train_data.at(j), a,c,d, feature_length);
            //DistList.at(i).at(j) = KernelDist_3(test_data.at(i), train_data.at(j), gama, feature_length);
            //DistList.at(i).at(j) = KernelDist_4(test_data.at(i), train_data.at(j), gama, r, feature_length);
    //printf("log\n");
    for (i = 0; i < DistList.size(); i++)
    {
        kMinIndex = GetKMinIndex(DistList.at(i), k);
        //cout<<"KMindex size: "<<kMinIndex.size()<<endl;
        for (j = 0; j < k; j++)
            LabelCount.at(j) = 0;
        for (j = 0; j < k; j++)
            for (m = 0; m < k; m++)
                if (train_label.at(kMinIndex.at(j)) == train_label.at(kMinIndex.at(m)))
                    LabelCount.at(j)++;
        predictLabelIndex = kMinIndex.at(max_element(LabelCount.begin(), LabelCount.end()) - LabelCount.begin());
        predict_result.push_back(train_label.at(predictLabelIndex));
    }
    /*
    for (i = 0; i < test_data.size(); i++)
    {
        for (j = 0; j < train_label.size(); j++)
        {
            cout << DistList.at(i).at(j) << " ";
        }

        cout << endl;
    }
    */
    //printf("predict finish\n");
    return predict_result;
}

vector<vector<int>> KNNClassifier_x(vector<float *> train_data, vector<vector<int>> train_label, vector<float *> test_data, int feature_length, int k)
{
    printf("Run KNN Classifier..\n");
    int i, j, m;
    int predictLabelIndex;
    float a = Parameter_A;
    float c = Parameter_C;
    int d = Parameter_D;
    float gama = Parameter_gama;
    float r = Parameter_R;
    vector<int> LabelCount;
    for (i = 0; i < k; i++)
        LabelCount.push_back(0);
    vector<int> kMinIndex;
    //feature*MulRate
    for (i = 0; i < train_data.size(); i++)
        for (j = 0; j < feature_length; j++)
            *(train_data.at(i) + j) *= MulRate;

    for (i = 0; i < test_data.size(); i++)
        for (j = 0; j < feature_length; j++)
            *(test_data.at(i) + j) *= MulRate;

    vector<vector<int>> predict_result;
    //printFeature(train_data.at(0),10);
    vector<vector<double>> DistList;
    vector<double> zerodist;
    //init DistList
    for (i = 0; i < train_data.size(); i++)
        zerodist.push_back(0);
    for (i = 0; i < test_data.size(); i++)
        DistList.push_back(zerodist);

    //cout<<"zerodist size: "<<zerodist.size()<<endl;
    //cout<<"DistList size: "<<DistList.size()<<endl;

    for (i = 0; i < test_data.size(); i++)
        for (j = 0; j < train_data.size(); j++)
            //DistList.at(i).at(j) = EuclideanDist(test_data.at(i), train_data.at(j), feature_length);
            //DistList.at(i).at(j) = ManhanttenDist(test_data.at(i), train_data.at(j), feature_length);
            //DistList.at(i).at(j) = KernelDist_1(test_data.at(i), train_data.at(j),c, feature_length);
            //DistList.at(i).at(j) = KernelDist_2(test_data.at(i), train_data.at(j), a,c,d, feature_length);
            //DistList.at(i).at(j) = KernelDist_3(test_data.at(i), train_data.at(j), gama, feature_length);
            DistList.at(i).at(j) = KernelDist_4(test_data.at(i), train_data.at(j), gama, r, feature_length);
    //printf("log\n");
    for (i = 0; i < DistList.size(); i++)
    {
        kMinIndex = GetKMinIndex(DistList.at(i), k);
        //cout<<"KMindex size: "<<kMinIndex.size()<<endl;
        for (j = 0; j < k; j++)
            LabelCount.at(j) = 0;
        for (j = 0; j < k; j++)
            for (m = 0; m < k; m++)
                if (train_label.at(kMinIndex.at(j)) == train_label.at(kMinIndex.at(m)))
                    LabelCount.at(j)++;
        predictLabelIndex = kMinIndex.at(max_element(LabelCount.begin(), LabelCount.end()) - LabelCount.begin());
        predict_result.push_back(train_label.at(predictLabelIndex));
    }
    /*
    for (i = 0; i < test_data.size(); i++)
    {
        for (j = 0; j < train_label.size(); j++)
        {
            cout << DistList.at(i).at(j) << " ";
        }

        cout << endl;
    }
    */
    //printf("predict finish\n");
    return predict_result;
}

vector<int> GetKMinIndex(vector<double> distlist, int k)
{
    //printf("get k min index...\n");
    vector<int> ResultIndex;
    int tempIndex;
    int i, j;
    //cout<<"distlist size: "<<distlist.size()<<endl;
    for (i = 0; i < distlist.size(); i++)
        ResultIndex.push_back(i);
    for (i = 0; i < distlist.size(); i++)
        for (j = 0; j < distlist.size() - i - 1; j++)
        {
            if (distlist.at(ResultIndex.at(j)) > distlist.at(ResultIndex.at(j + 1)))
            {
                tempIndex = ResultIndex.at(j);
                ResultIndex.at(j) = ResultIndex.at(j + 1);
                ResultIndex.at(j + 1) = tempIndex;
            }
        }
    //printf("soft finish\n");
    //cout<<"ResultIndex size: "<<ResultIndex.size()<<endl;
    //ResultIndex.erase(ResultIndex.begin()+k,ResultIndex.end()+ResultIndex.size());
    for (i = 0; i < ResultIndex.size() - k; i++)
        ResultIndex.pop_back();
    //printf("get k min index finish\n");
    return ResultIndex;
}

int getAbsoluteFiles(string directory, vector<string> &filesAbsolutePath) //参数1[in]要变量的目录  参数2[out]存储文件名
{
    DIR *dir = opendir(directory.c_str()); //打开目录   DIR-->类似目录句柄的东西
    if (dir == NULL)
    {
        cout << directory << " is not a directory or not exist!" << endl;
        return -1;
    }

    struct dirent *d_ent = NULL; //dirent-->会存储文件的各种属性
    char fullpath[128] = {0};
    char dot[3] = "."; //linux每个下面都有一个 .  和 ..  要把这两个都去掉
    char dotdot[6] = "..";

    while ((d_ent = readdir(dir)) != NULL) //一行一行的读目录下的东西,这个东西的属性放到dirent的变量中
    {
        if ((strcmp(d_ent->d_name, dot) != 0) && (strcmp(d_ent->d_name, dotdot) != 0)) //忽略 . 和 ..
        {
            if (d_ent->d_type == DT_DIR) //d_type可以看到当前的东西的类型,DT_DIR代表当前都到的是目录,在usr/include/dirent.h中定义的
            {

                string newDirectory = directory + string("/") + string(d_ent->d_name); //d_name中存储了子目录的名字
                if (directory[directory.length() - 1] == '/')
                {
                    newDirectory = directory + string(d_ent->d_name);
                }

                if (-1 == getAbsoluteFiles(newDirectory, filesAbsolutePath)) //递归子目录
                {
                    return -1;
                }
            }
            else //如果不是目录
            {
                string absolutePath = directory + string("/") + string(d_ent->d_name); //构建绝对路径
                if (directory[directory.length() - 1] == '/')                          //如果传入的目录最后是/--> 例如a/b/  那么后面直接链接文件名
                {
                    absolutePath = directory + string(d_ent->d_name); // /a/b/1.txt
                }
                filesAbsolutePath.push_back(absolutePath);
            }
        }
    }

    closedir(dir);
    return 0;
}

int compareLabel(vector<int> x, vector<int> y)
{
    int i = 0;
    int result = 0;
    for (i = 0; i < x.size(); i++)
        if (x.at(i) == y.at(i))
            result++;
    return result;
}

void printFeature(float *feature, int size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        printf("%.10f ", *(feature + i));
        if (i % 10 == 0)
            printf("\n");
    }
    printf("\n");
}

double EuclideanDist(float *x, float *y, int lenght)
{
    int i = 0;
    double result = 0;
    for (i = 0; i < lenght; i++)
        result += pow(*(x + i) - *(y + i), 2);
    return result;
}

double ManhanttenDist(float *x, float *y, int length)
{
    int i = 0;
    double result = 0;
    for (i = 0; i < length; i++)
        result += fabs(*(x + i) - *(y + i));
    return result;
}

double KernelDist_1(float *x, float *y, float c, int length)
{
    return LinearKernel(x, x, c, length) - LinearKernel(x, y, c, length) * 2 + LinearKernel(y, y, c, length);
}

double KernelDist_2(float *x, float *y, float a, float c, int d, int length)
{
    return PolynomialKernel(x, x, a, c, d, length) - PolynomialKernel(x, y, a, c, d, length) * 2 + PolynomialKernel(y, y, a, c, d, length);
}

double KernelDist_3(float *x, float *y, float gama, int length)
{
    return RadialBasisKernel(x, x, gama, length) - RadialBasisKernel(x, y, gama, length) * 2 + RadialBasisKernel(y, y, gama, length);
}

double KernelDist_4(float *x, float *y, float gama, float r, int length)
{
    return SigmoidKernel(x, x, gama, r, length) - SigmoidKernel(x, y, gama, r, length) * 2 + SigmoidKernel(y, y, gama, r, length);
}

double LinearKernel(float *x, float *y, float c, int length)
{
    int i = 0;
    double temp = 0;
    for (i = 0; i < length; i++)
        temp += (*(x + i)) * (*(y + i));
    return temp + c;
}

double PolynomialKernel(float *x, float *y, float a, float c, int d, int length)
{
    int i = 0;
    double temp = 0;
    for (i = 0; i < length; i++)
        temp += (*(x + i)) * (*(y + i));
    temp = pow(temp * a + c, d);
    return temp;
}

double RadialBasisKernel(float *x, float *y, float gama, int length)
{
    int i = 0;
    double temp = 0;
    for (i = 0; i < length; i++)
        temp += pow(*(x + i) - *(y + i), 2);
    temp = temp * gama * (-1);
    //temp = temp * gama ;
    temp = exp(temp);
    return temp;
}

double SigmoidKernel(float *x, float *y, float gama, float r, int length)
{
    int i = 0;
    double temp;
    for (i = 0; i < length; i++)
        temp += (*(x + i)) * (*(y + i));
    temp *= gama;
    temp += r;
    temp = tanh(temp);
    return temp;
}