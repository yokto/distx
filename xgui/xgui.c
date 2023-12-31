#include <zwolf.h>
#include "windows/xgui_windows.h"
#include "wayland/xgui_wayland.h"


__attribute__((constructor)) void init() {
        void* libc = 0;
        void* kernel32 = 0;
        void* user32 = 0;
        void* gdi32 = 0;
        void* wayland_client = 0;
        if (!libc) {
                libc = zwolf_open("msvcrt.dll", ZWOLF_OPEN_EXTERNAL);
                kernel32 = zwolf_open("KERNEL32.DLL", ZWOLF_OPEN_EXTERNAL);
                user32 = zwolf_open("User32.dll", ZWOLF_OPEN_EXTERNAL);
                gdi32 = zwolf_open("Gdi32.dll", ZWOLF_OPEN_EXTERNAL);
		if (libc && kernel32 && user32 && gdi32) {
			init_win(libc, kernel32, user32, gdi32);
		} else if (libc) {
			zwolf_write("could not open kernel32.dll");
		}
        }
        if (!libc) {
                libc = zwolf_open("libc.so.6", ZWOLF_OPEN_EXTERNAL);
                wayland_client = zwolf_open("libwayland-client.so", ZWOLF_OPEN_EXTERNAL);
		if (libc && wayland_client) {
			init_wayland(libc, wayland_client);
		} else if (libc) {
			zwolf_write("could not open wayland-client.so");
		}
        }
        if (!libc) {
                __builtin_trap();
        }
}


