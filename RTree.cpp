// RTree.cpp: 定义控制台应用程序的入口点。
//

// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//
#include"stdafx.h"
#include<string>
#include<fstream>
#include"RTree.h"
#include"readFile.h"
#include<time.h>
#include<math.h>
#include<map>
#include<algorithm>
using namespace std;

vector<string> imageNameList_found;  //查询到的图片名字列表
map<string, double> Image; //将图片名字与其与查询点的距离关联

bool MySearchCallback(string* id,void* arg)
{
	imageNameList_found.push_back(*id);
	return true; // keep going
}

bool cmp(const string &a,const string &b)       //距离排序
{
	return Image[a] < Image[b];
}

int main()
{	
	srand(time(NULL));
	string feature_Filename = "color_feature.txt";
	string image_Filename = "imagelist.txt";
	vector<vector<int>> featureArray;
	vector<string> imageNameArray;  //所有的imagename
	readFeature(feature_Filename, featureArray);
	readImage(image_Filename, imageNameArray);

	map<string, int> imageName_number;                    //统计不同种类的图片的数量
	map<string, int>::iterator it;
	for (int i = 0; i < imageNameArray.size(); i++)          //加载图片种类
	{
		string a = imageNameArray[i];
		a.erase(9);
		it = imageName_number.find(a);
		if (it != imageName_number.end())
			imageName_number[a]++;
		else
			imageName_number.insert(pair<string, int>(a, 1));
	}
	
	const int feature = 9;                      //维数

	int n = featureArray.size();
	int **array_feature = new int*[n];
	for (int i = 0; i < n; i++)
		array_feature[i] = new int[feature];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < feature; j++)
			array_feature[i][j] = featureArray[i][j];

	RTree<string*, int, feature, double> tree;

	printf("For number = %d\n", n);

	for (int i = 0; i< n; i++)
	{
		tree.Insert(array_feature[i], array_feature[i], &imageNameArray[i]); // Note, all values including zero are fine in this version
	}

	for (int i = 0; i < 1; i++)
	{
		int min[feature], max[feature];
		int hits = 0;
		int ranNum = rand() % n;
		vector<int> a = featureArray[ranNum];
		for (int i = 0; i < feature; i++)                         //定义相关性？
		{
			//min[i] = (a[i]  - rand() %  4000) >= 0? a[i] - rand() % 10000:0;
			//max[i] = (a[i] + rand() % 4000) <= 10000 ? a[i] + rand() % 10000 :10000;
			min[i] = a[i] *  5/6;
			max[i] = a[i] * 7 / 6;
		}
		int nhits = tree.Search(min, max, MySearchCallback, NULL);
		hits += nhits;
		cout << "Search results in hits " << nhits;
		cout << " ,disk access number is " << num << endl;


		for (int i = 0; i < imageNameList_found.size(); i++)          //加载图片种类
		{
			string temp = imageNameList_found[i];
			int m;
			for (m = 0; m < imageNameArray.size(); m++)
			{
				if (imageNameArray[m] == temp)
					break;
			}
			double dis = 0;
			for (int i = 0; i < feature; i++)
			{
				dis += (a[i] - featureArray[m][i])*(a[i] - featureArray[m][i]); 
				dis = sqrt(dis);
			}
			Image.insert(pair<string, double>(temp, dis));
		}

		sort(imageNameList_found.begin(), imageNameList_found.end(), cmp);
		for (int i = 0; i < imageNameList_found.size(); i++)
		{
			cout << "Hit data " << imageNameList_found[i] << ", ";
			cout << "Euclidean distance = " << Image[imageNameList_found[i]] << endl;
		}
		int hitNumber = 0;
		string temp = imageNameArray[ranNum];
		temp.erase(9);
		for (int i = 0; i < imageNameList_found.size(); i++)
		{
			string temp_ = imageNameList_found[i];
			temp_.erase(9);
			if (temp_ == temp)
				hitNumber++;
		}
		cout << "正确率 = " << (double)100 * hitNumber / (double)imageNameList_found.size() << " % " << endl;
		cout << "命中率 = " << (double)100 * hitNumber / (double)imageName_number[temp] << " % " << endl;
		imageNameList_found.clear();
	}
	

	

	for (int i = 0; i < n; i++)
	{
		delete[] array_feature[i];
		array_feature[i] = NULL;
	}
	delete[] array_feature;
	array_feature = NULL;

	system("pause");
	
	return 0;
}