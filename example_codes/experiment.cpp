#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  Vec3b val;

  cout<< "Have " << argc << " arguments: \n";
  for (int i =0; i< argc; i++){
      cout<< argv[i] << " , i = "<< i <<endl;
  }

  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE); // save the image
  if(!image.data)
    cout << "nao abriu bolhas.png" << endl;

  imshow("image", image);
  waitKey();
  return 0;  

}
