#include "core/time.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <GLFW/glfw3.h>
#endif

static double s_start_time = 0;
static double s_last_time = 0;
static float s_delta = 0;
static int s_fps = 0;
static int s_frame_count = 0;
static double s_fps_timer = 0;

#ifdef _WIN32
static double now(void) {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}
#else
static double now(void) {
    return glfwGetTime();
}
#endif

void time_init(void) {
    s_start_time = now();
    s_last_time = s_start_time;
}

float time_delta(void) {
    return s_delta;
}

float time_total(void) {
    return (float)(now() - s_start_time);
}

int time_fps(void) {
    return s_fps;
}

void time_tick(void) {
    double current = now();
    s_delta = (float)(current - s_last_time);
    s_last_time = current;

    s_frame_count++;
    s_fps_timer += s_delta;
    if (s_fps_timer >= 1.0) {
        s_fps = s_frame_count;
        s_frame_count = 0;
        s_fps_timer -= 1.0;
    }
}
