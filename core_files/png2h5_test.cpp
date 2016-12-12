// the file is used to augment data for hand detection network
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "stdlib.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/objdetect.hpp"
#include <sys/stat.h>
#include "H5Cpp.h"
#include <cstddef> 


#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

using namespace std;
using namespace cv;

const H5std_string  DATASET_NAME( "data" );
const H5std_string  LABEL_NAME( "label" );
const H5std_string  WEIGHT_NAME( "mask" );

//convert int to string part

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
//end

//step 1 brightness
float im_save[3][380][1030];
float label_save[380][1030];
float weight_save[380][1030];

int main(int argc, char **argv){
    string image_fd = "../test_images/RGBimages";
    //string image_fd = "/home/yuboz/data/baseline/"+video_name_str+"/RGBimages";
    //mkdir(image_fd.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    string fd = image_fd+"/";

    string save_fd = "../test_images/H5_file";
    //string save_fd = "/home/yuboz/data/baseline/"+video_name_str+"/H5_file";
    mkdir(save_fd.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    string save = save_fd+"/";

    string gt_fd1  = "../test_images/Masks";
    string gt_fd = gt_fd1+"/";
    
    //cout<<"1"<<endl;
    Mat im, im_gt,im_gt1;
    string im_path,im_gt_path,im_out;
    
    FloatType datatype(PredType::IEEE_F32BE);
    hsize_t  dimsf[4];    
    hsize_t dimsf_label[4];
    int rank_data = 4;
    int rank_label = 4;
    H5File file;
    dimsf_label[0] = 1;
    dimsf_label[1] = 1;
    dimsf_label[2] = 380;
    dimsf_label[3] = 1030;
    dimsf[0] = 1;
    dimsf[1] = 3;
    dimsf[2] = 380;
    dimsf[3] = 1030;
    //DataSpace dataspace1( rank_label, dimsf_label);
    DataSpace dataspace( rank_data, dimsf);
    DataSet dataset,dataset1;
    int num = 1;
    int index = 1;
    while(num<10000){
    	im_path = fd+patch::to_string(num)+".png";
    	im = imread(im_path,CV_LOAD_IMAGE_COLOR);
    	if (im.data==NULL){
          cout<<"no data~!"<<endl;
          break;
        }
    	im_out = save+patch::to_string(index)+".h5";
        //cout<<im_out<<endl;
/*
        for (int i=0;i<380;i++){
            for(int j = 0;j<1030;j++){
               cout<<im_gt.at<uchar>(i,j)<<endl;
               label_save[i][j]=(float)(im_gt.at<uchar>(i,j));
              //cout<< label_save<<endl;
            }
        }
*/
        //cout<<"3"<<endl;
        for (int i=0;i<380;i++){
            for(int j = 0;j<1030;j++){
              im_save[0][i][j]=im.at<Vec3b>(i,j)[0];
              im_save[1][i][j]=im.at<Vec3b>(i,j)[1];
              im_save[2][i][j]=im.at<Vec3b>(i,j)[2];
            }
          } 
        H5File file(im_out,H5F_ACC_TRUNC);
        dataset= file.createDataSet( DATASET_NAME, datatype, dataspace );
        //dataset1 = file.createDataSet( LABEL_NAME, datatype, dataspace1 );
        //dataset1.write(label_save, PredType::NATIVE_FLOAT);
        dataset.write(im_save, PredType::NATIVE_FLOAT);
        num+=10;
        cout<<num<<endl;
        file.close();
        index+=1;
    }

    return 0;
}
