#include "event_manager.h"
#include "structs.h"

LOG_MODULE_REGISTER(event_manager, CONFIG_LOG_DEFAULT_LEVEL);

K_THREAD_STACK_DEFINE(event_manager_stack, EVENT_MANAGER_STACK_SIZE);

void init_event_queue(event_queue_t* event_queue) {
    event_queue->head = 0;
    event_queue->tail = 0;
    event_queue->count = 0;
    k_sem_init(&event_queue->sem, 0, NUM_EVENTS);
}

void create_event(event_t* event, event_type_t type, const void* data, size_t size) {
    event->type = type;
    event->data_size = size;
    event->data = data;
}

event_t create_motion_event(motion_event_t* motion_event) {
    event_t event;
    size_t size = sizeof(motion_event_t);
    create_event(&event, MOTION_EVENT, motion_event, size);
    return event;
}

event_t create_hid_event(hid_event_t* hid_event) {
    event_t event;
    size_t size = sizeof(hid_event_t);
    create_event(&event, HID_EVENT, hid_event, size);
    return event;
}

void enqueue_event(event_queue_t* event_queue, event_t event) {
    if (is_queue_full(event_queue)) {
        return;
    }
    event_queue->events[event_queue->tail] = event;
    event_queue->tail = (event_queue->tail + 1) % NUM_EVENTS;
    event_queue->count++;
    k_sem_give(&event_queue->sem);
}

event_t* dequeue_event(event_queue_t* event_queue) {
    if (is_queue_empty(event_queue)) {
        return NULL;
    }
    event_t* event = &event_queue->events[event_queue->head];
    event_queue->head = (event_queue->head + 1) & NUM_EVENTS;
    event_queue->count--;
    return event;
}

bool is_queue_empty(event_queue_t* event_queue) {
    return event_queue->count == 0;
}

bool is_queue_full(event_queue_t* event_queue) {
    return event_queue->count == NUM_EVENTS;
}

void init_event_manager(event_manager_t* event_manager) {
    init_event_queue(&event_manager->event_queue);
    k_sem_init(&event_manager->event_sem, 0, NUM_EVENTS);
    event_manager->event_manager_tid = k_thread_create(
        &event_manager->thread,
        event_manager_stack,
        EVENT_MANAGER_STACK_SIZE,
        (k_thread_entry_t)event_manager_thread,
        event_manager,
        NULL,
        NULL,
        K_PRIO_COOP(7),
        0,
        K_NO_WAIT
    );
    k_thread_name_set(event_manager->event_manager_tid, "Event Manager");
}

void start_event_manager(event_manager_t* event_manager) {
    k_thread_start(event_manager->event_manager_tid);
}

void stop_event_manager(event_manager_t* event_manager) {
    k_thread_abort(event_manager->event_manager_tid);
}

void event_manager_thread(void* arg1, void* arg2, void* arg3) {
    event_manager_t* event_manager = (event_manager_t*)arg1;

    for(;;) {
        k_sem_take(&event_manager->event_sem, K_FOREVER);
        k_spinlock_key_t key = k_spin_lock(&event_manager->queue_lock);
        while (!is_queue_empty(&event_manager->event_queue)) {
            event_t* event = dequeue_event(&event_manager->event_queue);
            if (!event) {
                break;
            }
            switch (event->type) {
                case MOTION_EVENT: {
                    motion_event_t* motion_event = (motion_event_t*)event->data;
                    handle_motion_event(motion_event);
                    break;
                }
                case BUTTON_EVENT: {
                    break;
                }
                case WHEEL_EVENT: {
                    break;
                }
                case HID_EVENT: {
                    // event.data;
                    hid_event_t* hid_event = (hid_event_t*)event->data;
                   
                    // k_sleep(K_MSEC(1));
                    int ret = hid_write(&usb, hid_event->message);
                    // if (ret) {
                    //     // LOG_DBG("USB write error: %i", ret);
                    // }
                    break;
                }
                case INVALID_EVENT:
                    break;
                default:
                    break;
            }
        }
        k_spin_unlock(&event_manager->queue_lock, key);
    }
}



