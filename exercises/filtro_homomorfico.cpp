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
  cv::Mat original_image, log_original_image, padded_image, complexImage, filter, tmp, H_pra_visualizacao;
  cv::Mat_<float> float_padded_image, zeros, realInput, H,tmp_exp , g, log_visualiza;
  std::vector<cv::Mat> planos;
  int dft_M, dft_N;
  float D0=80.0, C=1.0, gamma_h = 2.0, gamma_l=0.250;

  original_image = cv::imread(argv[1],cv::IMREAD_GRAYSCALE); 
  if(original_image.empty()){
    std::cout<<"no valid image found"<<std::endl;
    return -1;
  }
  cv::imshow("original_image", original_image);
  cv::waitKey();

  // ajeitar o tamanho da imagem original
    // identifica os tamanhos otimos para
    // calculo do FFT
  dft_M = cv::getOptimalDFTSize(original_image.rows);
  dft_N = cv::getOptimalDFTSize(original_image.cols);

    // realiza o padding da imagem
  cv::copyMakeBorder(original_image, padded_image, 0, dft_M - original_image.rows, 0,
                     dft_N - original_image.cols, cv::BORDER_CONSTANT,
                     cv::Scalar::all(1));

  cv::imshow("padded_image", padded_image);
  cv::waitKey();

  // tirar o log
  padded_image.convertTo(float_padded_image,CV_32F); // se não converter, o log não funciona
  log_original_image = cv::Mat(padded_image.size(),CV_32F);
  cv::log(float_padded_image,log_original_image);

  cv::normalize(log_original_image, log_visualiza, 0, 1, cv::NORM_MINMAX);
  cv::imshow("LOG", log_visualiza);
  cv::waitKey();

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = cv::Mat_<float>::zeros(float_padded_image.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = cv::Mat(float_padded_image.size(), CV_32FC2, cv::Scalar(0));

  // cria a compoente real
  realInput = cv::Mat_<float>(float_padded_image);
  // insere as duas componentes no array de matrizes
  planos.push_back(realInput);
  planos.push_back(zeros);

  // combina o array de matrizes em uma unica
  // componente complexa
  cv::merge(planos, complexImage);

  // calcula o dft
  cv::dft(complexImage, complexImage);
  // realiza a troca de quadrantes
  deslocaDFT(complexImage);

  // NO ARQUIVO dft.cpp, DA LINHA 142 A 164 É SÓ SOBRE FILTRO

  // a função de transferencia (filtro de frequencia) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = cv::Mat(dft_M, dft_N, CV_32F);
  H = cv::Mat_<float>(dft_M,dft_N,CV_32F);
  filter = cv::Mat_<float>(dft_M,dft_N,CV_32F);

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
  cv::Mat comps[] = {H, H};
  cv::merge(comps, 2, filter);

  cv::normalize(H, H_pra_visualizacao, 0, 255, cv::NORM_MINMAX,CV_8UC1);
  cv::imshow("H",H_pra_visualizacao);
  cv::waitKey();

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

  std::cout<<planos[0].type()<<std::endl;
  cv::exp(planos[0],g);
  std::cout<<g.type()<<std::endl;
//  std::cout<<planos[1];
  // normaliza a parte real para exibicao
  cv::normalize(g, g, 0, 1, cv::NORM_MINMAX);
  cv::normalize(planos[0], planos[0], 0, 1, cv::NORM_MINMAX);
  cv::imshow("g",g);
  cv::imshow("filtrada",planos[0]);  
  cv::waitKey();
  
  return 0; 
}
