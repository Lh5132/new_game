#include "opencv2/opencv.hpp"  
#include "opencv2/highgui.hpp"
#include<iostream>  
#include <fstream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

RNG g_rng(12345);

Mat src;  //ԭͼ
Mat hui;   //�Ҷ�ͼ

Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));	//���͸�ʴ�����
Mat out;   //���


int main(int argc, char** argv) {
	ofstream write;
	double scal = 0.6;
	Mat show_res, show_src,show_hui;
	Size outsize;
	string shape;
	src = imread("D:\\OpenCV-test\\cvtest\\test.tif");


	//ת���Ҷ�ͼ
	cvtColor(src, hui, CV_RGB2GRAY);
	/*
	//��˹ģ��
	GaussianBlur(hui, hui, Size(3, 3), 3, 3);
	//��ʴ
	erode(hui, hui, element, Point(-1, -1), 1);
	//����
	dilate(hui, hui, element, Point(-1, -1), 1);
	*/
	vector<vector<Point>>cont;
	//��ֵ�� 
	threshold(hui, hui, 100, 255, THRESH_OTSU);
	//���͸�ʴ����
	GaussianBlur(hui, hui, Size(3, 3), 3, 3);
	dilate(hui, hui, element, Point(-1, -1), 1);
	erode(hui, hui, element, Point(-1, -1), 1);
	//��������
	findContours(hui, cont, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

	Mat ImgColor;
	cvtColor(hui,ImgColor, CV_GRAY2BGR, 0);

	Mat res;
	cvtColor(hui, res, CV_GRAY2BGR, 0);

	write.open("D:\\OpenCV-test\\cvtest\\text.txt");
	write << "覴ñ��\t�ܳ�\t���\t����\t������Ȼ�Բ��ֱ��\t���ĵ�����x\t���ĵ�����y\n";

	for (int i = 0; i < cont.size(); i++) {
		double area = abs(contourArea(cont[i], false));		//��������������ܳ�
		double lenth = arcLength(cont[i], true);
		double r;
		int x, y;
		if (area*lenth == 0) {
			continue;
		}																									//������״�жϲ��ò�ͬ��ɫ���
		else {
			RotatedRect rect = minAreaRect(cont[i]);
			Point2f P[4],C;
			rect.points(P);
			C = (P[0] + P[1] + P[2] + P[3]) / 4;
			x = C.x;
			y = C.y;
			double M = pow(lenth, 2) / area / 4 / 3.1415;
			if (M < 1.2) {
				shape = "Բ��";
				Scalar color = Scalar(255, 0, 0);  
				drawContours(res, cont, i,color, -1, 8);
				for (int j = 0; j <= 3; j++)
				{
					line(ImgColor, P[j], P[(j + 1) % 4], Scalar(255, 0, 0), 2);
				}
				
				r = 2 * area / lenth;
			}
			else if (M < 1.5) {
				shape = "����";
				Scalar color = Scalar(0, 255, 0);
				drawContours(res, cont, i, color, -1, 8);
				r = 0;
				for (int j = 0; j <= 3; j++)
				{
					line(ImgColor, P[j], P[(j + 1) % 4], Scalar(0, 255, 0), 2);
				}
			}
			else {
				shape = "������";
				Scalar color = Scalar(0, 0, 255);
				drawContours(res, cont, i, color, -1, 8);
				r = rect.size.width < rect.size.height ? rect.size.width : rect.size.height;
				for (int j = 0; j <= 3; j++)
				{
					line(ImgColor, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}
			}
		}

		

		write << i + 1 << "\t";
		write << setiosflags(ios::fixed) << setprecision(2);
		write << lenth << "\t" << area << "\t" << shape << "\t" << r << "\t" << x << "\t" <<y << "\n";

	}
	write.close();

	outsize.width = src.cols*scal;
	outsize.height = src.rows*scal;


	namedWindow("ԭͼ");
	namedWindow("��ֵ��");
	namedWindow("��������ͼ");

	resize(src, show_src, outsize);
	imshow("ԭͼ", show_src);

	resize(ImgColor, show_hui, outsize);
	imshow("��ֵ��", show_hui);

	resize(res, show_res, outsize);
	imshow("��������ͼ", show_res);

	waitKey(0);
	return 0;
}
