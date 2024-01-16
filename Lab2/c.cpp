#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<sstream>
#include<ctime>
#include<math.h>
using namespace std;
int wall=100000;
class Point{
	public :
		int x,y;
		Point(){};
		Point(int a,int b):x(a),y(b){};
};
class Block {
	private:
		int left,right,up,down;
	public:
		Block():left(0),right(0),up(0),down(0){}
		Block(int l,int r,int u,int d):left(l),right(r),up(u),down(d){}
		void print(){
			cout<<left<<" "<<right<<" "<<up<<" "<<down<<endl;
		}
};
class Net {
	private:
		int sourceX,sourceY,targetX,targetY;
	public:
		int dis=0,pa=0,pa_p=0;
		vector<Point> p,p_p;
		Net():sourceX(0),sourceY(0),targetX(0),targetY(0){}
		Net(int sx,int sy,int tx,int ty,int d):sourceX(sx),sourceY(sy),targetX(tx),targetY(ty),dis(d){}
		void print(){
			cout<<sourceX<<" "<<sourceY<<" "<<targetX<<" "<<targetY<<" "<<dis<<endl;
		}
		void get_checkpoint(vector< pair<int,int> > &t){
			t.push_back(make_pair(sourceX,targetY));
			t.push_back(make_pair(targetX,sourceY));
		}
		void get_value(int &sx,int &sy,int &tx,int &ty,int &d){
			sx=sourceX;
			sy=sourceY;
			tx=targetX;
			ty=targetY;
			d=dis;
		}
		void set_value(int sx,int sy,int tx,int ty,int d){
			sourceX=sx;
			sourceY=sy;
			targetX=tx;
			targetY=ty;
			dis=d;
		}
};

void swap(Net &a,Net &b){
	int sx1,sy1,tx1,ty1,d1;
	int sx2,sy2,tx2,ty2,d2;
	a.get_value(sx1,sy1,tx1,ty1,d1);
	b.get_value(sx2,sy2,tx2,ty2,d2);
	a.set_value(sx2,sy2,tx2,ty2,d2);
	b.set_value(sx1,sy1,tx1,ty1,d1);
}

void visit(vector< vector<int> > &map,int RowSize,int ColSize,int sourceX,int sourceY,int targetX,int targetY,int cost) {
    if((sourceX>=ColSize)||(targetX>=ColSize)||(sourceY>=RowSize)||(targetY>=RowSize)){
		return;
	}
	if((sourceX<0)||(targetX<0)||(sourceY<0)||(targetY<0)){
		return;
	}
	if(!map[sourceY][sourceX]) {
		
		map[sourceY][sourceX] = (cost+1);
        //if((targetY==sourceY)&&(targetX==sourceX))
		//	return ;
		visit(map, RowSize,ColSize,sourceX,sourceY+1,targetX,targetY,map[sourceY][sourceX]);
		visit(map, RowSize,ColSize,sourceX+1,sourceY,targetX,targetY,map[sourceY][sourceX]);
		visit(map, RowSize,ColSize,sourceX,sourceY-1,targetX,targetY,map[sourceY][sourceX]);
		visit(map, RowSize,ColSize,sourceX-1,sourceY,targetX,targetY,map[sourceY][sourceX]);
		
    }
	else{
		if(map[sourceY][sourceX]<wall&&map[sourceY][sourceX]>(cost+1)){
			map[sourceY][sourceX]=(cost+1);
			//if((targetY==sourceY)&&(targetX==sourceX))
			//	return ;
			visit(map, RowSize,ColSize,sourceX,sourceY+1,targetX,targetY,map[sourceY][sourceX]);
			visit(map, RowSize,ColSize,sourceX+1,sourceY,targetX,targetY,map[sourceY][sourceX]);
			visit(map, RowSize,ColSize,sourceX,sourceY-1,targetX,targetY,map[sourceY][sourceX]);
			visit(map, RowSize,ColSize,sourceX-1,sourceY,targetX,targetY,map[sourceY][sourceX]);
		}
	}
}

