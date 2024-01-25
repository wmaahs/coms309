#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 100  //max amount of characters the user is allowed to input

#define WIDTH 5   // width of chess board
#define HEIGHT 5  // height of chess board
#define CHESSPACE 25
//move patterns in the x and y coordinates
#define KNIGHT_MOVES 8
static int mpx[KNIGHT_MOVES] = {1, 1, 2, 2, -1, -1, -2, -2};
static int mpy[KNIGHT_MOVES] = {2, -2, 1, -1, 2, -2, 1, -1};

int boarder_check(int x, int y);
int is_jump_valid(int x, int y);
void format_output(int curr_path[WIDTH*HEIGHT]);
void knights_tour_recursion(int x, int y, int curr_path[WIDTH*HEIGHT], int count);

// arrray to check if space has been visited
int chess_board_tracker[5][5] = {-1};
//array to hold the values of each square
int chess_board[5][5];

int main() {
  
  //fill the chess_board with their respective values
  int count = 1;
  for(int y = 0; y < HEIGHT; y++){
    for(int x = 0; x < WIDTH; x++){
      chess_board[y][x] = count++;
    }
  }

  //reset the count to be used to track how many spaces the knight has been
  count = 0;
  
  //array to hold the curr_path path
  int curr_path[CHESSPACE];

  //loop through the chess_board and call the knights_tour_recursion function for each space
  for(int y = 0; y < HEIGHT; y++){
    for(int x = 0; x < WIDTH; x++){
      knights_tour_recursion(x, y, curr_path, count);
    }
  }

  return 0;
}

void knights_tour_recursion(int x, int y, int curr_path[WIDTH*HEIGHT], int count) {

    // add the current
    curr_path[count] = chess_board[y][x];
    chess_board_tracker[y][x] = 1;

    //have completed the chessboard
    if(count == CHESSPACE -1){
        //go back to finding more solutions
        format_output(curr_path);
    }
    else{
        for(int move = 0; move < KNIGHT_MOVES; move++){
            int next_x = x + mpx[move];
            int next_y = y + mpy[move];

            if(next_x >= 0 && next_x < WIDTH && next_y < HEIGHT && next_y >= 0 && chess_board_tracker[next_y][next_x] != 1) {
                knights_tour_recursion(next_x, next_y, curr_path, count + 1);
            }
        }
        
    }

    chess_board_tracker[y][x] = -1;
    return;
}

// function to print the tours once they're found
void format_output(int curr_path[WIDTH*HEIGHT]){
  for(int i = 0; i < CHESSPACE - 1; i++) {
    printf("%d,", curr_path[i]);
  }
  printf("%d\n", curr_path[CHESSPACE - 1]);
  return;
}