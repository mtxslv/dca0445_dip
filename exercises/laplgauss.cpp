#include <iostream>
#include <opencv2/opencv.hpp>

void printmask(cv::Mat &m) {
  for (int i = 0; i < m.size().height; i++) {
    for (int j = 0; j < m.size().width; j++) {
      std::cout << m.at<float>(i, j) << ",";
    }
    std::cout << "\n";
  }
}

int main(int, char **) {
  cv::VideoCapture cap;  // open the default camera
  float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                   0.1111, 0.1111, 0.1111, 0.1111};
  float gauss[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                   0.125,  0.0625, 0.125,  0.0625};
  float horizontal[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  float vertical[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
  float boost[] = {0, -1, 0, -1, 5.2, -1, 0, -1, 0};
  float laplgauss[] = {0.    , -0.0625, -0.125 , -0.0625,  0. ,
                      -0.0625, -0.5   , -0.875 , -0.5   , -0.0625,
                      -0.125 , -0.875 , -1.5   , -0.875 , -0.125 ,
                      -0.0625, -0.5   , -0.875 , -0.5   , -0.0625,
                      0.    , -0.0625, -0.125 , -0.0625,  0. };

  cv::Mat frame, framegray, frame32f, frameFiltered;
  cv::Mat mask(3, 3, CV_32F);
  cv::Mat result;
  double width, height;
  int absolut;
  char key;

  cap.open(0);

  if (!cap.isOpened())  // check if we succeeded
    return -1;

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";
  ;
  std::cout << "altura =" << height << "\n";
  ;
  std::cout << "fps    =" << cap.get(cv::CAP_PROP_FPS) << "\n";
  std::cout << "format =" << cap.get(cv::CAP_PROP_FORMAT) << "\n";

  cv::namedWindow("filtroespacial", cv::WINDOW_NORMAL);
  cv::namedWindow("original", cv::WINDOW_NORMAL);

  mask = cv::Mat(3, 3, CV_32F, media);

  absolut = 1;  // calcs abs of the image

  for (;;) {
    cap >> frame;  // get a new frame from camera
    cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);
    cv::flip(framegray, framegray, 1);
    cv::imshow("original", framegray);
    framegray.convertTo(frame32f, CV_32F);
    cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                 cv::Point(1, 1), 0);
    if (absolut) {
      frameFiltered = cv::abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);

    cv::imshow("filtroespacial", result);

    key = (char)cv::waitKey(10);
    if (key == 27) break;  // esc pressed!
    if(key == 32){
      //salvar imagens
      cv::imwrite("./exercises_images/result_mask.png",result);
      std::cout<<"imagens salvas!"<<std::endl;
    }
    switch (key) {
      case 'a':
        absolut = !absolut;
        break;
      case 'm':
        mask = cv::Mat(3, 3, CV_32F, media);
        std::cout<<"filtro da media:"<<std::endl;
        printmask(mask);
        break;
      case 'g':
        mask = cv::Mat(3, 3, CV_32F, gauss);
        std::cout<<"filtro gaussiano:"<<std::endl;
        printmask(mask);
        break;
      case 'h':
        mask = cv::Mat(3, 3, CV_32F, horizontal);
        std::cout<<"filtro sobel horizontal:"<<std::endl;
        printmask(mask);
        break;
      case 'v':
        mask = cv::Mat(3, 3, CV_32F, vertical);
        std::cout<<"filtro sobel vertical:"<<std::endl;
        printmask(mask);
        break;
      case 'l':
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        std::cout<<"filtro laplaciano:"<<std::endl;
        printmask(mask);
        cv::imwrite("./exercises_images/result_laplacian.png",result);
        break;
      case 'b':
        mask = cv::Mat(3, 3, CV_32F, boost);
        std::cout<<"filtro boost:"<<std::endl;
        break;
      case 'k':
        mask = cv::Mat(5,5,CV_32F, laplgauss);
        std::cout<<"filtro laplgauss via convolucao:"<<std::endl;
        printmask(mask);
        break; 
      case 'p':
        mask = cv::Mat(3,3,CV_32F,gauss);
        std::cout<<"\t primeiro se aplica gauss:"<<std::endl;
        printmask(mask);
        cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                     cv::Point(1, 1), 0);
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        std::cout<<"\t e agora se aplica laplace:"<<std::endl;
        printmask(mask);
        cv::imwrite("./exercises_images/result_laplgauss.png",result);
        break;             
      default:
        break;
    }
  }
  return 0;
}