void show(vector< vector<int> > &map,int RowSize,int ColSize,int sx,int sy,int tx,int ty){
	for(int j=0;j<RowSize;j++){
		for(int w=0;w<ColSize;w++){
			if(map[j][w]>=wall)
				cout<<"x "<<"  ";
			else if((j==sy)&&(w==sx))
				cout<<"B"<<"   ";
			else if((j==ty)&&(w==tx))
				cout<<"F"<<"   ";
			else if(map[j][w]-1>0&&map[j][w]-1<10)
				cout<<map[j][w]-1<<"   ";
			else if(map[j][w]-1>=10)
				cout<<map[j][w]-1<<"  ";
			else
				cout<<0<<"   ";
		}
		cout<<endl;
	}
	cout<<endl;
}
void print_ans(Net &n,ofstream &out,int i){
	out<<"net"<<i<<" "<<n.pa_p<<endl;
	out<<"begin"<<endl;
	for(int i=0;i<n.p_p.size();i++){
		if(i==0)
			out<<n.p_p[i].x<<" "<<n.p_p[i].y<<" ";
		else if(i==(n.p_p.size()-1))
			out<<n.p_p[i].x<<" "<<n.p_p[i].y<<endl;
		else{
			out<<n.p_p[i].x<<" "<<n.p_p[i].y<<endl;
			out<<n.p_p[i].x<<" "<<n.p_p[i].y<<" ";
		}
	}
	out<<"end"<<endl;
}
void path(vector< vector<int> > &map,Net &n,int RowSize,int ColSize){
	int sx,sy,tx,ty,d,pos;
	vector<int> stk;
	n.get_value(sx,sy,tx,ty,d);
	stk.push_back(0);
	if((map[ty][tx]>=wall)||(!map[ty][tx])){
		return;
	}
	while((sx!=tx)||(sy!=ty)){
		if(((ty+1)<RowSize)&& (map[ty+1][tx]<map[ty][tx])){
			stk.push_back(1);
			ty=ty+1;
		}
		else if(((ty-1)>=0)&& (map[ty-1][tx]<map[ty][tx])){
			stk.push_back(2);
			ty=ty-1;
		}
		else if(((tx+1)<ColSize)&&(map[ty][tx+1]<map[ty][tx])){
			stk.push_back(3);
			tx=tx+1;
		}
		else if(((tx-1)>=0)&&(map[ty][tx-1]<map[ty][tx])){
			stk.push_back(4);
			tx=tx-1;
		}
	}
	
	n.get_value(sx,sy,tx,ty,d);
	Point t=Point(sx,sy);
	n.p.push_back(t);
	for(int i=stk.size()-1;i>0;i--){
		switch(stk[i]){
			case 1:{
				if(stk[i]==stk[i-1]){
					sy-=1;
				}
				else{
					sy-=1;
					Point t=Point(sx,sy);
					n.p.push_back(t);
				}
				break;
			}
			case 2:{
				if(stk[i]==stk[i-1]){
					sy+=1;
				}
				else{
					sy+=1;
					Point t=Point(sx,sy);
					n.p.push_back(t);
				}
				break;
			}
			case 3:{
				if(stk[i]==stk[i-1]){
					sx-=1;
				}
				else{
					sx-=1;
					Point t=Point(sx,sy);
					n.p.push_back(t);
				}
				break;
			}
			case 4:{
				if(stk[i]==stk[i-1]){
					sx+=1;
				}
				else{
					sx+=1;
					Point t=Point(sx,sy);
					n.p.push_back(t);
				}
				break;
			}
		}
	}
}
void initial(vector< vector<int> > &map,vector< vector<int> > &ans,vector<Net> &n,int RowSize,int ColSize,int i){
	for(int j=0;j<RowSize;j++){
		for(int w=0;w<ColSize;w++){
			ans[j][w]=map[j][w];
		}
	}
	for(int j=0;j<=i;j++){
		for(int w=0;w<(n[j].p.size()-1);w++){
			if(n[j].p.empty())
				break;
			if(n[j].p[w].x==n[j].p[w+1].x){
				if(n[j].p[w].y>n[j].p[w+1].y){
					for(int z=n[j].p[w+1].y;z<=n[j].p[w].y;z++){
						ans[z][n[j].p[w].x]=wall+j+1;
					}
				}
				else{
					for(int z=n[j].p[w].y;z<=n[j].p[w+1].y;z++){
						ans[z][n[j].p[w].x]=wall+j+1;
					}
				}
			}
			else{
				if(n[j].p[w].x>n[j].p[w+1].x){
					for(int z=n[j].p[w+1].x;z<=n[j].p[w].x;z++){
						ans[n[j].p[w].y][z]=wall+j+1;
					}
				}
				else{
					for(int z=n[j].p[w].x;z<=n[j].p[w+1].x;z++){
						ans[n[j].p[w].y][z]=wall+j+1;
					}
				}	
			}
		}
	}
}


int myrandom (int i) { 
	return std::rand()%i;
}

