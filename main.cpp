#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ncurses.h>

using namespace std;

//gameover is false by default as player has not lost
bool gameOver = false;

//dimensions of playing area
const int widthArea = 50;
const int heightArea = 20;

//coordinate of head of snake
int snakeX;
int snakeY;

//coordinate of fruit
int fruitX;
int fruitY;

//coordinate of bomb
int bombX;
int bombY;

//score of player
int score;

//Four directions snake can move at any moment
enum direction {LEFT, RIGHT, UP, DOWN};
direction current_direction;

//Arrays that store the coordinate of the head of snake as well as its tail
//TailX only stores xcoordinate of snake and TailY only stores the ycoordinate of the snake. The element at a particular index in TailX matches with another element with the same index in TailY as part of the snake i.e the coordinate (TailX[0],TailY[0]) is the head of the snake
int TailX[200];
int TailY[200];
int nTail = 0;

//Whether the player chooses to play the harder version(true) or not(false)
bool hard;

/**
This function initializes the variables that will be important before 
outputing what we have on the terminal
*/
void Setup() {
	//Head of snake will start in middle of playing area
    snakeX = widthArea/2;
    snakeY = heightArea/2;
	
	//seed random number
    srand(time(nullptr));
	//width will be from 0 to 49 so fruit must be from 1 to 48 inclusive
    fruitX = (rand() % (widthArea-2))+1;
	//height will be from 0 to 19 so fruit must be from 1 to 18 inclusive
    fruitY = (rand() % (heightArea-2))+1;
	//user chooses hard setting
	if (hard) {
		//create bombs
		bombX = (rand() % (widthArea-2))+1;
		bombY = (rand() % (heightArea-2))+1;
		
		//As long as bomb has smae coordinate as fruit
		while (bombX == fruitX && bombY == fruitY) {
			//we want bomb to have different coordinate from fruit
			bombX = (rand() % (widthArea-2))+1;
			bombY = (rand() % (heightArea-2))+1;
		}
	}
	//Score of player is initially zero
    score = 0;
}

/**
This function outputs the values of the variables in a certain format on the terminal
, basically the format of snake game.
*/
void Draw() {
	//clear the terminal
    clear();
	//for each element that makes the width of the playing area
    for (int i = 0; i < widthArea; ++i) {
		//these element will all be # for the first row
		mvprintw(0, i, "#");
	}
	
	//if user chooses to play the hard version
	if (hard) {
		//for each element that makes the height of the playing area
	    for (int i = 0; i < heightArea; ++i) {
			//and for each corresponding element that makes the width of the player i.e for each x,y coordinate on the window
	        for (int j = 0; j < widthArea; ++j) {
				//if the x,y coordinate is the same as the coordinate of the head of the snake
	            if (i == snakeY && j == snakeX) {
					//output O at that coordinate
	                mvprintw(i, j, "O");
	            }
				//otherwise, if the x,y coordinate is the same as the coordinate of the fruit
	            else if (i == fruitY && j == fruitX) {
					//output F at that coordinate
	                mvprintw(i, j, "F");
	            }
				//otherwise, if the x,y coordinate is the same as the coordinate of the bomb
				else if (i == bombY && j == bombX) {
					//output B at that coordinate
					mvprintw(i,j,"B");
				}
				//otherwise if the coordinate is (0,y) or (width-1,y) width-1 because we start from 0
	            else if (j == 0 || j == widthArea-1) {
					//output # at that coordinate
	                mvprintw(i,j, "#");
	            }
				//otherwise if the coordinate is (x,0) or (x,height -1) height-1 because we start from 0
	            else if (i == 0 || i == heightArea-1){
					//output # at that coordinate
	                mvprintw(i,j, "#");
	            }
				//otherwise
	            else {
					//for each element that make up the length of the tail
	            	for (int k = 0; k < nTail; ++k) {
						//if the coordinate matches the coordinate stored in array TailX and TailY
	            		if (TailX[k] == j && TailY[k] == i) {
							//output o at that coordinate
	            			mvprintw(i,j,"o");
	            		}
	            	}
	            }
	        }
	    }
	}
	//but if user chooses to play the normal mode, do the same thing as hard mode but this time do not include bombs. 
   else {
	  for (int i = 0; i < heightArea; ++i) {
	      for (int j = 0; j < widthArea; ++j) {
	            if (i == snakeY && j == snakeX) {
	                mvprintw(i, j, "O");
	            }
	            else if (i == fruitY && j == fruitX) {
	                mvprintw(i, j, "F");
	            }
	            else if (j == 0 || j == widthArea-1) {
	                mvprintw(i,j, "#");
	            }
	            else if (i == 0 || i == heightArea-1){
	                mvprintw(i,j, "#");
	            }
            
	            else {
	            	for (int k = 0; k < nTail; ++k) {
	            		if (TailX[k] == j && TailY[k] == i) {
	            			mvprintw(i,j,"o");
	            		}
	            	}
	            }
	        }
		}
    }
	
	//Have a row two rows after the playing area to tell player his/her score
	mvprintw(heightArea +2, 0, "Score: %d", score);
	
	//Tell player that if he wants to quit the game he can press q
	mvprintw(heightArea + 5, 0, "Press 'q' to quit");
	
    //output the changes made to the window in terminal
    refresh();
}

