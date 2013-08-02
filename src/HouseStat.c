#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "resource_ids.auto.h"
#include <stdint.h>
#include <string.h>
// 42c86ea4-1c3e-4a07-b889-2cccca914198
#define MY_UUID {0x42, 0xc8, 0x6e, 0xa4, 0x1c, 0x3e, 0x4a, 0x7, 0xb8, 0x89, 0x2c, 0xcc, 0xca, 0x91, 0x41, 0x98}
PBL_APP_INFO(MY_UUID, "My House", "Haifisch LLP", 0x1, 0x0, DEFAULT_MENU_ICON, APP_INFO_STANDARD_APP);
#define current_state_key 1
static Window window;
static TextLayer temperature_layer;
static TextLayer house_layer;
static TextLayer mode_layer;
static TextLayer toTemp_layer;
static TextLayer toTime_layer;


AppSync sync;
uint8_t sync_buffer[420];
enum {
    WEATHER_TEMPERATURE_KEY = 0x1,
    HOUSE_HUMIDITY_KEY = 0x0,
    HOUSE_MODE_KEY = 0x2,
    HOUSE_TEMPTO_KEY = 0x3,
    HOUSE_TIMETO_KEY = 0x4,
};
// TODO: Error handling
 void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
}

 void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
    
    switch (key) {
        case WEATHER_TEMPERATURE_KEY:
            // App Sync keeps the new_tuple around, so we may use it directly
            text_layer_set_text(&temperature_layer, new_tuple->value->cstring);
            break;
        /*case HOUSE_HUMIDITY_KEY:
            text_layer_set_text(&house_layer, new_tuple->value->cstring);
            break;
        case HOUSE_MODE_KEY:
            text_layer_set_text(&mode_layer, new_tuple->value->cstring);
            break;
        case HOUSE_TEMPTO_KEY:
            text_layer_set_text(&toTemp_layer, new_tuple->value->cstring);
            break;
        case HOUSE_TIMETO_KEY:
            text_layer_set_text(&toTime_layer, new_tuple->value->cstring);
            break;*/
        default:
            return;
    }
}
 void weather_app_init(AppContextRef c) {
    
    window_init(&window, "My house");
    window_stack_push(&window, true);
    window_set_background_color(&window, GColorBlack);

   // window_set_fullscreen(window, true);
    // temp
    text_layer_init(&temperature_layer, GRect(1, 30, 140, 140));
    text_layer_set_text_color(&temperature_layer, GColorWhite);
    text_layer_set_background_color(&temperature_layer, GColorClear);
    text_layer_set_font(&temperature_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(&temperature_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &temperature_layer.layer);
   /*// humidity
    text_layer_init(&house_layer, GRect(0, 40, 144, 48));
    text_layer_set_text_color(&house_layer, GColorWhite);
    text_layer_set_background_color(&house_layer, GColorClear);
    text_layer_set_font(&house_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(&house_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &house_layer.layer);
    //mode 
    text_layer_init(&mode_layer, GRect(0, 60, 144, 48));
    text_layer_set_text_color(&mode_layer, GColorWhite);
    text_layer_set_background_color(&mode_layer, GColorClear);
    text_layer_set_font(&mode_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(&mode_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &mode_layer.layer);
    //toTemp 
    text_layer_init(&toTemp_layer, GRect(0, 80, 144, 48));
    text_layer_set_text_color(&toTemp_layer, GColorWhite);
    text_layer_set_background_color(&toTemp_layer, GColorClear);
    text_layer_set_font(&toTemp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(&toTemp_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &toTemp_layer.layer);
    //time 
    text_layer_init(&toTime_layer, GRect(0, 100, 144, 48));
    text_layer_set_text_color(&toTime_layer, GColorWhite);
    text_layer_set_background_color(&toTime_layer, GColorClear);
    text_layer_set_font(&toTime_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(&toTime_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &toTime_layer.layer);*/

    Tuplet initial_values[] = {
        TupletCString(WEATHER_TEMPERATURE_KEY, "Gathering Data"),
       /* TupletCString(HOUSE_HUMIDITY_KEY, "..."),
        TupletCString(HOUSE_MODE_KEY, ""),
        TupletCString(HOUSE_TEMPTO_KEY, ""),
        TupletCString(HOUSE_TIMETO_KEY, ""),*/
    };
    app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
                  sync_tuple_changed_callback, sync_error_callback, NULL);
}

 void weather_app_deinit(AppContextRef c) {
    app_sync_deinit(&sync);
}

void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &weather_app_init,
        .deinit_handler = &weather_app_deinit,
        .messaging_info = {
            .buffer_sizes = {
                .inbound = 800,
                .outbound = 16,
            }
        }
    };
    app_event_loop(params, &handlers);
}
