#ifndef MOUSE_HID_H
#define MOUSE_HID_H

#include <zephyr/kernel.h>
#include "hw_config.h"
#include "hid_config.h"
#include "event_types.h"
#include "event_manager.h"
#include <zephyr/sys/slist.h>
#include <zephyr/sys/byteorder.h>
// #include "usb.h"

#define HID_REPORT_QUEUE_SIZE 16
#define MAX_DATA_SIZE 32
#define HID_STACK_SIZE 1024 // bytes

typedef struct {
    uint8_t data[MAX_DATA_SIZE];
    size_t size;
} hid_report_t;

typedef struct {
    hid_report_t reports[HID_REPORT_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    struct k_sem sem;
} hid_report_queue_t;

typedef struct {
    hid_report_queue_t queue;
    struct k_thread thread;
    k_tid_t tid;
    uint16_t report_interval_hz;
} hid_t;


void init_hid(hid_t* hid);

void init_hid_report_queue(hid_report_queue_t* queue);

bool is_queue_full(hid_report_queue_t* queue);

bool is_queue_empty(hid_report_queue_t* queue);

void enqueue_hid_report(hid_report_queue_t* queue, hid_report_t* report);

hid_report_t* dequeue_hid_report(hid_report_queue_t* queue);

void send_hid_report(hid_t* hid, hid_report_t* report);

hid_report_t generate_motion_report(motion_event_t* event);

void hid_thread(void* arg1, void* arg2, void* arg3);

#endif // MOUSE_HID_H