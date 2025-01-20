// Compile using => gcc -o PacMan -std=c99 $(ncursesw6-config --cflags --libs) PacMan.c
// map height = 31, map width = 82

# include <ncurses.h>
# include <locale.h>

const int mapHeight = 31;
const int mapWidth = 82;

struct character {
  int yLoc;
  int xLoc;
  char *icon;
};
struct character pacman;

// Moment Functions
void mvup();
void mvdown();
void mvright();
void mvleft();

// Draw Map Functions
void drawMap(WINDOW *win);

const char map[31][82 + 1] = {
"+ ------------------------------------ +  + ------------------------------------ +",
"|  .  .  .  .  .  .  .  .  .  .  .  .  |  |  .  .  .  .  .  .  .  .  .  .  .  .  |",
"|  .  + ------ +  .  + --------- +  .  |  |  .  + --------- +  .  + ------ +  .  |",
"|  .  |        |  .  |           |  .  |  |  .  |           |  .  |        |  .  |",
"|  .  + ------ +  .  + --------- +  .  +--+  .  + --------- +  .  + ------ +  .  |",
"|  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  |",
"|  .  + ------ +  .  +  +  .  + ------------------ +  .  +  +  .  + ------ +  .  |",
"|  .  + ------ +  .  |  |  .  + ------ +  + ------ +  .  |  |  .  + ------ +  .  |",
"|  .  .  .  .  .  .  |  |  .  .  .  .  |  |  .  .  .  .  |  |  .  .  .  .  .  .  |",
"+ ------------ +  .  |  + ------ +     |  |     + ------ +  |  .  + ------------ +",
"               |  .  |  + ------ +     +--+     + ------ +  |  .  |               ",
"               |  .  |  |                                |  |  .  |               ",
"               |  .  |  |     + ------______------ +     |  |  .  |               ",
"+------------- +  .  +  +     |                    |     +  +  .  + -------------+",
"|  .  .  .  .  .  .           |                    |           .  .  .  .  .  .  |",
"+------------- +  .  +  +     |                    |     +  +  .  + -------------+",
"               |  .  |  |     + ------------------ +     |  |  .  |               ",
"               |  .  |  |                                |  |  .  |               ",
"               |  .  |  |     + ------------------ +     |  |  .  |               ",
"+ ------------ +  .  +  +     + ------ +  + ------ +     +  +  .  + ------------ +",
"|  .  .  .  .  .  .  .  .  .  .  .  .  |  |  .  .  .  .  .  .  .  .  .  .  .  .  |",
"|  .  + ------ +  .  + --------- +  .  |  |  .  + --------- +  .  + ------ +  .  |",
"|  .  + --- +  |  .  + --------- +  .  +--+  .  + --------- +  .  |  + --- +  .  |",
"|  .  .  .  |  |  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  |  |  .  .  .  |",
"+ --- +  .  |  |  .  +  +  .  + ------------------ +  .  +  +  .  |  |  .  + --- +",
"+ --- +  .  +  +  .  |  |  .  + ------ +  + ------ +  .  |  |  .  +  +  .  + --- +",
"|  .  .  .  .  .  .  |  |  .  .  .  .  |  |  .  .  .  .  |  |  .  .  .  .  .  .  |",
"|  .  + ------------ +  + ------ +  .  |  |  .  + ------ +  + ------------ +  .  |",
"|  .  + ------------------------ +  .  +--+  .  + ------------------------ +  .  |",
"|  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  |",
"+ ------------------------------------------------------------------------------ +"
};

// main 
int main(){

  setlocale(LC_ALL, "");
  initscr(); //initalize ncurses and clears screen
  start_color(); //Enable colour output
  {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_color(COLOR_WHITE + 1, 1000, 1000, 400);
    init_pair(2, COLOR_WHITE + 1, COLOR_BLACK);
  }
  noecho(); //hides user inputs
  curs_set(0); //hides curser

  int isRunning = 1;

  //window parameters
  int height = mapHeight + 4, width = mapWidth + 4, start_y = 0, start_x = 0;
  WINDOW *win = newwin(height, width, start_y, start_x);

  refresh();
  box(win, 0, 0);
  mvwprintw(win, 0, 2, " PacMan ᗧ···ᗝ···ᗝ·· ");
  wrefresh(win);

  while(isRunning){
    drawMap(win);
    wrefresh(win);
  }

  getch();

  endwin(); //dealocates memory and exits ncurses
  return 0;
}

void drawMap(WINDOW *win){ 
  for(int x=0; x < mapWidth; x++){
    for(int y=0; y < mapHeight; y++){
      char str[2] = {map[y][x], '\0'};
      if(map[y][x] != '.'){
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y+2, x+2, str);
        wattroff(win, COLOR_PAIR(1));
      }else{
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, y+2, x+2, str);
        wattroff(win, COLOR_PAIR(2));
      }
    }
  }
}
