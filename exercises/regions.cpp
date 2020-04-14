#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>    // std::min and std::max

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  Vec3b val;

  int pts[2][2] = {{-1,-1},{-1,-1}}; // vertices positions

  // checking image reading
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "image could not be opened" << endl;

  //inputing  vertices and checking if they don't match image size
  do{
    cout<< "Please input the upper vertex: \n";
    cin>> pts[0][0] >> pts[0][1];  
  }while(pts[0][0] < 0 || pts[0][0] > image.cols || pts[0][1] < 0 || pts[0][1] > image.rows );

  do{
    cout<< "Please input the lower vertex: \n";
    cin>> pts[1][0] >> pts[1][1]; 
  }while(pts[1][0] < 0 || pts[1][0] > image.cols || pts[1][1] < 0 || pts[1][1] > image.rows );


  // testing only. Remove later
  cout<<"rows = "<< image.rows<<" ; \n cols = "<< image.cols <<endl;
  // testing only. Remove later


  namedWindow("Original Image",WINDOW_AUTOSIZE);
  imshow("Original Image", image);
  waitKey();

 
  for(int i = std::min(pts[0][0],pts[1][0]); i < std::max(pts[0][0],pts[1][0]); i++){
    for(int j = std::min(pts[0][1],pts[1][1]) ; j < std::max(pts[0][1],pts[1][1]); j++){
      image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
    }
  }
  
  imshow("Processed Image", image); 

  waitKey();

  return 0;
}