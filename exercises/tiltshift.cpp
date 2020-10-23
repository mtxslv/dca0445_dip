#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>

cv::Mat image1, image2, img_ponder;

double x0 = 0; // center of the weighted filter image
int x0_slider;
const int x0_slider_max = 100;

double band = 0; // band size
int band_slider;
const int band_slider_max = 100;

double coef = 0;
int coef_slider;
const int coef_slider_max = 100;

//int l1=-20,l2=30;
//float d=6;
//int altura=0;

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
      imagem_ponder.at<float>(i,j) = alpha(i,x0*rows/2, band*rows/2,coef*50+1);
    }
  }
 //std::cout<<"\n passei por aki \n";
}


void on_x0_trackbar(int, void *){
  x0 = (double) x0_slider/x0_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tela Teste", img_ponder);
}

void on_band_trackbar(int, void *){
  band = (double) band_slider/band_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tela Teste", img_ponder);
}

void on_coef_trackbar(int, void *){
  coef = (double) coef_slider/coef_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tela Teste", img_ponder);
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

  /*
  gerar_imagem_ponderacao(img_ponder);
  cv::imshow("Imagem ponderacao",img_ponder);
  */
  x0 = 0;
  
  // a partir daqui cria-se a janela
  cv::namedWindow("Tela Teste");

  char TrackbarName[50];
  
  std::sprintf(TrackbarName, "x0 x %d :", x0_slider_max);
  cv::createTrackbar(TrackbarName,"Tela Teste", &x0_slider, x0_slider_max, on_x0_trackbar);
  on_x0_trackbar(x0_slider,0);

  std::sprintf(TrackbarName, "band x %d :", band_slider_max);
  cv::createTrackbar(TrackbarName,"Tela Teste", &band_slider, band_slider_max, on_band_trackbar);
  on_band_trackbar(band_slider,0);  

  std::sprintf(TrackbarName, "coef x %d :", coef_slider_max);
  cv::createTrackbar(TrackbarName,"Tela Teste", &coef_slider, coef_slider_max, on_coef_trackbar);
  on_coef_trackbar(coef_slider,0); 

  cv::waitKey(0);

  return 0;
}