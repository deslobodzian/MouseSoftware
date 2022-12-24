#ifndef MOUSE_pmw3360_H
#define MOUSE_pmw3360_H

#include <sensor/pmw3360.h>
#include <zephyr/spinlock.h>
#include <zephyr/sys/sem.h>
#include "event_manager.h"

#define DISABLE_REST    false
#define INIT_CPI        800

typedef enum {
    PMW3360_DISABLED,
    PMW3360_DISCONNECTED,
    PMW3360_IDLE,
    PMW3360_FETCHING,
    PMW3360_SUSPENDED,
} state;

typedef struct {
    struct k_spinlock lock;
    state state;
} pmw3360_state;

typedef struct {
    struct k_sem sem;
    const struct device *device;
    const struct sensor_trigger *trigger;
    pmw3360_state state;
} pmw3360_device;


const struct device *get_pmw3360_device(void);

void init_pmw3360(void);

void enable_trigger(const struct device *pmw3360);
void disable_trigger(const struct device *pmw3360);

int fetch_pmw3360_data(const struct device *pmw3360);

void data_ready_handler(const struct device *pmw3360, const struct sensor_trigger *trig);

void read_motion(void);

void motion_thread(void);

int16_t get_dx(const struct device *pmw3360);
int16_t get_dy(const struct device *pmw3360);

/**
 * @brief Set the cpi object
 * Minimum = 100
 * Maximum = 12000
 * Increments of 100
 * @param dev 
 * @param cpi 
 */
void set_cpi(const struct device *pmw3360, int cpi);

/**
 * @brief Get the cpi object
 * 
 * @param dev 
 * @return int 
 */
int get_cpi(const struct device *pmw3360);

#endif //MOUSE_pmw3360_H



 