/**
This function is responsible for creating user interface and allowing user to interact with
the game
*/
void Input() {
	//Allow the use of arrow keys
	keypad(stdscr, TRUE);
	//Delays the taking in of input so that user do not have to constantly press the key
	halfdelay(1);
	//Create variable that will store the key that user inputs
	int input_character = getch();
	
	switch(input_character) {
		//if the user presses left arrow key
		case KEY_LEFT:
		//current_direction will be left
			current_direction = LEFT;
			break;
		//if the user presses right arrow key
		case KEY_RIGHT:
		//current_direction will be right
			current_direction = RIGHT;
			break;
		//if the user presses up arrow key
		case KEY_UP:
		//current_direction will be up
			current_direction = UP;
			break;
		//if the user presses down arrow key
		case KEY_DOWN:
		//current_direction will be down
			current_direction = DOWN;
			break;
		//if the user presses q	
		case static_cast<int>('q'):
		//gameOver will be true meaning game will end
			gameOver = true;
			break;			
		
	}
}

/**
This function modifies TailX and TailY to store the coordinates of the tail of the snake
and also takes in the user input to change the coordinate of the head of the snake
*/
void Logic() {
	//Before letting the head of snake replace first element of array, we store the coordinate of first unit of tail after the head in prevX and prevY
    int prevX = TailX[0];
	int prevY = TailY[0];
	//Create variables prev2X and prev2Y 
	int prev2X;
	int prev2Y;
	//let the current coordinate of head of snake be the first element of the arrays TailX and TailY
	TailX[0] = snakeX;
	TailY[0] = snakeY;
	
	//For the length of the tail of the snake
	for (int i = 1; i < nTail; ++i) {
		//initialize prev2X with the value on array TailX that will move back by one
		prev2X = TailX[i];
		//initialize prev2Y with the value on array TailY that will move back by one
		prev2Y = TailY[i];
		//replace the position with the value in front of it
		TailX[i] = prevX;
		TailY[i] = prevY;
		//Then ensure prevX and prevY are the values of prev2X and prev2Y which will make one last shift on their arrays in the next iteration
		prevX = prev2X;
		prevY = prev2Y;
	}
	
	switch(current_direction) {
		//if the direction snake is going is left
		case LEFT:
		//x coordinate of head will decrease by one
			--snakeX;
			//if x coordinate is less that 0
			if (snakeX <= 0) {
				//snake will appear on the right side of the playing area
				snakeX = widthArea -1;
			}
			break;
		//if the direction snake is going is right
		case RIGHT:
		//x coordinate of head will increase by one
			++snakeX;
			//if x coordinate is greaterthan or equal than width of playing area
			if (snakeX >= widthArea) {
				//snake will appear on the left side of the playing area
				snakeX = 1;
			}
			break;
		//if the direction snake is going is up
		case UP:
		//y-coordinate will decrease since (0,0) is top left corner of our grid
			--snakeY;
			//if y coordinate of head is less than 0
			if (snakeY <= 0) {
				//snake will appear on the bottom of the playing area
				snakeY = heightArea-1;
			}
			break;
		//if the direction snake is going is down
		case DOWN:
		//y coordinate of head will increase since (0,0) is top left corner of our grid
			++snakeY;
			//if y coordinate is greater than or equal to height of playing area
			if (snakeY >= heightArea) {
				//snake will appear at top of playing area
				snakeY = 0;
			}
			break;
			//if user did not provide any input do nothing to change coordinate
		default:
			break;
	}
	
	//for each element that makes up length of snake
	for (int i = 1; i < nTail; ++i) {
		//if head of snake eats any part of itself
		if (snakeX == TailX[i] && snakeY == TailY[i]) {
			// the game is over
			gameOver = true;
		}
	}
	
	//if the user chooses to play hard
	if (hard) {
		//if the snake eats the bomb
		if (snakeX == bombX && snakeY == bombY) {
			//score will decrease by one
			score -= 1;
			//if score is less than or equal to zero
			if (score <= 0) {
				//the game ends
				gameOver = true;
			}
			//shift bomb to new location if snake eats bomb
			bombX = (rand() % (widthArea-2))+1;
			bombY = (rand() % (heightArea-2))+1;
			//while bomb has same coordinate as fruit
			while (bombX == fruitX && bombY == fruitY) {
				//change coordinate of bomb
				bombX = (rand() % (widthArea-2))+1;
				bombY = (rand() % (heightArea-2))+1;
			}
			//Tail of snake decrease by one
			--nTail;
		}
	}
	//if snake eats the fruit
	if (snakeX == fruitX && snakeY == fruitY) {
		//score will increase by one
		score += 1;
		//change coordinate of fruit
		fruitX = (rand() % (widthArea-2))+1;
		fruitY = (rand() % (heightArea-2))+1;
		//length of tail increase by one
		++nTail;
    }
	
	
	
}

