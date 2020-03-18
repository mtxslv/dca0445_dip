#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>    // std::min and std::max

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  Vec3b val;

  // checking image reading
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data){
    cout << "image could not be opened" << endl;
    return 0;
  }

  namedWindow("Original Image",WINDOW_AUTOSIZE);
  imshow("Original Image", image);
  waitKey();

  // testing only. Remove later
  cout<<"rows = "<< image.rows<<" ; \n cols = "<< image.cols <<endl;
  // testing only. Remove later

  int dim_x = image.rows/2;
  int dim_y = image.cols/2;

  cout<<"dim_x = "<<dim_x<<"; dim_y = "<< dim_y; 

  Rect A_area(0, 0, dim_x, dim_y);  
  Rect B_area(image.rows/2,0 ,dim_x, dim_y);
  Rect C_area(0, image.cols/2, dim_x, dim_y);
  Rect D_area(image.rows/2 , image.cols/2, dim_x, dim_y);

  Mat A = image(A_area);
  Mat B = image(B_area);
  Mat C = image(C_area);
  Mat D = image(D_area);

  A.copyTo(image(D_area));
  B.copyTo(image(C_area));
  C.copyTo(image(B_area));
  D.copyTo(image(A_area));  
  namedWindow("Processed Image",WINDOW_AUTOSIZE);
  imshow("Processed Image", image);

/*
  imshow("A", A);
  imshow("B", B);
  imshow("C", C);
  imshow("D", D); */

  waitKey();  


  return 0;
}
