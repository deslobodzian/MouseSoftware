#ifndef MOUSE_EVENT_MANGER_H
#define MOUSE_EVENT_MANGER_H

#include <zephyr/kernel.h>
#include <zephyr/sys/sem.h>
#include <zephyr/spinlock.h>
#include "hid.h"
#include "event_types.h"

#define NUM_EVENTS 10
#define EVENT_MANAGER_STACK_SIZE 512// bytes


typedef struct {
    event_t events[NUM_EVENTS];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    struct k_sem sem;
    struct k_spinlock lock;
} event_queue_t;

typedef struct {
    event_queue_t event_queue;
    struct k_spinlock queue_lock;
    struct k_sem event_sem;
    k_tid_t event_manager_tid;
    struct k_thread thread;
} event_manager_t;

void init_event_queue(event_queue_t* event_queue);

void create_event(event_t* event, event_type_t type, const void* data, size_t size);

event_t create_motion_event(motion_event_t* motion_event);
event_t create_hid_event(hid_event_t* hid_event);

void enqueue_event(event_queue_t* event_queue, event_t event);

event_t dequeue_event(event_queue_t* event_queue);

bool is_queue_empty(event_queue_t* event_queue);

bool is_queue_full(event_queue_t* event_queue);

void init_event_manager(event_manager_t* event_manager);

void start_event_manager(event_manager_t* event_manager);

void stop_event_manager(event_manager_t* event_manager);

void event_manager_thread(void* arg1, void* arg2, void* arg3);


#endif // MOUSE_EVENT_MANGER_H