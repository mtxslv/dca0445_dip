#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  cv::Mat image, realce;
  int width, height;
  // int nobjects; using an int datatype, we can only count up to 254 objects 
  float counting_constant= -1; // arbitrary initialization 
  float floating_counting = 0; // floating_counting have to be initialized to 0.
  /*
   The logic follows from the idea:
    floating_counting = floating_constant+floating_costant+...
    floating_counting = nobjects*floating_constant
    => nobjects = floating_counting/floating_constant
   If counting_constant = 0.5, the code can counts up to 2*254= 508 objects.
  */
 
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

  do{
    std::cout<<"please insert counting constant: \n";
    std::cin>>counting_constant;
  }while((counting_constant<0)||(counting_constant>1));

  // busca objetos presentes
  // nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        // achou um objeto
        floating_counting = floating_counting + counting_constant;
        p.x=j;
        p.y=i;
  		// preenche o objeto com o contador
		  cv::floodFill(image,p,floating_counting);
      }
    }
  }
  std::cout << "a figura tem " << floating_counting/counting_constant << " bolhas\n";
  
  //cv::equalizeHist(image, realce);
  cv::imshow("image", image);
  //cv::imshow("realce", realce);
  //cv::imwrite("labeling.png", image);
  cv::waitKey();
  
  return 0;
}
