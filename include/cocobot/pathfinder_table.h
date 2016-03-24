#ifndef COCOBOT_PATHFINDER_TABLE_H
#define COCOBOT_PATHFINDER_TABLE_H

#include <stdint.h>

#define TABLE_LENGTH 3000
#define TABLE_WIDTH 2000
#define MAXIMUM_NODE_IN_LIST 200
#define GRID_SIZE 50

typedef enum
{
    NEW_NODE = 0,
    OBSTACLE,
    CLOSED_LIST,
    FINAL_TRAJ,
    OPEN_LIST,
    ROBOT
}cocobot_nodeType_e;

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t nodeType; //is a cocobot_nodeType_e cast in uint8_t to be sure of the size
    float cost;
    uint8_t pX;
    uint8_t pY;
}cocobot_node_s;


/**
 * cocobot_list_t is a sorted list (using the node of each node
 * table[0] is the node of the list with the smallest cost
 */
typedef struct
{
    cocobot_node_s table[MAXIMUM_NODE_IN_LIST];//Arbitrary chosen value
    uint8_t nb_elements;
} cocobot_list_s;

/**
 * Initialize the table used for a*
 * Arguments:
 *  - table : static 2nd array representing the playground area for a*
 *  - length : length of the table
 *  - width : width of the table
 *  
 */
void cocobot_pathfinder_initialise_table(cocobot_node_s table[][TABLE_WIDTH/GRID_SIZE], int length, int width);

/**
 * Set a point on the table
 * Arguments:
 *  - table : static 2nd array representing the playground area for a*
 *  - x : x coordinate of the point
 *  - y : y coordinate of the point
 *  - node_type : node_type wanted for the point
 *  
 */
void cocobot_pathfinder_set_point(cocobot_node_s table[][TABLE_WIDTH/GRID_SIZE], int x, int y, cocobot_nodeType_e node_type);

/**
 * Set a rectangle on the table of the same nodetype
 * Arguments:
 *  - table : static 2nd array representing the playground area for a*
 *  - x_dimension : x dimension of the rectangle
 *  - y_dimension : y dimension of the rectangle
 *  - x_position : x position of the rectangle (x correspond to the smaller x coordinate of the rectangle)
 *  - y_position : y position of the rectangle (y correspond to the smaller y coordinate of the rectangle)
 *  - node_type : node_type wanted for the rectangle
 *  
 */
void cocobot_pathfinder_set_rectangle(cocobot_node_s table[][TABLE_WIDTH/GRID_SIZE], int x_dimension, int y_dimension, int x_position, int y_position, cocobot_nodeType_e node_type);

/**
 * Set a circle on the table of the same nodetype
 * Arguments:
 *  - table : static 2nd array representing the playground area for a*
 *  - x_center : x coordinate of the center of the circle
 *  - y_center : y coordinate of the center of the circle
 *  - radius : radius of the circle
 *  - node_type : node_type wanted for the point
 *  
 */
void cocobot_pathfinder_set_circle(cocobot_node_s table[][TABLE_WIDTH/GRID_SIZE], int x_center, int y_center, int radius, cocobot_nodeType_e node_type);

#endif //COCOBOT_PATHFINDER_TABLE_H
