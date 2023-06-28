#pragma once
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"error.h"
#include<cmath>
#define FPS 60
#define TIME_A_FRAME (1000/FPS)
enum Name_Box{
    RESET,
    PAUSE,
    RUN,
    TANG,
    GIAM
};


class SDLInit {
public:
    SDLInit() {
        std::cout<<"Init SDL\n";
        if (SDL_Init(SDL_INIT_VIDEO) != 0) throw Error(Name_Error::CAN_NOT_INIT_SDL);
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) throw Error(Name_Error::CAN_NOT_INIT_IMG);
        if (TTF_Init() != 0) throw Error(Name_Error::CAN_NOT_INIT_TTF);
        
    }
    ~SDLInit() {
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        std::cout<<"Quit SDL\n";
    }
};

class MyWindow{
    private:
    SDL_Window* window = nullptr;
    public:
    MyWindow(const char* nameProgram,int screen_width,int screen_height){
        std::cout<<"Init Window\n";
        this->window = SDL_CreateWindow(nameProgram, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height,SDL_WINDOW_SHOWN);
        if (this->window == nullptr) throw Error(Name_Error::CAN_NOT_CREATE_WINDOW);
        
    }
    SDL_Window* get_window() const {
        return this->window;
    }
    ~MyWindow(){
        SDL_DestroyWindow(window);
        std::cout<<"Destroy Window\n";
    }
};
class MyRenderer{
    private:
    SDL_Renderer* renderer = nullptr;
    public:
    MyRenderer(SDL_Window* window){
        std::cout<<"Init Renderer\n";
        this->renderer = SDL_CreateRenderer(window, -1, 0);
        if (this->renderer == nullptr) throw Error(Name_Error::CAN_NOT_CREATE_RENDERER);
    }
    SDL_Renderer* get_renderer() const {
        return this->renderer;
    }

    ~MyRenderer(){
        SDL_DestroyRenderer(renderer);
        std::cout<<"Destroy Renderer\n";
    }
};
class MyFont{
    private:
    TTF_Font* font ;
    int fontSize;
    std::string fontName;
    public:
    MyFont(const char* nameFileFont, int fontSize){
        std::cout<<"Load Font\n";
        this->font = TTF_OpenFont(nameFileFont, fontSize);
        if(!this->font) throw Error(Name_Error::CAN_NOT_OPEN_FONT);
        this->fontSize=fontSize;
        this->fontName = nameFileFont;
    }
    TTF_Font* get_font() const{
        return font;
    }
    int get_font_size() const {
        return fontSize;
    }
    std::string get_font_name() const {
        return fontName;
    }
    ~MyFont(){
            TTF_CloseFont(font);
            std::cout<<"Destroy Font\n";
    }
};
class MyTexture{
    private:
    SDL_Texture* texture = nullptr;
    public:
    MyTexture(const char* file,SDL_Renderer* renderer) {
        std::cout<<"Create Texture\n";
        SDL_Surface* image = IMG_Load(file);    //load ảnh vào surface
        if (image == nullptr) throw Error(Name_Error::CAN_NOT_GET_IMG);
        this->texture = SDL_CreateTextureFromSurface(renderer, image); 
        SDL_FreeSurface(image); //xóa surface
        if(this->texture == nullptr) throw Error(Name_Error::CAN_NOT_CREATE_TEXTURE);
    }
    SDL_Texture* get_texture() const{
        return this->texture;
    }
    ~MyTexture(){
            SDL_DestroyTexture(this->texture);
            std::cout<<"Delete a texture\n";
    }

};

