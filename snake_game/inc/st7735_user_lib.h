
#ifndef BSP_LCD_H_
#define BSP_LCD_H_

#include "st7735.h"
#include <stdint.h>

#define DEVICE_PATH                 "/dev/st7735"

#define CMD_MODE                    SPI_SEND_CMD
#define DATA_MODE                   SPI_SEND_DATA

#define BSP_LCD_WIDTH  		160
#define BSP_LCD_HEIGHT 		128

#define BSP_LCD_HSW 		10
#define BSP_LCD_HBP			20
#define BSP_LCD_HFP			10
#define BSP_LCD_VSW			2
#define BSP_LCD_VBP			2
#define BSP_LCD_VFP			4

#define BLACK 							0x0000
#define WHITE 							0xFFFF
#define RED 							0xF800
#define GREEN 							0x07E0
#define BLUE 							0x001F
#define CYAN 							0x07FF
#define MAGENTA 						0xF81F
#define YELLOW 							0xFFE0
#define ORANGE 							0xFC00


/*Select pixel format */
#define	BSP_LCD_PIXEL_FMT_L8 		1
#define	BSP_LCD_PIXEL_FMT_RGB565	2
#define BSP_LCD_PIXEL_FMT_RGB666    3
#define	BSP_LCD_PIXEL_FMT_RGB888	4
#define BSP_LCD_PIXEL_FMT 			BSP_LCD_PIXEL_FMT_RGB565


/*Select orientation*/
#define PORTRAIT  0
#define LANDSCAPE 1
#define BSP_LCD_ORIENTATION   PORTRAIT

#if(BSP_LCD_ORIENTATION == PORTRAIT)
	#define  BSP_LCD_ACTIVE_WIDTH 			BSP_LCD_WIDTH
	#define  BSP_LCD_ACTIVE_HEIGHT  		BSP_LCD_HEIGHT
#elif(BSP_LCD_ORIENTATION == LANDSCAPE)
	#define  BSP_LCD_ACTIVE_WIDTH 			BSP_LCD_HEIGHT
	#define  BSP_LCD_ACTIVE_HEIGHT 			BSP_LCD_WIDTH
#endif


#define AUTO				 1
#define MANUAL				 0
#define BSP_LCD_CS_MANAGE    MANUAL

#define USE_DMA 0

typedef struct{
    uint16_t x1;
    uint16_t x2;
    uint16_t y1;
    uint16_t y2;
}lcd_area_t;


typedef struct{
    uint8_t orientation;
    uint8_t pixel_format;
    uint8_t * draw_buffer1;
    uint8_t * draw_buffer2;
    uint32_t write_length;
    uint8_t *buff_to_draw;
    uint8_t *buff_to_flush;
    lcd_area_t area;
}bsp_lcd_t;





void bsp_lcd_init(void);
void bsp_lcd_set_orientation(int orientation);
void bsp_lcd_write(uint8_t *buffer, uint32_t nbytes);
void bsp_lcd_set_background_color(uint16_t rgb568);
void bsp_lcd_fill_rect(uint16_t rgb568, uint32_t x_start, uint32_t x_width,uint32_t y_start,uint32_t y_height);
void bsp_lcd_set_display_area(uint16_t x1, uint16_t x2, uint16_t y1 , uint16_t y2);
void bsp_lcd_send_cmd_mem_write(void);
void *bsp_lcd_get_draw_buffer1_addr(void);
void *bsp_lcd_get_draw_buffer2_addr(void);

uint16_t lcd_swap_color(uint16_t color);

void bsp_lcd_fill_circle(uint8_t x_c, uint8_t y_c, uint8_t r, uint16_t color, uint16_t b_color);
void bsp_lcd_draw_font_pixel(uint8_t x_pos, uint8_t y_pos, uint16_t color, uint16_t pixel_size);
void bsp_lcd_print_char(uint8_t x_pos, uint8_t y_pos, uint8_t font_size, uint16_t color, uint16_t b_color, char c);
void bsp_lcd_print_str(uint8_t x_pos, uint8_t y_pos, uint8_t font_size, uint16_t color, uint16_t b_color, char *str);

#endif /* BSP_LCD_H_ */
