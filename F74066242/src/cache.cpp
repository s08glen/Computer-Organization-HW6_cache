#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<cmath>
#include<vector>
using namespace std;
struct caches{
	bool valid;
	int tag;
	int block;
};

vector<caches> cache;

int bintodec(string sbin){
	int iReturn = 0,count = 0;
	for(int i=sbin.length()-1 ; i >= 0 ; i--){
		if(sbin[i] == '1'){
			iReturn += pow(2,count);
		}
		count++;
	}
	return iReturn;
}

string hextobin(string shex){
	string sReturn = "";
	for (int i=0 ; i<shex.length() ; i++){
		switch (shex [i]){
			case '0': sReturn.append ("0000"); break;
			case '1': sReturn.append ("0001"); break;
			case '2': sReturn.append ("0010"); break;
			case '3': sReturn.append ("0011"); break;
			case '4': sReturn.append ("0100"); break;
			case '5': sReturn.append ("0101"); break;
			case '6': sReturn.append ("0110"); break;
			case '7': sReturn.append ("0111"); break;
			case '8': sReturn.append ("1000"); break;
			case '9': sReturn.append ("1001"); break;
			case 'a': sReturn.append ("1010"); break;
			case 'b': sReturn.append ("1011"); break;
			case 'c': sReturn.append ("1100"); break;
			case 'd': sReturn.append ("1101"); break;
			case 'e': sReturn.append ("1110"); break;
			case 'f': sReturn.append ("1111"); break;
		}
	}
	return sReturn;
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
	int csize,bsize,asso,replace,temp,block,taglth,indexlth,tag,index;
	int offset=4;//4 word = 16 bytes = 2^4  -> offset=4
	string hexread,stag,sindex;
	bool replacebool=0,full=0;
	fin>>csize;
	fin>>bsize;
	fin>>asso;
	fin>>replace;
	block = csize*1024/bsize;
	caches ini;
	ini.valid = 0;	ini.tag = 0;	ini.block = 0;
	for(int i=0;i<block;i++){
		cache.push_back(ini);
	}
//	indexlth = log(block)/log(2);
//	taglth = 32 - indexlth - offset;
	//
	while(fin>>hexread){
		replacebool=0;
		hexread = hexread.substr(2);	
		hexread = hextobin(hexread);
		if(asso==0){ //direct map
			indexlth = log(block)/log(2);
			taglth = 32 - indexlth - offset;
			stag = hexread.substr(0,taglth);
			tag = bintodec(stag);
			sindex = hexread.substr(taglth,indexlth);
			index = bintodec(sindex);
//			cout<<index<<endl;
//			if(!cache[index].valid){//valid==0 -> cache is miss -> replace
//				replacebool=1;
//			}
//			if(replacebool){
			if(cache[index].valid){ //valid=1 -> replace
				if(cache[index].tag != tag){
					cout<<cache[index].tag<<endl;
				}
				else{
					cout<<"-1"<<endl;
				}
				cache[index].tag = tag;
			}
			else{ //valid=0 -> add
				cache[index].valid = 1;
				cout<<"-1"<<endl;
				cache[index].tag = tag;
			}
//			cache[index].tag = tag;
//			}//replacebool
		}//direct map

		else if(asso==1){//four way
			if(replace==0){//FIFO

            }//FIFO
	        else if(replace==1){//LRU
	
			}//LRU
            else if(replace==2){//my policy
	
			}
		}//four way

		else if(asso==2){//full asso
			if(replace==0){//FIFO

        	}//FIFO
	        else if(replace==1){//LRU

			}//LRU
       		else if(replace==2){//my policy

			}
		}//full asso
		stag = hexread.substr(0,taglth);
		tag = bintodec(stag);
		sindex = hexread.substr(taglth,indexlth);
		index = bintodec(sindex);
	}
//	cout<<hex<<temp<<endl;
	return 0;
}
