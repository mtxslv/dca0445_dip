#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char** argv){
  cv::Mat original_image, log_original_image;
  cv::Mat_<float> original_float_image;

  original_image = cv::imread(argv[1],cv::IMREAD_GRAYSCALE); // obtendo a imagem
  original_image.convertTo(original_float_image,CV_32F); // convertendo e realocando para um tipo float de 32 bits
  cv::log(original_float_image,log_original_image); // logaritmo da imagem original
  cv::imshow("image:",original_image); //exibição
  cv::imshow("log image:",log_original_image); //exibição
  cv::waitKey();
  return 0; 
}


/*
Mat_<float> fm;

m.convertTo(fm,CV_32F);

*/