#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

#include "misc.h"

cairo_surface_t* create_surface(int width, int height) {
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
        width ? : gdk_screen_get_width(lualock.scr),
        height ? : gdk_screen_get_height(lualock.scr));
    return surface;
}

ClutterActor* create_actor(int width, int height) {
	ClutterActor *actor = clutter_cairo_texture_new(
        width ? : gdk_screen_get_width(lualock.scr),
        height ? : gdk_screen_get_height(lualock.scr));
    return actor;
}

void add_actor(ClutterActor *actor) {
    // find the index of the layer at the end of the array
    int i = 0;
    for (; i < lualock.actors_alloc && lualock.actors[i] != NULL; i++);
    
    if (i >= lualock.actors_alloc) {
        lualock.actors_alloc += 20;
        lualock.actors = realloc(lualock.actors, lualock.actors_alloc);
    }
    
    lualock.actors[i] = actor;
    lualock.actors[i + 1] = NULL;
    
    clutter_container_add_actor(CLUTTER_CONTAINER(lualock.stage), actor);
}

void update_layer(ClutterActor *actor, ClutterActor *new) {
    for (int i = 0; i < lualock.actors_alloc && lualock.actors[i] != NULL; i++) {
        if (lualock.actors[i] == actor)
            lualock.actors[i] = new;
    }
}

void parse_color(const char *color, double *r, double *g, double *b, double *a) {
    if (!color) {
        *r = 0, *g = 0, *b = 0, *a = 1;
        return;
    }
    GdkColor color_gdk;
    gdk_color_parse(color, &color_gdk);
    *r = color_gdk.red / (float) (1 << 16);
    *g = color_gdk.green / (float) (1 << 16);
    *b = color_gdk.blue / (float) (1 << 16);
    *a = 1;
}

char* get_password_mask() {
    char password_mask[strlen(lualock.password) + 1];
    for (unsigned int i = 0; i < strlen(lualock.password); i++)
        password_mask[i] = '*';
    password_mask[strlen(lualock.password)] = '\0';
    return strdup(password_mask);
}
