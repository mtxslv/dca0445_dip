#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>

cv::Mat image1, image2, img_ponder;
int x0 = 0; // center of the weighted filter image
int band = 0; // band size
int l1=-20,l2=30;
float d=6;
int altura=0;

float alpha(float domain_point,float x0, float b,float d){
  float l1 = x0 - b;
  float l2 = x0 + b;
  float y = 0.5*(tanhf((domain_point-x0-l1)/d)-tanhf((domain_point-x0-l2)/d));
  return y;
}

void gerar_imagem_ponderacao(cv::Mat &imagem_ponder){
  int rows, cols;
  cols = imagem_ponder.cols;
  rows = imagem_ponder.rows;
  for(int i=0; i<rows; i++){
    for(int j=0; j<cols; j++){
      imagem_ponder.at<float>(i,j) = alpha(i,120,30.0,d);
    }
  }
 std::cout<<"\n passei por aki \n";
}

int main(){
  image1 = cv::imread("example_images/blend1.jpg");
  image2 = cv::imread("example_images/blend2.jpg");

  int width1, height1, width2, height2;  

  //show dimensions
  width1=image1.cols;
  height1=image1.rows;
  std::cout <<"\t width1 = "<< width1 << "\n\t height1 = " << height1 << std::endl;

  width2=image2.cols;
  height2=image2.rows;
  std::cout <<"\t width2 = "<< width2 << "\n\t height2 = " << height2 << std::endl;

  float alphinha = alpha(5.5,-20,30,6);

  std::cout<<"valor de alpha = "<<alphinha<<std::endl;

  img_ponder = cv::Mat(width1,height1,CV_32F);
  gerar_imagem_ponderacao(img_ponder);
  cv::imshow("Imagem ponderacao",img_ponder);
  cv::waitKey();
  return 0;
}