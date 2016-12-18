#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


void Sharpen(const Mat& myImage,Mat& Result);

int main( int argc, char* argv[])
{
    const char* filename = argc >=2 ? argv[1] : "/home/jyldyz/Pictures/cat.jpeg";

    Mat A, B, K;

    if (argc >= 3 && !strcmp("G", argv[2]))
        A = imread( filename, CV_LOAD_IMAGE_GRAYSCALE);
    else
        A = imread( filename, CV_LOAD_IMAGE_COLOR);

    namedWindow("Ishodnik", WINDOW_AUTOSIZE);
    namedWindow("Rezultat", WINDOW_AUTOSIZE);

    imshow("Ishodnik", A);
    double t = (double)getTickCount();

    Sharpen(A, B);

    t = ((double)getTickCount() - t)/getTickFrequency();

    imshow("Rezultat", B);
    waitKey(0);

    Mat kern = (Mat_<char>(3,3) <<  1,  0,  3,
                                    0, -4, 0,
                                    1,  0,  1);
    t = (double)getTickCount();
    filter2D(A, K, A.depth(), kern );
    t = ((double)getTickCount() - t)/getTickFrequency();

    imshow("Rezultat", K);

    waitKey(0);
    return 0;
}
void Sharpen(const Mat& myImage,Mat& Result) {
    CV_Assert(myImage.depth() == CV_8U);

    const int nChannels = myImage.channels();
    Result.create(myImage.size(), myImage.type());

    for (int j = 1; j < myImage.rows - 1; ++j) {
        const uchar *previous = myImage.ptr<uchar>(j - 1);
        const uchar *current = myImage.ptr<uchar>(j);
        const uchar *next = myImage.ptr<uchar>(j + 1);

        uchar *output = Result.ptr<uchar>(j);

        for (int i = nChannels; i < nChannels * (myImage.cols - 1); ++i) {
            *output++ = saturate_cast<uchar>(5 * current[i] - current[i - nChannels] - current[i + nChannels] - previous[i] - next[i]);
        }
    }

    Result.row(0).setTo(Scalar(0));
    Result.row(Result.rows - 1).setTo(Scalar(0));
    Result.col(0).setTo(Scalar(0));
    Result.col(Result.cols - 1).setTo(Scalar(0));
}
