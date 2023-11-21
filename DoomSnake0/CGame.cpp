
#define _CRT_SECURE_NO_WARNINGS

#include "CGame.h"

#include <iostream>
#include <cstring>
#include <conio.h>
#include <string>

const char BORDER = '#';    // символ для рисования рамки игрового поля
const char FOOD = '$';      // символ для вывода еды
const char *recordFormatStr = "%-20s  %9.4f  %4u  %7.2f  %s"; // форматная строка для форматирования результата игры
//const char* recordFormatStr = "%-20.4f  %4u  %7.2f  %s"; // форматная строка для форматирования результата игры

const int grow_size = 40; // размер увеличения змеи после простой еды
const char* top10_filename = "snake.txt";   // имя файла для хранения 10 лучших результатов

SRecord::SRecord() {
    sname = "";
    rating = 0.0;
    length = 0;
    game_time = 0;
    date = static_cast<time_t>(0);
}

void SRecord::as_string(char *buffer) {
    sprintf(buffer, recordFormatStr, sname.c_str(), rating, length, game_time, ctime(&date));
}

ostream& operator << (ostream& os, const SRecord& rec) {
    os
        << rec.rating << ' '
        << rec.length << ' '
        << rec.game_time << ' '
        << rec.date << ' '
        << rec.sname << endl;
    return os;
}

istream& operator >> (istream& is, SRecord& rec) {
    is
        >> rec.rating
        >> rec.length
        >> rec.game_time
        >> rec.date
        >> rec.sname;
    return is;
}

// Функция сравнения результатов по рейтингу
int rec_compare(const void *_op1, const void *_op2) {
    const SRecord *op1 = reinterpret_cast<const SRecord *>(_op1);
    const SRecord *op2 = reinterpret_cast<const SRecord *>(_op2);
    return static_cast<int>(op2->rating - op1->rating);
}

/*------------------------------------------*/

// очистка буфера клавиатуры
void clearkeys() {
    while (_kbhit())
        _getch();
}

// Конструктор
CGame::CGame(int _width, int _height, int _latency) :
    width(_width), height(_height), latency(_latency) {

    srand(static_cast<unsigned int>(time(NULL)));

    duration_game = 0;
    rating = 0.0;

    // инициализация таблицы команд управления игрой
    cmd_table[0] = CmdPair(27, CMD_EXIT);   // escape key
    cmd_table[1] = CmdPair('K', CMD_LEFT);  // стрелка влево
    cmd_table[2] = CmdPair('M', CMD_RIGHT); // стрелка вправо
    cmd_table[3] = CmdPair('H', CMD_UP);    // стрелка вверх
    cmd_table[4] = CmdPair('P', CMD_DOWN);  // стрелка вниз
}

CGame::Command CGame::get_command() {
    int ch;

    ch = _getch();
    if (ch == 0 || ch == 0xe0) {
        ch = _getch();
    }

    for (int i = 0; i < 5; i++) {
        if (cmd_table[i].first == ch) {
            return cmd_table[i].second;
        }
    }
    return CMD_NOCOMMAND;
}

// Координата еды вычисляется случайным образом.
// Ограничение: координата не должна попадать в тело змеи.
SCoord CGame::make_food() {
    SCoord food;
    do {
        food.x = rand() % (width - 2) + 1;
        food.y = rand() % (height - 2) + 1;
    } while (snake.into(food));

    return food;
}


void CGame::draw_field() {
    cls();
    for (int y = 0; y < height; y++) {
        if (y == 0 || y == height - 1) {
            for (int x = 0; x < width; x++)
                pos(x, y, BORDER);
        }
        else {
            pos(0, y, BORDER);
            pos(width - 1, y, BORDER);
        }
    }
    pos(0, height);
    _cprintf("Length: ****  Rating: ****.**** (****.****)  Time: ****.**");
}

void CGame::print_stat() {
    pos(8, height);
    _cprintf("%04u", snake.size());
    pos(22, height);
    _cprintf("%09.4f", rating);
    pos(33, height);
    _cprintf("%09.4f", rating_i);
    pos(51, height);
    _cprintf("%07.2f", duration_game);
}

void CGame::top10_table() {
    cls();
    char buf[80];
    pos_string(width / 2 - 22, 2, "~~~~~~~~~~~~~~~|    T O P   10    |~~~~~~~~~~~~~~~");
    pos_string(5, 4, "Name\t\t    Rating    Length  Time   Date");
    for (int i = 0; i < 10; i++) {
        ttop10[i].as_string(buf);
        pos_string(5, 5 + i, buf);
    }
}

void CGame::top10(bool after_game) {
    string current_sname;
    top10_table();      // показать таблицу 10 лучших результатов
    time_t date = time(NULL);
    if (after_game) {
        // если игра была сыграна, то показать текущий результат
        pos(5, 16);
        _cprintf(recordFormatStr, "Your result", rating, snake.size(), duration_game, ctime(&date));
    }

    if (rating > ttop10[9].rating) {    // если рейтинг игры больше, чем меньший из 10 лучших...
        // запросить имя игрока
        pos_string(5, 20, "Your name: _");
        pos(16, 20);
        cin >> current_sname;
        clearkeys();
        //OemToCharBuffW(buf, buf_encoded, static_cast<DWORD>(NAMELENGTH));
        // заменить последнюю запись в таблице 10 лучших результатов
        //strcpy(ttop10[9].name, buf_encoded);
        ttop10[9].sname = current_sname;
        ttop10[9].date = date;
        ttop10[9].game_time = duration_game;
        ttop10[9].length = snake.size();
        ttop10[9].rating = rating;
        // отсортировать результаты по убыванию
        qsort(ttop10, 10, sizeof(SRecord), rec_compare);
        // обновить таблицу на экране
        top10_table();
        // обновить файл с 10 лучшими результатами
        write_top10();
    }
}

