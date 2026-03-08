/**
 * @file osal_freertos.c
 * @brief FreeRTOS implementation of the OSAL (Operating System Abstraction Layer)
 *
 * This file implements the OSAL interface for FreeRTOS, providing a consistent
 * API that can be used across different RTOS implementations.
 */

#include "osal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"
#include <string.h>

/* ========================================================================== */
/* Internal Helper Functions                                                  */
/* ========================================================================== */

/**
 * @brief Convert OSAL priority to FreeRTOS priority
 */
static UBaseType_t osal_to_freertos_priority(osal_task_priority_t osal_prio)
{
  /* Map OSAL priority levels to FreeRTOS priorities */
  static const UBaseType_t priority_map[] = {
      tskIDLE_PRIORITY,         /* OSAL_PRIORITY_IDLE */
      tskIDLE_PRIORITY + 1,     /* OSAL_PRIORITY_LOW */
      tskIDLE_PRIORITY + 2,     /* OSAL_PRIORITY_BELOW_NORMAL */
      tskIDLE_PRIORITY + 3,     /* OSAL_PRIORITY_NORMAL */
      tskIDLE_PRIORITY + 4,     /* OSAL_PRIORITY_ABOVE_NORMAL */
      tskIDLE_PRIORITY + 5,     /* OSAL_PRIORITY_HIGH */
      configMAX_PRIORITIES - 1, /* OSAL_PRIORITY_REALTIME */
  };

  if (osal_prio >= (sizeof(priority_map) / sizeof(priority_map[0])))
  {
    return tskIDLE_PRIORITY + 3; /* Default to normal priority */
  }

  return priority_map[osal_prio];
}

/**
 * @brief Convert milliseconds to FreeRTOS ticks
 */
static TickType_t ms_to_ticks(uint32_t ms)
{
  if (ms == OSAL_WAIT_FOREVER)
  {
    return portMAX_DELAY;
  }

  TickType_t ticks = pdMS_TO_TICKS(ms);
  return (ticks == 0) ? 1 : ticks; /* Minimum 1 tick */
}

/**
 * @brief Create OSAL return code
 */
static uint32_t make_osal_return(ReturnStatus success, ErrorLevel error_level,
                                 ErrorType error_type, uint16_t specific_code)
{
  return MAKE_RETURN_CODE(success, error_level, MODULE_OSAL, error_type, specific_code);
}

/* ========================================================================== */
/* Task Management Implementation                                             */
/* ========================================================================== */

