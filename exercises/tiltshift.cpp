#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>

cv::Mat image1, image1_borrada, img_ponder, img_ponder_negativo, imagem_renderizada;

double x0 = 0; // center of the weighted filter image
int x0_slider;
const int x0_slider_max = 100;

double band = 0; // band size
int band_slider;
const int band_slider_max = 100;

double coef = 0;
int coef_slider;
const int coef_slider_max = 100;


float alpha(float domain_point,float x0, float b,float d){
  float l1 = x0 - b;
  float l2 = x0 + b;
  float y = 0.5*(tanhf((domain_point-x0-l1)/d)-tanhf((domain_point-x0-l2)/d));
  return y;
}

void montar_imagem(){
  cv::Mat canais_imagem_original[3];
  cv::split(image1.clone(),canais_imagem_original);
  /*
  imshow("1",canais_imagem_original[0]);
  imshow("2",canais_imagem_original[1]);
  imshow("3",canais_imagem_original[2]);
  */
  cv::Mat canais_imagem_blur[3];
  cv::split(image1_borrada.clone(),canais_imagem_blur);

  cv::Mat canais_produto_original[3];
  cv::Mat canal_convertido;

  for(int iterador=0;iterador<3;iterador++){
    canais_imagem_original[iterador].convertTo(canal_convertido,CV_32F);
    cv::multiply(canal_convertido,img_ponder,canais_produto_original[iterador]);
    std::cout<<"\n tipo do canal original = "<<canais_imagem_original->type();
    std::cout<<"\n tipo do img_ponder = "<<img_ponder.type();
  }

  cv::Mat poha_da_imagem_processada;
  
  cv::merge(canais_produto_original,3,poha_da_imagem_processada);
  imshow("KARALHA",poha_da_imagem_processada);

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
  img_ponder_negativo = cv::Mat::ones(rows,cols,CV_32F)- imagem_ponder;
  montar_imagem();
}

void on_x0_trackbar(int, void *){
  x0 = (double) x0_slider/x0_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tela Teste", img_ponder_negativo);
}

void on_band_trackbar(int, void *){
  band = (double) band_slider/band_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tela Teste", img_ponder_negativo);
}

void on_coef_trackbar(int, void *){
  coef = (double) coef_slider/coef_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tela Teste", img_ponder_negativo);
}

int main(){
  image1 = cv::imread("exercises_images/brazilian_beach_with_mountains.jpg");

  int width1, height1;  

  //show dimensions
  width1=image1.cols;
  height1=image1.rows;
  std::cout <<"\t width1 = "<< width1 << "\n\t height1 = " << height1 << std::endl;

  img_ponder = cv::Mat(height1,width1,CV_32F);

  // vamos borrar a imagem
  cv::GaussianBlur(image1,image1_borrada,cv::Size(5,5),1.0,0.0);

  //display images
  imshow("imagem borrada", image1_borrada);
  imshow("imagem original", image1);

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