void CGame::pak(int y) {
    pos_string(width / 2 - 13, y, "Press any key for continue...");
    _getch();
    clearkeys();
}

bool CGame::once_more() {
    pos_string(width / 2 - 15, height - 3, "Press any key to play again...");
    pos_string(width / 2 - 12, height - 2, "(Or press ESC to exit)");

    int ch = _getch();
    clearkeys();
    if (ch == 27)
        return false;
    return true;
}

void CGame::read_top10() {
    ifstream fin(top10_filename);
    if (fin) {
        for (int i = 0; i < 10; i++)
            fin >> ttop10[i];
    }
    fin.close();
}

void CGame::write_top10() {
    ofstream fout(top10_filename);
    if (fout) {
        for (int i = 0; i < 10; i++)
            fout << ttop10[i];
    }
    fout.close();
}

void CGame::logo() {
    mciSendString(L"play doomSnakeSoundtrack.mp3", NULL, 0, NULL);
    pos_string(width / 2 - 4, 10, "D O O M");
    pos_string(width / 2 - 7, 12, "S  N  A  K  E");
    pos_string(width / 2 - 16, height - 2, "2022 copyright David M. Antonyan");
    pak(22);
}

void CGame::goodbye() {
    cls();
    pos_string(width / 2 - 9, 10, "G O O D B Y E !");
    pos_string(width / 2 - 15, 12, "S E E    Y O U    S O O N !");
    for (int i = 0; i < 3; i++) {
        pos_string(width / 2 - 20, 20, "This window will be closed in " + to_string(3 - i) + " seconds");
        Sleep(1000);
    }
}


void CGame::play() { //процесс игры
    mciSendString(L"play doomSnakeSoundtrack.mp3", NULL, 0, NULL);
    duration_game = 0;
    rating = rating_i = 0.0;
    CFood* food = nullptr;
    food = new CSweet; // первая еда - конфета

    draw_field();           // нарисовать игровое поле
    snake.reset(SCoord(width / 2, height / 2));     // установить змею: длина 2, в центре
  
    Command cmd = CMD_NOCOMMAND;
    State stt = STATE_OK;
    // delta  содержит приращение координат (dx, dy) для каждого перемещения змеи по полю
    SCoord delta(-1, 0);                // начальное движение - влево

    snake.draw(*this);                    // первичное рисование змеи
    print_stat();                       // вывести начальную статистику игры
    *food = make_food();          // вычислить координаты еды
    pos(food->x, food->y, FOOD); // вывести еду на экран
    clock_t time1, time2, duration;
    time1 = clock();

    do {

        if (_kbhit())                   // если в буфере клавиатуры есть информация,
            cmd = get_command();        // то принять команду

        // обработка команд
        switch (cmd) {
        case CMD_LEFT:
            delta = SCoord(-1, 0);
            break;
        case CMD_RIGHT:
            delta = SCoord(1, 0);
            break;
        case CMD_UP:
            delta = SCoord(0, -1);
            break;
        case CMD_DOWN:
            delta = SCoord(0, 1);
            break;
        case CMD_EXIT:
            stt = STATE_EXIT;
        default:
            break;
        };

        SCoord hd = snake.head();       // координата головы змеи
        hd += delta;                    // координата головы змеи после приращения (следующая позиция)
        // если голова змеи столкнулась с границей поля или со телом змеи, то змея умирает
        if (hd.x == 0 || hd.x == width - 1 || hd.y == 0 || hd.y == height - 1 || snake.into(hd))
            stt = STATE_DIED;

        if (stt == STATE_OK) {          // если змея ещё жива, то
            snake.move(delta, *this);     // сдвинуть змею на delta
            if (snake.head() == *food) { // если координата головы змеи совпадает с координатой еды, то
                snake.grow(*food, food->eaten());    // увеличить длину змеи в зависимости от съеденной еды
                // если еда несъедобная, змея не вырастит.
                delete food;
                rand() % 4 == 0 ? food = new CPoison : food = new CSweet;
                *food = make_food();     // вычислить координаты новой еды
                pos(food->x, food->y, FOOD); // вывести еду на экран
                // Вычисление времени игры, частичного и общего рейтинга.
                // Частичный рейтинг вычисляется как длина змеи, делённая на время в секундах,
                // затраченное на подход к еде (время от съедения предыдущей еды до съедения следующей).
                // Чем чаще змея ест и чем она длиннее, тем выше частичный рейтинг.
                time2 = clock();
                duration = time2 - time1;
                duration_game += static_cast<double>(duration) / CLOCKS_PER_SEC;
                rating_i = static_cast<double>(snake.size()) / duration * CLOCKS_PER_SEC;
                rating += rating_i;     // общий рейтинг - сумма частичных рейтингов за игру
                time1 = time2;

                print_stat();           // вывод текущей статистики игры
            }

            Sleep(latency);             // задержка перед следующим изменением позиции
        }

    } while (stt == STATE_OK);          // играем, пока змея жива

    pos_string(width / 2 - 8, 10, " G A M E    O V E R ");
    clearkeys();
    _getch();
    clearkeys();
    delete food;
}


