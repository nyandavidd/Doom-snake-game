#include "CScreen.h"
#include <conio.h>
#include <string>
#include <iostream>

const char *msgs[] = {
    "",
    "Failed GetStdHandle(): INVALID_HANDLE_VALUE",
    "Failed GetConsoleCursorInfo()",
    "Failed SetConsoleCursorInfo()",
    "Failed SetConsoleCursorPosition()"
};

const char *cscreen_exception::what() {
    return msgs[err];
}

CScreen::CScreen() {
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsoleOutput == INVALID_HANDLE_VALUE)
        throw cscreen_exception(1);    // "INVALID_HANDLE_VALUE"

    if (!GetConsoleCursorInfo(hConsoleOutput, &oldCursorInfo))
        throw cscreen_exception(2);
    curCursorInfo.dwSize = oldCursorInfo.dwSize;
    curCursorInfo.bVisible = oldCursorInfo.bVisible;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
    oldTextAttr = csbi.wAttributes;
}

CScreen::~CScreen() {
    SetConsoleCursorInfo(hConsoleOutput, &oldCursorInfo);
    SetConsoleTextAttribute(hConsoleOutput, oldTextAttr);
}

void CScreen::cursor_show(bool visible) {
    curCursorInfo.bVisible = visible;
    if (!SetConsoleCursorInfo(hConsoleOutput, &curCursorInfo))
        throw cscreen_exception(3);
}

void CScreen::init(WORD attr) {
    SetConsoleTextAttribute(hConsoleOutput, attr);
    MoveWindow(GetConsoleWindow(), 400, 180, 751, 522, FALSE); //изменение размера окна

    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // получаем дескриптор окна
    GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);
    
    COORD new_screen_buffer_size; //избавляемся от прокрутки, устанавливая экран под размер окна:
    new_screen_buffer_size.X = screenBufferInfo.srWindow.Right -
      screenBufferInfo.srWindow.Left + 1;
    new_screen_buffer_size.Y = screenBufferInfo.srWindow.Bottom -
      screenBufferInfo.srWindow.Top + 1;

    SetConsoleScreenBufferSize(hConsole, new_screen_buffer_size);
}

void CScreen::pos(int x, int y, char ch) {
    COORD point;
    point.X = static_cast<SHORT>(x);
    point.Y = static_cast<SHORT>(y);
    if (!SetConsoleCursorPosition(hConsoleOutput, point)) //установили курсор !!!
        throw cscreen_exception(4);
    if (ch > 0)
        _putch(ch); 
}


void CScreen::pos_string(int x, int y, std::string str) {
    pos(x, y);
    std::cout << str;
}

void CScreen::cls() {
    system("cls");
}
