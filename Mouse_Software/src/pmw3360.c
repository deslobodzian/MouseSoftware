#include "pmw3360.h"
#include <zephyr/logging/log.h>
#include "structs.h"

LOG_MODULE_REGISTER(pmw3360_custom, CONFIG_LOG_DEFAULT_LEVEL);
K_THREAD_STACK_DEFINE(thread_stack, 1024);

const struct device *get_pmw3360_device(void) {
	const struct device *const dev = DEVICE_DT_GET_ANY(pixart_pmw3360); 
	if (dev == NULL) {
		LOG_ERR("Could not get %s device\n", DEVICE_DT_GET_ANY(pixart_pmw3360));
		return NULL;
	}
	
	if (!device_is_ready(dev)) {
		LOG_ERR("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}
	LOG_INF("Found device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

void data_ready_handler(const struct device *pmw3360, const struct sensor_trigger *trig) {
    k_spinlock_key_t key = k_spin_lock(&pmw3360_dev.state.lock);

    disable_trigger(pmw3360);

    switch(pmw3360_dev.state.state) {
        case PMW3360_IDLE:
            pmw3360_dev.state.state = PMW3360_FETCHING;
            pmw3360_dev.state.sample = true;
        case PMW3360_DISCONNECTED:
            k_sem_give(&pmw3360_dev.sem);
            break;
        case PMW3360_FETCHING:
        case PMW3360_DISABLED:
        case PMW3360_SUSPENDED:
            break;
        default:
            break;
    }
    k_spin_unlock(&pmw3360_dev.state.lock, key);
}

void init_pmw3360(void) {
    // set up pmw3360 device;
    int err;
    pmw3360_dev.device = get_pmw3360_device();
    bool is_connected = (pmw3360_dev.device != NULL);
    
    k_spinlock_key_t key = k_spin_lock(&pmw3360_dev.state.lock);
    switch(pmw3360_dev.state.state) {
        case PMW3360_DISCONNECTED:
            if (is_connected) {
                pmw3360_dev.state.state = PMW3360_IDLE;
            } else {
                pmw3360_dev.state.state = PMW3360_DISCONNECTED;
            }
            break;
        default:
            break;
    }
    k_spin_unlock(&pmw3360_dev.state.lock, key);

    do {
        err = enable_trigger();
        if (err == -EBUSY) {
            k_sleep(K_MSEC(1));
        }
    } while (err == -EBUSY);

    pmw3360_dev.thread_id = k_thread_create(&pmw3360_dev.thread,
        thread_stack,
	    1024,
		(k_thread_entry_t)motion_thread,
		NULL, NULL, NULL,
		K_PRIO_PREEMPT(0), 0, K_NO_WAIT);
	k_thread_name_set(pmw3360_dev.thread_id, "pmw3360_thread");
}

void start_thread(void) {
    k_thread_start(pmw3360_dev.thread_id);
}

void enable_trigger(const struct device *pmw3360) {
    // pmw3360_dev.trigger->chan = SENSOR_TRIG_DATA_READY;
    // pmw3360_dev.trigger->type = SENSOR_CHAN_ALL;

    // int rc = sensor_trigger_set(pmw3360, pmw3360_dev.trigger, data_ready_handler);
    // if (rc) {
    //     LOG_DBG("Failed to enable trigger");
    // }
}

void disable_trigger(const struct device *pmw3360) {
    // pmw3360_dev.trigger->chan = SENSOR_TRIG_DATA_READY;
    // pmw3360_dev.trigger->type = SENSOR_CHAN_ALL;

    // int rc = sensor_trigger_set(pmw3360, pmw3360_dev.trigger, NULL);
    // if (rc) {
    //     LOG_DBG("Failed to disable trigger");
    // }
}

int fetch_pmw3360_data(const struct device *pmw3360) {
    int rc = sensor_sample_fetch(pmw3360);
    return rc;
}

int read_motion(bool send_event) {
    int16_t dx;
    int16_t dy;

    int rc = fetch_pmw3360_data(pmw3360_dev.device);
    if (!rc) {
        dx = get_dx(pmw3360_dev.device);
        dy = get_dy(pmw3360_dev.device);
    }

    if (!dx && !dy) {
        if (pmw3360_dev.nodata < MAX_NODATA) {
            pmw3360_dev.nodata++;
        } else {
            pmw3360_dev.nodata = 0;
            return -ENODATA;
        }
    } else {
        pmw3360_dev.nodata = 0;
    }

    motion_event_t motion_event; 
    motion_event.dx = dx; 
    motion_event.dy = dy; 

    event_t event = create_motion_event(&motion_event);
    enqueue_event(&manager.event_queue, event);
    k_sem_give(&manager.event_sem);
    return rc;
}

void motion_thread(void) {
    LOG_DBG("Starting PMW3360 thread");
    uint32_t start_time;
    uint32_t stop_time;
    uint32_t cycles_spent;
    uint32_t nanoseconds_spent;
    // init_pmw3360();
    for (;;) {
        // bool send_event;
        // k_sem_take(&pmw3360_dev.sem, K_FOREVER);
        // k_spinlock_key_t key = k_spin_lock(&pmw3360_dev.state.lock);
        // event = (pmw3360_dev.state.state == PMW3360_FETCHING) && pmw3360_dev.state.sample;
        // pmw3360_dev.state.sample = false;
        // k_spin_unlock(&pmw3360_dev.state.lock, key);

        read_motion(true);

        // key = k_spin_lock(&pmw3360_dev.state.lock);
        // if (pmw3360_dev.state.state != PMW3360_FETCHING) {
        //     pmw3360_dev.state.state = PMW3360_IDLE;
        // }
        //     enable_trigger(pmw3360_dev.device);
        // k_spin_unlock(&pmw3360_dev.state.lock, key);
        // if (event) {
            // start_time = k_cycle_get_32();
            // stop_time = k_cycle_get_32();
            // cycles_spent = stop_time - start_time;
            // nanoseconds_spent = (cycles_spent * 1000000000) / 32768;
            // LOG_DBG("PMW3360 took %i seconds", nanoseconds_spent);
        // }
    }
}

int16_t get_dx(const struct device *pmw3360) {
    struct sensor_value x;
	sensor_channel_get(pmw3360, SENSOR_CHAN_POS_DX, &x);
    return x.val1;
}

/* return negative value for Y-axis, as thats how the sensor works.*/
int16_t get_dy(const struct device *pmw3360) {
    struct sensor_value y;
	sensor_channel_get(pmw3360, SENSOR_CHAN_POS_DY, &y);
    return y.val1;
}

/**
 * @brief Set the cpi object
 * Minimum = 100
 * Maximum = 12000
 * Increments of 100
 * @param dev 
 * @param cpi 
 */
void set_cpi(const struct device *pmw3360, int cpi) {
    const struct sensor_value val = {
        .val1 = cpi,
    };
    int rc = sensor_attr_set(pmw3360, SENSOR_CHAN_ALL, PMW3360_ATTR_CPI, &val);
    if (rc == 0) {
        LOG_INF("Set CPI to: %d\n", val.val1);
        // pmw3360->cpi = val.val1;
    } else {
        LOG_INF("Error setting CPI, error %d\n", rc);
    }
}

/**
 * @brief Get the cpi object
 * 
 * @param dev 
 * @return int 
 */
int get_cpi(const struct device *pmw3360) {
    struct sensor_value cpi;
    int rc = 0;
    rc = sensor_attr_get(pmw3360, SENSOR_CHAN_ALL, PMW3360_ATTR_CPI, &cpi);
    LOG_INF("CPI rc = %d\n", rc);
    return cpi.val1;
}