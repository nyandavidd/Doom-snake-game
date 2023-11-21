#include "CSnake.h"

const char SNAKE_TAIL = '@';        // символ для отрисовки сегментов тела змеи, кроме головы

CSnake::CSnake() {
    head_mark = '<';
}

void CSnake::reset(SCoord start_pos) // cброс змеи
{
    worm.clear();
    worm.reserve(1000);         
    worm.push_back(start_pos);  
    worm.push_back(start_pos);  
    worm[0].x++;                // <@
}

void CSnake::draw(CScreen& scr) // отображение змеи
{
    unsigned int wsize = worm.size() - 1;
    for (unsigned int i = 0; i < wsize; i++)
        scr.pos(worm[i].x, worm[i].y, SNAKE_TAIL);

    scr.pos(worm[wsize].x, worm[wsize].y, head_mark);
    drawn = worm.size();
}

void CSnake::move(const SCoord& delta, CScreen& scr) //перемещение змеи
{
    // При перемещении змеи перерисовывается только положение головы (и первого сегмента) и положение хвоста. 
    // Остальные сегменты змеи не перерисовываются.
    // Перерисовка хвоста:
    // Длина змеи увеличивается, когда змея растёт (метод grow()),
    // но змея на экране не изменяется. Поэтому, если отрисованная длина змеи
    // совпадает с реальной длиной, то на экране затирается последний сегмент змеи (хвост).
    // В противном случае, хвост остаётся на месте, голова сдвигается на единицу, а отрисованная длина увеличивается.
    if (drawn == worm.size())
        scr.pos(worm[0].x, worm[0].y, ' ');
    else
        drawn++;

    // сдвиг координат в векторе без отрисовки
    for (unsigned int i = 1; i < worm.size(); i++)
        worm[i - 1] = worm[i];

    worm[worm.size()-1] += delta;       // координата головы изменяется на приращение

    // выбор символа для отрисовки головы в зависимости от направления движения
    if (delta.x < 0)
        head_mark = '<';
    else if (delta.x > 0)
        head_mark = '>';
    else if (delta.y < 0)
        head_mark = 'A';
    else // (delta.y > 0)
        head_mark = 'V';

    // Перерисовка головы и первого сегмента змеи
    scr.pos(worm[worm.size() - 1].x, worm[worm.size() - 1].y, head_mark);
    scr.pos(worm[worm.size() - 2].x, worm[worm.size() - 2].y, SNAKE_TAIL);
}

void CSnake::grow(const SCoord& pos, int growbits) {
    for (int i = 0; i < growbits; ++i)
        worm.insert(worm.begin(), pos);
}

bool CSnake::into(const SCoord& pos) // проверка попадания координаты в тело змеи
{ 
    for (unsigned int i = 0; i < worm.size(); i++)
        if (worm[i].x == pos.x && worm[i].y == pos.y)
            return true;
    return false;
}

SCoord CSnake::head() //возврат координат головы 
{
    return worm[worm.size() - 1];
}

int CSnake::size() {
    return static_cast<int>(worm.size());
}
