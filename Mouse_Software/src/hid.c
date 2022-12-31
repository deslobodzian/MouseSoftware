#include "hid.h"

K_THREAD_STACK_DEFINE(hid_stack, HID_STACK_SIZE);

void init_hid(hid_t* hid) {
    init_hid_report_queue(hid->queue);
    hid->tid = k_thread_create(
        &hid->thread,
        hid_stack,
        HID_STACK_SIZE,
        (k_thread_entry_t)hid_thread,
        hid,
        NULL,
        NULL,
        K_PRIO_COOP(7),
        0,
        K_NO_WAIT
    );
    k_thread_name_set(hid->tid, "HID Manager");
}

void init_hid_report_queue(hid_report_queue_t* queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    k_sem_init(&queue->sem, 0, HID_REPORT_QUEUE_SIZE);
}

bool is_queue_full(hid_report_queue_t* queue) {
    return queue->count == HID_REPORT_QUEUE_SIZE;
}

bool is_queue_empty(hid_report_queue_t* queue) {
    return queue->count == 0;
}

void enqueue_hid_report(hid_report_queue_t* queue, hid_report_t* report) {
    if (is_queue_full(queue)) {
        return;
    }
    queue->reports[queue->tail] == *report;
    queue->tail = (queue->tail + 1) % HID_REPORT_QUEUE_SIZE;
    queue->count++;
    k_sem_give(&queue->sem);
}

hid_report_t* dequeue_hid_report(hid_report_queue_t* queue) {
    if (is_queue_empty(queue)) {
        return NULL;
    }
    hid_report_t* report = &queue->reports[queue->head];
    queue->head = (queue->head + 1) % HID_REPORT_QUEUE_SIZE;
    queue->count--;
    return report;
}

void send_hid_report(hid_t* hid, hid_report_t* report) {
    hid_write(&usb, report);
}

hid_report_t generate_motion_report(motion_event_t* event) {
    int16_t dx = event->dx;
    int16_t dy = event->dy;
    hid_report_t report;
    report.size = 6;

    uint8_t x_buf[sizeof(dx)];
    uint8_t y_buf[sizeof(dy)];
    sys_put_le16(dx, x_buf);
    sys_put_le16(dy, y_buf);
    uint8_t buffer[6];

    buffer[0] = 0x01;
    buffer[1] = 0x0; //button_bit_mask;
    buffer[2] = 0x0; // wheel;
    buffer[3] = x_buf[0];
    buffer[4] = (y_buf[0] << 4) | (x_buf[1] & 0x0f);
    buffer[5] = (y_buf[1] << 4) | (y_buf[0] >> 4);

    memcpy(report.data, buffer, sizeof(buffer));
    return report;
}

void hid_thread(void* arg1, void* arg2, void* arg3) {
    hid_t* hid = (hid_t*)arg1;
    while (true) {
        k_sem_take(&hid->queue.sem, K_FOREVER);

        hid_report_t* hid_report = dequeue_hid_report(&hid->queue);
        send_hid_report(hid, hid_report);
        uint32_t micro_seconds = (1000000 / hid->report_interval_hz);
        k_sleep(K_USEC(micro_seconds));
    }
}
