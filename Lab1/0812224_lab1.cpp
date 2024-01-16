#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>


using namespace std;

struct Node {
    int num_node;
    int part;
    int gain;
    vector<int> connected_nets;  //Indices of nets connected to the node
    bool locked = false;
};

struct Net {
    vector<int> connected_nodes; // Indices of nodes connected to the net
};

int find_partial_sum(vector <Node> & circuit);
void calculate_gain(vector <Node> & circuit,vector<Net> & nets);
void up_date_gain(vector <Node> & circuit,vector<Net> & nets,int moved_node);
int find_max_gain_node(vector <Node> & circuit);
void unlocked(vector <Node> & circuit,vector <Net>& nets);
void fm_partition(vector <Node> & circuit,vector<Net> & nets);

int main(int argc, char* argv[]) {
    //clock_t start_time=clock();
    int numOfNet, numOfNode;
    ifstream infile;
    ofstream ofile;
    infile.open(argv[1], ios::in);
    infile >> numOfNet ;
    infile >> numOfNode;
    char a;
    
    // Initialize circuit nodes
    vector<Node> circuit(numOfNode);

    for (int i = 0; i < numOfNode; i++) {
        circuit[i].num_node = i+1 ;
        circuit[i].part=(2*i)/numOfNode;
    }
    // Initialize nets and connect to nodes
    vector <Net> nets(numOfNet);

    for (int i = 0; i < numOfNet; i++) {
        int node_idx;

        while(1) {
            infile>>node_idx;
            if(infile.peek()==' '){
                infile.get(a);
            }
            nets[i].connected_nodes.push_back(node_idx);
            circuit[node_idx -1].connected_nets.push_back(i);
            if(infile.peek()=='\n')
                break;
        }
    }
    infile.close();
    fm_partition(circuit, nets);
    ofile.open("output.txt");
    // Output results
    //calculate_gain(circuit,nets);
    
    for (int i = 0; i < numOfNode; i++) {
        ofile<<circuit[i].part << endl;
        }
    ofile.close();
    //cout<<start_time-clock();
    //system("pause");
    return 0;
}
   

int find_partial_sum(vector <Node> & circuit){
    int sum=0;
    for(int i=0;i<circuit.size();i++){
        sum=circuit[i].gain+sum;
    }
    return sum;
}
void calculate_gain(vector <Node> & circuit,vector<Net> & nets){
    for( int i=0;i<circuit.size();i++){                                                             //遍歷circuit
        if(!circuit[i].locked){
            int count=0;
            for( int j=0;j<circuit[i].connected_nets.size();j++){                                       //遍歷每個Node連接的net
                int count2=0;
                int count3=0;
                for( int k=0;k<nets[circuit[i].connected_nets[j]].connected_nodes.size();k++){          //遍歷net上的node
                    if(circuit[i].part != circuit[nets[circuit[i].connected_nets[j]].connected_nodes[k]-1].part){
                        count2++;
                    }
                    else if(circuit[i].part == circuit[nets[circuit[i].connected_nets[j]].connected_nodes[k]-1].part){
                        count3++;
                    }
                }
                if(count2==nets[circuit[i].connected_nets[j]].connected_nodes.size()-1){
                    count++;
                }
                if(count3==nets[circuit[i].connected_nets[j]].connected_nodes.size()){
                    count=count-1;
                }
                
            }
            circuit[i].gain=count;
        }
    }
}

void up_date_gain(vector <Node> & circuit,vector<Net> & nets,int moved_node){
    vector<int> node_to_node;
    for(int i=0;i<circuit[moved_node].connected_nets.size();i++)                      //遍歷移動的node上的net
        for(int j=0;j<nets[circuit[moved_node].connected_nets[i]].connected_nodes.size();j++) {         //遍歷每條net上的node 
            node_to_node.push_back(nets[circuit[moved_node].connected_nets[i]].connected_nodes[j]-1);           
        }
    for(int i=0;i<node_to_node.size();i++){
        if(!circuit[node_to_node[i]].locked){
            int count=0;
            for(int j=0;j<circuit[node_to_node[i]].connected_nets.size();j++){
                int count2=0;
                int count3=0;
                for(int k=0;k<nets[circuit[node_to_node[i]].connected_nets[j]].connected_nodes.size();k++){
                    if(circuit[node_to_node[i]].part!=circuit[nets[circuit[node_to_node[i]].connected_nets[j]].connected_nodes[k]-1].part)
                        count2++;
                    else if(circuit[node_to_node[i]].part==circuit[nets[circuit[node_to_node[i]].connected_nets[j]].connected_nodes[k]-1].part)
                        count3++;
                }
                if(count2==nets[circuit[node_to_node[i]].connected_nets[j]].connected_nodes.size()-1)
                    count++;
                if(count3==nets[circuit[node_to_node[i]].connected_nets[j]].connected_nodes.size())
                    count--;
            }
            circuit[node_to_node[i]].gain=count;
        }
    }
    
}


int find_max_gain_node(vector <Node> & circuit){
    int num_lnode=0;                            //left is 0, reght is 1
    int num_rnode=0;
    bool check=true;
    int lmax=0;
    int rmax=0;
    int count=0;
    for( int i=0;i<circuit.size();i++){     //遍歷每個node
        check=check&&circuit[i].locked;
        if(circuit[i].part==0)                      //計算左邊node數
            count++;
        if(circuit[i].part==0 && circuit[i].gain>= lmax && !circuit[i].locked){     
            lmax=circuit[i].gain;
            num_lnode=i;
        }
        else if(circuit[i].part==1 && circuit[i].gain>=rmax && !circuit[i].locked){
            num_rnode=i;      
            rmax=circuit[i].gain;
        }  
    }
    float ltr=(float)(count-1)/circuit.size();
    float rtl=(float)(count+1)/circuit.size();
    if(circuit.size()<10){
        if(lmax>=rmax&&(ltr>=0.4)&&lmax>=0){
            circuit[num_lnode].locked=1;
            circuit[num_lnode].part=1;
            return num_lnode;
        }
        else if(lmax>=rmax&&rmax>=0){
            circuit[num_rnode].locked=1;
            circuit[num_rnode].part=0;
            return num_rnode;
        } 

        else if(lmax<rmax&&(rtl<=0.6)&&rmax>=0){
            circuit[num_rnode].locked=1;
            circuit[num_rnode].part=0;
            return num_rnode;
            
        }
        else if(lmax<rmax&&lmax>=0){
            circuit[num_lnode].locked=1;
            circuit[num_lnode].part=1;
            return num_lnode;
        }
        
        else if(check)
            return 2147383647;
        else 
            return -1;
    }
    else{
        if(lmax>=rmax){
            if((ltr>=0.45)&&lmax>=0){
                //cout<<'a';
                circuit[num_lnode].locked=1;
                circuit[num_lnode].part=1;
                return num_lnode;
            }
            else if(rmax>=0){
                //cout<<'b';
                circuit[num_rnode].locked=1;
                circuit[num_rnode].part=0;
                return num_rnode;
            } 
        }
        else if(lmax<rmax){
            if((rtl<=0.55)&&rmax>=0){
                //cout<<'c';
                circuit[num_rnode].locked=1;
                circuit[num_rnode].part=0;
                return num_rnode;
            
            }
            else if(lmax<=0){
                //cout<<'d';
                circuit[num_lnode].locked=1;
                circuit[num_lnode].part=1;
                return num_lnode;
            }
        }
        else if(check){
            //cout<<'e';
            return 2147383647;
        }
        else {
            //cout<<'f';
            return -1;
        }
    }
}


void unlocked(vector <Node> & circuit,vector <Net>& nets){
    for(unsigned int i=0;i<circuit.size();i++){
        circuit[i].locked=false;
    }
    calculate_gain(circuit,nets);
}

void fm_partition(vector <Node> & circuit,vector<Net> & nets){
    clock_t start_time,end_time;
    int sup=2147383647;
    int moved_node;          
    int j=0;
    start_time=clock();
    calculate_gain(circuit,nets);
    vector <int> temp(circuit.size());
    while(j<circuit.size()){
        j++;
        //cout<<'g';
        moved_node=find_max_gain_node(circuit);
        if(moved_node!=-1&&moved_node!=2147383647){
            up_date_gain(circuit,nets,moved_node);
            if(sup>find_partial_sum(circuit)){
                sup=find_partial_sum(circuit);
                for(int i=0;i<circuit.size();i++){
                    temp[i]=circuit[i].part;
                }
            }
        }

        else if(moved_node==-1){
            for( int i=0;i<circuit.size();i++){
                temp[i]=circuit[i].part;
            }
            break;

        }
        end_time=clock();
        if(end_time-start_time >29200000 || moved_node==2147383647){
            for( int i=0;i<circuit.size();i++){
                circuit[i].part=temp[i];
            } 
            break;
        }
    }
}               