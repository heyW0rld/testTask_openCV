#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

typedef std::vector<cv::Point> line;

int main(int argc, char** argv)
{
  //загружаем исходное изображение
  cv::Mat srcImage= cv::imread(argv[1]);

  //подготавливаем изображение для нахождения контуров
  cv::Mat gray;
  cv::Mat bin;
  cv::cvtColor(srcImage, gray, cv::COLOR_BGR2GRAY);
  const double THRESHDOLD = 128.;
  const double MAXVAL = 255.;
  cv::threshold(gray, bin, THRESHDOLD, MAXVAL, cv::THRESH_BINARY);

  const int ALL_CONTOURS = -1;
  const int THICKNESS_OF_CONTOUR = 2;
  //ищем контуры
  std::vector<line> contours;
  cv::findContours( bin, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
  cv::drawContours(srcImage, contours, ALL_CONTOURS, cv::Scalar(255, 255, 0), THICKNESS_OF_CONTOUR);

  //ищем контур с максимальной длиной
  auto maxContour = *std::max_element(contours.begin(), contours.end(),[](line first, line second){
                                                                                          return first.size() < second.size();
                                                                                          });
  std::vector<line> tmp{maxContour};
  cv::drawContours(srcImage, tmp, ALL_CONTOURS, cv::Scalar(0, 0, 255), THICKNESS_OF_CONTOUR);

  //ищем экстремумы контура
  cv::Point minX, minY, maxX, maxY;
  minX = minY = maxX = maxY = maxContour[0];
  for(const auto& it : maxContour)
  {
    if(minX.x > it.x)
    {
      minX = it;
    }
    if(minY.y > it.y)
    {
      minY = it;
    }
    if(maxX.x < it.x)
    {
      maxX = it;
    }
    if(maxY.y < it.y)
    {
      maxY = it;
    }
  }
  line poly {minX, maxY, maxX, minY};
  const int RADIUS_OF_CIRCLE = 1;
  const int THICKNESS_OF_CIRCLE = 2;
  for(const auto& point : poly)
  {
    cv::circle(srcImage, point, RADIUS_OF_CIRCLE, cv::Scalar(0, 255, 0), THICKNESS_OF_CIRCLE);
  }
  cv::polylines(srcImage, poly, true, cv::Scalar(0, 255, 255));

  cv::imshow("", srcImage);
  cv::waitKey(0);

  return 0;
}