#pragma once
#include <time.h>
#include <fstream>
#include <utility>
#include <string>
#include "CScreen.h"
#include "CSnake.h"
#include "SCoord.h"

using namespace std;

struct SRecord  // Структура для хранения результата игры
{ 
    string sname;               // имя игрока
    double rating;              // рейтинг
    int length;                 // длина змеи
    double game_time;           // время игры
    time_t date;                // дата и время окончания игры
    SRecord();
    void as_string(char *buffer);   // отформатированная строка результата
};
class CGame : public CScreen { //класс игры
public:
    CGame(int _width = 80, int _height = 24, int _latency = 100);
    void play();           // процесс игры
    void top10(bool after_game); // работа с таблицей 10 лучших результатов
    bool once_more();           // ожидание сообщения о повторе или выходе
    void pak(int y);            // "Press any key for continue..."
    void read_top10();          // чтение из файла таблицы 10 лучших результатов
    void write_top10();         // запись в файл таблицы 10 лучших результатов
    void logo();                // вывод заставки игры
    void goodbye();             // прощание с игроком, конец игры
private:
    enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
    enum State { STATE_OK, STATE_EXIT, STATE_DIED };
    typedef pair<int, Command> CmdPair;
    CmdPair cmd_table[5];   // таблица команд управления игрой
    SRecord ttop10[10];     // таблица 10 лучших результатов

    Command get_command();  // приём команды с клавиатуры
    int width, height;      // ширина и высота игрового поля
    int latency;            // задержка между изменением позиции в миллисекундах
    CSnake snake;           // змейка
    double duration_game;   // длительность игры
    double rating, rating_i; // рейтинг итоговый и частичный
    void draw_field();      // прорисовка игрового поля
    SCoord make_food();     // вычисление позиции для еды
    void print_stat();      // вывод текущей статистики ниже игрового поля
    void top10_table();     // вывод таблицы 10 лучших результатов
};
