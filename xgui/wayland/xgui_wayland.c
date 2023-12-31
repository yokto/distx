#include <zwolf.h>
#include "xgui_wayland.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wayland-client.h"
#include "xdg-shell-client-protocol.h"
#include "wayland_wrapper.h"


#include "shm.h"

const struct wl_interface * wl_shm_interface_p;
const struct wl_interface * wl_compositor_interface_p;
const struct wl_interface * wl_registry_interface_p;
const struct wl_interface * wl_surface_interface_p;
const struct wl_interface * wl_shm_pool_interface_p;
const struct wl_interface * wl_buffer_interface_p;


/* Wayland code */
struct client_state {
    /* Globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_shm *wl_shm;
    struct wl_compositor *wl_compositor;
    struct xdg_wm_base *xdg_wm_base;
    /* Objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
};

static int width = 800;
static int height = 600;
static unsigned int* pixels = 0;


// Function to update pixel buffer
static void UpdatePixels() {
    // Set all pixels to red (ARGB format)
    for (int i = 0; i < width * height; ++i)
    {
        pixels[i] = 0xFFFF0000; // Red
        if (i < 12000)
        {
            pixels[i] = 0xFF00FF00; // Red
        }
    }
}

static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void
registry_global(void *data, struct wl_registry *wl_registry,
        uint32_t name, const char *interface, uint32_t version)
{
    printf("connected %s\n", interface);
    struct client_state *state = data;
    if (strcmp(interface, wl_shm_interface_p->name) == 0) {
        state->wl_shm = wl_registry_bind(
                wl_registry, name, wl_shm_interface_p, 1);
    } else if (strcmp(interface, wl_compositor_interface_p->name) == 0) {

        printf("compositor\n");
        state->wl_compositor = wl_registry_bind(
                wl_registry, name, wl_compositor_interface_p, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(
                wl_registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base,
                &xdg_wm_base_listener, state);
    }
}

static void
registry_global_remove(void *data,
        struct wl_registry *wl_registry, uint32_t name)
{
    /* This space deliberately left blank */
}

static const struct wl_registry_listener wl_registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

static void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

static struct wl_buffer *
draw_frame(struct client_state *state)
{
    const int width = 640, height = 480;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = shm_map(size, fd);
    if (data == 0) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(state->wl_shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
            width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    /* Draw checkerboxed background */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8)
                data[y * width + x] = 0xFF666666;
            else
                data[y * width + x] = 0xFFEEEEEE;
        }
    }

    shm_unmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static void
xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial)
{
    struct client_state *state = data;
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_buffer *buffer = draw_frame(state);
    wl_surface_attach(state->wl_surface, buffer, 0, 0);
    wl_surface_commit(state->wl_surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

void init_wayland(void* libc, void* wayland_client) {
    zwolf_write("init wayland\n");
    init_wrapper(wayland_client);

    wl_shm_interface_p = zwolf_sym(wayland_client, "wl_shm_interface");
    wl_compositor_interface_p = zwolf_sym(wayland_client, "wl_compositor_interface");
    wl_registry_interface_p = zwolf_sym(wayland_client, "wl_registry_interface");
    wl_surface_interface_p = zwolf_sym(wayland_client, "wl_surface_interface");
    wl_shm_pool_interface_p = zwolf_sym(wayland_client, "wl_shm_pool_interface");
    wl_buffer_interface_p = zwolf_sym(wayland_client, "wl_buffer_interface");

    //xdg_shell_types[7]
    init_shm(libc);

    struct client_state state = { 0 };
    state.wl_display = wl_display_connect(NULL);
    state.wl_registry = wl_display_get_registry(state.wl_display);
    wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);
    wl_display_roundtrip(state.wl_display);

    state.wl_surface = wl_compositor_create_surface(state.wl_compositor);
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.wl_surface);
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);
    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_set_title(state.xdg_toplevel, "Example client");
    wl_surface_commit(state.wl_surface);

    while (wl_display_dispatch(state.wl_display)) {
        /* This space deliberately left blank */
    }
}
