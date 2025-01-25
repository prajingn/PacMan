// Compile using => gcc -o PacMan -std=c99 $(ncursesw6-config --cflags --libs) PacMan.c
// map height = 31, map width = 82

# include <ncurses.h>
# include <locale.h>
# include <unistd.h>
# include <stdlib.h>
# include <time.h>

const int mapHeight = 31;
const int mapWidth = 82;
const int border_padding = 2;

// Directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int isRunning = 1, direction = LEFT, score = 0, end;

// charcters structure
struct character {
  int yLoc;
  int xLoc;
  char *icon;
}pacman, red_ghost, yellow_ghost, blue_ghost, pink_ghost;

// input Functions
void input(WINDOW*);

// Draw Map Functions
void drawMap(WINDOW*);

// Draw Characters function
void drawChar(WINDOW*);

// Window decor
void printBorderDecor(WINDOW*, int, int);

// Set Color Pallet
void setColors();

// Set character properties
void setCharacterProperties();

// Check score function
int checkScore();

// Function for ghost movement
void moveGhosts();

// Check for game termination
int terminate();

// print winner
void printWinner();

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
"+ ------------ +  .  |  + ------ +  .  |  |  .  + ------ +  |  .  + ------------ +",
"               |  .  |  + ------ +  .  +--+  .  + ------ +  |  .  |               ",
"               |  .  |  |  .  .  .  .  .  .  .  .  .  .  |  |  .  |               ",
"               |  .  |  |  .  + ------______------ +  .  |  |  .  |               ",
"+------------- +  .  +  +  .  |                    |  .  +  +  .  + -------------+",
"|  .  .  .  .  .  .  .  .  .  |                    |  .  .  .  .  .  .  .  .  .  |",
"+------------- +  .  +  +  .  |                    |  .  +  +  .  + -------------+",
"               |  .  |  |  .  + ------------------ +  .  |  |  .  |               ",
"               |  .  |  |  .  .  .  .  .  .  .  .  .  .  |  |  .  |               ",
"               |  .  |  |  .  + ------------------ +  .  |  |  .  |               ",
"+ ------------ +  .  +  +  .  + ------ +  + ------ +  .  +  +  .  + ------------ +",
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
"|  pppppppppppppppp  |  |  pppppppppp  |  |  pppppppppp  |  |  pppppppppppppppp  |",
"+ ------------ +  p  |  + ------ +  p  |  |  p  + ------ +  |  p  + ------------ +",
"               |  p  |  + ------ +  p  +--+  p  + ------ +  |  p  |               ",
"               |  p  |  |  pppppppppppppppppppppppppppp  |  |  p  |               ",
"               |  p  |  |  p  + ------gggggg------ +  p  |  |  p  |               ",
"+------------- +  p  +  +  p  |  gggggggggggggggg  |  p  +  +  p  + -------------+",
"| pppppppppppppppppppppppppp  |  gggggggggggggggg  |  pppppppppppppppppppppppppp |",
"+------------- +  p  +  +  p  |                    |  p  +  +  p  + -------------+",
"               |  p  |  |  p  + ------------------ +  p  |  |  p  |               ",
"               |  p  |  |  pppppppppppppppppppppppppppp  |  |  p  |               ",
"               |  p  |  |  p  + ------------------ +  p  |  |  p  |               ",
"+ ------------ +  p  +  +  p  + ------ +  + ------ +  p  +  +  p  + ------------ +",
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
  srand(time(NULL));

  initscr(); //initalize ncurses and clears screen
  start_color(); //Enable colour output
  setColors();
  noecho(); //hides user inputs
  curs_set(0); //hides curser
  setCharacterProperties();

  //window parameters
  int height = mapHeight + 4, width = mapWidth + 4, start_y = 0, start_x = 0;
  WINDOW *win = newwin(height, width, start_y, start_x);
  nodelay(win, TRUE); // Enable non-blocking input
  refresh();

  while(isRunning){
    box(win, 0, 0); // border
    printBorderDecor(win, height, width);
    drawMap(win); // map
    drawChar(win); // pacman, ghosts
    moveGhosts();
    terminate();
    score = checkScore(); //checks pellets left
    usleep(50000); // delay
    wrefresh(win);
    input(win);
  }

  printWinner();

  endwin(); //dealocates memory and exits ncurses
  return 0;
}

void setColors(){
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // walls
    init_color(COLOR_WHITE + 1, 1000, 1000, 400);
    init_pair(2, COLOR_WHITE + 1, COLOR_BLACK);// pills
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); //ghost
    init_pair(4, COLOR_BLUE, COLOR_BLACK);  //ghost
    init_pair(5, COLOR_RED, COLOR_BLACK); //ghost
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); //ghost
}

void setCharacterProperties(){
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
}

void printBorderDecor(WINDOW* win, int height, int width){
  mvwprintw(win, 0, 2, " PacMan ᗧ···ᗝ···ᗝ·· "); // border decoration
  mvwprintw(win, 0, width - 16, " ᗧ Score: %d ", score); // border decoration (score)
  mvwprintw(win, height-1, width - 30, " MOVE - W/A/S/D, QUIT - q "); // keys
}

