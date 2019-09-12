#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <highgui.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define height		256
#define width		256
#define height2		512
#define width2		512

using namespace cv;
typedef unsigned char  BYTE;

int truncate(int value)				//修正overflow，因char 範圍是-128~127，unsigned char範圍為0~255
														//超過255的話會超過8bit，ex. 256=>100000000(9bit)，溢位會捨去最左邊的bit
														//所以會變成00000000(8bit) =>0。
{
	if (value > 255)		return 255;
	if (value < 0)			return 0;

	return value;
}

int main()
{
	FILE* fp = NULL;
	FILE* fp2 = NULL;

	BYTE B[height][width];
	BYTE B2[height][width];
	BYTE B3[height][width];
	BYTE B4[height][width];
	BYTE B5[height2][width2];
	BYTE *ptr,*ptr2;

	unsigned char path1[65536];

	int i, j,fr,fc,pe;

	//讀檔
	//printf("Input the raw image path:");
	//scanf("%s",path);
	fp = fopen("lena256.raw", "rb");
	if (fp == NULL)
	{
		printf("can not open the raw image ");
		return 0;
	}
	else
	{
		printf("read 256 OK !\n");
	}
	
	fp2 = fopen("lena_512.raw", "rb");
	if (fp2 == NULL)
	{
		printf("can not open the raw image ");
		return 0;
	}
	else
	{
		printf("read 512 OK !\n");
	}
	
	
	// 分配内存并将图像读到二维数组中    	
	ptr = (BYTE*)malloc(width * height * sizeof(BYTE));		//malloc 代表 memory allocation，用來配置指定大小的記憶體空間，
																										//傳回新空間第一個位元組的記憶體位址，配置的空間處於尚未初始化的狀態。
	int c1 = 0;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			fread(ptr, 1, 1, fp);
			B[i][j] = *ptr;	// 把图像输入到2维数组中,变成矩阵型式
			path1[c1] = *ptr;
			ptr++;
			c1++;
		}
	}
	fclose(fp);

	ptr2 = (BYTE*)malloc(width2 * height2 * sizeof(BYTE));		//malloc 代表 memory allocation，用來配置指定大小的記憶體空間，
																										//傳回新空間第一個位元組的記憶體位址，配置的空間處於尚未初始化的狀態。
	for (i = 0; i < height2; i++)
	{
		for (j = 0; j < width2; j++)
		{
			fread(ptr2, 1, 1, fp2);
			B5[i][j] = *ptr2;	// 把图像输入到2维数组中,变成矩阵型式
			ptr2++;
		}
	}
	fclose(fp2);

	//rot
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			B2[width-j-1][height-i-1]=B[i][j];	// 丟入正確pixel
		}
	}

	fp = fopen("lena256rot.raw", "wb");
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			fwrite(&B2[i][j], 1, 1, fp);
		}
	}
	fclose(fp);


	//face
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if ((192 > i && i> 64 && 192 > j && j > 64))
			{
				B3[j-1][256-i] = B[i][j];
			}
			else
			{
				B3[255-j][i] = B[i][j];
			}
		}
	}
	fp = fopen("lena256face.raw", "wb");
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			fwrite(&B3[i][j], 1, 1, fp);
		}
	}
	fclose(fp);

	//+99
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			B4[i][j] =truncate (B[i][j] + 99);
			//B5[i][j] = B[i][j] + 99;
		}
	}
	/*fp = fopen("lena256_99.raw", "wb");
	fp2 = fopen("lena256_99o.raw", "wb");
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			fwrite(&B4[i][j], 1, 1, fp);
			fwrite(&B5[i][j], 1, 1, fp2);
		}
	}
	fclose(fp);*/

	Mat dst1(height, width, CV_8UC1, &B);
	Mat dst2(height, width, CV_8UC1, &B2);
	Mat dst3(height, width, CV_8UC1, &B3);
	Mat dst4(height, width, CV_8UC1, &B4);
	Mat dst512(height2, width2, CV_8UC1, &B5);

	//draws
	

	circle(dst512, Point(240, 224), 4, Scalar(255, 255, 255), 7, 8);
	ellipse(dst512, Point(240, 224),	Size(80,30), 75, 0, 360, Scalar(255, 255,255),2,8);
	ellipse(dst512, Point(240, 224), Size(120, 70), 75, 0, 360, Scalar(255, 255, 255), 2, 8);
	ellipse(dst512, Point(240, 224), Size(160, 110), 75, 0, 360, Scalar(255, 255, 255), 2, 8);
	ellipse(dst512, Point(240, 224), Size(200, 150), 75, 0, 360, Scalar(255, 255, 255), 2, 8);
	putText(dst512, "1 0 8 3 1 8 0 4 7", Point(92, 466), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 2, false);
	line(dst512, Point(102, 476), Point(392, 476), Scalar(255, 255, 255), 2, 8);
	imwrite("lena512.png", dst512);

	//Mat output = Mat(height,width,CV_8UC1,dst);
	printf("Which row:");
	scanf("%d", &fr);
	printf("Which column:");
	scanf("%d", &fc);
	if (((fr - 1) < 0) || ((fc-1) < 0))
	{
		printf("pixel: Null\n");
	}
	else
	{
		printf("pixel: %d\n", B[fr - 1][fc - 1]);
	}
	printf("Find pixel: ");
	scanf("%d", &pe);
	printf("pixel vaiues: %d\n", path1[pe-1]);
	namedWindow("lena256",WINDOW_AUTOSIZE);
	imshow("lena256", dst1);
	waitKey(0);
	namedWindow("lena256rot", WINDOW_AUTOSIZE);
	imshow("lena256rot", dst2);
	waitKey(0);
	namedWindow("lena256face", WINDOW_AUTOSIZE);
	imshow("lena256face", dst3);
	waitKey(0);
	namedWindow("lena256_99", WINDOW_AUTOSIZE);
	imshow("lena256_99", dst4);
	waitKey(0);
	namedWindow("lena512", WINDOW_AUTOSIZE);
	imshow("lena512", dst512);
	waitKey(0);
	
	destroyAllWindows();
}