int main() {
    initscr();
	//clear the screen
    clear();
    //not showing user inputs
    noecho();
	cbreak();
    //sets cursor invisible
    curs_set(0);
	
	//yMax and xMax are coordinates of the whole terminal the player sees
	int yMax;
	int xMax;
	//get the coordinates of the whole terminal the player sees
	getmaxyx(stdscr, yMax, xMax);
	
	//create a new window
	WINDOW* menuwin = newwin(5, 0.25*xMax, yMax/2-2, xMax*0.35);
	//create a border for the new window
	box(menuwin, 0 ,0);
	//output the standard screen
	refresh();
	//output the menuwin
	wrefresh(menuwin);
	//Allow the use of arrow keys in menuwin
	keypad(menuwin, true);
	
	//create array that stores choices user can make
	string choices[2] = {"Hard", "Normal"};
	//create variable that stores choice of user
	int choice;
	//create variable hightlight
	int highlight;
	
	//welcome message for user
	string welcome_message = "Welcome to snake game.";
	
	//output the welcome message on the standard screen
	mvprintw(yMax/2-5,xMax*0.35, "%s",welcome_message.c_str());
	refresh();
	
	while (true) {
		//for each element in choices
		for (int i =0; i < 2; ++i) {
			//if index is equal to hightlight
			if (i == highlight) {
				//turn on the attribute reverse video for the window menuwin
				wattron(menuwin, A_REVERSE);
			}
			//output the choice, hard or normal at a certian coordinate
			mvwprintw(menuwin, i+1,1, choices[i].c_str());
			//turn off the attribute of reverse video
			wattroff(menuwin, A_REVERSE);
		}
		//choice variable stores the input of user  up key or down key
		choice = wgetch(menuwin);
		
		switch(choice) {
			//if user presses up key
			case KEY_UP:
			//highlight will decrease by one since we want to move the hightlight to the element above it
				highlight--;
				//however if we are the topmost element
				if (highlight == -1) {
					//we will highlight the bottom element
					highlight = 1;
				}
				break;
			//if user presses down key
			case KEY_DOWN:
			//highlight will increase by one since we want to move the hightlight to the element below it
				highlight++;
				//if we are already at the bottommost element
				if (highlight == 2) {
					//we will highlight the topmost element
					highlight = 0;
				}
				break;
				//if user does nothing do not change highlight
			default:
				break;
		}
		//if user presses enter key, break out of loop to start the game
		if (choice == 10) {
			break;
		}
	}
	
	//if the last highlighted element is hard; meaning user chooses hard
	if (choices[highlight] == "Hard") {
		//user wants to plays the hard version
		hard = true;
	}
	//otherwise
	else {
		//user wants to play normal version
		hard = false;
	}
	
  	Setup();
	//while it is not yet game over for user
    while (!gameOver) {

        Draw();
		Input();
		Logic();
		clear();
		
	}
	
	//close the window
    endwin();
}
