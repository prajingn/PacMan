// Compile usin => gcc -o PacMan -std=c99 $(ncursesw6-config --cflags --libs) PacMan.c

# include <ncurses.h>
# include <locale.h>

int main(){

  setlocale(LC_ALL, "");

  initscr(); //initalize ncurses and clears screen

  //window parameters
  int height = 40,
      width = 80,
      start_y = 0,
      start_x = 0;

  WINDOW *win = newwin(height, width, start_y, start_x);
  refresh();
  box(win, 0, 0);
  mvwprintw(win, 0, 2, " PacMan ᗧ···ᗣ···ᗣ·· ");
  wrefresh(win);

  getch();

  endwin(); //dealocates memory and exits ncurses
  return 0;
}
