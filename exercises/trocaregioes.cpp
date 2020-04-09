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

  int dim_x = image.rows/2;
  int dim_y = image.cols/2;

  cout<<"dim_x = "<<dim_x<<"; dim_y = "<< dim_y<<endl; 

  //creating clone image which will become the processed image
  Mat processed_image = image.clone();
  
  Rect A_area(0, 0, dim_x, dim_y);  
  Rect B_area(dim_x,0 ,dim_x, dim_y);
  Rect C_area(0 , dim_y, dim_x, dim_y);
  Rect D_area(dim_x , dim_y, dim_x, dim_y);

  Mat A = image(A_area);
  Mat B = image(B_area);
  Mat C = image(C_area);
  Mat D = image(D_area);

  A.copyTo(processed_image(D_area));
  B.copyTo(processed_image(C_area));
  C.copyTo(processed_image(B_area));
  D.copyTo(processed_image(A_area));
  
  namedWindow("Processed Image",WINDOW_AUTOSIZE);
  imshow("Processed Image", processed_image);

  waitKey();  


  return 0;
}
