#pragma once

struct xgui_instance;
struct xgui_error;

void wayland_xgui_create_instance(
    void (*on_instance)(
        struct xgui_instance *instance,
        struct xgui_error *error,
        void *data
        ),
    void *loop,
    void *data);
void init_wayland(void* libc, void* wayland_client);

