#ifndef COCOBOT_PATHFINDER_INTERNAL_H
#define COCOBOT_PATHFINDER_INTERNAL_H

#include "cocobot/pathfinder_table.h"
#include "cocobot/pathfinder.h"

/**
 * Compute a node of the table
 * Arguments:
 *  - open_list : pointer on a* openlist
 *  - node : pointer on the node to be computed
 *  - parent_node : pointer on the parent of node. 
 *  
 * Return Value: void
 */
void cocobot_pathfinder_compute_node(cocobot_list_s *open_list, cocobot_node_s *node, cocobot_node_s *parent_node);

/**
 * Get the distance between two node (Using pythagore)
 * Arguments:
 *  - source: pointer on the start node
 *  - dest : pointer on the destination node. 
 *  
 * Return Value: The distance
 */
float cocobot_pathfinder_get_distance(cocobot_node_s *source, cocobot_node_s *dest);

/**
 * Initialize a list
 * Arguments:
 *  - list : pointer on the list
 *  
 */
void cocobot_pathfinder_initialize_list(cocobot_list_s *list);

/**
 * Add a node to a list
 * Arguments:
 *  - list : pointer on the list
 *  - node : pointer on the node to be added. 
 *  
 */
void cocobot_pathfinder_add_to_list(cocobot_list_s *list, cocobot_node_s *node);

/**
 * TODO: To be modified when list do not use malloc anymore
 * Remove a node from a list
 * Arguments:
 *  - list : pointer on the list
 *  - node : pointer on the node to be added. 
 *  
 *  return : TODO
 */
int cocobot_pathfinder_remove_from_list(cocobot_list_s *list, cocobot_node_s *node);

/**
 * Set target node
 * Arguments:
 *  - node : pointer on target node. 
 *  
 */
void cocobot_pathfinder_set_target_node(cocobot_node_s *target_node);

/**
 * Set start node
 * Arguments:
 *  - node : pointer on start node. 
 *  
 */
void cocobot_pathfinder_set_start_node(cocobot_node_s *start_node);

/**
 * Retreive final path
 * Arguments:
 *  - final_node : pointer on final node of the path
 *  - table : table used for computing
 *  - trajectory : pointer on the structure of the final trajectory
 *  
 */
void cocobot_pathfinder_get_path(cocobot_node_s *final_node, cocobot_node_s table[][TABLE_WIDTH/GRID_SIZE], cocobot_trajectory_s *trajectory);

/**
 * Convert a table node into a point containing real coordonates
 * Arguments:
 *  - node : pointer on node to be translated
 *
 * Return value: real coordonates point
 */
cocobot_point_s cocobot_pathfinder_get_point_from_node(cocobot_node_s *node);

/**
 * Initialise struct containing trajectory
 * Arguments:
 *  - trajectory : pointer on the struct to initialise
 */
void cocobot_pathfinder_init_trajectory(cocobot_trajectory_s *trajectory);

#endif  //COCOBOT_PATHFINDER_INTERNAL_H
