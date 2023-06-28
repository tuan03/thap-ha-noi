#pragma once
#include<iostream>
enum Name_Error{
    CAN_NOT_INIT_SDL,
    CAN_NOT_INIT_IMG,
    CAN_NOT_INIT_TTF,
    CAN_NOT_OPEN_FONT,
    CAN_NOT_CREATE_WINDOW,
    CAN_NOT_CREATE_RENDERER,
    CAN_NOT_GET_IMG,
    CAN_NOT_CREATE_TEXTURE,
    UNKNOWN_RENDERER,
    NOT_CONNECTED
};

class Error{
    public:
    Name_Error type;
    Error(Name_Error type){
        this->type = type;
    }
    std::string getMessage(){
        switch (type)
        {
        case CAN_NOT_INIT_SDL:
            return "Khong the khoi tao SDL";
        case CAN_NOT_INIT_IMG:
            return "Khong the khoi tao IMG";
        case CAN_NOT_INIT_TTF:
            return "Khong the khoi tao TTF";
        case CAN_NOT_CREATE_WINDOW:
            return "Khong the tao Window";
        case CAN_NOT_CREATE_RENDERER:
            return "Khong the tao Renderer";
        case CAN_NOT_OPEN_FONT:
            return "Khong the mo Font";
        case CAN_NOT_GET_IMG:
            return "Lay anh that bai";
        case UNKNOWN_RENDERER:
            return "Chua ket noi renderer";
        case NOT_CONNECTED:
            return "Chua ket noi Stack";
        default:
            return "Loi Khong Xac Dinh";
        }
    }
    
};
