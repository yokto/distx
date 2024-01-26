#include <xgui/xgui.h>
#include <stdio.h>
#include <stdlib.h>

struct my_state {
	struct xgui_instance *instance;
	struct xgui_window *window;
	struct xgui_interface_base *base;
};

void print_error(char* error) {
	printf("Error: %s\n", error);
}

void on_instance(struct xgui_instance *instance, struct xgui_error *error, void *state) {
	if (error) {
		printf("Error: No instance\n");
		error->type(&print_error, error->data);
		exit(1);
	}

	struct my_state* my_state = state;
	my_state->instance = instance;

	struct xgui_interface * interface = 0;
	xgui_array_for_each(interface, &instance->interfaces) {
		if (interface->type == &xgui_interface_base_type) {
			my_state->base = (struct xgui_interface_base *)interface->interface;
			printf("found base interface %p\n", my_state->base->create_window);
		}
	}
}

int main() {
	struct my_state my_state = { 0 };
	xgui_create_instance(&on_instance, &on_instance, &my_state);

	return 0;
}
