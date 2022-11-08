#ifndef MOUSE_PMW3360_H
#define MOUSE_PMW3360_H

#include "sensor/pmw3360.h"

#define DISABLE_REST    false
#define INIT_CPI        800

const struct device *get_pmw3360_device(void);

void init_pmw3360(const struct device *dev);
int fetch_pmw3360_data(const struct device *dev);

int get_dx(const struct device *dev);

int get_dy(const struct device *dev);

/**
 * @brief Set the cpi object
 * Minimum = 100
 * Maximum = 12000
 * Increments of 100
 * @param dev 
 * @param cpi 
 */
void set_cpi(const struct device *dev, int cpi);

/**
 * @brief Get the cpi object
 * 
 * @param dev 
 * @return int 
 */
int get_cpi(const struct device *dev);

#endif //MOUSE_PMW3360_H



 