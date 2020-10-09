#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  cv::Mat image, realce;
  int width, height;
  int nobjects,nobjects_hole;
  
  cv::Point p;
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

  width=image.cols;
  height=image.rows;
  std::cout << width << "x" << height << std::endl;

  p.x=0;
  p.y=0;


  // zerar objetos tocando a borda
  int colunas_esquerda_e_direita[2] = {0, width-1};
  int linhas_topo_e_fundo[2] = {0, height-1};
 
  int n_objects_removed = 0;
  cv::imshow("original image", image);

  for(int i=0; i < 2; i++){
    for(int j=0; j < width;j++){
      if(image.at<uchar>(linhas_topo_e_fundo[i],j) == 255){
        n_objects_removed++;
        p.x= j;
        p.y= linhas_topo_e_fundo[i]; 
        // preenche o objeto com a cor do fundo
        cv::floodFill(image,p,0);
      }
    }
  }

  for(int j=0; j < 2; j++){
    for(int i=0; i < height;i++){
      if(image.at<uchar>(i,colunas_esquerda_e_direita[j]) == 255){
        n_objects_removed++;
        p.x= colunas_esquerda_e_direita[j];
        p.y= i;
        // preenche o objeto com a cor do fundo
        cv::floodFill(image,p,0);
      }
    }
  }

  // nesse ponto do processe não há mais nenhuma bolha tocando as bordas
  std::cout<<"foram removidas "<<n_objects_removed<<" bolhas"<<std::endl;
  cv::imshow("image with no bubblue touching borders", image);

  p.x=0;
  p.y=0;

  // busca objetos presentes
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        // achou um objeto
        nobjects++;
        p.x=j;
        p.y=i;
  		// preenche o objeto com o contador
		  cv::floodFill(image,p,nobjects);
      }
    }
  }

  std::cout<<"Foram encontradas "<<nobjects<<" bolhas \n";
  // pintar o fundo com 255
  p.x = 0; 
  p.y = 0;
  cv::floodFill(image,p,255);
  // nesse ponto do processo o fundo está branco
  std::cout<<"fundo branco"<<std::endl;
  cv::imshow("image with white background", image);  

  // busca objetos com furos
  nobjects_hole = 0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){
        // achou um furo
        nobjects_hole++;
        p.x= j;
        p.y= i;
  		  // preenche o objeto com o contador
		    cv::floodFill(image,p,255);
        p.y--;
        cv::floodFill(image,p,255);
      }
    }
  }

  std::cout << "a figura tem " << nobjects_hole << " bolhas com furos \n";
  std::cout << "a figura tem " << nobjects-nobjects_hole << " bolhas sem furos \n";
  cv::equalizeHist(image, realce);
  cv::imshow("image", image);
  cv::imshow("realce", realce);
  // cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}
