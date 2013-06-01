#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x68, 0xE7, 0x89, 0xE6, 0xBC, 0x9F, 0x49, 0xB4, 0x99, 0x9D, 0x3D, 0x8A, 0xA5, 0x83, 0xE5, 0x66 }
PBL_APP_INFO(MY_UUID,
             "Simple Mono", "Kajong",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_ICON,
             APP_INFO_WATCH_FACE);

Window window;

/* This layer has the hours and minutes. */
TextLayer hourMinute;

/* This layer has the seconds. */
TextLayer seconds;

TextLayer weeDay;

TextLayer monthy;

TextLayer numDay;
TextLayer numMon;

TextLayer amPM;

int first_time = 1;

void handle_tick(AppContextRef ctx, PebbleTickEvent *t);

/* 
 * This is the initialization function. 
 * It sets up all the layers we will use for the rest of
 * the program. I usually get the commands I need from various
 * sample watch faces.
 */
void handle_init(AppContextRef ctx) {
  (void)ctx;

  /* Initialize our root window. */
  window_init(&window, "Militia");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  /* Get all of our app resources ready. */
  resource_init_current_app(&APP_RESOURCE);

  /* Set up a white text layer near the middle of the screen. */
  text_layer_init(&hourMinute, GRect(10, 75, 138, 60)); 
  text_layer_set_text_color(&hourMinute, GColorWhite);
 
  /* The bg is white by default. This will not do. */
  text_layer_set_background_color(&hourMinute, GColorClear);

  /* Important note: the name of the font resource determines size. */
  text_layer_set_font(&hourMinute, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SAXMONO_48)));

  /* Add that text layer as a child of the root window. */
  layer_add_child(&window.layer, &hourMinute.layer);

  text_layer_init(&seconds, GRect(102, 125, 50, 50)); 
  text_layer_set_text_color(&seconds, GColorWhite);
 
  text_layer_set_background_color(&seconds, GColorClear);

  text_layer_set_font(&seconds, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SAXMONO_36)));

  /* Add that text layer as a child of the root window. */
  layer_add_child(&window.layer, &seconds.layer);

  text_layer_init(&weeDay, GRect(4, -7, 70, 45));
  text_layer_set_font(&weeDay, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SAXMONO_36)));
  text_layer_set_text_color(&weeDay, GColorWhite);
  text_layer_set_background_color(&weeDay, GColorClear);

  layer_add_child(&window.layer, &weeDay.layer);

  text_layer_init(&numDay, GRect(102, -7, 70, 45));
  text_layer_set_font(&numDay, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SAXMONO_36)));
  text_layer_set_text_color(&numDay, GColorWhite);
  text_layer_set_background_color(&numDay, GColorClear);

  layer_add_child(&window.layer, &numDay.layer);

  text_layer_init(&monthy, GRect(4, 22, 70, 45));
  text_layer_set_font(&monthy, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SAXMONO_36)));
  text_layer_set_text_color(&monthy, GColorWhite);
  text_layer_set_background_color(&monthy, GColorClear);

  layer_add_child(&window.layer, &monthy.layer);

  text_layer_init(&numMon, GRect(102, 22, 70, 45));
  text_layer_set_font(&numMon, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SAXMONO_36)));
  text_layer_set_text_color(&numMon, GColorWhite);
  text_layer_set_background_color(&numMon, GColorClear);

  layer_add_child(&window.layer, &numMon.layer);

  text_layer_init(&amPM, GRect(4, 125, 60, 50));
  text_layer_set_font(&amPM, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SAXMONO_36)));
  text_layer_set_text_color(&amPM, GColorWhite);
  text_layer_set_background_color(&amPM, GColorClear);
  layer_add_child(&window.layer, &amPM.layer);
  
  handle_tick(0, NULL);
}

