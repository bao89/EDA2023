#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

struct cell{
    bool unblocked;
    bool finded;
};
struct column{
    vector<cell> cel;
};
struct node{
    int x;
    int y;
    int order;
    int last_x;
    int last_y;
};
struct net{
    int s_x;
    int s_y;
    int t_x;
    int t_y;
};

void bfs(net net_A,vector <column> & coord,int r,int c,vector <node> & path,bool & check);

int main(int argc, char* argv[]){
    int r,c,num_block,num_net,s_x,s_y,t_x,t_y;
    int l,rr,d,u,iter=0;
    //char a;
    bool check=0;
    vector <int> typx;
    vector <int> typy;
    string s;
    ifstream infile;
    ofstream ofile;

    infile.open(argv[1], ios::in);
    infile>>s>>r>>s>>c>>s>>num_block;
    vector <column> coord(r);
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            cell a;
            a.unblocked=true;
            a.finded=false;
            coord[i].cel.push_back(a);
        }
    }
    for(int i=0;i<num_block;i++){
        infile>>l>>rr>>d>>u;
        for(int j=l;j<=rr;j++){
            for(int k=d;k<=u;k++){
                coord[k].cel[j].unblocked=false;
            }
        }
    }
    infile>>s>>num_net;
    int per=1;
    for(int i=0;i<num_net;i++){
        per=per*(i+1);
    }
    vector <int> num1(num_net);
    for(int i=0;i<num_net;i++){
        num1[i]=num_net-i-1;
    }
    int min=21474836;
    vector <vector <node>> path(num_net);
    vector <vector <node>> temp(num_net);
    vector <net> nets(num_net);
    for(int i=0;i<num_net;i++){
        infile>>s;
        infile>>s_x>>s_y>>t_x>>t_y;
        nets[i].s_x=s_x;
        nets[i].s_y=s_y;
        nets[i].t_x=t_x;
        nets[i].t_y=t_y;
    }
    infile.close();
    
    do{
        for(int i=0;i<num_net;i++){
            coord[nets[i].s_y].cel[nets[i].s_x].unblocked=false;
        }
        iter++;
        for(int p=0;p<num_net;p++){
            //cout<<num1[p]-1<<' ';  
            bfs(nets[num1[p]],coord,r,c,path[num1[p]],check);
            
            if(check==1){
                next_permutation(num1.begin(),num1.end());
                //cout<<'a'<<' ';
                for(int j=l;j<=rr;j++){
                    for(int k=d;k<=u;k++){
                        coord[k].cel[j].unblocked=false;
                    }   
                }
                
                for(int i=0;i<num_net;i++){
                    if(!path[i].empty()){
                        for(int u=0;u<path[i].size();u++){
                            coord[path[i][u].y].cel[path[i][u].x].unblocked=true;
                        }
                        path[i].clear();
                    }
                    //vector<node>().swap(path[i]);
                }
                break;
            }
        }
        int suum=0;
        if(check==0){
            for (int i=0;i<num_net;i++){
                suum=path[i].size()+suum;
            }
            //cout<<suum<<' ';
            if(suum<min){
                for(int i=0;i<num_net;i++){
                    temp[i].clear();
                    for(int j=0;j<path[i].size();j++)
                        temp[i].push_back(path[i][j]);
                }
                min=suum;
            }
            for(int i=0;i<num_net;i++){
                    if(!path[i].empty()){
                        for(int u=0;u<path[i].size();u++){
                            coord[path[i][u].y].cel[path[i][u].x].unblocked=true;
                        }
                        path[i].clear();
                    }
                    //vector<node>().swap(path[i]);
            }
        }
        if(iter==per){
            for(int i=0;i<num_net;i++){
                path[i].clear();
                for(int j=0;j<temp[i].size();j++){
                    path[i].push_back(temp[i][j]);
                }
                
            }
        }
        //cout<<iter;
    }while(iter<per);
    ofile.open(argv[2], ios::out);
    for(int i=0;i<num_net;i++){
        ofile<<"net"<<i+1<<' '<<path[i].size()-2<<endl<<"begin"<<endl;
        int temp__x=path[i][0].x;
        int temp__y=path[i][0].y;
        typx.push_back(path[i][0].x);
        typy.push_back(path[i][0].y);
        for(int ii=0;ii<path[i].size();ii++){
            if((path[i][ii].x!=temp__x) && (path[i][ii].y!=temp__y)){
                typx.push_back(path[i][ii-1].x);
                typx.push_back(path[i][ii-1].x);
                typy.push_back(path[i][ii-1].y);
                typy.push_back(path[i][ii-1].y);
                temp__x=path[i][ii-1].x;
                temp__y=path[i][ii-1].y;
            }
        }
        typx.push_back(path[i][path[i].size()-1].x);
        typy.push_back(path[i][path[i].size()-1].y);
        for(int ii=typx.size()-1;ii>=0;ii--){
            if(ii<typx.size()-1&&(ii-1)%2==0){
                ofile<<endl;
            }
            ofile<<typx[ii]<<" "<<typy[ii]<<" ";
        }
        typx.clear();
        typy.clear();
        if(i==num_net-1){
            ofile<<endl<<"end";
        }
        else{
            ofile<<endl<<"end"<<endl;
        }
    }
    ofile.close();
    //system("pause");
    return 0;
}

void bfs(net net_A,vector <column> & coord,int r,int c,vector <node> & path,bool & check){
    int ord=0;
    node in;
    int det=0;
    vector <node> save;
    in.x=net_A.s_x;
    in.y=net_A.s_y;
    in.order=ord;
    in.last_x=-1;
    in.last_y=-1;
    queue <node> q;
    q.push(in);
    save.push_back(in);
    while((!q.empty())){
        
        ord++;
        for(int i=0;i<4;i++){
            if(i==3&&((q.front().y-1)>=0)&&coord[q.front().y-1].cel[q.front().x].unblocked&&!coord[q.front().y-1].cel[q.front().x].finded){
                coord[q.front().y-1].cel[q.front().x].finded=1;
                in.x=q.front().x;
                in.y=q.front().y-1;
                in.order=ord;
                in.last_x=q.front().x;
                in.last_y=q.front().y;
                q.push(in);
                save.push_back(in);
            }
            else if(i==2&&((q.front().x-1)>=0)&&coord[q.front().y].cel[q.front().x-1].unblocked&&!coord[q.front().y].cel[q.front().x-1].finded){
                coord[q.front().y].cel[q.front().x-1].finded=1;
                in.x=q.front().x-1;
                in.y=q.front().y;
                in.order=ord;
                in.last_x=q.front().x;
                in.last_y=q.front().y;
                q.push(in);
                save.push_back(in);
            }
            else if(i==1&&((q.front().x+1)<c)&&coord[q.front().y].cel[q.front().x+1].unblocked&&!coord[q.front().y].cel[q.front().x+1].finded){
                coord[q.front().y].cel[q.front().x+1].finded=1;
                in.x=q.front().x+1;
                in.y=q.front().y;
                in.order=ord;
                in.last_x=q.front().x;
                in.last_y=q.front().y;
                q.push(in);
                save.push_back(in);
            }
            else if(i == 0 && ((q.front().y + 1) <r) && coord[q.front().y + 1].cel[q.front().x].unblocked && !coord[q.front().y + 1].cel[q.front().x].finded) {
                coord[q.front().y + 1].cel[q.front().x].finded = 1;
                in.x = q.front().x;
                in.y = q.front().y+1;
                in.order = ord;
                in.last_x = q.front().x;
                in.last_y = q.front().y;
                q.push(in);
                save.push_back(in);
            }
            if(in.x == net_A.t_x && in.y == net_A.t_y) {
                det=1;
                check=0;
                int temp_x=in.last_x;
                int temp_y=in.last_y;
                path.push_back(in);
                while(temp_x!=-1&&temp_y!=-1){
                    for(int j=0;j<save.size();j++){
                        if(save[j].x==temp_x&&save[j].y==temp_y){
                            path.push_back(save[j]);
                            temp_x=save[j].last_x;
                            temp_y=save[j].last_y;
                            break;
                        }
                    }
                }
                i=4;
            }
        }
        q.pop();
        if((q.empty()&&det==0)){
            check=1;
        }
        if((in.x == net_A.t_x && in.y == net_A.t_y)){
            break;
        }
    }
    
    for(int u=0;u<path.size();u++){
        coord[path[u].y].cel[path[u].x].unblocked=false;
            }
    for(int j=0;j<r;j++){
        for(int k=0;k<c;k++){
            coord[j].cel[k].finded=false;
        }
    }
}

