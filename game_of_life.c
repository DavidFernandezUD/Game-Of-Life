#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define FPS 8


void init_buffer(char* buffer, int height, int width, float density);

void print_buffer(char* buffer, int height, int width);

char update_cell(char* buffer, int height, int width, int row, int col);

void iterate(char* buffer, char* temp_buffer, int height, int width);

void run(int height, int width, float density, float fps);


int main(int argc, char* argv[]) {

    int height;
    int width;
    float density = 0.5f;

    if (argc == 3 || argc == 4) {

        if ((height = atoi(argv[1])) <= 0 || (width = atoi(argv[2])) <= 0)
            fprintf(stderr, "Invalid dimensio\n");
        
        if (argc == 4) {

            if ((density = atof(argv[3])) <= 0)
                fprintf(stderr, "Invalid density\n");
        }
    } else {
        fprintf(stderr, "Invalid number of argumenst\n");
    }

    srand(time(NULL));
    
    run(height, width, density, FPS);

    return 0;
}


void init_buffer(char* buffer, int height, int width, float density) {
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            buffer[i * width + j] = (((float) rand() / RAND_MAX) > density) ? 0 : 1;
        }
    }
}


void print_buffer(char* buffer, int height, int width) {
    
    system("clear");

    printf("+-");
    for (int i = 0; i < width; i++) printf("--");
    printf("+\n");

    for (int i = 0; i < height; i++) {
        printf("| ");
        for (int j = 0; j < width; j++) {
            printf("%c ", buffer[i * width + j] ? '*' : ' ');
        }
        printf("|\n");
    }

    printf("+-");
    for (int i = 0; i < width; i++) printf("--");
    printf("+\n");
}


char update_cell(char* buffer, int height, int width, int row, int col) {
        
    int num_neighbors = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {    
            if ( i != 0 || j != 0) {

                int neighbor_row;
                int neighbor_col;

                if ((neighbor_row = (row + i) % height) < 0)
                    neighbor_row = height - neighbor_row;

                if ((neighbor_col = (col + j) % width) < 0)
                    neighbor_col = width - neighbor_col;
            
                num_neighbors += buffer[neighbor_row * width + neighbor_col];
            }
        }
    }
    
    if (num_neighbors < 2 || num_neighbors > 3) {
        return 0;
    } else if (num_neighbors == 3) {
        return 1;
    } else {
        return buffer[row * width + col];
    }
}


void iterate(char* buffer, char* temp_buffer, int height, int width) {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            temp_buffer[i * width + j] = update_cell(buffer, height, width, i, j);
        }
    }

    memcpy(buffer, temp_buffer, height * width);
}


void run(int height, int width, float density, float fps) {
    
    char      buffer[height * width];
    char temp_buffer[height * width];

    init_buffer(buffer, height, width, density);

    while (1) {

        print_buffer(buffer, height, width);
        iterate(buffer, temp_buffer, height, width);

        usleep((1.f / fps) * 1000000);
    }
}