/* From K&R's second edition: */
/* reverse: reverse string s in place */
void reverse(char s[])
{
  int c, i, j;
  
  for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/* itoa: convert n to characters in s */
void itoa(int n, char s[])
{
  int i, sign;
  if ((sign = n) < 0)	/* record sign */
    n = -n;		/* make n positive */
  i = 0;
  do {			/* generate digits in reverse order*/
    s[i++] = n % 10 + '0'; 	/* get next digit */
  } while ((n /= 10) > 0);	/* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

void numToDay(int n, char *buf) {
  switch(n) {
    case 0:
      buf[0] = 'S';
      buf[1] = 'u';
      buf[2] = 'n';
      buf[3] = '\0';
      break;
    case 1:
      buf[0] = 'M';
      buf[1] = 'o';
      buf[2] = 'n';
      buf[3] = '\0';
      break;
    case 2:
      buf[0] = 'T';
      buf[1] = 'u';
      buf[2] = 'e';
      buf[3] = '\0';
      break;
    case 3:
      buf[0] = 'W';
      buf[1] = 'e';
      buf[2] = 'd';
      buf[3] = '\0';
      break;
    case 4:
      buf[0] = 'T';
      buf[1] = 'h';
      buf[2] = 'u';
      buf[3] = '\0';
      break;
    case 5:
      buf[0] = 'F';
      buf[1] = 'r';
      buf[2] = 'i';
      buf[3] = '\0';
      break;
    case 6:
      buf[0] = 'S';
      buf[1] = 'a';
      buf[2] = 't';
      buf[3] = '\0';
      break;
  } 
}

void numToMonth(int n, char *buf) {
  switch(n) {
    case 0:
      buf[0] = 'J';
      buf[1] = 'a';
      buf[2] = 'n';
      buf[3] = '\0';
      break;
    case 1:
      buf[0] = 'F';
      buf[1] = 'e';
      buf[2] = 'b';
      buf[3] = '\0';
      break;
    case 2:
      buf[0] = 'M';
      buf[1] = 'a';
      buf[2] = 'r';
      buf[3] = '\0';
      break;
    case 3:
      buf[0] = 'A';
      buf[1] = 'p';
      buf[2] = 'r';
      buf[3] = '\0';
      break;
    case 4:
      buf[0] = 'M';
      buf[1] = 'a';
      buf[2] = 'y';
      buf[3] = '\0';
      break;
    case 5:
      buf[0] = 'J';
      buf[1] = 'u';
      buf[2] = 'n';
      buf[3] = '\0';
      break;
    case 6:
      buf[0] = 'J';
      buf[1] = 'u';
      buf[2] = 'l';
      buf[3] = '\0';
      break;
    case 7:
      buf[0] = 'A';
      buf[1] = 'u';
      buf[2] = 'g';
      buf[3] = '\0';
      break;
    case 8:
      buf[0] = 'S';
      buf[1] = 'e';
      buf[2] = 'p';
      buf[3] = '\0';
      break;
    case 9:
      buf[0] = 'O';
      buf[1] = 'c';
      buf[2] = 't';
      buf[3] = '\0';
      break;
    case 10:
      buf[0] = 'N';
      buf[1] = 'o';
      buf[2] = 'v';
      buf[3] = '\0';
      break;
    case 11:
      buf[0] = 'D';
      buf[1] = 'e';
      buf[2] = 'c';
      buf[3] = '\0';
      break;
  }
}

/*
 * Watches tick. This is where the time changes on the face of the watch.
 */
void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void) ctx;
  (void) t;
  PblTm tick_time;
  int mhours;
  int mmin;
  int msec;
  int wday;
  int mday;
  int month;
  static char timeText[7];
  static char tempbuf[4];
  static char secText[3];
  static char dayText[4];
  static char numDayText[3];
  static char monthText[4];
  static char numMonthy[3];
  static char amPMText[3];

  timeText[0] = '\0';
  secText[0] = '\0';
  numDayText[0] = '\0';
  numMonthy[0] = '\0';
  amPMText[0] = '\0';

  get_time(&tick_time);
  mhours = tick_time.tm_hour;
  mmin = tick_time.tm_min;
  msec = tick_time.tm_sec;
  wday = tick_time.tm_wday;
  mday = tick_time.tm_mday;
  month = tick_time.tm_mon;

  numToDay(wday, dayText);
  text_layer_set_text(&weeDay, dayText);

  if (!(clock_is_24h_style())) {
    if (mhours >= 12) {
      mhours = mhours % 12;
      strcat(amPMText, "PM");
    } else {
      strcat(amPMText, "AM");
    }
    text_layer_set_text(&amPM, amPMText);
    if (mhours == 0)
      mhours = 12;
  } else {
    text_layer_set_text(&amPM, " ");
  }


  itoa(mhours, tempbuf);
  if (mhours < 10)
    strcat(timeText, " ");
  strcat(timeText, tempbuf);

  strcat(timeText, ":");

  itoa(mmin, tempbuf);
  if (mmin < 10)
    strcat(timeText, "0");
  strcat(timeText, tempbuf);

  text_layer_set_text(&hourMinute, timeText);

  itoa(msec, tempbuf);
  if (msec < 10)
    strcat(secText, "0");
  strcat(secText, tempbuf);

  text_layer_set_text(&seconds, secText);

  numToMonth(month, monthText);
  text_layer_set_text(&monthy, monthText);

  itoa(month + 1, tempbuf);
  if (month < 10)
    strcat(numMonthy, " ");
  strcat(numMonthy, tempbuf);

  text_layer_set_text(&numMon, numMonthy);

  itoa(mday, tempbuf);

  if (mday < 10)
    strcat(numDayText, " ");
  strcat(numDayText, tempbuf);

  text_layer_set_text(&numDay, numDayText);

  if (first_time)
    first_time = 0;

}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
