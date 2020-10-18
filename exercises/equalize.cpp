#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::Mat image, greyscale_image, equalized_greyscale_image;
  int width, height;
  cv::VideoCapture cap;
  int key;

	cap.open(0);
  
  if(!cap.isOpened()){
    std::cout << "cameras indisponiveis";
    return -1;
  }
  
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  std::cout << "largura = " << width << std::endl;
  std::cout << "altura  = " << height << std::endl;

  while(1){
    cap >> image;
    cv::cvtColor(image,greyscale_image,6); //turn image to grey

    // equalizing histograms:
    cv::equalizeHist(greyscale_image,equalized_greyscale_image);

    //cv::namedWindow("image");
    cv::imshow("image", image);
    //cv::namedWindow("greyscale image");    
    cv::imshow("greyscale image",greyscale_image);
    cv::imshow("equalized greyscale image",equalized_greyscale_image);
    key = cv::waitKey(30);
    if(key == 27) break;
    if(key == 32){
      //salvar imagens
      cv::imwrite("./exercises_images/greyscale_image.png",greyscale_image);
      cv::imwrite("./exercises_images/equalized_greyscale_image.png",equalized_greyscale_image);
      std::cout<<"imagens salvas!"<<std::endl;
    }
  }
  return 0;
}
