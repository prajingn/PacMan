// Compile using => gcc -o PacMan -std=c99 $(ncursesw6-config --cflags --libs) PacMan.c
// map height = 31, map width = 82

# include <ncurses.h>
# include <locale.h>
# include <unistd.h>

const int mapHeight = 31;
const int mapWidth = 82;
const int border_padding = 2;

// Directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int isRunning = 1, direction = LEFT;

// charcters structure
struct character {
  int yLoc;
  int xLoc;
  char *icon;
}pacman, red_ghost, yellow_ghost, blue_ghost, pink_ghost;

// input Functions
void input(WINDOW *charWin);

// Draw Map Functions
void drawMap(WINDOW *win);

// Draw Characters function
void drawChar(WINDOW *win);

char map[31][82 + 1] = {
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

const char path_map[31][82 + 1] = {
"+ ------------------------------------ +  + ------------------------------------ +",
"|  pppppppppppppppppppppppppppppppppp  |  |  pppppppppppppppppppppppppppppppppp  |",
"|  p  + ------ +  p  + --------- +  p  |  |  p  + --------- +  p  + ------ +  p  |",
"|  p  |        |  p  |           |  p  |  |  p  |           |  p  |        |  p  |",
"|  p  + ------ +  p  + --------- +  p  +--+  p  + --------- +  p  + ------ +  p  |",
"|  pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp  |",
"|  p  + ------ +  p  +  +  p  + ------------------ +  p  +  +  p  + ------ +  p  |",
"|  p  + ------ +  p  |  |  p  + ------ +  + ------ +  p  |  |  p  + ------ +  p  |",
"|  pppppppppppppppp  |  |  pppppppppp  |  |  pppppppppp  |  |  p pppppppppppppp  |",
"+ ------------ +  p  |  + ------ +  p  |  |  p  + ------ +  |  p  + ------------ +",
"               |  p  |  + ------ +  p  +--+  p  + ------ +  |  p  |               ",
"               |  p  |  |  pppppppppppppppppppppppppppp  |  |  p  |               ",
"               |  p  |  |  p  + ------gggggg------ +  p  |  |  p  |               ",
"+------------- +  p  +  +  p  |  gggggggggggggggg  |  p  +  +  p  + -------------+",
"| pppppppppppppppppppppppppp  |  gggggggggggggggg  |  pppppppppppppppppppppppppp |",
"+------------- +  p  +  +  p  |                    |  p  +  +  p  + -------------+",
"               |  p  |  |  p  + ------------------ +  p  |  |  p  |               ",
"               |  p  |  |  pppppppppppppppppppppppppppp  |  |  p  |               ",
"               |  p  |  |     + ------------------ +     |  |  p  |               ",
"+ ------------ +  p  +  +     + ------ +  + ------ +     +  +  p  + ------------ +",
"|  pppppppppppppppppppppppppppppppppp  |  |  pppppppppppppppppppppppppppppppppp  |",
"|  p  + ------ +  p  + --------- +  p  |  |  p  + --------- +  p  + ------ +  p  |",
"|  p  + --- +  |  p  + --------- +  p  +--+  p  + --------- +  p  |  + --- +  p  |",
"|  ppppppp  |  |  pppppppppppppppppppppppppppppppppppppppppppppp  |  |  ppppppp  |",
"+ --- +  p  |  |  p  +  +  p  + ------------------ +  p  +  +  p  |  |  p  + --- +",
"+ --- +  p  +  +  p  |  |  p  + ------ +  + ------ +  p  |  |  p  +  +  p  + --- +",
"|  pppppppppppppppp  |  |  pppppppppp  |  |  pppppppppp  |  |  pppppppppppppppp  |",
"|  p  + ------------ +  + ------ +  p  |  |  p  + ------ +  + ------------ +  p  |",
"|  p  + ------------------------ +  p  +--+  p  + ------------------------ +  p  |",
"|  pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp  |",
"+ ------------------------------------------------------------------------------ +"
}; // p => pacman path; p || g => ghost path

// main 
int main(){

  setlocale(LC_ALL, "");
  initscr(); //initalize ncurses and clears screen
  start_color(); //Enable colour output
  {
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // walls
    init_color(COLOR_WHITE + 1, 1000, 1000, 400);
    init_pair(2, COLOR_WHITE + 1, COLOR_BLACK);// pills
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); //ghost
    init_pair(4, COLOR_BLUE, COLOR_BLACK);  //ghost
    init_pair(5, COLOR_RED, COLOR_BLACK); //ghost
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); //ghost
  }
  noecho(); //hides user inputs
  curs_set(0); //hides curser

  //pacman properties
  pacman.xLoc = 23;
  pacman.yLoc = 40;
  pacman.icon = "ᗧ";

  //Red Ghost
  red_ghost.xLoc = 14;
  red_ghost.yLoc = 33;
  red_ghost.icon = "ᗝ";

  //Blue Ghost
  blue_ghost.xLoc = 14;
  blue_ghost.yLoc = 38;
  blue_ghost.icon = "ᗝ";

  //Yellow Ghost
  yellow_ghost.xLoc = 14;
  yellow_ghost.yLoc = 43;
  yellow_ghost.icon = "ᗝ";

  //Pink Ghost
  pink_ghost.xLoc = 14;
  pink_ghost.yLoc = 48;
  pink_ghost.icon = "ᗝ";

  //window parameters
  int height = mapHeight + 4, width = mapWidth + 4, start_y = 0, start_x = 0;
  WINDOW *win = newwin(height, width, start_y, start_x);
  nodelay(win, TRUE); // Enable non-blocking input
  refresh();

  while(isRunning){
    box(win, 0, 0); // border
    mvwprintw(win, 0, 2, " PacMan ᗧ···ᗝ···ᗝ·· "); // border decoration
    drawMap(win); // map
    drawChar(win); // pacman, ghosts
    usleep(50000); // delay
    wrefresh(win);
    input(win);
  }

  endwin(); //dealocates memory and exits ncurses
  return 0;
}

