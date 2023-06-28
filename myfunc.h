#pragma once
#include"render_lib_NAT.h"
#include<string>

//  int time_dl = 80;
#define BOX_HEIGHT 50
#define COT_WIDTH 40
#define X_COT_1 300
#define Y_COT_1 200


int getWidthBoxOfN(int n){
    return 100+(50*(n-1));
}
int getCotHeight(int n){
    return 100 + (n*50);
}
int getXCot(int n, int cot){
    return X_COT_1 + (getWidthBoxOfN(n) + 50)*(cot-1);
}
int getCenter(int stt, int cot, int n){
    return getXCot(n,cot) + 20 - (getWidthBoxOfN(stt) /2);
}

class Stack {
public:
    Box* cot = nullptr; // box cột, nắm vị trí của cột
    Box* arr[20];
    int top;

    void set(Box* cot) {
        if(cot == nullptr) throw "Error Stack::set - cot is NULL";
        this->cot = cot;
        top = -1;
    }
    int get_x_block_for_center(Box* box){
        return cot->get_rect().x + 20 - (box->get_rect().w /2);
    }
    int get_y_first_block(){
        return cot->get_rect().y+cot->get_rect().h - BOX_HEIGHT*(top+2);
    }
    int get_x_cot(){
        return cot->get_rect().x;
    }
    Box* get_first_block(){
        if(!isEmpty())
            return arr[top];
        else 
            return nullptr;
    }
    bool push(Box* x) {
        if(this->isFull()){
            return false;
        }
            arr[++top] = x;
            return true;
    }
    void reset(){
        top=-1;
    }
    Box* pop() {
        if (top >= 0) {
            Box* x = arr[top--];
            return x;
        }
        return nullptr;
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == 20;
    }
};

class Animation{
    public:
    int step = 0; //1 : up, 2: Ngang, 3: down, 0: done
    bool run = false;
    Box* box = nullptr;
    Stack* from = nullptr;
    Stack* cott = nullptr;
    int* count = nullptr;
    int time = 0;
    int x1,x2,y1,y2;
    void set_animation(Box* boxx, Stack* from,Stack* cot, int time_move, int * count){
        if(boxx == nullptr){
            throw "Truyen boxx null vao ham set_animation";
        } else
        if(!run){
            this->count = count;
        time = time_move;
        cott = cot;
        box = boxx;
        this->from = from;
        x1=boxx->get_rect().x;
        y1=100;
        x2=cot->get_x_block_for_center(boxx);
        y2=cot->get_y_first_block();
        run = true;
        } 
    }
    void reset(){
        step = 0; //1 : up, 2: Ngang, 3: down, 0: done
        run = false;
        box = nullptr;
        from = nullptr;
        cott = nullptr;
        count = nullptr;
        time = 0;
    }
    void runner(){
        if(run){
        if(step == 0){
            box->set_animation(x1,y1,time/3,"UP");
            step = 1;
        }
        if(step == 1 && box->handle_exec_completed() && box->get_mess() == "UP"){
            box->reset_mess();
            box->set_animation(x2,y1,time/3,"NGANG");
            step = 2;
        }
        if(step == 2 && box->handle_exec_completed() && box->get_mess() == "NGANG"){
            box->reset_mess();
            box->set_animation(x2,y2,time/3,"DOWN");
            step = 3;
        }
        if(step == 3 && box->handle_exec_completed() && box->get_mess() == "DOWN"){
            box->reset_mess();
            step = 0;
            run = false;
            this->from->pop();
            cott->push(box);
            (*count)++;
        }
        }
    }
    bool is_runner(){
        return step != 0 && run;
    }
};