void drawMap(WINDOW *win){ 
  for(int x=0; x < mapWidth; x++){
    for(int y=0; y < mapHeight; y++){
      if(map[y][x] == '.'){
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, y+border_padding, x+border_padding, "%c", map[y][x]);
        wattroff(win, COLOR_PAIR(2));
      }

      else if(map[y][x] == '_'){
        wattron(win, COLOR_PAIR(5));
        mvwprintw(win, y+border_padding, x+border_padding, "%c", map[y][x]);
        wattroff(win, COLOR_PAIR(5));
      }

      else{
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y+border_padding, x+border_padding, "%c", map[y][x]);
        wattroff(win, COLOR_PAIR(1));
      }
    }
  }
}

void drawChar(WINDOW *win){
  wattron(win, COLOR_PAIR(3));
  mvwprintw(win, pacman.xLoc + border_padding, pacman.yLoc + border_padding, pacman.icon); // pacman
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(5));
  mvwprintw(win, red_ghost.xLoc + border_padding, red_ghost.yLoc + border_padding, red_ghost.icon); //Red Ghost
  wattroff(win, COLOR_PAIR(5));

  wattron(win, COLOR_PAIR(4));
  mvwprintw(win, blue_ghost.xLoc + border_padding, blue_ghost.yLoc + border_padding, blue_ghost.icon); //Blue Ghost
  wattroff(win, COLOR_PAIR(4));

  wattron(win, COLOR_PAIR(3));
  mvwprintw(win, yellow_ghost.xLoc + border_padding, yellow_ghost.yLoc + border_padding, yellow_ghost.icon); //Yellow Ghost
  wattroff(win, COLOR_PAIR(3));

  wattron(win, COLOR_PAIR(6));
  mvwprintw(win, pink_ghost.xLoc + border_padding, pink_ghost.yLoc + border_padding, pink_ghost.icon); //Pink Ghost
  wattroff(win, COLOR_PAIR(6));
}

void input(WINDOW *win) {
    static int lastValidDirection = LEFT; // Retain the last valid direction
    static int desiredDirection = LEFT;  // Track the desired direction from user input
    char ch = wgetch(win); // Get user input

    // Update desired direction based on key press
    if (ch == 'w') desiredDirection = UP;
    if (ch == 's') desiredDirection = DOWN;
    if (ch == 'a') desiredDirection = LEFT;
    if (ch == 'd') desiredDirection = RIGHT;
    if (ch == 'q') isRunning = 0; // Quit on 'q'

    int newX = pacman.xLoc;
    int newY = pacman.yLoc;
    char *newIcon = pacman.icon; // Temporary variable to update the icon only if the move is valid

    // Check if the desired direction is valid
    switch (desiredDirection) {
        case UP:    newX = pacman.xLoc - 1; newY = pacman.yLoc; newIcon = "ᗢ"; break;
        case DOWN:  newX = pacman.xLoc + 1; newY = pacman.yLoc; newIcon = "ᗣ"; break;
        case LEFT:  newX = pacman.xLoc;     newY = pacman.yLoc - 1; newIcon = "ᗤ"; break;
        case RIGHT: newX = pacman.xLoc;     newY = pacman.yLoc + 1; newIcon = "ᗧ"; break;
        default: break;
    }

    if (path_map[newX][newY] == 'p') {
        // If the desired direction is valid, move Pac-Man and update the direction and icon
        mvwprintw(win, pacman.xLoc + border_padding, pacman.yLoc + border_padding, " "); // Clear old position
        pacman.xLoc = newX;
        pacman.yLoc = newY;
        pacman.icon = newIcon; // Update the icon
        lastValidDirection = desiredDirection; // Update last valid direction
    } else {
        // If the desired direction is not valid, keep moving in the last valid direction
        newX = pacman.xLoc;
        newY = pacman.yLoc;

        switch (lastValidDirection) {
            case UP:    newX = pacman.xLoc - 1; newY = pacman.yLoc; break;
            case DOWN:  newX = pacman.xLoc + 1; newY = pacman.yLoc; break;
            case LEFT:  newX = pacman.xLoc;     newY = pacman.yLoc - 1; break;
            case RIGHT: newX = pacman.xLoc;     newY = pacman.yLoc + 1; break;
            default: break;
        }

        if (path_map[newX][newY] == 'p') {
            // Continue moving in the last valid direction if possible
            mvwprintw(win, pacman.xLoc + border_padding, pacman.yLoc + border_padding, " "); // Clear old position
            pacman.xLoc = newX;
            pacman.yLoc = newY;
        }
    }
}

int checkScore(){
    if(map[pacman.xLoc][pacman.yLoc] == '.'){
      map[pacman.xLoc][pacman.yLoc] = ' ';
      score += 5;
    }

  return score;
}

void moveGhosts(){
  
}

int terminate(){
  if (score == 1490){
    end = 1; // won
    isRunning = 0;
  }

  if(pacman.xLoc == red_ghost.xLoc && pacman.yLoc == red_ghost.yLoc){
    end = 0; // lose
    isRunning = 0;
  }

  if(pacman.xLoc == blue_ghost.xLoc && pacman.yLoc == blue_ghost.yLoc){
    end = 0; // lose
    isRunning = 0;
  }

  if(pacman.xLoc == yellow_ghost.xLoc && pacman.yLoc == yellow_ghost.yLoc){
    end = 0; // lose
    isRunning = 0;
  }

  if(pacman.xLoc == pink_ghost.xLoc && pacman.yLoc == pink_ghost.yLoc){
    end = 0; // lose
    isRunning = 0;
  }
}

void printWinner(){
  if (end == 1){
    printf("You WON!!, score: %d + bonus %d = %d", score, 10, score+10);
  }
  if (end == 0){
    printf("You LOST!!, score: %d + bonus 0 = %d", score, score);
  }
}