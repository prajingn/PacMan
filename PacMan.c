// Compile using => gcc -o PacMan -std=c99 $(ncursesw6-config --cflags --libs) PacMan.c

# include <ncurses.h>
# include <locale.h>

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

// main 
int main(){
  setlocale(LC_ALL, "");

  int isRunning = 1;

  initscr(); //initalize ncurses and clears screen
  noecho(); //hides user inputs
  curs_set(0); //hides curser

  //window parameters
  int height = 35, width = 80, start_y = 0, start_x = 0;

  WINDOW *win = newwin(height, width, start_y, start_x);
  refresh();
  box(win, 0, 0);
  mvwprintw(win, 0, 2, " PacMan ᗧ···ᗝ···ᗝ·· ");
  wrefresh(win);

  while(isRunning){

  }

  getch();

  endwin(); //dealocates memory and exits ncurses
  return 0;
}


