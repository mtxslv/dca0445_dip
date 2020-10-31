#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include<cmath>

cv::Mat image1, image1_borrada, img_ponder, img_ponder_negativo, imagem_renderizada;

double x0 = 0; // center of the weighted filter image
int x0_slider;
const int x0_slider_max = 100;

double band = 0; // band size
int band_slider;
const int band_slider_max = 100;

double coef = 0; // d parameter
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

  cv::Mat canais_imagem_blur[3];
  cv::split(image1_borrada.clone(),canais_imagem_blur);

  cv::Mat canais_produto_original[3];
  cv::Mat canais_produto_blur[3];

  cv::Mat canal_convertido_original, produto_nao_convertido_original;
  cv::Mat canal_convertido_blur, produto_nao_convertido_blur;

  for(int iterador=0;iterador<3;iterador++){
    canais_imagem_original[iterador].convertTo(canal_convertido_original,CV_32F);
    cv::multiply(canal_convertido_original,img_ponder,produto_nao_convertido_original);
    produto_nao_convertido_original.convertTo(canais_produto_original[iterador],CV_8UC3);

    canais_imagem_blur[iterador].convertTo(canal_convertido_blur,CV_32F);
    cv::multiply(canal_convertido_blur,img_ponder_negativo,produto_nao_convertido_blur);
    produto_nao_convertido_blur.convertTo(canais_produto_blur[iterador],CV_8UC3);
  }

  cv::Mat a_imagem_processada;
  cv::Mat a_imagem_blur;
  
  cv::merge(canais_produto_original,3,a_imagem_processada);
  cv::merge(canais_produto_blur,3,a_imagem_blur);
  imagem_renderizada = a_imagem_blur + a_imagem_processada;
  
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

  imshow("Weight Matrix",imagem_ponder);
  imshow("Negative Weight Matrix", img_ponder_negativo);

  montar_imagem();
}

void on_x0_trackbar(int, void *){
  x0 = (double) x0_slider/x0_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tiltshift Screen", imagem_renderizada);
}

void on_band_trackbar(int, void *){
  band = (double) band_slider/band_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tiltshift Screen", imagem_renderizada);
}

void on_coef_trackbar(int, void *){
  coef = (double) coef_slider/coef_slider_max;
  gerar_imagem_ponderacao(img_ponder);
  imshow("Tiltshift Screen", imagem_renderizada);
}


int main(int argvc, char** argv){
            
    cv::Mat frame;
    int frame_counter = 0;

    cv::VideoCapture cap("exercises_images/Ocean - 7518.mp4");
    std::cout<<"\n existem "<<cap.get(cv::CAP_PROP_FRAME_COUNT)<<" frames no video \n";
    if (!cap.isOpened()){
        std::cout<<"Error opening video stream \n";
        return -1;
    }

    cap >> image1;

    int width1, height1;


    //show dimensions
    width1  = image1.cols;
    height1 = image1.rows;

    img_ponder = cv::Mat(height1,width1,CV_32F);

    // vamos borrar a imagem
    cv::GaussianBlur(image1,image1_borrada,cv::Size(101,101),1.0,0.0);

    // a partir daqui cria-se a janela
    cv::namedWindow("Tiltshift Screen");

    char TrackbarName[50];

    std::sprintf(TrackbarName, "center x %d :", x0_slider_max);
    cv::createTrackbar(TrackbarName,"Tiltshift Screen", &x0_slider, x0_slider_max, on_x0_trackbar);
    on_x0_trackbar(x0_slider,0);

    std::sprintf(TrackbarName, "band x %d :", band_slider_max);
    cv::createTrackbar(TrackbarName,"Tiltshift Screen", &band_slider, band_slider_max, on_band_trackbar);
    on_band_trackbar(band_slider,0);  

    std::sprintf(TrackbarName, "coef x %d :", coef_slider_max);
    cv::createTrackbar(TrackbarName,"Tiltshift Screen", &coef_slider, coef_slider_max, on_coef_trackbar);
    on_coef_trackbar(coef_slider,0);    


    char key; 
    key = (char)cv::waitKey(0); 
    cv::destroyAllWindows();

    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
	  cv::VideoWriter video("./exercises_images/outcpp.avi",CV_FOURCC('M','J','P','G'),10, cv::Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT))); 
	

    while(1){
        frame_counter = frame_counter + 1;

        std::cout<<"frame_counter = "<<frame_counter<<std::endl;

        cap >> frame;
        cap >> image1;

        if(frame.empty()){
            std::cout<<"frame empty"<<std::endl;
            break;
        }    
        
        cv::GaussianBlur(image1,image1_borrada,cv::Size(101,101),1.0,0.0);
        gerar_imagem_ponderacao(img_ponder);


        if (frame_counter%4==0){
            cv::imshow("Frame",imagem_renderizada);

            // Write the frame into the file 'outcpp.avi'
            video.write(frame);
        }    
        char c = (char)cv::waitKey(25);
        if(c==27)
            break;    
    }
    std::cout<<"Video Processing Finished!"<<std::endl;

    cap.release();
    video.release();

    cv::destroyAllWindows();
  
  return 0;
}