class MyScreen {
    private:
    SDLInit sdl;    
    MyWindow* mywindow = nullptr;
    MyRenderer* myrenderer = nullptr;
    MyFont* myfont = nullptr;
    int screen_width;       //kích thước màn hình, để sau này resposive sau
    int screen_height;
    public:
    MyScreen(int width, int height,const char * nameProgram, const char * fileFont, int fontSize){
        std::cout<<"Create Screen\n";
        mywindow = new MyWindow(nameProgram,width,height);
        screen_width = width;
        screen_height = height;
        myrenderer = new MyRenderer(mywindow->get_window());
        myfont = new MyFont(fileFont,fontSize);
    }
    ~MyScreen(){      
        delete myfont;
        delete myrenderer;
        delete mywindow;
        std::cout<<"Quit Screen\n";
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
    MyWindow* get_my_window() const {
        return mywindow;
    }
    SDL_Window* get_window() const {
        return mywindow->get_window();
    }
    MyRenderer* get_my_renderer() const{
        return myrenderer;
    }
    SDL_Renderer* get_renderer() const {
        return myrenderer->get_renderer();
    }
    
    void set_font_size(int font_size){
        std::string fontName = this->myfont->get_font_name();
        delete myfont;
        myfont = new MyFont(fontName.c_str(),font_size);
    }
    void render_Text(const std::string& text,SDL_Rect &vitri,SDL_Color text_color,bool type_center_box = false){ // true: in text giữa box vitri, false: in text tại vitri
        SDL_Color textColor = text_color;
        SDL_Surface* textSurface = TTF_RenderText_Solid(myfont->get_font(), text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(myrenderer->get_renderer(), textSurface);

        if(type_center_box == false){
            vitri.w =  (textSurface != NULL ? textSurface->w : vitri.w = 0);
            vitri.h = myfont->get_font_size();
            SDL_RenderCopy(myrenderer->get_renderer(), textTexture, NULL, &vitri);

        } else {
            SDL_Rect vt_render;
            vt_render.w =  (textSurface != NULL ? textSurface->w : vt_render.w = 0);
            vt_render.h = myfont->get_font_size();
            SDL_Rect vt_box = vitri ; //box->get_rect()
            vt_render.x = vt_box.x + (vt_box.w/2) - (vt_render.w/2);
            vt_render.y = vt_box.y + (vt_box.h/2) - (vt_render.h/2);
            SDL_RenderCopy(myrenderer->get_renderer(), textTexture, NULL, &vt_render);
        }
                    SDL_FreeSurface(textSurface);
                    SDL_DestroyTexture(textTexture);
    }

};

class Box{
    private:
    MyTexture* mytexture = nullptr; //chứa texture
    SDL_Rect rect;                  // chứa vị trí, kích thước render
    int p_x;                        //vị trí của x, vị trí thực, vị trí rect sẽ di chuyên đến p_x
    int p_y;                        //vị trí y, như trên
    int time_completed;             //thời gian hoàn thành dịch chuyển
    bool is_clone = false;          //check xem phải là Box clone không, để không cần xóa texture của Box này, vì box được clone xóa rồi
    Name_Box namebox;               //đặt tên cho Box, đễ truy vấn
    Box* next;                      //danh sách liên kết đơn
    MyTexture* convert = nullptr;   //chuyển đổi qua lại 2 texture
    int vantoc_x, vantoc_y;
    std::string message =""; 
    bool exec = false;              // flag, lần khi đang thực thi di chuyển, lần render di chuyển xong thì biến này sẽ là exec. mình sẽ từ đổi nó về false bằng hàm set_exec
    public:
    Box(){
        next = nullptr; 
    }
    ~Box(){
        if(!is_clone) delete mytexture;
        if(convert!=nullptr) delete convert;
        std::cout<<"Delete a box\n";
        delete next;
    }
    int get_px(){
        return this->p_x;
    }
    int get_py(){
        return this->p_y;
    }
    MyTexture* get_my_texture(){
        return mytexture;
    }
    SDL_Rect& get_rect(){
        return this->rect;
    }
    Name_Box get_name_box(){
        return this->namebox;
    }
    Box* set_name_box(Name_Box n){
        this->namebox = n;
        return this;
    }
    Box* get_next(){
        return next;
    }
    Box* set_corvert(const char* file, MyRenderer* renderer){
        this->convert = new MyTexture(file,renderer->get_renderer());
        return this;
    }
    void convert_texture(){
        if(convert != nullptr){
        MyTexture* temp = this->convert;
        convert = mytexture;
        mytexture = temp;}
    }
    void set_next(Box*& box){
        next = box;
    }
    void create(const char * file, MyRenderer* renderer){
        mytexture = new MyTexture(file,renderer->get_renderer());
    }
    void create_from(Box* box){ //clone box
        mytexture = box->get_my_texture();
        this->is_clone = true;
        this->convert = box->convert;
    }
    void create_empty(SDL_Rect &r){ 
        mytexture = nullptr;
        rect = r;
        p_x = r.x;
        p_y = r.y;
    }

    void set_rect(int x, int y, int w, int h){
        this->rect = {x,y,w,h};
        p_x = x;
        p_y = y;
    }
    void set_rect(SDL_Rect& rect){
        this->rect = rect;
        p_x = this->rect.x;
        p_y = this->rect.y;
    }
    std::string get_mess(){
        return this->message;
    }
    void reset_mess(){
        this->message = "";
    }
    void set_animation(int x, int y,int time_completed, std::string mess = ""){
            this->p_x = x;
            this->p_y = y;
                if(time_completed >= TIME_A_FRAME){
            this->time_completed = time_completed;
            } else {
                this->time_completed = TIME_A_FRAME;
            }
            int so_lan_render = (this->time_completed)/(TIME_A_FRAME);
            vantoc_x = std::abs(p_x-rect.x)/so_lan_render;
            vantoc_y = std::abs(p_y-rect.y)/so_lan_render;
            if(mess.length()>0) this->message = mess;

    }
    bool get_exec(){
        return exec;
    }
    void set_exec(bool v){
        this->exec = v;
    }
    void move(){
        
        int khoang_cach_x = std::abs(p_x-rect.x);
        int khoang_cach_y = std::abs(p_y-rect.y);
        if(khoang_cach_x > 0 && vantoc_x == 0){
            vantoc_x = 1;
        }
        if(khoang_cach_x <= vantoc_x){
            vantoc_x = 0;
            rect.x = p_x;
            this->exec = true;
        } else rect.x += (p_x > rect.x ? vantoc_x : -vantoc_x);
        if(khoang_cach_y > 0 && vantoc_y == 0){
            vantoc_y = 1;
        }
        if(khoang_cach_y <= vantoc_y){
            vantoc_y = 0;
            rect.y = p_y;
        } rect.y += (p_y > rect.y ? vantoc_y : -vantoc_y);
    }
    bool done_animation(){
        return p_x == rect.x && p_y == rect.y;
    }

    bool handle_exec_completed(){
        if(this->exec == true && this->done_animation()==true){
            this->exec = false;
            return true;
        }
        return false;
    }



    void render(MyRenderer* renderer,bool is_pause = false , bool choose = true){ //true: render texture chính, false: render convert
        if(choose){
            if(!is_pause && (p_x != rect.x || p_y != rect.y)){
            this->move();}
            if(mytexture!=nullptr)
            SDL_RenderCopy(renderer->get_renderer(),mytexture->get_texture(),nullptr,&this->rect);
        } else {
            if(p_x != rect.x || p_y != rect.y){
            this->move();}
            SDL_RenderCopy(renderer->get_renderer(),convert->get_texture(),nullptr,&this->rect);
        }
    }
    
    bool is_in_box(int x, int y){
        return (x>=this->rect.x) && (x<=this->rect.x+this->rect.w) && (y>=this->rect.y) && (y<=this->rect.y+this->rect.h);
    }
};


class ListBox{
    private:
    Box* head;
    MyRenderer* renderer;
    public:
    ListBox(){
        head = nullptr;
        renderer = nullptr;
    }
    void connect_my_renderer(MyRenderer* renderer){
        this->renderer = renderer;
    }
  
    Box* insert(const char * file, SDL_Rect rect){ // thêm box no name
        //create box
        Box* newBox = new Box;
        if(!renderer) throw Error(Name_Error::UNKNOWN_RENDERER);
        newBox->create(file,this->renderer);
        newBox->set_rect(rect);

        //add
        if(this->head == nullptr){
            this->head = newBox;
            return newBox;
        }
        Box* current = head;
        while (current->get_next() != nullptr)
        {
            current = current->get_next();
        }
        current->set_next(newBox);
        return newBox;
    }
    
    Box* insert_clone(Box* box, SDL_Rect rect){ // clone box, noname
        //create box
        Box* newBox = new Box;
        if(!renderer) throw Error(Name_Error::UNKNOWN_RENDERER);
        newBox->create_from(box);
        newBox->set_rect(rect);
        //add
        if(this->head == nullptr){
            this->head = newBox;
            return newBox;
        }
        Box* current = head;
        while (current->get_next() != nullptr)
        {
            current = current->get_next();
        }
        current->set_next(newBox);
        return newBox;
    }

    void render_list(){ //render list lên cửa sổ
        Box* current = head;
        while (current != NULL) {
            current->render(renderer);
            current = current->get_next();
        }
    }
    Box* checkClick(int x, int y){ //kiểm tra click cái nào ?
        Box* current = this->head;
        while (current != nullptr)
        {
            if(current->is_in_box(x,y))  return current;
            current = current->get_next();
        }
        return nullptr;
        
    }
    ~ListBox(){
        delete head;
        std::cout<<"Delete Listbox\n";
    }
    Box* operator[] (int index){
        Box* current = head;
        int i=0;
        while (current != NULL) {
            if(index == i){
                return current;
            }
            current = current->get_next();
            i++;
        }
        return nullptr;
    }
    Box* operator[] (Name_Box name){
        Box* current = head;
        while (current != NULL) {
            if(current->get_name_box() == name){
                return current;
            }
            current = current->get_next();
        }
        return nullptr;
    }
};