/*
    ListBlock vẫn còn phụ thuộc vào vt cột, mỗi lần up hoặc down phải loop lại để update - > không tôi ưu cho lắm
    
*/
class List_Block{
    public:
    Box* datas[11];
    int n;
    Box* box;
    List_Block(Box* box){
        this->box = box;
        n = 0;
    }
    void set(int n){
        this->reset();
        for(int i = 1; i<= n; i++){
            this->up(false);
        }
    }
    void reset(){
        for (int i = 1; i <= n; i++) {
            delete datas[i]; 
        }
        n=0;
    }
    void init_default(){
        int nn = this->n;
        this->reset();
        for(int i = 1; i<= nn; i++){
            this->up(false);
        }
    }
    void up(bool type = true){ // tăng số block, false: cho this->set, defauld cho child
        if(n<10){
        n++;
        datas[n] = new Box;
        datas[n]->create_from(box);
        datas[n]->set_rect(getCenter(n,1,n),200+n*50,getWidthBoxOfN(n),BOX_HEIGHT);
        }
        if(type) this->update();
    }
    void down(){ //giảm số block
        if (n > 1) {
            delete datas[n--];
        }
        this->update();
    }
    void update(){ // update vị trí 
        for(int i = 1; i<= n; i++){
        datas[i]->set_rect(getCenter(i,1,n),200+i*50,getWidthBoxOfN(i),BOX_HEIGHT);
        }
    }
    void render(MyScreen& screen, bool is_pause ){
        for(int i = 1; i<= n ; i++){
            datas[i]->render(screen.get_my_renderer(),is_pause);
        }
    }
    Box* stt(int index){
        return datas[index];
    }
    ~List_Block(){
        for (int i = 1; i <= n; i++) {
            delete datas[i]; 
            std::cout<<"xoa box\n";
        }
    }
};
class List_Cot{
    public:
    Stack cot1;
    Stack cot2;
    Stack cot3;
    ListBox* list_cot;

    List_Cot(Box& cot,ListBox& list_cot, int n){
        this->list_cot = &list_cot;
        SDL_Rect c;
        for(int i=1; i<=3;i++){
            c = {getXCot(n,i),Y_COT_1,40,50+n*50};
            list_cot.insert_clone(&cot,c);
        }
        cot1.set(list_cot[0]);
        cot2.set(list_cot[1]);
        cot3.set(list_cot[2]);
    }
    void reset_cot(int n, List_Block& list){ 
        SDL_Rect c;
        for(int i=1; i<=3;i++){
        c = {getXCot(n,i),Y_COT_1,40,50+n*50};
        (*list_cot)[i-1]->set_rect(c);
        }
        cot1.reset();
        cot2.reset();
        cot3.reset();
        for(int i=n; i>=1;i--){
            cot1.push(list.stt(i));
        }
    }
    void render(){
        list_cot->render_list();
    }
};

enum Name_Cot{
    COT_1 = 0,
    COT_2,
    COT_3
};
class My_Render: public List_Cot, public List_Block{
    public:
        int n;
        Box* box = nullptr;
        bool is_default = true;
        bool is_pause = true;
        int time_move = 500;
        My_Render(Box& box_cot, ListBox& lb, Box* box_block, int n=8): List_Cot(box_cot,lb,n), List_Block(box_block){
            List_Block::set(n);
            this->box = box_block;
            this->n = n;
            List_Cot::reset_cot(n,*this);
        }
        void render(MyScreen& screen){
            List_Cot::render();
            List_Block::render(screen, is_pause);
        }
        void up(){
            if(is_default)
        if(n<10){
            this->n++;
            List_Block::up();
            List_Cot::reset_cot(n,*this);
        }
        }
        void down(){
            if(is_default)
        if(n>1){
            this->n--;
            List_Block::down();
            List_Cot::reset_cot(n,*this);
        }
        }
        void reset_defaultt(){
            if(!is_default){
                List_Block::init_default();
                List_Cot::reset_cot(n,*this);
            }
        }
        Box* get_first_box_from_Name_Cot(Name_Cot name){
            switch(name){
                case COT_1:
                    return List_Cot::cot1.get_first_block();
                case COT_2:
                    return List_Cot::cot2.get_first_block();
                case COT_3:
                    return List_Cot::cot3.get_first_block();

            }
            return nullptr;
        }
        Stack* get_cot(Name_Cot name){
            switch(name){
                case COT_1:
                    return &(List_Cot::cot1);
                case COT_2:
                    return &(List_Cot::cot2);
                case COT_3:
                    return &(List_Cot::cot3);
            }
            return nullptr;
        }
        void move(Name_Cot from,Name_Cot to,Animation& am, int*count){
            if(!(am.is_runner())){
            Box* box_from = get_first_box_from_Name_Cot(from);
            Stack* stack_from = get_cot(from);
            Stack* stack_to = get_cot(to);
            if(!(stack_from->isEmpty()) && !(stack_to->isFull())){
                is_default = false;
            am.set_animation( box_from,stack_from,stack_to, this->time_move, count);
            }
            }
        }

        

};