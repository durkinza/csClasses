/* Zane Durkin
 * Lab 8
 * October 21 2016
 */

#include <iostream> 
#include <curses.h> // include the curses library
using namespace std;
const int MAXX = 40; // maximum size of the 'room'
const int MAXY = 20;

void Setup( char room[][MAXY], int x, int y ); // room setup
void print( char arr[MAXX][MAXY]);
void find( char arr[MAXX][MAXY]);
void WaitForQuit();

int main(){
	WINDOW *wnd;
	char room[MAXX][MAXY]; // stores the room
	int x = MAXX/2, y = MAXY/2; // robot's initial location
	wnd = initscr(); // 'initializes' the window
	clear(); // clears the window
	refresh(); // reprints the window
	Setup(room,x,y); // setup the room with the robot
	find(room);
//	print(room); // write this function!!
	refresh();
	WaitForQuit();
	endwin(); // frees the screen for normal use
}

/* Sets up the room, adding walls, and the robot */
void Setup( char room[][MAXY], int x, int y ){
	for(int i = 0; i < MAXX; i++){
		for(int j = 0; j < MAXY; j++){
			room[i][j] = ' '; // empty the room
		}
	}
	for(int i = 0; i < MAXY; i++) {
		room[0][i] = '|'; // left wall
		room[MAXX-1][i] = '|'; // right wall
	}
	for(int i = 0; i < MAXX; i++) {
		room[i][0] = '-'; // top wall
		room[i][MAXY-1] = '-'; // bottom wall
	}
	room[x][y] = 'R'; // place the robot
}

void WaitForQuit(){
	char ch;
	do {
		cin >> ch;
	} while( ch != 'q' );
}

void print(char arr[][MAXY]){
	for(int i = 0; i < MAXY; i++){
		for(int j = 0; j < MAXX; j++){
			move(i,j);
			insch(arr[j][i]);
		}
	}
}

void find(char arr[MAXX][MAXY]){
	for(int i = 0; i < MAXY; i++){
		for(int j = 0; j < MAXX; j++){
			move(i,j);
			if(arr[j][i] == 'R'){
				insch('X');
			}else{
				insch('.');
			}
		}
	}
}