uint32_t osal_task_create(const char *name,
                          osal_task_func_t func,
                          void *param,
                          uint32_t stack_size,
                          osal_task_priority_t priority,
                          osal_task_handle_t *task_handle,
                          uint32_t flags)
{
  if (name == NULL || func == NULL || task_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  if (stack_size < configMINIMAL_STACK_SIZE)
  {
    stack_size = configMINIMAL_STACK_SIZE;
  }

  UBaseType_t freertos_priority = osal_to_freertos_priority(priority);
  BaseType_t result;

  if (flags & OSAL_TASK_STATIC)
  {
    /* Static allocation not fully implemented in this example */
    /* For now, fall back to dynamic allocation */
    result = xTaskCreate(func, name, stack_size / sizeof(StackType_t),
                         param, freertos_priority, (TaskHandle_t *)task_handle);
  }
  else
  {
    result = xTaskCreate(func, name, stack_size / sizeof(StackType_t),
                         param, freertos_priority, (TaskHandle_t *)task_handle);
  }

  if (result != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_task_delete(osal_task_handle_t task_handle)
{
  if (task_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  vTaskDelete((TaskHandle_t)task_handle);
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_task_delay(uint32_t ms)
{
  if (ms == 0)
  {
    taskYIELD();
  }
  else
  {
    vTaskDelay(ms_to_ticks(ms));
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

void osal_task_yield(void)
{
  taskYIELD();
}

osal_task_handle_t osal_task_get_current(void)
{
  return (osal_task_handle_t)xTaskGetCurrentTaskHandle();
}

uint32_t osal_task_get_name(osal_task_handle_t task_handle,
                            char *buffer,
                            uint32_t buffer_size)
{
  if (buffer == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  if (buffer_size == 0)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_INVALID_PARAM, 0);
  }

  const char *name;
  if (task_handle == NULL)
  {
    name = pcTaskGetName(NULL);
  }
  else
  {
    name = pcTaskGetName((TaskHandle_t)task_handle);
  }

  if (name == NULL)
  {
    buffer[0] = '\0';
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_DATA, 0);
  }

  strncpy(buffer, name, buffer_size - 1);
  buffer[buffer_size - 1] = '\0';

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

/* ========================================================================== */
/* Semaphore Implementation                                                   */
/* ========================================================================== */

uint32_t osal_semaphore_create(osal_semaphore_type_t type,
                               uint32_t max_count,
                               uint32_t initial_count,
                               osal_semaphore_handle_t *sem_handle)
{
  if (sem_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  SemaphoreHandle_t semaphore;

  if (type == OSAL_SEMAPHORE_BINARY)
  {
    semaphore = xSemaphoreCreateBinary();
    if (semaphore != NULL && initial_count > 0)
    {
      xSemaphoreGive(semaphore);
    }
  }
  else
  { /* OSAL_SEMAPHORE_COUNTING */
    semaphore = xSemaphoreCreateCounting(max_count, initial_count);
  }

  if (semaphore == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  *sem_handle = (osal_semaphore_handle_t)semaphore;
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_semaphore_delete(osal_semaphore_handle_t sem_handle)
{
  if (sem_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  vSemaphoreDelete((SemaphoreHandle_t)sem_handle);
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_semaphore_take(osal_semaphore_handle_t sem_handle,
                             uint32_t timeout_ms)
{
  if (sem_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  BaseType_t result = xSemaphoreTake((SemaphoreHandle_t)sem_handle,
                                     ms_to_ticks(timeout_ms));

  if (result != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_TIMEOUT, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_semaphore_give(osal_semaphore_handle_t sem_handle)
{
  if (sem_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  BaseType_t result = xSemaphoreGive((SemaphoreHandle_t)sem_handle);

  if (result != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

/* ========================================================================== */
/* Mutex Implementation                                                       */
/* ========================================================================== */

uint32_t osal_mutex_create(osal_mutex_handle_t *mutex_handle)
{
  if (mutex_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

  if (mutex == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  *mutex_handle = (osal_mutex_handle_t)mutex;
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_mutex_delete(osal_mutex_handle_t mutex_handle)
{
  if (mutex_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  vSemaphoreDelete((SemaphoreHandle_t)mutex_handle);
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_mutex_lock(osal_mutex_handle_t mutex_handle,
                         uint32_t timeout_ms)
{
  if (mutex_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  BaseType_t result = xSemaphoreTake((SemaphoreHandle_t)mutex_handle,
                                     ms_to_ticks(timeout_ms));

  if (result != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_TIMEOUT, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_mutex_unlock(osal_mutex_handle_t mutex_handle)
{
  if (mutex_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  BaseType_t result = xSemaphoreGive((SemaphoreHandle_t)mutex_handle);

  if (result != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_OPERATION_DENIED, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

/* ========================================================================== */
/* Queue Implementation                                                       */
/* ========================================================================== */

uint32_t osal_queue_create(uint32_t item_size,
                           uint32_t queue_length,
                           osal_queue_handle_t *queue_handle)
{
  if (queue_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  if (item_size == 0 || queue_length == 0)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_INVALID_PARAM, 0);
  }

  QueueHandle_t queue = xQueueCreate(queue_length, item_size);

  if (queue == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  *queue_handle = (osal_queue_handle_t)queue;
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_queue_delete(osal_queue_handle_t queue_handle)
{
  if (queue_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  vQueueDelete((QueueHandle_t)queue_handle);
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_queue_send(osal_queue_handle_t queue_handle,
                         const void *item,
                         uint32_t timeout_ms)
{
  if (queue_handle == NULL || item == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  BaseType_t result = xQueueSend((QueueHandle_t)queue_handle, item,
                                 ms_to_ticks(timeout_ms));

  if (result != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_TIMEOUT, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_queue_receive(osal_queue_handle_t queue_handle,
                            void *item,
                            uint32_t timeout_ms)
{
  if (queue_handle == NULL || item == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  BaseType_t result = xQueueReceive((QueueHandle_t)queue_handle, item,
                                    ms_to_ticks(timeout_ms));

  if (result != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_TIMEOUT, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_queue_get_waiting(osal_queue_handle_t queue_handle)
{
  if (queue_handle == NULL)
  {
    return 0;
  }

  return (uint32_t)uxQueueMessagesWaiting((QueueHandle_t)queue_handle);
}

uint32_t osal_queue_get_free(osal_queue_handle_t queue_handle)
{
  if (queue_handle == NULL)
  {
    return 0;
  }

  return (uint32_t)uxQueueSpacesAvailable((QueueHandle_t)queue_handle);
}

/* ========================================================================== */
/* Timer Implementation                                                       */
/* ========================================================================== */

/**
 * @brief Structure to store timer callback and parameter
 */
typedef struct
{
  osal_timer_callback_t callback;
  void *param;
} osal_timer_data_t;

/**
 * @brief Timer callback wrapper for FreeRTOS
 */
static void timer_callback_wrapper(TimerHandle_t timer_handle)
{
  osal_timer_data_t *timer_data = (osal_timer_data_t *)pvTimerGetTimerID(timer_handle);

  if (timer_data != NULL && timer_data->callback != NULL)
  {
    timer_data->callback(timer_data->param);
  }
}

uint32_t osal_timer_create(const char *name,
                           osal_timer_callback_t callback,
                           void *param,
                           uint32_t period_ms,
                           osal_timer_type_t type,
                           bool auto_start,
                           osal_timer_handle_t *timer_handle)
{
  if (name == NULL || callback == NULL || timer_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  if (period_ms == 0)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_INVALID_PARAM, 0);
  }

  /* Allocate memory for timer data */
  osal_timer_data_t *timer_data = (osal_timer_data_t *)pvPortMalloc(sizeof(osal_timer_data_t));
  if (timer_data == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  timer_data->callback = callback;
  timer_data->param = param;

  UBaseType_t auto_reload = (type == OSAL_TIMER_PERIODIC) ? pdTRUE : pdFALSE;
  TimerHandle_t timer = xTimerCreate(name, pdMS_TO_TICKS(period_ms),
                                     auto_reload, timer_data,
                                     timer_callback_wrapper);

  if (timer == NULL)
  {
    vPortFree(timer_data);
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  if (auto_start)
  {
    if (xTimerStart(timer, 0) != pdPASS)
    {
      xTimerDelete(timer, 0);
      vPortFree(timer_data);
      return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_INIT_FAIL, 0);
    }
  }

  *timer_handle = (osal_timer_handle_t)timer;
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_timer_delete(osal_timer_handle_t timer_handle)
{
  if (timer_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  /* Free the timer data structure */
  osal_timer_data_t *timer_data = (osal_timer_data_t *)pvTimerGetTimerID((TimerHandle_t)timer_handle);
  if (timer_data != NULL)
  {
    vPortFree(timer_data);
  }

  xTimerDelete((TimerHandle_t)timer_handle, 0);
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_timer_start(osal_timer_handle_t timer_handle)
{
  if (timer_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  if (xTimerStart((TimerHandle_t)timer_handle, 0) != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_OPERATION_DENIED, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_timer_stop(osal_timer_handle_t timer_handle)
{
  if (timer_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  if (xTimerStop((TimerHandle_t)timer_handle, 0) != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_OPERATION_DENIED, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_timer_reset(osal_timer_handle_t timer_handle)
{
  if (timer_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  if (xTimerReset((TimerHandle_t)timer_handle, 0) != pdPASS)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_OPERATION_DENIED, 0);
  }

  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

/* ========================================================================== */
/* Event Group Implementation                                                 */
/* ========================================================================== */

uint32_t osal_event_group_create(osal_event_group_handle_t *event_group_handle)
{
  if (event_group_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  EventGroupHandle_t event_group = xEventGroupCreate();

  if (event_group == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NO_RESOURCE, 0);
  }

  *event_group_handle = (osal_event_group_handle_t)event_group;
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_event_group_delete(osal_event_group_handle_t event_group_handle)
{
  if (event_group_handle == NULL)
  {
    return make_osal_return(ERROR, ERROR_LEVEL_MINOR, ERR_TYPE_NULL_POINTER, 0);
  }

  vEventGroupDelete((EventGroupHandle_t)event_group_handle);
  return make_osal_return(SUCCESS, ERROR_LEVEL_MINOR, ERR_TYPE_SUCCESS, 0);
}

uint32_t osal_event_group_set_bits(osal_event_group_handle_t event_group_handle,
                                   uint32_t bits_to_set)
{
  if (event_group_handle == NULL)
  {
    return 0;
  }

  return (uint32_t)xEventGroupSetBits((EventGroupHandle_t)event_group_handle,
                                      (EventBits_t)bits_to_set);
}

uint32_t osal_event_group_clear_bits(osal_event_group_handle_t event_group_handle,
                                     uint32_t bits_to_clear)
{
  if (event_group_handle == NULL)
  {
    return 0;
  }

  return (uint32_t)xEventGroupClearBits((EventGroupHandle_t)event_group_handle,
                                        (EventBits_t)bits_to_clear);
}

uint32_t osal_event_group_wait_bits(osal_event_group_handle_t event_group_handle,
                                    uint32_t bits_to_wait_for,
                                    bool clear_on_exit,
                                    bool wait_for_all,
                                    uint32_t timeout_ms)
{
  if (event_group_handle == NULL)
  {
    return 0;
  }

  BaseType_t clear_on_exit_flag = clear_on_exit ? pdTRUE : pdFALSE;
  BaseType_t wait_for_all_flag = wait_for_all ? pdTRUE : pdFALSE;

  return (uint32_t)xEventGroupWaitBits((EventGroupHandle_t)event_group_handle,
                                       (EventBits_t)bits_to_wait_for,
                                       clear_on_exit_flag,
                                       wait_for_all_flag,
                                       ms_to_ticks(timeout_ms));
}

/* ========================================================================== */
/* System API Implementation                                                  */
/* ========================================================================== */

uint32_t osal_get_version(void)
{
  return OSAL_VERSION;
}

uint32_t osal_get_tick_count(void)
{
  return (uint32_t)xTaskGetTickCount();
}

uint32_t osal_ms_to_ticks(uint32_t ms)
{
  return (uint32_t)pdMS_TO_TICKS(ms);
}

uint32_t osal_ticks_to_ms(uint32_t ticks)
{
  return (uint32_t)((ticks * 1000) / configTICK_RATE_HZ);
}
