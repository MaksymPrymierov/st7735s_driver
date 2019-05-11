#ifndef ST7735S_TYPES_H
#define ST7735S_TYPES_H

struct st7735s_device_functions
{
        void (*draw_rect) (u16, u16, u16, u16, u16);
        void (*fill_screen) (u16);
        void (*update_screen) (void);
};

#endif