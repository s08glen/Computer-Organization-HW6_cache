#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
using namespace std;

void transfer(int* array,int){

}

int main(int argc, char **argv){
	if(argc!=3){
		cout << "Input error!\n";
		return 0;
	}
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);
	if (!fin){
		cout << "Can not open file!\n";
		return 0;
	}
	int csize,bsize,asso,replace;
	fin>>csize;
	fin>>bsize;
	fic>>asso;
	fin>>replace;
	cout<<csize<<" "<<bsize<<" "<<asso<<" "<<replace<<endl;
	return 0;
}
