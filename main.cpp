#include <iostream>
using namespace std;
int main()
{
	int chinese,mathematics,english,physics,total,average;
	cout<<"pls input the score of chinese,mathematics,english and physics: ";
	cin>>chinese>>mathematics>>english>>physics;
	total = chinese + mathematics + english + physics;
	average = total/4;
	cout<<"the total score is: "<<total<<endl;
	cout<<"the average score is: "<<average<<endl;
	return 0;
}
