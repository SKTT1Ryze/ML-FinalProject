#pragma warning(disable: 4819)

#include <seeta/FaceEngine.h>
#include <seeta/Struct_cv.h>
#include <seeta/Struct.h>
#include <seeta/QualityAssessor.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <array>
#include <map>
#include <iostream>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
int main()
{
    seeta::ModelSetting::Device device = seeta::ModelSetting::CPU;
    int id = 0;
    seeta::ModelSetting FD_model( "./model/fd_2_00.dat", device, id );
    seeta::ModelSetting PD_model( "./model/pd_2_00_pts5.dat", device, id );
    seeta::ModelSetting FR_model( "./model/fr_2_10.dat", device, id );
    seeta::FaceEngine engine( FD_model, PD_model, FR_model, 2, 16 );

    seeta::QualityAssessor QA;

    // recognization threshold
    float threshold = 0.7f;

    //set face detector's min face size
    engine.FD.set( seeta::FaceDetector::PROPERTY_MIN_FACE_SIZE, 80 );

    //Pitures store path
    string PitureStorePath="/home/hustccc/Pictures/FaceImage/Train_01/";
    std::vector<std::string> GalleryImageFilename = {"chechunchi_01_00.jpg","chechunchi_02_00.jpg","chechunchi_03_00.jpg","chechunchi_04_00.jpg"};
    GalleryImageFilename.push_back("chechunchi_05_00.jpg");
    GalleryImageFilename.push_back("Faker2_02.jpg");
    GalleryImageFilename.push_back("theshy3_03.jpg");
    GalleryImageFilename.push_back("7772_04.jpg");
    GalleryImageFilename.push_back("Faker1_02.jpg");
    GalleryImageFilename.push_back("Messi_10_01.jpg");
    GalleryImageFilename.push_back("luweiqin1_10.jpg");
    std::vector<int64_t> GalleryIndex( GalleryImageFilename.size() );
    for( size_t i = 0; i < GalleryImageFilename.size(); ++i )
    {
        //register face into facedatabase
        std::string &filename =GalleryImageFilename[i];
        int64_t &index = GalleryIndex[i];
		std::cerr << "Registering... " << filename << std::endl;
        seeta::cv::ImageData image = cv::imread( PitureStorePath+ filename );
        auto id = engine.Register( image );
        index = id;
        std::cerr << "Registered id = " << id << std::endl;
    }
    std::map<int64_t, std::string> GalleryIndexMap;
    for( size_t i = 0; i < GalleryIndex.size(); ++i )
    {
        // save index and name pair
        if( GalleryIndex[i] < 0 ) continue;
        GalleryIndexMap.insert( std::make_pair( GalleryIndex[i], GalleryImageFilename[i] ) );
    }
    
    std::cout << "----open camera----" << std::endl;
    // Open default USB camera
    cv::VideoCapture capture;
    capture.open( 0 );

    cv::Mat frame;

    while( capture.isOpened() )
    {
        capture >> frame;
        if( frame.empty() ) continue;

        seeta::cv::ImageData image = frame;

        // Detect all faces
        std::vector<SeetaFaceInfo> faces = engine.DetectFaces( image );

        for( SeetaFaceInfo &face : faces )
        {
            // Query top 1
            int64_t index = -1;
            float similarity = 0;

			auto points = engine.DetectPoints(image, face);

            cv::rectangle( frame, cv::Rect( face.pos.x, face.pos.y, face.pos.width, face.pos.height ), CV_RGB( 128, 128, 255 ), 3 );
			for (int i = 0; i < 5; ++i)
			{
				auto &point = points[i];
				cv::circle(frame, cv::Point(int(point.x), int(point.y)), 2, CV_RGB(128, 255, 128), -1);
			}

			std::string name;

            auto score = QA.evaluate(image, face.pos, points.data());
            if (score == 0) {
                name = "ignored";
            } else {
                auto queried = engine.QueryTop( image, points.data(), 1, &index, &similarity );
		        //printf("similarity: %f\n",similarity);
		        //printf("threshold: %f\n",threshold);
                // no face queried from database
                if (queried < 1) continue;
                // similarity greater than threshold, means recognized
                if( similarity > threshold )
                {
	  	            //std::cout << "recognized ID: "<<"["<<index<<"]" << std::endl;
                    name = GalleryIndexMap[index];
                }
            }

            if( !name.empty() )
            {
                cv::putText( frame, name, cv::Point( face.pos.x, face.pos.y - 5 ), 3, 1, CV_RGB( 255, 128, 128 ) );
            }
        }

        cv::imshow( "Frame", frame );

        auto key = cv::waitKey( 20 );
        if( key == 27 )
        {
            break;
        }
    }
}
