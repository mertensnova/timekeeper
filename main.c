#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

struct wl_compositor *comp;
void reg_glob(void *data, struct wl_registry *reg, uint32_t name, int8_t *intf,
              uint32_t v) {

  if (!strcmp(intf, wl_compositor_interface.name)) {

    comp = wl_registry_bind(reg, name, intf, 4);
  }
  printf("%u", name);
};

void reg_glob_rem(void *data, struct wl_registry *reg, uint32_t name) {}

struct wl_registry_listener reg_list = {.global = reg_glob,
                                        .global_remove = reg_glob_rem};

int main() {

  struct wl_display *wl_display = wl_display_connect(0);

  struct wl_registry *registry = wl_display_get_registry(wl_display);

  wl_registry_add_listener(registry, &reg_list, 0);

  wl_display_roundtrip(wl_display);
  if (!wl_display) {
    return -1;
  };

  wl_display_disconnect(wl_display);

  return 0;
}
