#include <allegro5/allegro.h>
#include <allegro5/keyboard.h>
#include <allegro5/keycodes.h>

int main() {
    if (!al_init()) {
        fprintf(stderr, "Erro ao inicializar Allegro.\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Erro ao inicializar teclado.\n");
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        fprintf(stderr, "Erro ao criar janela.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Erro ao criar fila de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0, 128, 255));
    al_flip_display();

    bool running = true;
    while (running) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                }
            }
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}