#include"thaphanoi.h"
#undef main
int main(){try{
    MyScreen screen(1800,900,"Nguyễn Anh Tuấn - 090 - Mô Phỏng Tháp Hà Nội","Arial.ttf",26); // tạo 1 màn hình

    ListBox list_func; //danh sách sách nút pause, run, reset, + , - (tạo danh sách render)

    list_func.connect_my_renderer(screen.get_my_renderer());
    SDL_Rect rect_pause = {300,800,100,50};  
    list_func.insert("img\\pause.png",rect_pause)->set_name_box(Name_Box::PAUSE);
    SDL_Rect rect_run = {450,800,100,50};  
    list_func.insert("img\\run.png",rect_run)->set_name_box(Name_Box::RUN);
    SDL_Rect rect_reset = {600,800,100,50};  
    list_func.insert("img\\reset.png",rect_reset)->set_name_box(Name_Box::RESET);
    SDL_Rect rect_up = {750,800,100,50};  
    list_func.insert("img\\plus.png",rect_up)->set_name_box(Name_Box::TANG);
    SDL_Rect rect_down = {900,800,100,50};  
    list_func.insert("img\\sub.png",rect_down)->set_name_box(Name_Box::GIAM);
    



    Box block; 
    block.create("img\\block.png",screen.get_my_renderer());

    ListBox list_cot;
    list_cot.connect_my_renderer(screen.get_my_renderer());
    Box cot;
    cot.create("img\\cot.png",screen.get_my_renderer());

    Animation exec_animation;

    My_Render NAT(cot,list_cot,&block,8);

    My_Game mygame;

    


    // text color RED, GREEN, BLUE, ALPHA
    SDL_Color cl_text{0,0,0,0};
    // data text render 
    std::string text_timemove = "Time Delay ::: " + std::to_string(NAT.time_move);
    SDL_Rect vt_text_timemove{1000,860,0,0};
    //
    std::string text_sodia = "So dia ::: " + std::to_string(NAT.n);
    SDL_Rect vt_text_sodia{200,860,0,0};
    //
    int count = 0;
    std::string text = "So lan move ::: " + std::to_string(count);
    SDL_Rect vt_text{700,860,0,0};
    


    bool quit = false; // điều kiện thoát chương trình
    SDL_Event e;
    Uint32 timeCurrent = SDL_GetTicks();
    Uint32 timePre = SDL_GetTicks();

    int time_delay_click = 450; // tạo delay cho mỗi lần click các nút


    bool is_start = true;
    Uint32 time_last_click = 0;
    Box* getClick = nullptr; // lấy box mình đã click
    //  mỗi lần thực hiện sự kiện SDL_MOUSEBUTTONDOWN
    //
    while (!quit){
        while (SDL_PollEvent(&e) != 0)  {
            switch (e.type) {
                case SDL_MOUSEBUTTONDOWN: // sự kiện nhấn vào các box
                if(SDL_GetTicks() - time_last_click > time_delay_click ){
                    getClick = list_func.checkClick(e.button.x,e.button.y);
                    if(getClick!=nullptr){
                        switch(getClick->get_name_box()){
                            case TANG: // nhấn dấu +
                                NAT.up();
                                break;
                            case GIAM: // dấu trừ -
                                NAT.down();
                                break;
                            case RESET: // nút RESET
                                NAT.reset_defaultt();
                                NAT.is_default = true;
                                NAT.is_pause = true;
                                is_start = true;
                                exec_animation.reset();
                                count = 0;
                                break;
                            case RUN: // nút RUN
                                if(is_start){
                                    mygame.reset();
                                    std::ofstream outFile("data.txt", std::ofstream::trunc);
                                    if (outFile.is_open()) {
                                        outFile<<"n = "<<NAT.n<<"\n";
                                        towerOfHanoi_nondq(NAT.n,COT_1,COT_3,COT_2,mygame,outFile);                                        
                                        // towerOfHanoi(NAT.n,COT_1,COT_3,COT_2,mygame,outFile);
                                        outFile.close();
                                        std::cout << "Successfully wrote to file." << std::endl;
                                    }
                                    else {
                                        std::cout << "Unable to open file." << std::endl;
                                    }
                                    is_start = false;
                                }
                                NAT.is_pause = false;
                                break;
                            case PAUSE: // nút pause
                                NAT.is_pause = true;
                                break;
                    }
                    }
                  
                    time_last_click = SDL_GetTicks();
                }
                    break;
                
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_UP){ // nhấn lên thì cộng thời gian delay 100ms
                        NAT.time_move += 100;
                    }
                    // thời gian delay là thời gian thực hiện quá trình dời cột : dời lên, dời ngang, dời xuống ( 1 quá trình dời )
                    // nên quá trình dời lên mất time_move/3, dời ngang mất time_move/3 , dời dọc time_move/3. => tổng time_move
                    if (e.key.keysym.sym == SDLK_DOWN){// nhấn lên thì cộng thời gian delay 100ms
                        if(NAT.time_move > 100)
                        NAT.time_move -= 100;
                    }
                    break;
                case SDL_QUIT: // sự kiện nhất thoát
                    quit = true;
                    break;
            }
            
        }
//render
        SDL_SetRenderDrawColor(screen.get_my_renderer()->get_renderer(), 255, 255, 255, 0);
        SDL_RenderClear(screen.get_my_renderer()->get_renderer());

        exec_animation.runner(); // thực thi animation

        list_func.render_list();//render danh sách sách nút pause, run, reset, + , -  lên màn hình

        mygame.render(exec_animation,NAT,count,NAT.is_pause); 

        text_sodia = "So dia ::: " + std::to_string(NAT.n); //cập nhật số dĩa, mỗi lần bấm + - thì số dĩa sẽ thay đổi
        text = "So lan move ::: " + std::to_string(count);   // cập nhật text render số lần move, mỗi lần move thành công thì count++
        text_timemove = "Time Delay ::: " + std::to_string(NAT.time_move)+"    (- mui ten len: tang, xuong: giam)";
        // start render text 
        screen.render_Text(text,vt_text,cl_text);
        screen.render_Text(text_timemove,vt_text_timemove,cl_text);
        screen.render_Text(text_sodia,vt_text_sodia,cl_text);
        // end render text

        NAT.render(screen); // render các cột và các đĩa



        timeCurrent = SDL_GetTicks();
        SDL_RenderPresent(screen.get_my_renderer()->get_renderer());
        //render

        if(timeCurrent-timePre < TIME_A_FRAME){
            SDL_Delay(TIME_A_FRAME - (timeCurrent-timePre));
        }
        timePre = SDL_GetTicks();
        
    }

    return 0;
}catch(Error& merr){
std::cerr<<merr.getMessage();
}catch(const char* mess){
    std::cerr<<mess;
}
}