#include <stdio.h>
#include <stdlib.h>
#include <wayland-server-core.h>
#include <wlr/backend.h>
#include <wlr/backend/multi.h>
#include <wlr/backend/wayland.h>
#include <wlr/backend/x11.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/util/log.h>

struct sample_state {
  struct wlr_compositor *compositor;
  struct wlr_seat *seat;
};

void focus_change_notify(struct wl_listener *listener, void *data) {
  struct sample_state *state = wl_container_of(listener, state, seat);
  struct wlr_surface *focused_surface =
      state->seat->keyboard_state.focused_surface;

  if (focused_surface) {
    wlr_log(WLR_DEBUG, "Focused surface: %p", focused_surface);
  } else {
    wlr_log(WLR_DEBUG, "No surface focused.");
  }
}

int main(int argc, char **argv) {
  wlr_log_init(WLR_DEBUG, NULL);

  struct wl_display *display = wl_display_create();
  struct wlr_backend *backend = wlr_backend_autocreate(display, NULL);
  if (!backend) {
    wlr_log(WLR_ERROR, "Failed to create backend");
    return EXIT_FAILURE;
  }

  struct sample_state state = {0};
  struct wlr_renderer *renderer = wlr_renderer_autocreate(backend);
  state.compositor = wlr_compositor_create(display, (uint32_t)12.1, renderer);

  // Seat to track input devices and focused surfaces
  state.seat = wlr_seat_create(display, "seat0");

  // Set up listener for focus changes
  struct wl_listener focus_listener;
  focus_listener.notify = focus_change_notify;
  wl_signal_add(&state.seat->events.request_set_selection, &focus_listener);

  if (!wlr_backend_start(backend)) {
    wlr_log(WLR_ERROR, "Failed to start backend");
    wl_display_destroy(display);
    return EXIT_FAILURE;
  }

  wlr_log(WLR_DEBUG, "Running Wayland compositor...");
  wl_display_run(display);

  wl_display_destroy(display);
  return EXIT_SUCCESS;
}
