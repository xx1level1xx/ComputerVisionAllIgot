#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <time.h>
#include <stdio.h>
//#include "Windows.h"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <wininet.h>
#pragma comment (lib,"wininet.lib")
//#include <cassert>
using namespace cv;
using namespace std;

double square(double);
int convert(string);
int power10(int);
string int2String(int num);
void FileSubmit(int num){
	HINTERNET hInternet;
    HINTERNET hFtpSession;
    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	string file="";
	string path="";
    if (hInternet == NULL)
    {
        cout << "Error: InternetOpen = " << GetLastError() << endl;
    }
    else
    {
        hFtpSession = InternetConnect(hInternet, "192.168.1.88", 21, "sieelmon@outlook.com", "onlyNo", INTERNET_SERVICE_FTP, 0, 0);
        if (hFtpSession == NULL)
        {
            cout << "Error: InternetConnect = " << GetLastError() << endl;
        }
        else
        {
			for(int i=0;i<num;i++){
				file = "Segment" + int2String(i) + ".avi";
				path = "C:\\Users\\xx1le\\Desktop\\Segment" + int2String(i) + ".avi";
				LPSTR s = const_cast<char *>(path.c_str());
				LPSTR t = const_cast<char *>(file.c_str());
				if (FtpPutFile(hFtpSession, s, t, FTP_TRANSFER_TYPE_BINARY, 0))
					cout << "File send" << endl;
				else
					cout << "Error: FtpPutFile = " << GetLastError() << endl;
				InternetCloseHandle(hFtpSession);
			}
        }
        InternetCloseHandle(hInternet);
    }
}
int main(){
	Mat src, src_gray, tempMat; //tempMat2;
	int thresh = 190;
	int max_thresh = 255;
	VideoCapture cap("survey.avi");
	if (!cap.isOpened()) {
		cout << "Error opening video stream of file" << endl;
		system("pause");
		return 0;
	}
	ofstream fout;
	fout.open("data.txt");
	int numLoop = 0;
	int numCorners = 0;
	const int numTrainingPictures = 10;
	const int SIZE = 20;
	int numCornersArr[numTrainingPictures];
	while (1) {
		cap >> src;
		if (src.empty()) {
			break;
		}
		Size size = src.size();
		cvtColor(src, src_gray, COLOR_BGR2GRAY);

		int blockSize = 2;
		int apertureSize = 3;
		double k = 0.04;

		tempMat = Mat::zeros(size, CV_32FC1);
		cornerHarris(src_gray, tempMat, blockSize, apertureSize, k);
		normalize(tempMat, src_gray, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
		//convertScaleAbs( dst_norm, dst_norm_scaled );
		numCorners = 0;
		for (int i = 0; i < src_gray.rows; i++)
		{
			for (int j = 0; j < src_gray.cols; j++)
			{
				if ((int)src_gray.at<float>(i, j) > thresh)//tried to be the same
				{
					circle(src, Point(j, i), 5, Scalar(0), 2, 8, 0);
					numCorners++;
				}
			}
		}
		numCornersArr[numLoop] = numCorners;
		imshow("Frame", src);
		char c = (char)waitKey(25);
		if (c == 27) {
			break;
		}
		numLoop++;
		if (numLoop == numTrainingPictures) {
			break;
		}
	}
	cap.release();
	destroyAllWindows();
	for (int i = 0;i < numLoop;i++) {
		fout << numCornersArr[i] << endl;
	}
	fout.close();
	ifstream fin;
	fin.open("data.txt");
	char corner;
	int corners[numTrainingPictures];
	bool first = true;
	int numCornersEachPicture = 0;
	string temp = "";
	while (fin.get(corner)) {
		if (isdigit(corner)) {
			temp += corner;
		}
		else {
			corners[numCornersEachPicture] = convert(temp);
			temp = "";
			numCornersEachPicture++;
		}
	}
	fin.close();
	int* x[numTrainingPictures];
	int* y[numTrainingPictures];
	for (int i = 0;i < numTrainingPictures;i++) {
		x[i] = new int[corners[i]];
		y[i] = new int[corners[i]];
	}

	cap.open("Survey.avi");
	numLoop = 0;
	numCorners = 0;
	int numCornersEachLoop = 0;
	int temp2;

	int count3;
	bool repeatX;
	int beginning;
	int diffEachSetSquaredX = 25;
	first = true;
	int prev;
	numLoop = 0;
	int count = 0;
	int medianX[numTrainingPictures][30];
	int medianY[numTrainingPictures][30];
	int numPtsEachCluster[SIZE];
	int xCluster[SIZE][30];
	int yCluster[SIZE][30];
	cap.open("survey.avi");
	if (!cap.isOpened()) {
		cout << "Error opening video stream of file" << endl;
		system("pause");
		return 0;
	}
	numLoop = 0;
	int numCluster[numTrainingPictures];
	while (1) {
		cap >> src;
		if (src.empty()) {
			break;
		}
		Size size = src.size();
		cvtColor(src, src_gray, COLOR_BGR2GRAY);

		int blockSize = 2;
		int apertureSize = 3;
		double k = 0.04;

		tempMat = Mat::zeros(size, CV_32FC1);
		cornerHarris(src_gray, tempMat, blockSize, apertureSize, k);

		//Mat dst_norm;//, dst_norm_scaled;
		//Mat temp2;
		normalize(tempMat, src_gray, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
		//convertScaleAbs( dst_norm, dst_norm_scaled );
		numCornersEachLoop = 0;
		for (int i = 0; i < src_gray.rows; i++)
		{
			for (int j = 0; j < src_gray.cols; j++)
			{
				if ((int)src_gray.at<float>(i, j) > thresh)//tried to be the same
				{
					//circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
					//circle( src, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
					x[numLoop][numCornersEachLoop] = j;
					y[numLoop][numCornersEachLoop] = i;
					numCornersEachLoop++;
				}
			}
		}
		//sort
		for (int i = 0;i < numCornersEachLoop - 1;i++) {
			for (int j = i + 1;j < numCornersEachLoop;j++) {
				if (x[numLoop][i] > x[numLoop][j]) {
					temp2 = x[numLoop][i];
					x[numLoop][i] = x[numLoop][j];
					x[numLoop][j] = temp2;
					temp2 = y[numLoop][i];
					y[numLoop][i] = y[numLoop][j];
					y[numLoop][j] = temp2;
				}
			}
		}
		//sort again for the similar x's so the same x with very different y would be in a different cluster
		count3 = 0;
		repeatX = false;
		prev = x[numLoop][0];
		bool first = true;
		for (int i = 0;i < numCornersEachLoop - 1;i++) {
			count3 = 0;
			if (first) {
				first = false;
				beginning = i;
				prev = x[numLoop][i];
			}
			else {
				while (i + count3 + 1 < numCornersEachLoop&&square(prev - x[numLoop][i + count3 + 1]) <= diffEachSetSquaredX) {
					count3++;
					repeatX = true;
				}
				if (repeatX) {
					i = i + count3 + 1;
					for (int j = beginning;j < i - beginning - 1 + beginning;j++) {
						for (int k = j + 1;k < i - beginning + beginning;k++) {
							if (y[numLoop][j] > y[numLoop][k]) {
								temp2 = y[numLoop][j];
								y[numLoop][j] = y[numLoop][k];
								y[numLoop][k] = temp2;
								temp2 = x[numLoop][j];
								x[numLoop][j] = x[numLoop][k];
								x[numLoop][k] = temp2;
							}
						}
					}
					i--;
					repeatX = false;
				}
				first = true;
			}
		}
		count = 0;
		bool newSet = true;
		for (int i = 0;i < numCornersEachLoop;i++) {
			if (newSet) {
				numPtsEachCluster[count] = 0;
				xCluster[count][numPtsEachCluster[count]] = x[numLoop][i];
				yCluster[count][numPtsEachCluster[count]] = y[numLoop][i];
				numPtsEachCluster[count]++;
				newSet = false;
			}
			else {
				if ((square(xCluster[count][0] - x[numLoop][i]) > 49) || (square(yCluster[count][0] - y[numLoop][i]) > 49)) {///////////////////////////////////////////////////////////////////////49
					newSet = true;
					count++;
					i--;
				}
				else {
					xCluster[count][numPtsEachCluster[count]] = x[numLoop][i];
					yCluster[count][numPtsEachCluster[count]] = y[numLoop][i];
					numPtsEachCluster[count]++;
				}
			}
		}
		count++;
		numCluster[numLoop] = count;
		for (int i = 0;i < count;i++) {
			if (numPtsEachCluster[i] % 2 == 0) {
				medianX[numLoop][i] = (xCluster[i][numPtsEachCluster[i] / 2 - 1] + xCluster[i][numPtsEachCluster[i] / 2]) / 2.0;
				medianY[numLoop][i] = (yCluster[i][numPtsEachCluster[i] / 2 - 1] + yCluster[i][numPtsEachCluster[i] / 2]) / 2.0;
			}
			else {
				medianX[numLoop][i] = xCluster[i][numPtsEachCluster[i] / 2];
				medianY[numLoop][i] = yCluster[i][numPtsEachCluster[i] / 2];
			}
		}
		//Evidence
		numLoop++;
		if (numLoop == numTrainingPictures) {
			break;
		}
	}
	cap.release();
	destroyAllWindows();
	//get the number of time a median point appear in ten images
	int numTrainedMedian = 0;
	//int* savedX=new int[100];//////////////////////////////////////////////////////////////////////////////////
	//int* savedY=new int[100];//////////////////////////////////////////////////////////////////////////////////
	//int* countTraining=new int[100];//////////////////////////////////////////////////////////////////////////////////
	int savedX[SIZE];
	int savedY[SIZE];
	int countTraining[SIZE];
	savedX[numTrainedMedian] = medianX[0][0];
	savedY[numTrainedMedian] = medianY[0][0];
	countTraining[numTrainedMedian] = 0;
	numTrainedMedian++;
	for (int i = 0;i < numLoop;i++) {
		for (int j = 0;j < numCluster[i];j++) {
			for (int k = 0;k < numTrainedMedian;k++) {
				if (square(savedX[k] - medianX[i][j]) <= 25 && square(savedY[k] - medianY[i][j]) <= 25) {//median gets different
					countTraining[k]++;
					break;
				}
				else if (k + 1 == numTrainedMedian) {
					savedX[numTrainedMedian] = medianX[i][j];
					savedY[numTrainedMedian] = medianY[i][j];
					countTraining[numTrainedMedian] = 0;
					numTrainedMedian++;
				}
			}
		}
	}
	int numSelectedMedCorner = 0;
	int* cornerToMatchX = new int[numTrainedMedian];//////////////////////////////////////////////////////////////////////////////////
	int* cornerToMatchY = new int[numTrainedMedian];//////////////////////////////////////////////////////////////////////////////////
	//filtering the points
	for (int i = 0;i < numTrainedMedian;i++) {
		if (0.9*numTrainingPictures < countTraining[i]) {
			//cout<<0.9*numTrainingPictures<<endl;
			cornerToMatchX[numSelectedMedCorner] = savedX[i];
			cornerToMatchY[numSelectedMedCorner] = savedY[i];
			numSelectedMedCorner++;
		}
	}
	//Start camcorder and motion detection
	const int SIZE3 = 600;
	int realX[SIZE3];
	int realY[SIZE3];
	int realXCluster[SIZE3][30];
	int realYCluster[SIZE3][30];
	int realMedianX[SIZE3];
	int realMedianY[SIZE3];
	int lastingMemory[SIZE3][5];
	int lastingMemory2[5];
	int count4[SIZE3];
	int numPtsEachCluster2[SIZE3];
	bool firstFinal[SIZE3];
	bool full[SIZE3];
	bool in = false;
	bool startRecording = false;
	int countRecord = 0;
	clock_t start;
	cap.open("Survey.avi");
	if (!cap.isOpened()) {
		cout << "Error opening video stream of file" << endl;
		system("pause");
		return 0;
	}
	numLoop = 0;
	for (int i = 0;i < numSelectedMedCorner;i++) {
		count4[i] = 0;
		firstFinal[i] = true;
		full[i] = false;
	}
	VideoWriter video("C:\\Users\\xx1le\\Desktop\\videos\\Segment.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(640, 480), true);
	string videos[10];
	for(int i=0;i<10;i++){
		videos[i]="";
	}
	int numVideos=0; 
	while (1) {
		cap >> src;
		if (src.empty()) {
			break;
		}
		if (startRecording == false) {
			Size size = src.size();
			cvtColor(src, src_gray, COLOR_BGR2GRAY);

			int blockSize = 2;
			int apertureSize = 3;
			double k = 0.04;

			Mat tempMat = Mat::zeros(size, CV_32FC1);
			cornerHarris(src_gray, tempMat, blockSize, apertureSize, k);
			normalize(tempMat, src_gray, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
			numCornersEachLoop = 0;
			for (int i = 0; i < src_gray.rows; i++)
			{
				for (int j = 0; j < src_gray.cols; j++)
				{
					if ((int)src_gray.at<float>(i, j) > (thresh))//tried to be the same
					{
						realX[numCornersEachLoop] = j;
						realY[numCornersEachLoop] = i;
						numCornersEachLoop++;
					}
				}
			}
			for (int i = 0;i < numCornersEachLoop - 1;i++) {
				for (int j = i + 1;j < numCornersEachLoop;j++) {
					temp2 = realX[i];
					realX[i] = realX[j];
					realX[j] = temp2;
					temp2 = realY[i];
					realY[i] = realY[j];
					realY[j] = temp2;
				}
			}
			count3 = 0;
			repeatX = false;
			prev = realX[0];
			bool first = true;
			for (int i = 0;i < numCornersEachLoop - 1;i++) {
				count3 = 0;
				if (first) {
					first = false;
					beginning = i;
					prev = realX[i];
				}
				else {
					while (i + count3 + 1 < numCornersEachLoop&&square(prev - realX[i + count3 + 1]) <= diffEachSetSquaredX) {
						count3++;
						repeatX = true;
					}
					if (repeatX) {
						i = i + count3 + 1;
						for (int j = beginning;j < i - beginning - 1 + beginning;j++) {
							for (int k = j + 1;k < i - beginning + beginning;k++) {
								if (realY[j] > realY[k]) {
									temp2 = realY[j];
									realY[j] = realY[k];
									realY[k] = temp2;
									temp2 = realX[j];
									realX[j] = realX[k];
									realX[k] = temp2;
								}
							}
						}
						i--;
						repeatX = false;
					}
					first = true;
				}
			}
			count = 0;
			bool newSet = true;
			in = false;
			for (int i = 0;i < numCornersEachLoop;i++) {
				if (newSet) {
					numPtsEachCluster2[count] = 0;
					realXCluster[count][numPtsEachCluster2[count]] = realX[i];
					realYCluster[count][numPtsEachCluster2[count]] = realY[i];
					numPtsEachCluster2[count]++;
					newSet = false;
					in = true;
				}
				else {
					if ((square(realXCluster[count][0] - realX[i]) > 49) || (square(realYCluster[count][0] - realY[i]) > 49)) {
						newSet = true;
						count++;
						i--;
					}
					else {
						realXCluster[count][numPtsEachCluster2[count]] = realX[i];
						realYCluster[count][numPtsEachCluster2[count]] = realY[i];
						numPtsEachCluster2[count]++;
					}
				}
			}
			if (in) {
				count++;
			}
			for (int i = 0;i < count;i++) {
				if (numPtsEachCluster2[i] % 2 == 0) {
					realMedianX[i] = (realXCluster[i][numPtsEachCluster2[i] / 2 - 1] + realXCluster[i][numPtsEachCluster2[i] / 2]) / 2.0;
					realMedianY[i] = (realYCluster[i][numPtsEachCluster2[i] / 2 - 1] + realYCluster[i][numPtsEachCluster2[i] / 2]) / 2.0;
				}
				else {
					realMedianX[i] = realXCluster[i][numPtsEachCluster2[i] / 2];
					realMedianY[i] = realYCluster[i][numPtsEachCluster2[i] / 2];
				}
			}
			bool noMatched = true;
			int count2 = 0;
			int numDissappeared = 5;
			bool nope = false;
			//Motion detected if a point is blocked for three images
			numSelectedMedCorner = 5;
			for (int i = 0;i < numSelectedMedCorner;i++) {
				noMatched = false;
				for (int j = 0;j < count;j++) {
					if (square(cornerToMatchX[i] - realMedianX[j]) < 9 && square(cornerToMatchY[i] - realMedianY[j]) < 9) {//Error prone
						break;
					}
					//}
					else if (j + 1 == count) {
						noMatched = true;
					}
				}
				count2 = 0;
				if (firstFinal[i]) {
					count4[i] = 0;
					firstFinal[i] = false;
				}
				else {
					if (full[i]) {
						count2 = 0;
						nope = false;
						for (int j = 0;j < count4[i] + 1 - 1;j++) {///////////////////////////had a stuck here
							lastingMemory[i][j] = lastingMemory[i][j + 1];
							if (lastingMemory[i][j] == true) {
								nope = true;
							}
							if (lastingMemory[i][j] == false && nope == false) {/////////////////////////BIG BUG
								count2++;
							}
						}
						if (count2 == numDissappeared - 1 && noMatched) {
							//cout << "Motion Detected" << endl;
							startRecording = true;
						}
					}
				}
				if (noMatched) {
					lastingMemory[i][count4[i]] = false;//Picture does not have that corner anymore from the trained set

				}
				else {
					lastingMemory[i][count4[i]] = true;
				}
				count4[i]++;
				if ((count4[i] == numDissappeared) && full[i] == true) {
					count4[i] = numDissappeared - 1;
				}
				else if (count4[i] == numDissappeared) {
					count4[i] = numDissappeared - 1;
					full[i] = true;
				}
				if(startRecording==true&&i+1==numSelectedMedCorner){
					//cout<<numVideos<<endl;
					start = std::clock();
				}
			}
		}
		if (startRecording==true) {//record for 10 secconds
			double duration;
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			//cout << duration << endl;
			if (duration < 2) {
				video.write(src);
			}
			else {
				//check = true;
				video.release();
				videos[numVideos] = "Segment" + int2String(numVideos) + ".avi";
				numVideos++;
				startRecording = false;
				video.open("C:\\Users\\xx1le\\Desktop\\videos\\Segment" + int2String(countRecord) + ".avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(640, 480), true);
				countRecord++;
			}
		}
		imshow("Frame", src);
		char c = (char)waitKey(25);
		if (c == 27) {
			break;
		}	
	}
	video.release();
	cap.release();
	destroyAllWindows();
	//for(int i=0;i<numVideos;i++){
	//	cout<<videos[i]<<endl;
	//}
	FileSubmit(numVideos);
	for (int i = 0;i < numTrainingPictures;i++) {
		delete[] x[i];
		delete[] y[i];
	}
	delete[] cornerToMatchX;
	delete[] cornerToMatchY;
	system("pause");
	return 0;
}
double square(double num) {
	return num * num;
}
int convert(string num) {
	int sum = 0;
	for (int i = 0;i < num.length();i++) {
		sum += power10(num.length() - i - 1)*(num[i] - '0');
	}
	return sum;
}
int power10(int num) {
	int prod = 1;
	if (num == 0) {
		return 1;
	}
	else {
		for (int i = 1;i <= num;i++) {
			prod *= 10;
		}
	}
	return prod;
}
string int2String(int num){
	int power;
	//num=230;
	string tempStr="";
	int temp;
	for(int i=1;i<4;i++){
		if(num%power10(i)==num){
			power=i-1;
			break;
		}
	}
	for(int i=power;i>0;i--){
		temp = num / power10(i);
		tempStr += temp + '0';
		num = num - temp * power10(i);
	}
	tempStr += num + '0';
	return tempStr;
}