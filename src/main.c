#include <pebble.h>

Window *window;
Layer *display_layer;

char *cmonth[14] = {"NIL ", "Thout ", "Paope ", "Hathor ","Koiahk ","Tobe ","Meshir ","Paramhotep ","Parmoute ","Pashons ","Paone ","Epep ","Paope ","Nesi "};
char timeText[16] = "Just pray.";
char cdateText[20] = " ";
char gdateText[20] = " ";

/** Array used for fast number character lookup */
char *numbers1[10] = {"","1","2","3","4","5","6","7","8","9"};
char *numbers2[10] = {"0","1","2","3","4","5","6","7","8","9"};

void display_layer_update_callback(Layer *me, GContext* ctx) {    
    graphics_context_set_text_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
    
    graphics_draw_text(ctx,
                       timeText,
                       fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD),
                       GRect(0, 0, 144, 43),
                       GTextOverflowModeWordWrap,
                       GTextAlignmentCenter,
                       NULL);    
	
    graphics_draw_line(ctx,GPoint(1,49),GPoint(143,49));
    graphics_draw_line(ctx,GPoint(0,50),GPoint(144,50));
    graphics_draw_line(ctx,GPoint(1,51),GPoint(143,51));
    
    
    graphics_draw_text(ctx,
                       gdateText,
                       fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
                       GRect(2, 70, 142, 29),
                       GTextOverflowModeWordWrap,
                       GTextAlignmentLeft,
                       NULL);
    
    
    graphics_draw_text(ctx,
                       cdateText,
                       fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
                       GRect(2, 110, 142, 29),
                       GTextOverflowModeWordWrap,
                       GTextAlignmentRight,
                       NULL);
    
}

int julian(struct tm *gdate){
    int I = gdate->tm_year+1900;
    int J = gdate->tm_mon + 1;
    int K = gdate->tm_mday;
    int JD = K-32075+1461*(I+4800+(J-14)/12)/4+367*(J-2-(J-14)/12*12)/12-3*((I+4900+(J-14)/12)/100)/4;
    return JD; 
}

void update_dates(struct tm *t){ 
    strftime(gdateText, sizeof(gdateText), "%a.  %b %e", t);
        
    int JD = julian(t);
        
    if (t->tm_hour > 17) JD = JD+1;
    int r = ( (JD- 1824665) % 1461 ) ;
    int n = ( r % 365 ) + 365 * ( r/ 1460 ) ;
    int month = ( n/ 30 ) + 1 ;
    int day   = ( n% 30 ) + 1 ;
        
    char cday[10]= {'\0'};
    strcat(cday, numbers1[day / 10]);
    strcat(cday, numbers2[day % 10]);
        
    strcpy(cdateText, cmonth[month]);
    strcat(cdateText, cday);
}

void handle_minute_tick(struct tm *t, TimeUnits units){     
	if(clock_is_24h_style()){
		strftime(timeText, sizeof(timeText), "%H:%M", t);
	}
	else{
		strftime(timeText, sizeof(timeText), "%I:%M", t);
	}
	update_dates(t);
	layer_mark_dirty(display_layer);
}

void init() {    
    window = window_create();
    window_stack_push(window, true);
    
    window_set_background_color(window, GColorBlack);
        
    display_layer = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(display_layer, display_layer_update_callback);
    layer_add_child(window_get_root_layer(window), display_layer);
	
	tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
    
   // update_dates(thetime);
    layer_mark_dirty(display_layer);    
}

void deinit(){
	window_destroy(window);
	layer_destroy(display_layer);
}

int main() {
	init();
	app_event_loop();
	deinit();
};