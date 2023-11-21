#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "CScreen.h"
#include "CGame.h"
using namespace std;
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    try {
        CGame game(100, 27, 50);
        game.init((WORD)0x0a);
        game.cursor_show(false);
        game.logo();
        game.read_top10();
        game.top10(false);
        game.pak(18);
        do {
            game.play();
            game.top10(true);
        } while (game.once_more());
        game.goodbye();
        // ����� ����
    } // ����������
    catch (cscreen_exception& ex) {
        cerr  << ex.what() << endl;
    }
    catch (...) {
        system("cls");
        cerr << "another system exception";
    }
    return 0;
}
