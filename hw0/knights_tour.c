#include <stdio.h>
#include <stdlib.h>

int n;  //width of chess board
int m;  //height of chess board

#define MAX_INPUT_SIZE 100  //max amount of characters the user is allowed to input

//move patterns in the x and y coordinates
#define KNIGHT_MOVES 8
static int mpx[KNIGHT_MOVES] = {1, 1, 2, 2, -1, -1, -2, -2};
static int mpy[KNIGHT_MOVES] = {2, -2, 1, -1, 2, -2, 1, -1};


// check if (x,y) is on the board
int boarder_check(int x, int y) {
    if((x >= 0 && y >= 0) && (x <= n && y <= m)) {

        //success
        return 0;
    }
    else {

        //failure
        return 1;
    }
}

// function to check if the location (x,y)
// has been visited
int is_jump_valid(int a[], int x, int y) {
    
    if(boarder_check(x, y) == 1) {

        //boarder check failed
        printf("boarder check failed.... \n");
        return  1;
    }
    else if(a[y*m+x] > 0) {

        //square has already been reached
        return 1;
    }
    else {

        //success
        return 0;
    }
}

// gets the number of adjacent squares to (x,y) that
// are available to be jumped to next
int get_weight(int a[], int x, int y)
{

    int count = 0;
    for(int i = 0; i < KNIGHT_MOVES; i++)
    {
        if (is_jump_valid(a, x + mpx[i], y + mpy[i]) == 0)
        {
            count++;
        }
    }

    return count;
}

// get the next move based off of the lowest amount of
// future potential jumps (Warnsdorff's heuristic)
int get_next_move(int a[], int *x, int *y){

    int next_sqr_ind = -1;      //holder for best jump ind
    int next_sqr_weight = 10000;    //holder for best jump weight

    // May want to add random starting neighbor
    for(int i = 0; i < KNIGHT_MOVES; i++) {

        int adj_x = *x + mpx[i];
        int adj_y = *y + mpy[i];

        if(is_jump_valid(a, adj_x, adj_y) == 0) {

            if(get_weight(a, adj_x, adj_y) < next_sqr_weight) {
                next_sqr_ind = adj_y * m + adj_x;
            }
        }
    }

    return next_sqr_ind;
}



int main() {

    char input[MAX_INPUT_SIZE] = {'\0'};

    //prompt user for size of chess board
    printf("Enter width (n): \n");
    fgets(input, MAX_INPUT_SIZE, stdin);
    //write input to n
    n = atoi(input);

    //do the same for height
    printf("Enter height (m): \n");
    fgets(input, MAX_INPUT_SIZE, stdin);
    //write input to m
    m = atoi(input);

    // generate empty chess board (empty => -1)
    // make 2d array
    int cb[n*m];
    for(int i = 0; i < n*m; i++) {
        cb[i] = -1;
    }



}