#include <zwolf.h>
#include <stdio.h>
#include <stdint.h>
#include "wayland-client.h"
#include <stdlib.h>

void notImplemented(char * functionName) {
    printf("not implemented \"%s\"", functionName);
    fflush(stdout);
    exit(1);
}

void
(*wl_event_queue_destroy_p)(struct wl_event_queue *queue) = 0;

struct wl_proxy *
(*wl_proxy_marshal_flags)(struct wl_proxy *proxy, uint32_t opcode,
		       const struct wl_interface *interface,
		       uint32_t version,
		       uint32_t flags, ...) = 0;

struct wl_proxy *
(*wl_proxy_marshal_array_flags_p)(struct wl_proxy *proxy, uint32_t opcode,
			     const struct wl_interface *interface,
			     uint32_t version,
			     uint32_t flags,
			     union wl_argument *args) = 0;

void
(*wl_proxy_marshal_p)(struct wl_proxy *p, uint32_t opcode, ...) = 0;

void
(*wl_proxy_marshal_array_p)(struct wl_proxy *p, uint32_t opcode,
		       union wl_argument *args) = 0;

struct wl_proxy *
(*wl_proxy_create_p)(struct wl_proxy *factory,
		const struct wl_interface *interface) = 0;

void *
(*wl_proxy_create_wrapper_p)(void *proxy) = 0;

void
(*wl_proxy_wrapper_destroy_p)(void *proxy_wrapper) = 0;

struct wl_proxy *
(*wl_proxy_marshal_constructor_p)(struct wl_proxy *proxy,
			     uint32_t opcode,
			     const struct wl_interface *interface,
			     ...) = 0;

struct wl_proxy *
(*wl_proxy_marshal_constructor_versioned_p)(struct wl_proxy *proxy,
				       uint32_t opcode,
				       const struct wl_interface *interface,
				       uint32_t version,
				       ...) = 0;

struct wl_proxy *
(*wl_proxy_marshal_array_constructor_p)(struct wl_proxy *proxy,
				   uint32_t opcode, union wl_argument *args,
				   const struct wl_interface *interface) = 0;

struct wl_proxy *
(*wl_proxy_marshal_array_constructor_versioned_p)(struct wl_proxy *proxy,
					     uint32_t opcode,
					     union wl_argument *args,
					     const struct wl_interface *interface,
					     uint32_t version) = 0;

void
(*wl_proxy_destroy_p)(struct wl_proxy *proxy) = 0;

int
(*wl_proxy_add_listener_p)(struct wl_proxy *proxy,
		      void (**implementation)(void), void *data) = 0;

const void *
(*wl_proxy_get_listener_p)(struct wl_proxy *proxy) = 0;

int
(*wl_proxy_add_dispatcher_p)(struct wl_proxy *proxy,
			wl_dispatcher_func_t dispatcher_func,
			const void * dispatcher_data, void *data) = 0;

void
(*wl_proxy_set_user_data_p)(struct wl_proxy *proxy, void *user_data) = 0;

void *
(*wl_proxy_get_user_data_p)(struct wl_proxy *proxy) = 0;

uint32_t
(*wl_proxy_get_version_p)(struct wl_proxy *proxy) = 0;

uint32_t
(*wl_proxy_get_id_p)(struct wl_proxy *proxy) = 0;

void
(*wl_proxy_set_tag_p)(struct wl_proxy *proxy,
		 const char * const *tag) = 0;

const char * const *
(*wl_proxy_get_tag_p)(struct wl_proxy *proxy) = 0;

const char *
(*wl_proxy_get_class_p)(struct wl_proxy *proxy) = 0;

void
(*wl_proxy_set_queue_p)(struct wl_proxy *proxy, struct wl_event_queue *queue) = 0;

struct wl_display *
(*wl_display_connect_p)(const char *name) = 0;

struct wl_display *
(*wl_display_connect_to_fd_p)(int fd) = 0;

void
(*wl_display_disconnect_p)(struct wl_display *display) = 0;

int
(*wl_display_get_fd_p)(struct wl_display *display) = 0;

int
(*wl_display_dispatch_p)(struct wl_display *display) = 0;

int
(*wl_display_dispatch_queue_p)(struct wl_display *display,
			  struct wl_event_queue *queue) = 0;

