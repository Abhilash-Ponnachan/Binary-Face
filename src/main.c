#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_hr_layer;
static TextLayer *s_min_layer;
static TextLayer *s_footer_layer;
static GFont s_hr_font;
static GFont s_min_font;

/* format int as binary grouped in 4s */
#define BUFF_LEN_TIME 12
void int_to_bin(int num, int len, char *buff)
{
  buff[--len] = 0;
  while (--len >= 0)
  {
      if (len % 5 == 0)
      {
          buff[len] = ' ';
      }
      else
      {
          buff[len] = num & 0x1 ? '1' : '0';
          num >>= 1;
      }
  }
}

static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char buffer_hr[BUFF_LEN_TIME];
  static char buffer_min[BUFF_LEN_TIME];
  int_to_bin(tick_time->tm_hour, BUFF_LEN_TIME, buffer_hr);
  text_layer_set_text(s_hr_layer, buffer_hr);
  
  int_to_bin(tick_time->tm_min, BUFF_LEN_TIME, buffer_min);
  text_layer_set_text(s_min_layer, buffer_min);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  
  /* Hour text layer */
  s_hr_layer = text_layer_create(GRect(5, 30, 140, 50));
  text_layer_set_text_color(s_hr_layer, GColorBlack);
  text_layer_set_background_color(s_hr_layer, GColorClear);
  s_hr_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROCKB_24));
  //s_hr_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  text_layer_set_font(s_hr_layer, s_hr_font);
  text_layer_set_text_alignment(s_hr_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_hr_layer));
  
  /* Min text layer */
  s_min_layer = text_layer_create(GRect(5, 70, 140, 50));
  text_layer_set_text_color(s_min_layer, GColorBlack);
  text_layer_set_background_color(s_min_layer, GColorClear);
  //s_min_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROCKB_24));
  s_min_font = fonts_get_system_font(FONT_KEY_GOTHIC_28);
  text_layer_set_font(s_min_layer, s_min_font);
  text_layer_set_text_alignment(s_min_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_min_layer));
  
  /* Footer text layer */
  GRect bound = layer_get_bounds(window_layer);
  s_footer_layer = text_layer_create(GRect(5, 120, 140, 50));
  text_layer_set_text_color(s_footer_layer, GColorBlack);
  text_layer_set_background_color(s_footer_layer, GColorClear);
  text_layer_set_text(s_footer_layer, "Abhi @ UST");
  text_layer_set_font(s_footer_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_footer_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_footer_layer));
  
  /* subscribe to time ticker event */
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void main_window_unload(Window *window){
  fonts_unload_custom_font(s_hr_font);
  fonts_unload_custom_font(s_min_font);
  text_layer_destroy(s_hr_layer);
  text_layer_destroy(s_footer_layer);
}

static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  update_time();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}

/* working backup
static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char buffer[] = "00:00";
  
  char *format = clock_is_24h_style() ? "%H:%M" : "%I:%M";
  strftime(buffer, sizeof(buffer), format, tick_time);
  
  text_layer_set_text(s_hr_layer, buffer);
}
*/