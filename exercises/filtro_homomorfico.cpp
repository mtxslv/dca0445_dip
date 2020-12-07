#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

// troca os quadrantes da imagem da DFT
void deslocaDFT(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(cv::Rect(0, 0, cx, cy));
  B = image(cv::Rect(cx, 0, cx, cy));
  C = image(cv::Rect(0, cy, cx, cy));
  D = image(cv::Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

int main(int argc, char** argv){
  cv::Mat original_image, log_original_image, g;
  cv::Mat_<float> original_float_image,zeros, realInput, H,tmp,tmp_exp;
  cv::Mat padded, complexImage, magI, filter;
  std::vector<cv::Mat> planos;
  int dft_M, dft_N;
  float D0=1, C=1, gamma_h = 10, gamma_l=1;

  original_image = cv::imread(argv[1],cv::IMREAD_GRAYSCALE); // obtendo a imagem
  original_image.convertTo(original_float_image,CV_32F); // convertendo e realocando para um tipo float de 32 bits
  cv::log(original_float_image,log_original_image); // logaritmo da imagem original

  dft_M = cv::getOptimalDFTSize(log_original_image.rows);
  dft_N = cv::getOptimalDFTSize(log_original_image.cols);

  std::cout<<"\ndft_M = "<<dft_M<<"\ndft_N = "<<dft_N<<std::endl;

  cv::copyMakeBorder(log_original_image, padded, 0,
               dft_M - log_original_image.rows, 0,
               dft_N - log_original_image.cols,
               cv::BORDER_CONSTANT, cv::Scalar::all(0));
               
  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = cv::Mat_<float>::zeros(padded.size());
  
  // limpa o array de matrizes que vao compor a
  // imagem complexa
  planos.clear();
  // cria a compoente real
  realInput = cv::Mat_<float>(padded);
  // insere as duas componentes no array de matrizes
  planos.push_back(realInput);
  planos.push_back(zeros);

  // prepara a matriz complexa para ser preenchida
  complexImage = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));

  // combina o array de matrizes em uma unica
  // componente complexa
  cv::merge(planos, complexImage);

  // calcula o dft
  cv::dft(complexImage, complexImage);

  // realiza a troca de quadrantes
  deslocaDFT(complexImage);


  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = cv::Mat(dft_M, dft_N, CV_32F);
  H = cv::Mat(dft_M,dft_N,CV_32F);

  // prepara um pedaço de filtro: -C(D²(u,v)/D0²)
  for (int i = 0; i < dft_M; i++) {
    for (int j = 0; j < dft_N; j++) {
      tmp.at<float>(i, j) = -C*((i - dft_M / 2) * (i - dft_M / 2) + (j - dft_N / 2) * (j - dft_N / 2))/(D0*D0);
    }
  }

  cv::exp(tmp,tmp_exp);
  H = (gamma_h-gamma_l)*(cv::Mat::ones(H.size(),CV_32F)-tmp_exp)+gamma_l; //eis a função de transferência H

  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  cv::Mat comps[]= {H, H};
  filter = complexImage.clone();
  cv::merge(comps, 2, filter);

  // aplica o filtro de frequencia
  cv::mulSpectrums(complexImage, filter, complexImage, 0);

    // troca novamente os quadrantes
  deslocaDFT(complexImage);

  // calcula a DFT inversa
  cv::idft(complexImage, complexImage);

  // limpa o array de planos
  planos.clear();

  // separa as partes real e imaginaria da
  // imagem filtrada
  cv::split(complexImage, planos);



  // normaliza a parte real para exibicao
  cv::normalize(planos[0], planos[0], 0, 1, cv::NORM_MINMAX);
  cv::imshow("filtrada", planos[0]);  


  /*
  // não sei se mostro
  planos.clear();
  cv::split(complexImage, planos);

  cv::Mat magn, angl, anglInt, magnInt;
  cv::cartToPolar(planos[0], planos[1], magn, angl, false);
  cv::normalize(angl, angl, 0, 255, cv::NORM_MINMAX);
  angl.convertTo(anglInt, CV_8U);
  cv::imshow("Angulo de Fase", anglInt);

  cv::magnitude(planos[0], planos[1], planos[0]);
  magI = planos[0];
  magI += cv::Scalar::all(1);
  cv::log(magI, magI);
  cv::normalize(magI, magI, 0, 255, cv::NORM_MINMAX);
  magI.convertTo(magnInt, CV_8U);
  cv::imshow("Espectro", magnInt);
  */

 // cv::imshow("image:",original_image); //exibição
 // cv::imshow("log image:",log_original_image); //exibição
  cv::waitKey();
  return 0; 
}


/*
Mat_<float> fm;

m.convertTo(fm,CV_32F);

*/