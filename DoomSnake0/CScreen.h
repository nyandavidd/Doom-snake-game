#pragma once

#include <windows.h>
#include <string>

/*
Класс исключений консоли cscreen_exception
*/

class cscreen_exception {
public:
    cscreen_exception(int err) : err(err) {}
    int err;                    // код ошибки
    const char* what();         // возвращает строку с описанием ошибки
};

/*
Класс CScreen - для системных вызовов для работы с консолью
*/

class CScreen { //класс для работы с консолью
public:
    CScreen();
    ~CScreen();
    void cursor_show(bool visible);                 // показать/скрыть курсор
    void init(WORD attr);                      // установить цвет текста/фона
    void pos(int x, int y, char ch = 0);            // позиционирование курсора и вывод символа, если ch != 0
    void pos_string(int x, int y, std::string str);
    void cls();                                     // очистка экрана
protected:
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO oldCursorInfo, curCursorInfo;
    WORD oldTextAttr;
};

