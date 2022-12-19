#ifndef MOUSE_PMW3360_H
#define MOUSE_PMW3360_H

#include <sensor/pmw3360.h>

#define DISABLE_REST    false
#define INIT_CPI        1600

typedef struct {
    const struct device *device;
    const struct sensor_trigger *trigger;
    uint8_t cpi;
} PMW3360_Data;

const struct device *get_pmw3360_device(void);

void trigger_handler(const struct device *pmw3360, const struct sensor_trigger *trig);
void init_pmw3360(PMW3360_Data *pmw3360);
int fetch_pmw3360_data(const struct device *pmw3360);

int get_dx(const struct device *pmw3360);

int get_dy(const struct device *pmw3360);

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

#endif //MOUSE_PMW3360_H



 