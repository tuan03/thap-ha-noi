#pragma once
#include"myfunc.h"
#include <fstream>
#include"stack.h"
struct Move {
    Name_Cot from;
    Name_Cot to;
};
class My_Game{
    public:
    Tuan::Queue<Move> q;
    int c = -1;
    void reset(){
        q.~Queue();
        c = -1;
    }
    void push(Name_Cot from_rod, Name_Cot to_rod){
       q.push({from_rod,to_rod});
       c++;
    }
    void render(Animation& am,My_Render& mr,int &count, bool is_pause){
        if(c>=0 && !is_pause){
            if(!am.is_runner()){
                Move temp = q.pop();
                mr.move(temp.from,temp.to,am, &count);
                c--;
            }
        }
    } 
};

std::string getNameCot(Name_Cot name){
    switch (name)
    {
    case COT_1:
        return "Cot A";
    case COT_2:
        return "Cot B";
    case COT_3:
        return "Cot C";
    
    default:
        return "ERROR";
        break;
    }
}

void towerOfHanoi(int n, Name_Cot Nguon, Name_Cot Dich, Name_Cot TrungGian, My_Game& mg,std::ofstream& out) { // nguồn, đích, trung gian
    if(n<1) throw "Error n";
    out<<"Giai thuat THAP HA NOI de quy\n";
    if (n == 1) {
        mg.push(Nguon,Dich);
        out << "Move disk from " <<getNameCot(Nguon) << " to " << getNameCot(Dich) << std::endl;
        return;
    }
    
    towerOfHanoi(n - 1, Nguon, TrungGian, Dich, mg, out);
    mg.push(Nguon,Dich);
    out << "Move disk from " <<getNameCot(Nguon) << " to " << getNameCot(Dich) << std::endl;
    towerOfHanoi(n - 1, TrungGian, Dich, Nguon,mg, out);
}



class Step_Move{
    public:
    int n; // số lượng đĩa cần move
    Name_Cot Nguon;
    Name_Cot Dich;
    Name_Cot TrungGian;
    Step_Move(){

    }
    Step_Move(int n, Name_Cot Nguon, Name_Cot Dich, Name_Cot TrungGian){
        std::cout<<"Call in here\n";
        this->n = n;
        this->Nguon = Nguon;
        this->Dich = Dich;
        this->TrungGian = TrungGian;
    }
    void get(Step_Move stm){
        this->n =stm.n;
        this->Nguon = stm.Nguon;
        this->Dich = stm.Dich;
        this->TrungGian = stm.TrungGian;
    }
    
};
void towerOfHanoi_nondq(int n, Name_Cot Nguon, Name_Cot Dich, Name_Cot TrungGian, My_Game& mg,std::ofstream& out) { // nguồn, đích, trung gian
    if(n<1) throw "Error n";
    out<<"Giai thuat THAP HA NOI Khu de quy\n";
    Tuan::Stack<Step_Move> step;
    Step_Move temp;

    step.push(Step_Move(n,Nguon, Dich, TrungGian)); // di chuyển n đĩa từ nguồn C1 đến đích C3
    while (!step.empty()) {
        temp.get(step.pop()); 

        if (temp.n == 1) { // n là số đĩa cần dời.
            mg.push(temp.Nguon,temp.Dich);
            out << "Move disk from " <<getNameCot(temp.Nguon) << " to " <<getNameCot(temp.Dich) << std::endl;
        }
        else {
            step.push({ temp.n - 1, temp.TrungGian, temp.Dich, temp.Nguon }); // 3: di chuyển n-1 đĩa từ trung gian qua đích
            step.push({ 1, temp.Nguon, temp.Dich, temp.TrungGian });          // 2: di chuyển đĩa trên cùng từ nguồn qua đích (do đã di chuyển n-1 đĩa qua bên trung gian rồi, tại nguồn sẽ còn 1 đĩa, ta sẽ dời đĩa này qua đích)
            step.push({ temp.n - 1, temp.Nguon, temp.TrungGian, temp.Dich }); // 1: di chuyển n-1 từ nguồn qua trugn gian
        }
    }

}


