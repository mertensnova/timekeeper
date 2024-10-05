#include <stdio.h>
#include <stdlib.h>
#include <wlr/backend.h>
#include <wlr/backend/multi.h>
#include <wlr/backend/wayland.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/util/log.h>
#include <wayland-server-core.h>

#define COMPOSITOR_VERSION 4 // Adjust based on supported version

struct sample_state {
    struct wlr_compositor *compositor;
    struct wlr_seat *seat;
    struct wlr_xdg_shell *xdg_shell;
};

void focus_change_notify(struct wl_listener *listener, void *data) {
    struct sample_state *state = wl_container_of(listener, state, seat);
    struct wlr_surface *focused_surface = state->seat->keyboard_state.focused_surface;

    if (focused_surface) {
        // Check if this surface is an xdg_surface (window-like surface)
        struct wlr_xdg_surface *xdg_surface = wlr_xdg_surface_from_wlr_surface(focused_surface);
        if (xdg_surface && xdg_surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL) {
            // Print app id and window title
            wlr_log(WLR_DEBUG, "Focused window: app_id=%s, title=%s", 
                xdg_surface->toplevel->app_id, xdg_surface->toplevel->title);
        } else {
            wlr_log(WLR_DEBUG, "Focused surface: %p (non-xdg toplevel)", focused_surface);
        }
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

    struct wlr_renderer *renderer = wlr_renderer_autocreate(backend);
    if (!renderer) {
        wlr_log(WLR_ERROR, "Failed to create renderer");
        return EXIT_FAILURE;
    }

    struct sample_state state = {0};
    state.compositor = wlr_compositor_create(display,  COMPOSITOR_VERSION, renderer);

    // Create xdg_shell for window-like surfaces (important to get app_id, title)
    state.xdg_shell = wlr_xdg_shell_create(display,COMPOSITOR_VERSION);

    // Seat to track input devices and focused surfaces
    state.seat = wlr_seat_create(display, "seat0");

    // Set up listener for focus changes
    struct wl_listener focus_listener;
    focus_listener.notify = focus_change_notify;
    wl_signal_add(&state.seat->events.keyboard_grab_begin, &focus_listener);

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