int main(int argc, char *argv[]){
	//cal time
	clock_t start, stop;
	start = clock();
	double time_taken = 5;
	
	int RowSize,ColSize,BlockNum,NetNum;
	string t;
	vector< vector<int> > map; //block
	vector< vector<int> > ans; //to every net
	vector< vector<int> > weighted; //weighted map
	
	
	ifstream fin(argv[1],ios::in);
	ofstream fout(argv[2],ios::out);
	
	fin>>t>>RowSize;
	fin>>t>>ColSize;
	
	
	//set map size
	map.resize(RowSize);
	ans.resize(RowSize);
	weighted.resize(RowSize);
	for(int i=0;i<RowSize;i++){
		map[i].resize(ColSize);
		ans[i].resize(ColSize);
		weighted[i].resize(ColSize);
	}
	for(int i=0;i<RowSize;i++){
		for(int j=0;j<ColSize;j++){
			map[i][j]=0;
			ans[i][j]=0;
			weighted[i][j]=0;
		}
	}
	
	//get block
	fin>>t>>BlockNum;
	vector<Block> b;
	b.resize(BlockNum);
	for(int i=0;i<BlockNum;i++){
		int left,right,up,down;
		fin>>left>>right>>up>>down;
		b[i]=Block(left,right,up,down);
		//b[i].print();
		for(int j=(up);j<=(down);j++){
			for(int w=(left);w<=(right);w++){
				map[j][w]=wall;
				ans[j][w]=wall;
			}
		}
	}
	//initial map
	// cout<<"initial map : "<<endl;
	// show(map,RowSize,ColSize,1000,1000,1000,1000);
	
	
	//get net
	fin>>t>>NetNum;
	vector<Net> n;
	n.resize(NetNum);
	for(int i=0;i<NetNum;i++){
		fin>>t;
		int sx,sy,tx,ty,dis;
		fin>>sx>>sy>>tx>>ty;
		
		//insert
		dis=abs(sx-tx)+abs(sy-ty);
		n[i]=Net(sx,sy,tx,ty,dis);
	}
	
	// sort by dis
	// min_dis
	for(int i=0;i<NetNum;i++){
		for(int j=i+1;j<NetNum;j++){
			if(n[i].dis>n[j].dis){
				swap(n[i],n[j]);
			}
		}
	}
	//routing
	while(time_taken < 90){
		for(int j=0;j<NetNum;j++){
			n[j].pa=0;
			n[j].p.clear();
		}
		for(int j=0;j<RowSize;j++){
			for(int w=0;w<ColSize;w++){
				ans[j][w]=map[j][w];
			}
		}
		for(int i=0;i<NetNum;i++){
			int sx,sy,tx,ty,d;
			n[i].get_value(sx,sy,tx,ty,d);
			visit(ans, RowSize,ColSize,sx,sy,tx,ty,0);
			if((ans[ty][tx]>wall)||(ans[ty][tx]==0)){
				random_shuffle (n.begin(), n.end());
				continue;
			}
			n[i].pa=ans[ty][tx]-2;
			path(ans,n[i],RowSize,ColSize);
			//initial
			initial(map,ans,n,RowSize,ColSize,i);
		}
		//compare
		if(n[0].p_p.empty()){
			for(int j=0;j<NetNum;j++){
				n[j].pa_p=n[j].pa;
				for(int k=0;k<n[j].p.size();k++){
					n[j].p_p.push_back(n[j].p[k]);
				}
				n[j].pa=0;
				n[j].p.clear();
			}
		}
		else{
			int total=0;
			for(int j=0;j<NetNum;j++){
				total+=n[j].pa_p;
				total-=n[j].pa;
			}
			if(total>0){
				for(int j=0;j<NetNum;j++){
					n[j].pa_p=n[j].pa;
					n[j].p_p.clear();
					for(int k=0;k<n[j].p.size();k++){
						n[j].p_p.push_back(n[j].p[k]);
					}
					n[j].pa=0;
					n[j].p.clear();
				}
			}
		}
		cout << 1 << endl;
		stop = clock();
		time_taken = double(stop - start)/double(CLOCKS_PER_SEC);
	}
	for(int i=0;i<NetNum;i++){
		print_ans(n[i],fout,i);
		// cout<<n[i].p_p.size()<<endl;
	}
	stop = clock();
	time_taken = double(stop - start)/double(CLOCKS_PER_SEC);
	// cout<<time_taken<<endl;
	fin.close();
	fout.close();
	return 0;
}

