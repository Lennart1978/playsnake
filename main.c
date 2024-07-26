#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_LENGTH 100
#define SPEED 50000
#define SNAKE_CHAR '#'
#define FOOD_CHAR '0'

typedef struct {
    int x, y;
} Point;

int maxx, maxy, food_x, food_y, score;
Point snake[MAX_LENGTH];
int length = 3;
enum direction { UP, DOWN, LEFT, RIGHT };
enum direction dir = RIGHT;
bool is_eaten = false;

void print_score()
{
    attron(COLOR_PAIR(2));
    mvprintw(maxy - 1, 0, "Score: %3d | Press esc to exit", score);
    attroff(COLOR_PAIR(2));
}

void init_game() {
    score = 0;
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

    maxx = getmaxx(stdscr);
    maxy = getmaxy(stdscr);

    srand(time(NULL));
    snake[0].x = maxx / 4;
    snake[0].y = maxy / 2;
    snake[1].x = snake[0].x - 1;
    snake[1].y = snake[0].y;
    snake[2].x = snake[1].x - 1;
    snake[2].y = snake[1].y;

    food_x = rand() % (maxx - 2) + 1;
    food_y = rand() % (maxy - 2) + 1;
}

void draw_snake() {
    attron(COLOR_PAIR(4));
    for (int i = 0; i < length; i++) {
        mvaddch(snake[i].y, snake[i].x, SNAKE_CHAR);
    }
    attroff(COLOR_PAIR(4));
}

void draw_food() {
    attron(COLOR_PAIR(5));
    mvaddch(food_y, food_x, FOOD_CHAR);
    attroff(COLOR_PAIR(5));
}

void move_snake() {
    for (int i = length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    switch (dir) {
        case UP:
            snake[0].y--;
            break;
        case DOWN:
            snake[0].y++;
            break;
        case LEFT:
            snake[0].x--;
            break;
        case RIGHT:
            snake[0].x++;
            break;
    }
    // if snake reaches the edge, it will come from the other side
   if(snake[0].x >= maxx)
   {
        snake[0].x = 1;
   }
   if(snake[0].x <= 0)
   {
        snake[0].x = maxx - 1;
   }
   if(snake[0].y >= maxy)
   {
        snake[0].y = 1;
   }
   if(snake[0].y <= 0)
   {
        snake[0].y = maxy - 1;
   }
}

bool check_collision() {
    for (int i = 1; i < length; i++) {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
            return true;
        }
    }   
   return false;
}

void check_food() {
    if (snake[0].x == food_x && snake[0].y == food_y) {
        is_eaten = true;
        length++;
        score += 10;
        food_x = rand() % (maxx - 2) + 1;
        food_y = rand() % (maxy - 2) + 1;
    }
}

void handle_input() {
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            if (dir != DOWN) dir = UP;
            break;
        case KEY_DOWN:
            if (dir != UP) dir = DOWN;
            break;
        case KEY_LEFT:
            if (dir != RIGHT) dir = LEFT;
            break;
        case KEY_RIGHT:
            if (dir != LEFT) dir = RIGHT;
            break;
        case 27: // ESC
            endwin();
            exit(0);
        default:
            break;
    }
}

void game_over() {
    clear();
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
    mvprintw(maxy / 2, maxx / 2 - 40, "Game Over! Press ESC to exit. %d Points", score);
    attroff(COLOR_PAIR(4));
    attroff(A_BOLD);
    score = 0;
    length = 3;
    dir = RIGHT;
    refresh();
    sleep(2);
}

int main() {
    init_game();

    while (true) {
        clear();
        print_score();
        handle_input();
        move_snake();
        check_food();
        draw_food();
        draw_snake();
        if (check_collision()) {
            game_over();
            init_game();
            continue;
        }
        refresh();
        usleep(SPEED);
    }

    endwin();
    return 0;
}
