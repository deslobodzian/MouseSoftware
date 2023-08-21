#ifndef MOUSE_PWM3360_H
#define MOUSE_PWM3360_H

#include <nrfx_spim.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

/* Timings defined by spec */
#define T_NCS_SCLK	            1			            /* 120 ns */
#define T_SRX		            (20 - T_NCS_SCLK)	    /* 20 us */
#define T_SCLK_NCS_WR	        (35 - T_NCS_SCLK)	    /* 35 us */
#define T_SWX		            (180 - T_SCLK_NCS_WR)	/* 180 us */
#define T_SRAD		            160			            /* 160 us */
#define T_SRAD_MOTBR	        35			            /* 35 us */
#define T_BEXIT		            1			            /* 500 ns */

/* Timing defined on SROM download burst mode figure */
#define T_BRSEP		            15			            /* 15 us */

/* Sensor registers */
#define PMW3360_REG_PRODUCT_ID			            0x00
#define PMW3360_REG_REVISION_ID			            0x01
#define PMW3360_REG_MOTION			                0x02
#define PMW3360_REG_DELTA_X_L			            0x03
#define PMW3360_REG_DELTA_X_H			            0x04
#define PMW3360_REG_DELTA_Y_L			            0x05
#define PMW3360_REG_DELTA_Y_H			            0x06
#define PMW3360_REG_SQUAL			                0x07
#define PMW3360_REG_RAW_DATA_SUM		            0x08
#define PMW3360_REG_MAXIMUM_RAW_DATA		        0x09
#define PMW3360_REG_MINIMUM_RAW_DATA		        0x0A
#define PMW3360_REG_SHUTTER_LOWER		            0x0B
#define PMW3360_REG_SHUTTER_UPPER		            0x0C
#define PMW3360_REG_CONTROL			                0x0D
#define PMW3360_REG_CONFIG1			                0x0F
#define PMW3360_REG_CONFIG2			                0x10
#define PMW3360_REG_ANGLE_TUNE			            0x11
#define PMW3360_REG_FRAME_CAPTURE		            0x12
#define PMW3360_REG_SROM_ENABLE			            0x13
#define PMW3360_REG_RUN_DOWNSHIFT		            0x14
#define PMW3360_REG_REST1_RATE_LOWER		        0x15
#define PMW3360_REG_REST1_RATE_UPPER		        0x16
#define PMW3360_REG_REST1_DOWNSHIFT		            0x17
#define PMW3360_REG_REST2_RATE_LOWER		        0x18
#define PMW3360_REG_REST2_RATE_UPPER		        0x19
#define PMW3360_REG_REST2_DOWNSHIFT		            0x1A
#define PMW3360_REG_REST3_RATE_LOWER		        0x1B
#define PMW3360_REG_REST3_RATE_UPPER		        0x1C
#define PMW3360_REG_OBSERVATION			            0x24
#define PMW3360_REG_DATA_OUT_LOWER		            0x25
#define PMW3360_REG_DATA_OUT_UPPER		            0x26
#define PMW3360_REG_RAW_DATA_DUMP		            0x29
#define PMW3360_REG_SROM_ID			                0x2A
#define PMW3360_REG_MIN_SQ_RUN			            0x2B
#define PMW3360_REG_RAW_DATA_THRESHOLD		        0x2C
#define PMW3360_REG_CONFIG5			                0x2F
#define PMW3360_REG_POWER_UP_RESET		            0x3A
#define PMW3360_REG_SHUTDOWN			            0x3B
#define PMW3360_REG_INVERSE_PRODUCT_ID		        0x3F
#define PMW3360_REG_LIFTCUTOFF_TUNE3		        0x41
#define PMW3360_REG_ANGLE_SNAP			            0x42
#define PMW3360_REG_LIFTCUTOFF_TUNE1		        0x4A
#define PMW3360_REG_MOTION_BURST		            0x50
#define PMW3360_REG_LIFTCUTOFF_TUNE_TIMEOUT	        0x58
#define PMW3360_REG_LIFTCUTOFF_TUNE_MIN_LENGTH	    0x5A
#define PMW3360_REG_SROM_LOAD_BURST		            0x62
#define PMW3360_REG_LIFT_CONFIG			            0x63
#define PMW3360_REG_RAW_DATA_BURST		            0x64
#define PMW3360_REG_LIFTCUTOFF_TUNE2		        0x65

/* Sensor identification values */
#define PMW3360_PRODUCT_ID			                0x42
#define PMW3360_FIRMWARE_ID			                0x04

/* Max register count readable in a single motion burst */
#define PMW3360_MAX_BURST_SIZE		12

/* Register count used for reading a single motion burst */
#define PMW3360_BURST_SIZE			6

/* Position of X in motion burst data */
#define PMW3360_DX_POS				2
#define PMW3360_DY_POS				4

/* Rest_En position in Config2 register. */
#define PMW3360_REST_EN_POS			5

#define PMW3360_MAX_CPI				12000
#define PMW3360_MIN_CPI				100

#define SPI_WRITE_BIT				BIT(7)

extern const size_t pmw3360_firmware_length;
extern const uint8_t pmw3360_firmware_data[];

enum async_init_step {
	ASYNC_INIT_STEP_POWER_UP,
	ASYNC_INIT_STEP_FW_LOAD_START,
	ASYNC_INIT_STEP_FW_LOAD_CONTINUE,
	ASYNC_INIT_STEP_FW_LOAD_VERIFY,
	ASYNC_INIT_STEP_CONFIGURE,

	ASYNC_INIT_STEP_COUNT
};

typedef struct {
	struct gpio_callback         irq_gpio_cb;
	struct k_spinlock            lock;
	int16_t                      x;
	int16_t                      y;
    sensor_trigger_handler_t     data_ready_handler;
	// struct k_work                trigger_handler_work;
	struct k_work_delayable      init_work;
	enum async_init_step         async_init_step;
	int                          err;
	bool                         ready;
	bool                         last_read_burst;
} pmw3360_data_t;

typedef struct {
	uint32_t cpi;
} pmw3360_config_t;

bool is_pmw3360_ready(void);
int pmw3360_init(void);
int pmw3360_fetch(void);
int set_pmw3360_cpi(uint32_t cpi);
uint32_t get_pmw3360_cpi(void);

int16_t get_pmw3360_dx(void); 
int16_t get_pmw3360_dy(void); 

int update_rest1_sample_time(uint32_t time);
int update_rest2_sample_time(uint32_t time);
int update_rest3_sample_time(uint32_t time);
int pmw3360_enable_rest(bool rest);

#endif /* MOUSE_PWM3360_H */