int
(*wl_display_dispatch_queue_pending_p)(struct wl_display *display,
				  struct wl_event_queue *queue) = 0;

int
(*wl_display_dispatch_pending_p)(struct wl_display *display) = 0;

int
(*wl_display_get_error_p)(struct wl_display *display) = 0;

uint32_t
(*wl_display_get_protocol_error_p)(struct wl_display *display,
			      const struct wl_interface **interface,
			      uint32_t *id) = 0;

int
(*wl_display_flush_p)(struct wl_display *display) = 0;

int
(*wl_display_roundtrip_queue_p)(struct wl_display *display,
			   struct wl_event_queue *queue) = 0;

int
(*wl_display_roundtrip_p)(struct wl_display *display) = 0;

struct wl_event_queue *
(*wl_display_create_queue_p)(struct wl_display *display) = 0;

int
(*wl_display_prepare_read_queue_p)(struct wl_display *display,
			      struct wl_event_queue *queue) = 0;

int
(*wl_display_prepare_read_p)(struct wl_display *display) = 0;

void
(*wl_display_cancel_read_p)(struct wl_display *display) = 0;

int
(*wl_display_read_events_p)(struct wl_display *display) = 0;

void
(*wl_log_set_handler_client_p)(wl_log_func_t handler) = 0;

void wl_event_queue_destroy(struct wl_event_queue *queue)
{
    notImplemented("wl_event_queue_destroy");
}

struct wl_proxy *
wl_proxy_marshal_array_flags(struct wl_proxy *proxy, uint32_t opcode,
                             const struct wl_interface *interface,
                             uint32_t version,
                             uint32_t flags,
                             union wl_argument *args)
{
    wl_proxy_marshal_array_flags(proxy, opcode,
                                 interface,
                                 version,
                                 flags,
                                 args);
}

void wl_proxy_marshal(struct wl_proxy *p, uint32_t opcode, ...)
{
    notImplemented("wl_proxy_marshal");
}

void wl_proxy_marshal_array(struct wl_proxy *p, uint32_t opcode,
                            union wl_argument *args)
{
    notImplemented("wl_proxy_marshal_array");
}

struct wl_proxy *
wl_proxy_create(struct wl_proxy *factory,
                const struct wl_interface *interface)
{
    notImplemented("wl_proxy_create");
}

void *
wl_proxy_create_wrapper(void *proxy)
{
    notImplemented("wl_proxy_create_wrapper");
}

void wl_proxy_wrapper_destroy(void *proxy_wrapper)
{
    notImplemented("wl_proxy_wrapper_destroy");
}

struct wl_proxy *
wl_proxy_marshal_constructor(struct wl_proxy *proxy,
                             uint32_t opcode,
                             const struct wl_interface *interface,
                             ...)
{
    notImplemented("wl_proxy_marshal_constructor");
}

struct wl_proxy *
wl_proxy_marshal_constructor_versioned(struct wl_proxy *proxy,
                                       uint32_t opcode,
                                       const struct wl_interface *interface,
                                       uint32_t version,
                                       ...)
{
    notImplemented("wl_proxy_marshal_constructor_versioned");
}

struct wl_proxy *
wl_proxy_marshal_array_constructor(struct wl_proxy *proxy,
                                   uint32_t opcode, union wl_argument *args,
                                   const struct wl_interface *interface)
{
    notImplemented("wl_proxy_marshal_array_constructor");
}

struct wl_proxy *
wl_proxy_marshal_array_constructor_versioned(struct wl_proxy *proxy,
                                             uint32_t opcode,
                                             union wl_argument *args,
                                             const struct wl_interface *interface,
                                             uint32_t version)
{
    notImplemented("wl_proxy_marshal_array_constructor_versioned");
}

void wl_proxy_destroy(struct wl_proxy *proxy)
{
    notImplemented("wl_proxy_destroy");
}

int wl_proxy_add_listener(struct wl_proxy *proxy,
                          void (**implementation)(void), void *data)
{
    return wl_proxy_add_listener_p(proxy, implementation, data);
}

const void *
wl_proxy_get_listener(struct wl_proxy *proxy)
{
    notImplemented("wl_proxy_get_listener");
}