void drawMap(WINDOW *win){ 
  for(int x=0; x < mapWidth; x++){
    for(int y=0; y < mapHeight; y++){
      if(map[y][x] != '.'){
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y+border_padding, x+border_padding, "%c", map[y][x]);
        wattroff(win, COLOR_PAIR(1));
      }else{
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, y+border_padding, x+border_padding, "%c", map[y][x]);
        wattroff(win, COLOR_PAIR(2));
      }
    }
  }
}

void drawChar(WINDOW *charWin){
  wattron(charWin, COLOR_PAIR(3));
  mvwprintw(charWin, pacman.xLoc + border_padding, pacman.yLoc + border_padding, pacman.icon); // pacman
  wattroff(charWin, COLOR_PAIR(3));

  wattron(charWin, COLOR_PAIR(5));
  mvwprintw(charWin, red_ghost.xLoc + border_padding, red_ghost.yLoc + border_padding, red_ghost.icon); //Red Ghost
  wattroff(charWin, COLOR_PAIR(5));

  wattron(charWin, COLOR_PAIR(4));
  mvwprintw(charWin, blue_ghost.xLoc + border_padding, blue_ghost.yLoc + border_padding, blue_ghost.icon); //Blue Ghost
  wattroff(charWin, COLOR_PAIR(4));

  wattron(charWin, COLOR_PAIR(3));
  mvwprintw(charWin, yellow_ghost.xLoc + border_padding, yellow_ghost.yLoc + border_padding, yellow_ghost.icon); //Yellow Ghost
  wattroff(charWin, COLOR_PAIR(3));

  wattron(charWin, COLOR_PAIR(6));
  mvwprintw(charWin, pink_ghost.xLoc + border_padding, pink_ghost.yLoc + border_padding, pink_ghost.icon); //Pink Ghost
  wattroff(charWin, COLOR_PAIR(6));
}

// input function
void input(WINDOW *charWin){
  char ch = wgetch(charWin); // Check for user input

  // Update direction based on input
  if (ch == 'w') direction = UP;
  if (ch == 's') direction = DOWN;
  if (ch == 'a') direction = LEFT;
  if (ch == 'd') direction = RIGHT;
  if (ch == 'q') isRunning = 0; // Quit on 'q'
        
  switch(direction){

    case UP:
      mvwprintw(charWin, pacman.xLoc + border_padding, pacman.yLoc + border_padding, " ");
      pacman.xLoc--;
      pacman.icon = "ᗢ";
      break;

    case LEFT:
      mvwprintw(charWin, pacman.xLoc + border_padding, pacman.yLoc + border_padding, " ");
      pacman.yLoc--;
      pacman.icon = "ᗤ";
      break;

    case DOWN:
      mvwprintw(charWin, pacman.xLoc + border_padding, pacman.yLoc + border_padding, " ");
      pacman.xLoc++;
      pacman.icon = "ᗣ";
      break;

    case RIGHT:
      mvwprintw(charWin, pacman.xLoc + border_padding, pacman.yLoc + border_padding, " ");
      pacman.yLoc++;
      pacman.icon = "ᗧ";
      break;
    default:
      break;
  }
}
