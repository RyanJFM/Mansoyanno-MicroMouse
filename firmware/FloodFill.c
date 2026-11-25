#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAZE_SIZE 16
#define STACK_SIZE 2048
#define INF 65535

#define COST_STAIGHT 10
#define COST_TURN_90 25
#define COST_TURN_180 50

// Global state arrays
uint16_t distances[MAZE_SIZE][MAZE_SIZE][4];
uint8_t walls[MAZE_SIZE][MAZE_SIZE];

// Global stack defintion
typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t heading;
} State;

// place stack outside of anyfunction to use global memory
State stack[STACK_SIZE];
int stack_ptr = 0;