int wl_proxy_add_dispatcher(struct wl_proxy *proxy,
                            wl_dispatcher_func_t dispatcher_func,
                            const void *dispatcher_data, void *data)
{
    notImplemented("wl_proxy_add_dispatcher");
}

void wl_proxy_set_user_data(struct wl_proxy *proxy, void *user_data)
{
    notImplemented("wl_proxy_set_user_data");
}

void *
wl_proxy_get_user_data(struct wl_proxy *proxy)
{
    notImplemented("wl_proxy_get_user_data");
}

uint32_t
wl_proxy_get_version(struct wl_proxy *proxy)
{
    return wl_proxy_get_version_p(proxy);
}

uint32_t
wl_proxy_get_id(struct wl_proxy *proxy)
{
    notImplemented("wl_proxy_get_id");
}

void wl_proxy_set_tag(struct wl_proxy *proxy,
                      const char *const *tag)
{
    notImplemented("wl_proxy_set_tag");
}

const char *const *
wl_proxy_get_tag(struct wl_proxy *proxy)
{
    notImplemented("wl_proxy_get_tag");
}

const char *
wl_proxy_get_class(struct wl_proxy *proxy)
{
    notImplemented("wl_proxy_get_class");
}

void wl_proxy_set_queue(struct wl_proxy *proxy, struct wl_event_queue *queue)
{
    notImplemented("wl_proxy_set_queue");
}

struct wl_display *
wl_display_connect(const char *name)
{
    return wl_display_connect_p(name);
}

struct wl_display *
wl_display_connect_to_fd(int fd)
{
    notImplemented("wl_display_connect_to_fd");
}

void wl_display_disconnect(struct wl_display *display)
{
    notImplemented("wl_display_disconnect");
}

int wl_display_get_fd(struct wl_display *display)
{
    notImplemented("wl_display_get_fd");
}

int wl_display_dispatch(struct wl_display *display)
{
    return wl_display_dispatch_p(display);
}

int wl_display_dispatch_queue(struct wl_display *display,
                              struct wl_event_queue *queue)
{
    notImplemented("wl_display_dispatch_queue");
}

int wl_display_dispatch_queue_pending(struct wl_display *display,
                                      struct wl_event_queue *queue)
{
    notImplemented("wl_display_dispatch_queue_pending");
}

int wl_display_dispatch_pending(struct wl_display *display)
{
    notImplemented("wl_display_dispatch_pending");
}

int wl_display_get_error(struct wl_display *display)
{
    notImplemented("wl_display_get_error");
}

uint32_t
wl_display_get_protocol_error(struct wl_display *display,
                              const struct wl_interface **interface,
                              uint32_t *id)
{
    notImplemented("wl_display_get_protocol_error");
}

int wl_display_flush(struct wl_display *display)
{
    notImplemented("wl_display_flush");
}

int wl_display_roundtrip_queue(struct wl_display *display,
                               struct wl_event_queue *queue)
{
    notImplemented("wl_display_roundtrip_queue");
}

int wl_display_roundtrip(struct wl_display *display)
{
    return wl_display_roundtrip_p(display);
}

struct wl_event_queue *
wl_display_create_queue(struct wl_display *display)
{
    notImplemented("wl_display_create_queue");
}

int wl_display_prepare_read_queue(struct wl_display *display,
                                  struct wl_event_queue *queue)
{
    notImplemented("wl_display_prepare_read_queue");
}

int wl_display_prepare_read(struct wl_display *display)
{
    notImplemented("wl_display_prepare_read");
}

void wl_display_cancel_read(struct wl_display *display)
{
    notImplemented("wl_display_cancel_read");
}

int wl_display_read_events(struct wl_display *display)
{
    notImplemented("wl_display_read_events");
}

void wl_log_set_handler_client(wl_log_func_t handler)
{
    notImplemented("wl_log_set_handler_client");
}

