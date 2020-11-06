#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include "Windows.h"
#include <cstdint>
#include <cstdlib>

using namespace cv;
using namespace std;
Mat src, src_gray;
int thresh = 190;
int max_thresh = 255;
const char* source_window = "Source image";
const char* corners_window = "Corners detected";
void cornerHarris_demo( int, void* );
int square(int);
int main(){
	VideoCapture cap("survey.avi");
	if(!cap.isOpened()){
		cout<<"Error opening video stream of file"<<endl;
		system("pause");
		return 0;
	}
	const int SIZE1 = 100;
	const int SIZE2 = 100;
	int prevX[SIZE1];
	int prevY[SIZE2];
	int x[SIZE1];
	int y[SIZE2];
	int count=0;
	int prevCount=0;
	bool newSet=true;
	bool first=true;
	int temp;
	int diffEachSetSquaredX = 25;
	int diffEachSetSquaredY = 100; 
	int prevCluster[100][20];//max is 20 pts each cluster
	int prevNumPtsEachCluster[100]; //max is 100 clusters
	int cluster[100][20];//max is 20 pts each cluster
	int numPtsEachCluster[100];
	//Y
	int prevYCluster[100][20];
	int YCluster[100][20];
	//
	int count2=0;
	int prevCount2=0;
	Mat src;
	Mat src_gray;
	int beginning;
	bool repeatX = false;
	double avgX[SIZE1];
	double avgY[SIZE1];
	double total=0;
	double total2=0;
	bool matched=false;
	bool largeSet=false;
	while(1){
		cap>>src;
		if(src.empty()){
			break;
		}
		cvtColor(src,src_gray,COLOR_BGR2GRAY);

		count=0;
		int blockSize = 2;
		int apertureSize = 3;
		double k = 0.04;

		Mat dst = Mat::zeros( src.size(), CV_32FC1 );
		cornerHarris( src_gray, dst, blockSize, apertureSize, k );

		Mat dst_norm, dst_norm_scaled;
		normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
		convertScaleAbs( dst_norm, dst_norm_scaled );
		for( int i = 0; i < dst_norm.rows ; i++ )
		{
			for( int j = 0; j < dst_norm.cols; j++ )
			{
				if( (int) dst_norm.at<float>(i,j) > thresh )
				{
					circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
					if(first){
						prevX[count] = j;
						prevY[count] = i;
					}
					else{
						x[count] = j;
						y[count] = i;
					}
					count++;
				}
			}
		}
		//test
		/*for(int i=0;i<count-7;i++){
			prevX[i] = 1;
		}
		for(int i=count-7;i<count;i++){
			prevX[i] = 2;
		}
		for(int i=count-1;i>=0;i--){
			prevY[count - 1 - i] = i;
		}*/
		if(first){
			/*for(int i=0;i<count;i++){
				cout<<prevX[i]<<" ";
			}
			cout<<endl;
			system("pause");*/
			prevCount = count;
			for(int i=0;i<prevCount-1;i++){
				for(int j=i+1;j<prevCount;j++){
					if(prevX[i]>prevX[j]){
						temp = prevX[i];
						prevX[i] = prevX[j];
						prevX[j] = temp;
						temp = prevY[i];
						prevY[i] = prevY[j];
						prevY[j] = temp;
					}
				}
			}
			/*for(int i=0;i<count;i++){
				cout<<prevY[i]<<" ";
			}
			cout<<endl;
			system("pause");*/
			count2=0;
			repeatX=false;
			for(int i=0;i<prevCount;i++){
				beginning=i;
				count2=0;
				while(i+count2+1<prevCount&&prevX[i]==prevX[i+count2+1]){
					count2++;
					repeatX=true;
				}
				if(repeatX){
					//cout<<"beginning "<<beginning<<endl;
					//cout<<"end "<<i+count2<<endl;
					i=i+count2+1;
					for(int j=beginning;j<i-beginning-1;j++){
						for(int k=j+1;k<i-beginning;k++){
							if(prevY[j]>prevY[k]){
								temp = prevX[j];
								prevX[j] = prevX[k];
								prevX[k] = temp;
								temp = prevY[j];
								prevY[j] = prevY[k];
								prevY[k] = temp;
							}
						}
					}
					i--;
					repeatX=false;
				}
			}
			/*for(int i=0;i<count;i++){
				cout<<prevY[i]<<" ";
			}
			cout<<endl;*/
			prevCount2=0;
			newSet=true;
			for(int i=0;i<prevCount;i++){
				if(newSet){
					prevNumPtsEachCluster[prevCount2] = 0;
					prevCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevX[i];
					prevYCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevY[i];
					prevNumPtsEachCluster[prevCount2]++;
					newSet = false;
				}
				else{
					if(square(prevCluster[prevCount2][0] - prevX[i])>diffEachSetSquaredX){//simplify this/||
						newSet = true;
						prevCount2++;
						prevNumPtsEachCluster[prevCount2] = 0 ;
						i--;
					}
					else{
						prevCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevX[i];
						prevYCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevY[i];
						prevNumPtsEachCluster[prevCount2]++;
					}
				}
			}
			prevCount2++;
			first=false;
			/*for(int i=0;i<prevCount;i++){
				cout<<" ("<<prevX[i]<<","<<prevY[i]<<")"<<endl;
			}
			cout<<endl;
			for(int i=0;i<prevCount2;i++){
				for(int j=0;j<prevNumPtsEachCluster[i];j++){
					cout<<prevCluster[i][j]<<","<<prevYCluster[i][j]<<" ";
				}
				cout<<endl;
			}*/
			//system("pause");
		}
		else{
			for(int i=0;i<count-1;i++){
				for(int j=i+1;j<count;j++){
					if(x[i]>x[j]){
						temp = x[i];
						x[i] = x[j];
						x[j] = temp;
						temp = y[i];
						y[i] = y[j];
						y[j] = temp;
					}
				}
			}
			/*for(int i=0;i<count;i++){
				cout<<prevY[i]<<" ";
			}
			cout<<endl;
			system("pause");*/
			count2=0;
			repeatX=false;
			for(int i=0;i<count;i++){
				beginning=i;
				count2=0;
				while(i+count2+1<count&&x[i]==x[i+count2+1]){
					count2++;
					repeatX=true;
				}
				if(repeatX){
					//cout<<"beginning "<<beginning<<endl;
					//cout<<"end "<<i+count2<<endl;
					i=i+count2+1;
					for(int j=beginning;j<i-beginning-1;j++){
						for(int k=j+1;k<i-beginning;k++){
							if(y[j]>y[k]){
								temp = x[j];
								x[j] = x[k];
								x[k] = temp;
								temp = y[j];
								y[j] = y[k];
								y[k] = temp;
							}
						}
					}
					i--;
					repeatX=false;
				}
			}
			/*for(int i=0;i<count;i++){
				cout<<prevY[i]<<" ";
			}
			cout<<endl;*/
			count2=0;
			newSet=true;
			for(int i=0;i<count;i++){
				if(newSet){
					numPtsEachCluster[count2] = 0;
					cluster[count2][numPtsEachCluster[count2]] = x[i];
					YCluster[count2][numPtsEachCluster[count2]] = y[i];
					numPtsEachCluster[count2]++;
					newSet = false;
				}
				else{
					if(square(cluster[count2][0] - x[i])>diffEachSetSquaredX){//simplify this/||
						newSet = true;
						count2++;
						numPtsEachCluster[count2] = 0 ;
						i--;
					}
					else{
						cluster[count2][numPtsEachCluster[count2]] = x[i];
						YCluster[count2][numPtsEachCluster[count2]] = y[i];
						numPtsEachCluster[count2]++;
					}
				}
			}
			count2++;
			first=false;//new is prev and new with the old new which is prev
			/*for(int i=0;i<count;i++){
				cout<<" ("<<x[i]<<","<<y[i]<<")"<<endl;
			}
			cout<<endl;
			for(int i=0;i<count2;i++){
				for(int j=0;j<numPtsEachCluster[i];j++){
					cout<<cluster[i][j]<<","<<YCluster[i][j]<<" ";
				}
				cout<<endl;
			}
			system("pause");*/
			//The old set with many points disappear compared to the new
			total = 0;
			total2 = 0;
			matched = false;
			for(int i=0;i<prevCount2;i++){
				total = 0;
				total2 = 0;
				largeSet=false;
				if(prevNumPtsEachCluster[i]>4){
					largeSet = true;
					for(int j=0;j<prevNumPtsEachCluster[i];j++){
						total += prevCluster[i][j];
						total2 += prevYCluster[i][j];
					}
				}
				//cout<<prevNumPtsEachCluster[i]<<endl;
				//system("pause");
				avgX[i]=total/prevNumPtsEachCluster[i];
				avgY[i]=total2/prevNumPtsEachCluster[i];
				//compare
				//total = 0;
				//total2 = 0;
				matched = false;
				if(largeSet){
					for(int j=0;j<count2;j++){
						total=0;
						total2=0;
						for(int k=0;k<numPtsEachCluster[j];k++){
							total += cluster[j][k];
							total2 += YCluster[j][k];
						}
						//cout<<avgX[i]<<endl;
						//cout<<total/numPtsEachCluster[j]<<endl;
						//system("pause");
						if(square(avgX[i] - total/numPtsEachCluster[j])<=25){
							//cout<<"success"<<endl;
							matched = true;
							break;
						}
					}
					if(matched==false){
						for(int j=0;j<count2;j++){
							for(int k=0;k<numPtsEachCluster[j];k++){
								cout<<cluster[j][k]<<","<<YCluster[j][k]<<" ";
							}
							cout<<endl;
						}
						cout<<"x "<<avgX[i]<<endl;
						for(int j=0;j<prevCount2;j++){//something wrong//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							for(int k=0;k<prevNumPtsEachCluster[j];k++){
								cout<<prevCluster[j][k]<<","<<prevYCluster[j][k]<<" ";
							}
							cout<<endl;
						}
						cout<<"Motion Detected"<<endl;
						//Take a picture and call
						system("pause");
						//return 0;
					}
				 }
			 }
			//prev is set to current and stay first equals to false
			prevCount=count;
			for(int i=0;i<prevCount;i++){
				prevX[i] = x[i];
				prevY[i] = y[i];
			}
			for(int i=0;i<prevCount-1;i++){
				for(int j=i+1;j<prevCount;j++){
					if(prevX[i]>prevX[j]){
						temp = prevX[i];
						prevX[i] = prevX[j];
						prevX[j] = temp;
						temp = prevY[i];
						prevY[i] = prevY[j];
						prevY[j] = temp;
					}
				}
			}
			/*for(int i=0;i<count;i++){
				cout<<prevY[i]<<" ";
			}
			cout<<endl;
			system("pause");*/
			count2=0;
			repeatX=false;
			for(int i=0;i<prevCount;i++){
				beginning=i;
				count2=0;
				while(i+count2+1<prevCount&&prevX[i]==prevX[i+count2+1]){
					count2++;
					repeatX=true;
				}
				if(repeatX){
					//cout<<"beginning "<<beginning<<endl;
					//cout<<"end "<<i+count2<<endl;
					i=i+count2+1;
					for(int j=beginning;j<i-beginning-1;j++){
						for(int k=j+1;k<i-beginning;k++){
							if(prevY[j]>prevY[k]){
								temp = prevX[j];
								prevX[j] = prevX[k];
								prevX[k] = temp;
								temp = prevY[j];
								prevY[j] = prevY[k];
								prevY[k] = temp;
							}
						}
					}
					i--;
					repeatX=false;
				}
			}
			/*for(int i=0;i<count;i++){
				cout<<prevY[i]<<" ";
			}
			cout<<endl;*/
			prevCount2=0;
			newSet=true;
			for(int i=0;i<prevCount;i++){
				if(newSet){
					prevNumPtsEachCluster[prevCount2] = 0;
					prevCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevX[i];
					prevYCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevY[i];
					prevNumPtsEachCluster[prevCount2]++;
					newSet = false;
				}
				else{
					if(square(prevCluster[prevCount2][0] - prevX[i])>diffEachSetSquaredX){//simplify this//new set
						newSet = true;
						prevCount2++;
						prevNumPtsEachCluster[prevCount2] = 0 ;
						i--;
					}
					else{
						prevCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevX[i];
						prevYCluster[prevCount2][prevNumPtsEachCluster[prevCount2]] = prevY[i];
						prevNumPtsEachCluster[prevCount2]++;
					}
				}
			}
			prevCount2++;
		}
		namedWindow( corners_window );
		imshow( corners_window, dst_norm_scaled );
		//imshow( corners_window, dst_norm );
		//system("pause");
		char c = (char)waitKey(25);
		if(c==27){
			break;
		}
	}

	system("pause");
	return 0;
}

int square(int num){
	return num*num;
}
