#include <stdio.h>
#include <stdlib.h>



int n;  //width of chess board
int m;  //height of chess board

int totSquares; //n * m

#define MAX_INPUT_SIZE 100

int main(int argc, char * argv[]) {

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
    
    totSquares = n * m;


    


}