int init_wrapper(void* wayland_client) {
    // wayland-client-core
    wl_event_queue_destroy_p = zwolf_sym(wayland_client, "wl_event_queue_destroy");
    wl_proxy_marshal_flags = zwolf_sym(wayland_client, "wl_proxy_marshal_flags");
    wl_proxy_marshal_array_flags_p = zwolf_sym(wayland_client, "wl_proxy_marshal_array_flags");
    wl_proxy_marshal_p = zwolf_sym(wayland_client, "wl_proxy_marshal");
    wl_proxy_marshal_array_p = zwolf_sym(wayland_client, "wl_proxy_marshal_array");
    wl_proxy_create_p = zwolf_sym(wayland_client, "wl_proxy_create");
    wl_proxy_create_wrapper_p = zwolf_sym(wayland_client, "wl_proxy_create_wrapper");
    wl_proxy_wrapper_destroy_p = zwolf_sym(wayland_client, "wl_proxy_wrapper_destroy");
    wl_proxy_marshal_constructor_p = zwolf_sym(wayland_client, "wl_proxy_marshal_constructor");
    wl_proxy_marshal_constructor_versioned_p = zwolf_sym(wayland_client, "wl_proxy_marshal_constructor_versioned");
    wl_proxy_marshal_array_constructor_p = zwolf_sym(wayland_client, "wl_proxy_marshal_array_constructor");
    wl_proxy_marshal_array_constructor_versioned_p = zwolf_sym(wayland_client, "wl_proxy_marshal_array_constructor_versioned");
    wl_proxy_destroy_p = zwolf_sym(wayland_client, "wl_proxy_destroy");
    wl_proxy_add_listener_p = zwolf_sym(wayland_client, "wl_proxy_add_listener");
    wl_proxy_get_listener_p = zwolf_sym(wayland_client, "wl_proxy_get_listener");
    wl_proxy_add_dispatcher_p = zwolf_sym(wayland_client, "wl_proxy_add_dispatcher");
    wl_proxy_set_user_data_p = zwolf_sym(wayland_client, "wl_proxy_set_user_data");
    wl_proxy_get_user_data_p = zwolf_sym(wayland_client, "wl_proxy_get_user_data");
    wl_proxy_get_version_p = zwolf_sym(wayland_client, "wl_proxy_get_version");
    wl_proxy_get_id_p = zwolf_sym(wayland_client, "wl_proxy_get_id");
    wl_proxy_set_tag_p = zwolf_sym(wayland_client, "wl_proxy_set_tag");
    wl_proxy_get_tag_p = zwolf_sym(wayland_client, "wl_proxy_get_tag");
    wl_proxy_get_class_p = zwolf_sym(wayland_client, "wl_proxy_get_class");
    wl_proxy_set_queue_p = zwolf_sym(wayland_client, "wl_proxy_set_queue");
    wl_display_connect_p = zwolf_sym(wayland_client, "wl_display_connect");
    wl_display_connect_to_fd_p = zwolf_sym(wayland_client, "wl_display_connect_to_fd");
    wl_display_disconnect_p = zwolf_sym(wayland_client, "wl_display_disconnect");
    wl_display_get_fd_p = zwolf_sym(wayland_client, "wl_display_get_fd");
    wl_display_dispatch_p = zwolf_sym(wayland_client, "wl_display_dispatch");
    wl_display_dispatch_queue_p = zwolf_sym(wayland_client, "wl_display_dispatch_queue");
    wl_display_dispatch_queue_pending_p = zwolf_sym(wayland_client, "wl_display_dispatch_queue_pending");
    wl_display_dispatch_pending_p = zwolf_sym(wayland_client, "wl_display_dispatch_pending");
    wl_display_get_error_p = zwolf_sym(wayland_client, "wl_display_get_error");
    wl_display_get_protocol_error_p = zwolf_sym(wayland_client, "wl_display_get_protocol_error");
    wl_display_flush_p = zwolf_sym(wayland_client, "wl_display_flush");
    wl_display_roundtrip_queue_p = zwolf_sym(wayland_client, "wl_display_roundtrip_queue");
    wl_display_roundtrip_p = zwolf_sym(wayland_client, "wl_display_roundtrip");
    wl_display_create_queue_p = zwolf_sym(wayland_client, "wl_display_create_queue");
    wl_display_prepare_read_queue_p = zwolf_sym(wayland_client, "wl_display_prepare_read_queue");
    wl_display_prepare_read_p = zwolf_sym(wayland_client, "wl_display_prepare_read");
    wl_display_cancel_read_p = zwolf_sym(wayland_client, "wl_display_cancel_read");
    wl_display_read_events_p = zwolf_sym(wayland_client, "wl_display_read_events");
    wl_log_set_handler_client_p = zwolf_sym(wayland_client, "l_log_set_handler_client");
}