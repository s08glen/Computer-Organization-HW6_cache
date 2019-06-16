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
	int csize,bsize,asso,replace,temp,block,taglth,indexlth,tag,index,fullcount=0;
	int offset=4,offsetlth;//4 word = 16 bytes = 2^4  -> offset=4
	int quo,mod;
	string hexread,stag,sindex,soffset;
	bool replacebool=0,full=0,have=0;
	
	fin>>csize;
	fin>>bsize;
	fin>>asso;
	fin>>replace;
	block = csize*1024/bsize;
	vector<caches> cache(block);
	vector<caches> cache4(block/4);
	caches ini;
	ini.valid = 0;	ini.tag = 0;	ini.block = 0;
	cout<<"vec"<<endl;
	int **cache4set;
	cache4set = new int*[block/4];
	for(int i=0;i<block/4;i++){
		cache4set[i] = new int[4];
	}
	cout<<"pass"<<endl;
	
	while(fin>>hexread){
		replacebool=0;
		hexread = hexread.substr(2);	
		hexread = hextobin(hexread);
		if(asso==0){ //direct map
			indexlth = log(block)/log(2);
			offset = log(bsize)/log(2);
			taglth = 32 - indexlth - offset;
			stag = hexread.substr(0,taglth);
			tag = bintodec(stag);
			sindex = hexread.substr(taglth,indexlth);
			index = bintodec(sindex);
			if(cache[index].valid){ //valid=1 -> replace
				if(cache[index].tag != tag){
					fout<<cache[index].tag<<endl;
				}
				else{
					fout<<"-1"<<endl;
				}
				cache[index].tag = tag;
			}
			else{ //valid=0 -> add
				cache[index].valid = 1;
				fout<<"-1"<<endl;
				cache[index].tag = tag;
			}
		}//direct map

		else if(asso==1){//four way
			indexlth = log((block/4))/log(2);
			offsetlth = log(bsize)/log(2);
			if(bsize==1) offsetlth = 0;
			taglth = 32 - indexlth - offsetlth;
			soffset = hexread.substr(indexlth,32);
			offset = bintodec(soffset);
			stag = hexread.substr(0,taglth);
			tag = bintodec(stag);
			sindex = hexread.substr(taglth,indexlth);
			index = bintodec(sindex);
			quo = index/4;
			mod = index%4;
			if(replace==0){//FIFO
				have = 0;
				fullcount = 0;
				for(int i = 0;i < 4;i++){
					if(cache4set[index][i] == tag){
						have = 1;
						fout<<"-1"<<endl;
						break;						
					}					
				}
				if(!have){
					for(int i = 0;i < 4;i++){
						if(cache4set[index][i] != 0){
							fullcount += 1;
						}
					}
					if(fullcount != 4){//4 way have empty
						for(int i = 0;i < 4;i++){
							if(cache4set[index][i] == 0){
								cache4set[index][i] = tag;
								fout<<"-1"<<endl;
								break;
							}
						}
					}		
					else{//4 way is full
						fout<<cache4set[index][0]<<endl;
						for(int i = 0;i < 3;i++){
							cache4set[index][i] = cache4set[index][i+1];
						}
						cache4set[index][3] = tag;
					}
				}
            }//FIFO
	        else if(replace==1){//LRU
				have = 0;
				fullcount = 0;
				for(int i = 0;i < 4;i++){
					if(cache4set[index][i] == tag){
						have = 1;
						for(int j=i;j<3;j++){
							cache4set[index][i] = cache4set[index][i+1];
						}
						cache4set[index][3] = tag; //LRU -> move to rightest
						fout<<"-1"<<endl;
						break;						
					}					
				}
				if(!have){
					for(int i = 0;i < 4;i++){
						if(cache4set[index][i] != 0){
							fullcount += 1;
						}
					}
					if(fullcount != 4){//4 way have empty
						for(int i = 0;i < 4;i++){
							if(cache4set[index][i] == 0){
								cache4set[index][i] = tag;
								fout<<"-1"<<endl;
								break;
							}
						}
					}		
					else{//4 way is full
						fout<<cache4set[index][0]<<endl;
						for(int i = 0;i < 3;i++){
							cache4set[index][i] = cache4set[index][i+1];
						}
						cache4set[index][3] = tag;//LRU -> delete the most left one
					}
				}
	
			}//LRU
            else if(replace==2){//my policy
				have = 0;
				fullcount = 0;
				for(int i = 0;i < 4;i++){					
					if(cache4set[index][i] == tag){
						have = 1;
						for(int j=i;j<3;j++){
							cout<<tag<<" "<<cache4set[index][i]<<" "<<cache4set[index][i+1]<<" j= "<<j<<endl;
							cache4set[index][i] = cache4set[index][i+1];
							cout<<cache4set[index][i]<<" "<<cache4set[index][i+1]<<" done "<<endl;
						}
						cache4set[index][3] = tag; //LRU -> move to rightest
						cout<<"tagdone"<<endl;
						fout<<"-1"<<endl;
						break;						
					}					
				}
				if(!have){
					for(int i = 0;i < 4;i++){
						if(cache4set[index][i] != 0){
							fullcount += 1;
						}
					}
					if(fullcount != 4){//4 way have empty
						for(int i = 0;i < 4;i++){
							if(cache4set[index][i] == 0){
								cache4set[index][i] = tag;
								fout<<"-1"<<endl;
								break;
							}
						}
					}		
					else{//4 way is full
						fout<<cache4set[index][0]<<endl;
						for(int i = 0;i < 3;i++){
							cache4set[index][i] = cache4set[index][i+1];
						}
						cache4set[index][3] = tag;//LRU -> delete the most left one
					}
				}
			}
		}//four way

		else if(asso==2){//full asso
			index = log(block)/log(2);
			offset = log(bsize)/log(2);
			taglth = 32  - offset;
			stag = hexread.substr(0,taglth);
			tag = bintodec(stag);
			if(replace==0){//FIFO
				have = 0;
				for(int i = 0;i < block;i++){
					if(cache[i].tag == tag){
						have = 1;
						fout<<"-1"<<endl;
						break;
					}					
				}
				if(!have && fullcount < block){ //cache isn't full -> miss
					caches tempv;
					tempv.tag = tag;
					tempv.block = index;
					tempv.valid = 1;
					cache.erase(cache.end()-1);
					cache.insert(cache.begin(),tempv);
					fullcount += 1;
					fout<<"-1"<<endl;
				}
				else if(!have && fullcount >= block){//FIFO Policy
					fout<<cache[block-1].tag<<endl;
					cache.pop_back();
					caches tempv;
					tempv.tag = tag;
					tempv.block = index;
					cache.insert(cache.begin(),tempv);//first use put at the left
				}		
		
			}//FIFO
	    	else if(replace==1){//LRU
				have = 0;
				for(int i = 0;i < block;i++){
					if(cache[i].tag == tag){
						caches tempv;
						tempv.tag = tag;
						tempv.block = index;
						cache.erase(cache.begin()+i);//store in temp and put to left
						cache.insert(cache.begin(),tempv);//recent use put at the left
						have = 1;
						fout<<"-1"<<endl;
						break;
					}
					
				}
				
				if(!have && fullcount < block){ //cache isn't full -> miss
					caches tempv;
					tempv.tag = tag;
					tempv.block = index;
					tempv.valid = 1;
					cache.erase(cache.end()-1);
					cache.insert(cache.begin(),tempv);
					fullcount += 1;
					fout<<"-1"<<endl;
				}
				else if(!have && fullcount >= block){//LRU Policy
					fout<<cache[block-1].tag<<endl;
					cache.pop_back();
					caches tempv;
					tempv.tag = tag;
					tempv.block = index;
					cache.insert(cache.begin(),tempv);//recent use put at the left
				}	

			}//LRU
    	   	else if(replace==2){//my policy
			   have = 0;
				for(int i = 0;i < block;i++){
					if(cache[i].tag == tag){
						have = 1;
						fout<<"-1"<<endl;
						break;
					}					
				}
				if(!have && fullcount < block){ //cache isn't full -> miss
					caches tempv;
					tempv.tag = tag;
					tempv.block = index;
					tempv.valid = 1;
					cache.erase(cache.end()-1);
					cache.insert(cache.begin(),tempv);
					fullcount += 1;
					fout<<"-1"<<endl;
				}
				else if(!have && fullcount >= block){//my Policy MRU
					caches tempv;
					tempv.tag = tag;
					tempv.block = index;
					fout<<cache[0].tag<<endl;
					cache.erase(cache.begin());
					cache.insert(cache.begin(),tempv);//recent use put at the left
				
				}	
			}
			
		}//full asso
		stag = hexread.substr(0,taglth);
		tag = bintodec(stag);
		sindex = hexread.substr(taglth,indexlth);
		index = bintodec(sindex);
	}
	return 0;
}
