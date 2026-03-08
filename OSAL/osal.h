#ifndef OSAL_H
#define OSAL_H

#include <stdint.h>
#include <stdbool.h>
#include "settings.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/* OSAL Configuration                                                         */
/* ========================================================================== */

/**
 * @brief OSAL version information
 */
#define OSAL_VERSION_MAJOR 1
#define OSAL_VERSION_MINOR 0
#define OSAL_VERSION_PATCH 0
#define OSAL_VERSION ((OSAL_VERSION_MAJOR << 16) | \
                      (OSAL_VERSION_MINOR << 8) |  \
                      OSAL_VERSION_PATCH)

/**
 * @brief Maximum task name length
 */
#ifndef OSAL_MAX_TASK_NAME_LEN
#define OSAL_MAX_TASK_NAME_LEN 32
#endif

/**
 * @brief Infinite timeout value
 */
#define OSAL_WAIT_FOREVER 0xFFFFFFFFU
#define OSAL_NO_WAIT 0x00000000U

  /* ========================================================================== */
  /* OSAL Types and Handles                                                     */
  /* ========================================================================== */

  /**
   * @brief Opaque handle types for OSAL objects
   */
  typedef void *osal_task_handle_t;
  typedef void *osal_semaphore_handle_t;
  typedef void *osal_mutex_handle_t;
  typedef void *osal_queue_handle_t;
  typedef void *osal_timer_handle_t;
  typedef void *osal_event_group_handle_t;

  /**
   * @brief Task function prototype
   */
  typedef void (*osal_task_func_t)(void *param);

  /**
   * @brief Timer callback function prototype
   */
  typedef void (*osal_timer_callback_t)(void *param);

  /**
   * @brief Task priority levels
   */
  typedef enum
  {
    OSAL_PRIORITY_IDLE = 0,
    OSAL_PRIORITY_LOW,
    OSAL_PRIORITY_BELOW_NORMAL,
    OSAL_PRIORITY_NORMAL,
    OSAL_PRIORITY_ABOVE_NORMAL,
    OSAL_PRIORITY_HIGH,
    OSAL_PRIORITY_REALTIME,
    OSAL_PRIORITY_MAX
  } osal_task_priority_t;

  /**
   * @brief Task creation flags
   */
  typedef enum
  {
    OSAL_TASK_STATIC = 0x01,  /* Task uses statically allocated memory */
    OSAL_TASK_DYNAMIC = 0x02, /* Task uses dynamically allocated memory */
  } osal_task_flags_t;

  /**
   * @brief Semaphore types
   */
  typedef enum
  {
    OSAL_SEMAPHORE_BINARY,   /* Binary semaphore */
    OSAL_SEMAPHORE_COUNTING, /* Counting semaphore */
  } osal_semaphore_type_t;

  /**
   * @brief Timer types
   */
  typedef enum
  {
    OSAL_TIMER_ONE_SHOT, /* One-shot timer */
    OSAL_TIMER_PERIODIC, /* Periodic timer */
  } osal_timer_type_t;

  /* ========================================================================== */
  /* Task Management API                                                        */
  /* ========================================================================== */

  /**
   * @brief Create a new task
   *
   * @param name          Task name (for debugging)
   * @param func          Task function to execute
   * @param param         Parameter passed to task function
   * @param stack_size    Stack size in bytes
   * @param priority      Task priority
   * @param task_handle   Pointer to store task handle (output)
   * @param flags         Task creation flags
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_task_create(const char *name,
                            osal_task_func_t func,
                            void *param,
                            uint32_t stack_size,
                            osal_task_priority_t priority,
                            osal_task_handle_t *task_handle,
                            uint32_t flags);

  /**
   * @brief Delete a task
   *
   * @param task_handle   Handle of task to delete
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_task_delete(osal_task_handle_t task_handle);

  /**
   * @brief Delay/sleep the current task
   *
   * @param ms            Delay time in milliseconds
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_task_delay(uint32_t ms);

  /**
   * @brief Yield processor to other tasks
   */
  void osal_task_yield(void);

  /**
   * @brief Get current task handle
   *
   * @return osal_task_handle_t Current task handle
   */
  osal_task_handle_t osal_task_get_current(void);

  /**
   * @brief Get task name
   *
   * @param task_handle   Task handle
   * @param buffer        Buffer to store task name
   * @param buffer_size   Size of buffer
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_task_get_name(osal_task_handle_t task_handle,
                              char *buffer,
                              uint32_t buffer_size);

  /* ========================================================================== */
  /* Semaphore API                                                              */
  /* ========================================================================== */

  /**
   * @brief Create a semaphore
   *
   * @param type          Semaphore type (binary or counting)
   * @param max_count     Maximum count for counting semaphore
   * @param initial_count Initial semaphore count
   * @param sem_handle    Pointer to store semaphore handle (output)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_semaphore_create(osal_semaphore_type_t type,
                                 uint32_t max_count,
                                 uint32_t initial_count,
                                 osal_semaphore_handle_t *sem_handle);

  /**
   * @brief Delete a semaphore
   *
   * @param sem_handle    Semaphore handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_semaphore_delete(osal_semaphore_handle_t sem_handle);

  /**
   * @brief Take/wait for a semaphore
   *
   * @param sem_handle    Semaphore handle
   * @param timeout_ms    Timeout in milliseconds (OSAL_WAIT_FOREVER for infinite)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_semaphore_take(osal_semaphore_handle_t sem_handle,
                               uint32_t timeout_ms);

  /**
   * @brief Give/release a semaphore
   *
   * @param sem_handle    Semaphore handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_semaphore_give(osal_semaphore_handle_t sem_handle);

  /* ========================================================================== */
  /* Mutex API                                                                  */
  /* ========================================================================== */

  /**
   * @brief Create a mutex
   *
   * @param mutex_handle  Pointer to store mutex handle (output)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_mutex_create(osal_mutex_handle_t *mutex_handle);

  /**
   * @brief Delete a mutex
   *
   * @param mutex_handle  Mutex handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_mutex_delete(osal_mutex_handle_t mutex_handle);

  /**
   * @brief Lock a mutex
   *
   * @param mutex_handle  Mutex handle
   * @param timeout_ms    Timeout in milliseconds (OSAL_WAIT_FOREVER for infinite)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_mutex_lock(osal_mutex_handle_t mutex_handle,
                           uint32_t timeout_ms);

  /**
   * @brief Unlock a mutex
   *
   * @param mutex_handle  Mutex handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_mutex_unlock(osal_mutex_handle_t mutex_handle);

  /* ========================================================================== */
  /* Queue API                                                                  */
  /* ========================================================================== */

  /**
   * @brief Create a message queue
   *
   * @param item_size     Size of each queue item in bytes
   * @param queue_length  Maximum number of items in queue
   * @param queue_handle  Pointer to store queue handle (output)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_queue_create(uint32_t item_size,
                             uint32_t queue_length,
                             osal_queue_handle_t *queue_handle);

  /**
   * @brief Delete a message queue
   *
   * @param queue_handle  Queue handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_queue_delete(osal_queue_handle_t queue_handle);

  /**
   * @brief Send an item to the queue
   *
   * @param queue_handle  Queue handle
   * @param item          Pointer to item to send
   * @param timeout_ms    Timeout in milliseconds (OSAL_WAIT_FOREVER for infinite)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_queue_send(osal_queue_handle_t queue_handle,
                           const void *item,
                           uint32_t timeout_ms);

  /**
   * @brief Receive an item from the queue
   *
   * @param queue_handle  Queue handle
   * @param item          Pointer to buffer to receive item
   * @param timeout_ms    Timeout in milliseconds (OSAL_WAIT_FOREVER for infinite)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_queue_receive(osal_queue_handle_t queue_handle,
                              void *item,
                              uint32_t timeout_ms);

  /**
   * @brief Get number of items waiting in the queue
   *
   * @param queue_handle  Queue handle
   * @return uint32_t     Number of items waiting
   */
  uint32_t osal_queue_get_waiting(osal_queue_handle_t queue_handle);

  /**
   * @brief Get number of free spaces in the queue
   *
   * @param queue_handle  Queue handle
   * @return uint32_t     Number of free spaces
   */
  uint32_t osal_queue_get_free(osal_queue_handle_t queue_handle);

  /* ========================================================================== */
  /* Timer API                                                                  */
  /* ========================================================================== */

  /**
   * @brief Create a timer
   *
   * @param name          Timer name (for debugging)
   * @param callback      Timer callback function
   * @param param         Parameter passed to callback
   * @param period_ms     Timer period in milliseconds
   * @param type          Timer type (one-shot or periodic)
   * @param auto_start    Start timer immediately after creation
   * @param timer_handle  Pointer to store timer handle (output)
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_timer_create(const char *name,
                             osal_timer_callback_t callback,
                             void *param,
                             uint32_t period_ms,
                             osal_timer_type_t type,
                             bool auto_start,
                             osal_timer_handle_t *timer_handle);

  /**
   * @brief Delete a timer
   *
   * @param timer_handle  Timer handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_timer_delete(osal_timer_handle_t timer_handle);

  /**
   * @brief Start a timer
   *
   * @param timer_handle  Timer handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_timer_start(osal_timer_handle_t timer_handle);

  /**
   * @brief Stop a timer
   *
   * @param timer_handle  Timer handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_timer_stop(osal_timer_handle_t timer_handle);

  /**
   * @brief Reset a timer
   *
   * @param timer_handle  Timer handle
   * @return uint32_t     Return code (see settings.h)
   */
  uint32_t osal_timer_reset(osal_timer_handle_t timer_handle);

  /* ========================================================================== */
  /* Event Group API                                                            */
  /* ========================================================================== */

  /**
   * @brief Create an event group
   *
   * @param event_group_handle Pointer to store event group handle (output)
   * @return uint32_t          Return code (see settings.h)
   */
  uint32_t osal_event_group_create(osal_event_group_handle_t *event_group_handle);

  /**
   * @brief Delete an event group
   *
   * @param event_group_handle Event group handle
   * @return uint32_t          Return code (see settings.h)
   */
  uint32_t osal_event_group_delete(osal_event_group_handle_t event_group_handle);

  /**
   * @brief Set bits in an event group
   *
   * @param event_group_handle Event group handle
   * @param bits_to_set        Bits to set
   * @return uint32_t          The new event group value after setting bits
   */
  uint32_t osal_event_group_set_bits(osal_event_group_handle_t event_group_handle,
                                     uint32_t bits_to_set);

  /**
   * @brief Clear bits in an event group
   *
   * @param event_group_handle Event group handle
   * @param bits_to_clear      Bits to clear
   * @return uint32_t          The new event group value after clearing bits
   */
  uint32_t osal_event_group_clear_bits(osal_event_group_handle_t event_group_handle,
                                       uint32_t bits_to_clear);

  /**
   * @brief Wait for bits in an event group
   *
   * @param event_group_handle Event group handle
   * @param bits_to_wait_for   Bits to wait for
   * @param clear_on_exit      Clear the bits on exit if successful
   * @param wait_for_all       Wait for all bits (true) or any bit (false)
   * @param timeout_ms         Timeout in milliseconds (OSAL_WAIT_FOREVER for infinite)
   * @return uint32_t          The event group value before clearing (if successful)
   */
  uint32_t osal_event_group_wait_bits(osal_event_group_handle_t event_group_handle,
                                      uint32_t bits_to_wait_for,
                                      bool clear_on_exit,
                                      bool wait_for_all,
                                      uint32_t timeout_ms);

  /* ========================================================================== */
  /* System API                                                                 */
  /* ========================================================================== */

  /**
   * @brief Get OSAL version
   *
   * @return uint32_t OSAL version (major.minor.patch encoded)
   */
  uint32_t osal_get_version(void);

  /**
   * @brief Get system tick count
   *
   * @return uint32_t System tick count in milliseconds
   */
  uint32_t osal_get_tick_count(void);

  /**
   * @brief Convert milliseconds to system ticks
   *
   * @param ms Time in milliseconds
   * @return uint32_t Time in system ticks
   */
  uint32_t osal_ms_to_ticks(uint32_t ms);

  /**
   * @brief Convert system ticks to milliseconds
   *
   * @param ticks Time in system ticks
   * @return uint32_t Time in milliseconds
   */
  uint32_t osal_ticks_to_ms(uint32_t ticks);

  /* ========================================================================== */
  /* Compatibility API (for backward compatibility)                             */
  /* ========================================================================== */

  /**
   * @brief Compatibility function: Create task (old API)
   * @note This maintains compatibility with existing code
   */
  static inline uint32_t OsalTreateCreate(const char *name,
                                          osal_task_func_t func,
                                          void *param,
                                          int stack_size,
                                          void *stack_ptr,
                                          int priority)
  {
    (void)stack_ptr; /* Unused in new API */
    osal_task_handle_t task_handle;
    return osal_task_create(name, func, param, (uint32_t)stack_size,
                            (osal_task_priority_t)priority, &task_handle,
                            OSAL_TASK_DYNAMIC);
  }

  /**
   * @brief Compatibility function: Delay (old API)
   */
  static inline uint32_t OsalDelay(uint32_t ms)
  {
    return osal_task_delay(ms);
  }

  /**
   * @brief Compatibility function: Delete task (old API)
   */
  static inline uint32_t OsalTreateDelete(uint32_t task_id)
  {
    return osal_task_delete((osal_task_handle_t)(uintptr_t)task_id);
  }

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */