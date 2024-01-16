#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include "cudd.h"
#include "util.h"
using namespace std;

bool list_check(vector<char> list, char c){
	for(int i=0;i<list.size();i++){
		if(c == list[i] || c == list[i]+32 || c == list[i]-32){
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[]){
	ifstream fin;
	ofstream fout;
	fin.open(argv[1]);
	fout.open(argv[2]);

	DdManager *gbm;
	gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
	DdNode *bdd, *var, *tmp, *tmp_neg;
	int i;
	bdd = Cudd_ReadOne(gbm);
	Cudd_Ref(bdd);

	vector<char> clist;
	int type = 0;
	stringstream ss1;
	string s;
	char c;
	getline(fin,s);
	ss1.str(s);
	bool flag = false;
	while(ss1 >> c){
		
		//cout << Cudd_DagSize(bdd) << endl;

		if(c == '+'){
			break;
			/*if(!flag){
				flag = true;
				continue;
			}else{
				Cudd_Ref(bdd);
				bdd = Cudd_bddOr(gbm,bdd,bdd2);
			}*/
		}else if(c == '.'){
			/*Cudd_Ref(bdd);
			bdd = Cudd_bddOr(gbm,bdd,bdd2);*/
			break;
		}else{
			if(list_check(clist,c)){
				type++;
				clist.push_back(c);
			}
			if(c > 96){
				var = Cudd_bddIthVar(gbm,c-97);
				tmp = Cudd_bddAnd(gbm,var,bdd);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(gbm,bdd);
				bdd = tmp;
			}else{
				var = Cudd_bddIthVar(gbm,c-65);
				tmp_neg = Cudd_Not(var);
				tmp = Cudd_bddAnd(gbm,tmp_neg,bdd);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(gbm,bdd);
				bdd = tmp;
			}
			/*if(!flag){
				if(list_check(clist,c)){
					type++;
					clist.push_back(c);
				}
				if(c > 96){
					var = Cudd_bddIthVar(gbm,c-97);
					tmp = Cudd_bddAnd(gbm,var,bdd);
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm,bdd);
					bdd = tmp;
				}else{
					var = Cudd_bddIthVar(gbm,c-65);
					tmp_neg = Cudd_Not(var);
					tmp = Cudd_bddAnd(gbm,tmp_neg,bdd);
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm,bdd);
					bdd = tmp;
				}
			}else{
				//cout << c << endl;
				if(list_check(clist,c)){
					type++;
					clist.push_back(c);
				}
				if(c > 96){

					var = Cudd_bddIthVar(gbm,c-97);
					tmp = Cudd_bddAnd(gbm,var,bdd2);
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm,bdd2);
					bdd2 = tmp;

				}else{
					var = Cudd_bddIthVar(gbm,c-65);
					tmp_neg = Cudd_Not(var);
					tmp = Cudd_bddAnd(gbm,tmp_neg,bdd2);
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm,bdd2);
					bdd2 = tmp;
				}
			}*/
		}
	}
	//cout << Cudd_DagSize(bdd) << endl;
	if(c != '.'){
		while(c != '.'){
			ss1 >> c;
			DdNode *bdd2;
			bdd2 = Cudd_ReadOne(gbm);
			Cudd_Ref(bdd2);

			while(c != '+' && c != '.'){
				if(list_check(clist,c)){
					type++;
					clist.push_back(c);
				}
				if(c > 96){
					var = Cudd_bddIthVar(gbm, c - 97);
					tmp = Cudd_bddAnd(gbm, var, bdd2);
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm, bdd2);
					bdd2 = tmp;
				}else{
					var = Cudd_bddIthVar(gbm, c - 65);
					tmp_neg = Cudd_Not(var); /*Perform NOT Boolean operation*/
					tmp = Cudd_bddAnd(gbm, tmp_neg, bdd2); /*Perform AND Boolean operation*/
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm, bdd2);
					bdd2 = tmp;
				}
				ss1 >> c;
			}
			Cudd_Ref(bdd);
			bdd = Cudd_bddOr(gbm,bdd,bdd2);
		}
	}
	/*DdNode *bdd2;
	bdd2 = Cudd_ReadOne(gbm);
	Cudd_Ref(bdd2);

	while(ss1 >> c){
		
		if(c == '+'){
			Cudd_Ref(bdd);
			bdd = Cudd_bddOr(gbm,bdd,bdd2);
		}else if(c == '.'){
			Cudd_Ref(bdd);
			bdd = Cudd_bddOr(gbm,bdd,bdd2);
			break;
		}else{
			if(list_check(clist,c)){
				type++;
				clist.push_back(c);
			}
			if(c > 96){

				var = Cudd_bddIthVar(gbm,c-97);
				tmp = Cudd_bddAnd(gbm,var,bdd2);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(gbm,bdd2);
				bdd2 = tmp;

			}else{
				var = Cudd_bddIthVar(gbm,c-65);
				tmp_neg = Cudd_Not(var);
				tmp = Cudd_bddAnd(gbm,tmp_neg,bdd2);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(gbm,bdd2);
				bdd2 = tmp;
			}
		}
	}*/
	Cudd_Ref(bdd);
	//cout << Cudd_DagSize(bdd) << endl;
	//cout << type << endl;

	//vector<int> ans;
	int min = pow(2,type);
	//cout << min << endl;
	int* order = new int [type];
	int count;
	stringstream ss2;
	//getline(fin,s);
	while(getline(fin,s)){
		ss2.str(s);
		count = 0;
		while(ss2 >> c){
			if(c == '.'){
				break;
			}else{
				order[count] = c-97;
				count++;
			}
		}
		/*for(int i=0;i<type;++i){
			cout << order[i] << " ";
		}
		cout << "\n";*/
		Cudd_ShuffleHeap(gbm,order);
		//cout << Cudd_DagSize(bdd) << endl;
		/*if(Cudd_DagSize(bdd) > 1){
			cout << 11 << endl;
		}*/

		if(Cudd_DagSize(bdd) < min){
			//cout << Cudd_DagSize(bdd) << endl;
			min = Cudd_DagSize(bdd) + 1;
		}

	}

	if(Cudd_DagSize(bdd) > 1){
		fout << min << "\n";
	}else{
		fout << 1 << endl;
	}

	//fout << min << "\n";

	fin.close();
	fout.close();

	return 0;
}