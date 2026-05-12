#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

// linux
#include <linux/uinput.h>

// original
#include "uinput_dev.h"


static const uint32_t CP_BACKSPACE = 0x0008;

static int                         fd = -1;
static std::queue<uint32_t>        char_queue;
static std::mutex                  queue_mtx;
static std::condition_variable     queue_cv;
static std::thread                 worker;
static std::atomic<bool>           running{false};

static void emit(int type, int code, int val) {
    struct input_event ev{};
    ev.type  = type;
    ev.code  = code;
    ev.value = val;
    write(fd, &ev, sizeof(ev));
}

static void keyTap(int code) {
    emit(EV_KEY, code, 1); emit(EV_SYN, SYN_REPORT, 0); usleep(5000);
    emit(EV_KEY, code, 0); emit(EV_SYN, SYN_REPORT, 0); usleep(5000);
}

static int hexKeycode(char h) {
    switch (h) {
        case '0': return KEY_0; case '1': return KEY_1;
        case '2': return KEY_2; case '3': return KEY_3;
        case '4': return KEY_4; case '5': return KEY_5;
        case '6': return KEY_6; case '7': return KEY_7;
        case '8': return KEY_8; case '9': return KEY_9;
        case 'a': return KEY_A; case 'b': return KEY_B;
        case 'c': return KEY_C; case 'd': return KEY_D;
        case 'e': return KEY_E; case 'f': return KEY_F;
        default:  return -1;
    }
}

static void sendUnicode(uint32_t cp) {
    emit(EV_KEY, KEY_LEFTCTRL,  1); emit(EV_SYN, SYN_REPORT, 0);
    emit(EV_KEY, KEY_LEFTSHIFT, 1); emit(EV_SYN, SYN_REPORT, 0);
    emit(EV_KEY, KEY_U,         1); emit(EV_SYN, SYN_REPORT, 0);
    emit(EV_KEY, KEY_U,         0); emit(EV_SYN, SYN_REPORT, 0);
    emit(EV_KEY, KEY_LEFTSHIFT, 0); emit(EV_SYN, SYN_REPORT, 0);
    emit(EV_KEY, KEY_LEFTCTRL,  0); emit(EV_SYN, SYN_REPORT, 0);
    usleep(20000);

    char hex[9];
    snprintf(hex, sizeof(hex), "%x", cp);
    for (int i = 0; hex[i]; i++) {
        int k = hexKeycode(hex[i]);
        if (k >= 0) keyTap(k);
    }

    keyTap(KEY_ENTER);
    usleep(10000);
}

static void workerFunc() {
    while (running) {
        std::unique_lock<std::mutex> lock(queue_mtx);
        queue_cv.wait(lock, [] { return !char_queue.empty() || !running; });
        while (!char_queue.empty()) {
            uint32_t cp = char_queue.front();
            char_queue.pop();
            lock.unlock();
            if (cp == CP_BACKSPACE)
                keyTap(KEY_BACKSPACE);
            else
                sendUnicode(cp);
            lock.lock();
        }
    }
}

bool uinputInit() {
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("uinput: open /dev/uinput");
        return false;
    }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);

    const int keys[] = {
        KEY_LEFTCTRL, KEY_LEFTSHIFT, KEY_U, KEY_ENTER, KEY_BACKSPACE,
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
        KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F,
    };
    for (int k : keys) ioctl(fd, UI_SET_KEYBIT, k);

    struct uinput_setup usetup{};
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor  = 0x1234;
    usetup.id.product = 0x5678;
    strncpy(usetup.name, "wiikey", UINPUT_MAX_NAME_SIZE);

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);
    usleep(200000);

    running = true;
    worker  = std::thread(workerFunc);
    return true;
}

void uinputClose() {
    running = false;
    queue_cv.notify_one();
    if (worker.joinable()) worker.join();
    if (fd >= 0) {
        ioctl(fd, UI_DEV_DESTROY);
        close(fd);
        fd = -1;
    }
}

void uinputSendChar(uint32_t cp) {
    if (fd < 0) return;
    {
        std::lock_guard<std::mutex> lock(queue_mtx);
        char_queue.push(cp);
    }
    queue_cv.notify_one();
}

void uinputSendBackspace() {
    uinputSendChar(CP_BACKSPACE);
}
