#include <cocobot.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <math.h>

#define SCHEDULER_MAX_ACTIONS 16

#define EUCLIDEAN_DISTANCE(x, y) (sqrt((x)*(x) + (y)*(y)))

typedef struct
{
  float x, y; // in mm
  float a;    // in deg
} cocobot_pos_t;

typedef struct
{
  char            name[ACTION_NAME_LENGTH];
  unsigned int    score;
  cocobot_pos_t   pos;
  float           execution_time;
  float           success_proba;
  action_callback callback;
  uint8_t         done;
} cocobot_action_t;

static cocobot_action_t action_list[SCHEDULER_MAX_ACTIONS];
static unsigned int action_list_end;

static struct cocobot_game_state_t
{
  cocobot_strategy_t  strat;
  cocobot_pos_t       robot_pos;
  float               robot_average_linear_speed; // in m/s (or mm/ms)
  float               remaining_time; // in ms
} current_game_state;


static void cocobot_action_scheduler_init(void)
{
  current_game_state.strat = COCOBOT_STRATEGY_DEFENSIVE;
  current_game_state.robot_pos.x = 0;
  current_game_state.robot_pos.y = 0;
  current_game_state.robot_pos.a = 0;
  current_game_state.robot_average_linear_speed = 0;
  current_game_state.remaining_time = 90000;

  action_list_end = 0;
}

static void cocobot_action_scheduler_task(void * arg)
{
  // arg is always NULL. Prevent "variable unused" warning
  (void)arg;
  int action_return_value;

  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    current_game_state.remaining_time -= 100;

    // Wait 100ms
    vTaskDelayUntil(&xLastWakeTime, 100 / portTICK_PERIOD_MS);
  }
}

void cocobot_action_scheduler_start(unsigned int task_priority)
{
  cocobot_action_scheduler_init();

  // Start task
  xTaskCreate(cocobot_action_scheduler_task, "action_scheduler", 200, NULL, task_priority, NULL);
}

void cocobot_action_scheduler_set_strategy(cocobot_strategy_t strat)
{
  current_game_state.strat = strat;
}

void cocobot_action_scheduler_set_average_linear_speed(float speed)
{
  current_game_state.robot_average_linear_speed = speed;
}

static void cocobot_action_scheduler_update_game_state(void)
{
  current_game_state.robot_pos.x = cocobot_position_get_x();
  current_game_state.robot_pos.y = cocobot_position_get_y();
  current_game_state.robot_pos.a = cocobot_position_get_angle();
}

void cocobot_action_scheduler_add_action(char name[ACTION_NAME_LENGTH], unsigned int score, float x, float y, float a, float execution_time, float success_proba, action_callback callback)
{
  strncpy(action_list[action_list_end].name, name, ACTION_NAME_LENGTH);
  action_list[action_list_end].score = score;
  action_list[action_list_end].pos.x = x;
  action_list[action_list_end].pos.y = y;
  action_list[action_list_end].pos.a = a;
  action_list[action_list_end].execution_time = execution_time;
  action_list[action_list_end].success_proba = success_proba;
  action_list[action_list_end].callback = callback;
  action_list[action_list_end].done = 0;

  if (action_list_end < SCHEDULER_MAX_ACTIONS-1)
  {
    action_list_end++;
  }
  else
  {
    printf("[Warning] cocobot_action_scheduler: Action list full.\n");
  }
}

/* Returns the approximate time (in ms) needed by the robot to reach the action's
 * starting point, based on the average robot linear speed set by the user.
 * By default, this time is set to 0 ms.
 * TODO: this function should be replaced by a more precise time given by the
 * pathfinder.
 */
static float cocobot_action_scheduler_time_to_reach(cocobot_action_t * action)
{
  if (current_game_state.robot_average_linear_speed == 0)
  {
    return 0;
  }

  float x = current_game_state.robot_pos.x - action->pos.x;
  float y = current_game_state.robot_pos.y - action->pos.y;
  // 2 is a factor to approximate the length of the real path from the
  // straight-line distance between the two points
  float approximate_linear_distance = 2 * EUCLIDEAN_DISTANCE(x, y);

  return (approximate_linear_distance)/(current_game_state.robot_average_linear_speed);
}

/* Compute action's value based on current game's state
 * Argument:
 *  - action_id: id of the action to evaluate (see cocobot_action_scheduler_add_action)
 * Return:
 *  The action's value (bigger is better). If negative, action can't be done
 *  in time or as already been done.
 */
static float cocobot_action_scheduler_eval(cocobot_action_t * action)
{
  cocobot_action_scheduler_update_game_state();

  if (action->execution_time > current_game_state.remaining_time)
  {
    return -1;
  }
  if (action->done)
  {
    return -0.5f;
  }

  // TODO: Take strategy and remaining time to execute other actions into account
  float potential_elementary_value = action->score / (action->execution_time + cocobot_action_scheduler_time_to_reach(action));
  float effective_elementary_value = action->success_proba * potential_elementary_value;

  return effective_elementary_value;
}

int cocobot_action_scheduler_execute_best_action(void)
{
  unsigned int action_current_index = 0;
  int action_best_index = -1;
  float action_current_eval = 0;
  float action_best_eval = -1;

  for (; action_current_index < action_list_end; action_current_index++)
  {
    action_current_eval = cocobot_action_scheduler_eval(&action_list[action_current_index]);
    if (action_current_eval > action_best_eval)
    {
      action_best_eval = action_current_eval;
      action_best_index = action_current_index;
    }
  }

  if (action_best_index < 0 || action_best_eval < 0)
  {
    return 0;
  }

  int action_return_value = (*action_list[action_best_index].callback)();

  if (action_return_value > 0)
  {
    action_list[action_best_index].done = 1;
  }

  return action_return_value;
}
