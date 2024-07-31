
#include "st7735.h"
#include "st7735_user_lib.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <font.h>

bsp_lcd_t lcd_handle;

bsp_lcd_t *hlcd = &lcd_handle;

/*private helper functions*/

void lcd_config(void);
void lcd_write_cmd(uint8_t cmd);
void lcd_write_data(uint8_t *buffer,uint32_t len);
void lcd_set_orientation(uint8_t orientation);
void lcd_set_display_area(lcd_area_t *area);
void lcd_buffer_init(bsp_lcd_t *lcd);
void make_area(lcd_area_t *area,uint32_t x_start, uint32_t x_width,uint32_t y_start,uint32_t y_height);
void lcd_word_write(uint16_t value);

#define DB_SIZE 					(10UL * 1024UL)
uint8_t bsp_db[DB_SIZE];
uint8_t bsp_wb[DB_SIZE];

int st7735_write(uint8_t MODE, uint8_t *data, uint32_t size)
{
    uint8_t data_to_send[size + 1];
    int ret;
    int fd = open(DEVICE_PATH, O_RDWR);

    if (size < 1)    return -1;
    if (fd < 0)     return fd;

    data_to_send[0] = MODE;

    for (uint32_t i = 0; i < size; i++)
    {
        data_to_send[i+1] = data[i];
    }

    ret = write(fd, data_to_send, size + 1);

    close(fd);
    return ret;
}


void bsp_lcd_init(void)
{
	lcd_handle.orientation = BSP_LCD_ORIENTATION;
	lcd_handle.pixel_format = BSP_LCD_PIXEL_FMT;
	lcd_config();
	hlcd->area.x1 = 0;
	hlcd->area.x2 = BSP_LCD_ACTIVE_WIDTH-1;
	hlcd->area.y1 = 0;
	hlcd->area.y2 = BSP_LCD_ACTIVE_HEIGHT-1;
	lcd_set_display_area(&hlcd->area);
	lcd_set_orientation(hlcd->orientation);
	lcd_buffer_init(hlcd);
}


void bsp_lcd_set_orientation(int orientation)
{
	lcd_set_orientation(orientation);
}

void bsp_lcd_send_cmd_mem_write(void)
{
	lcd_write_cmd(ST7735_WRITE_RAM);
}



void *bsp_lcd_get_draw_buffer1_addr(void)
{
    return (void*)hlcd->draw_buffer1;
}
void *bsp_lcd_get_draw_buffer2_addr(void)
{
	return (void*)hlcd->draw_buffer2;
}

uint16_t lcd_swap_color(uint16_t color)
{
    return ((color << 0x000B)|(color & 0x07E0)|(color >> 0x000B));
}

// void bsp_lcd_fill_circle(uint8_t x_c, uint8_t y_c, uint8_t r, uint16_t color, uint16_t b_color)
// {
//     color = lcd_swap_color(color);
//     b_color = lcd_swap_color(color);

//     bsp_lcd_set_display_area(x_c - r, x_c + r, y_c - r, y_c + r);

//     for (int i = -r; i <= r; i++)
//     {
//         for (int j = -r; j <= r; j++)
//         {
//             if (j == (uint8_t)sqrt((double)(r*r - i*i)) || j == -(uint8_t)sqrt((double)(r*r - i*i)))
//             {
//                 lcd_word_write(color);
//             }else{
//                 lcd_word_write(b_color);
//             }
//         }
//     }
// }

void bsp_lcd_draw_font_pixel(uint8_t x_pos, uint8_t y_pos, uint16_t color, uint16_t pixel_size)
{
    bsp_lcd_set_display_area(x_pos, (x_pos + pixel_size - 1), y_pos, (y_pos + pixel_size - 1));
    for (int i = 0; i < (pixel_size*pixel_size); i++)
    {
        lcd_word_write(color);
    }
}

void bsp_lcd_print_char(uint8_t x_pos, uint8_t y_pos, uint8_t font_size, uint16_t color, uint16_t b_color, char c)
{
    color = lcd_swap_color(color);
    b_color = lcd_swap_color(b_color);

    uint8_t value = 0;

    if (x_pos < font_size)              x_pos = font_size;

    for (int i = 0; i < 0x05; i++)
    {
        for (int j = 0; j < 0x08; j++)
        {
            value = 0;
            value = ((font[c - 0x20][i]));

            if ((value >> j) & 0x01)
            {
                bsp_lcd_draw_font_pixel(x_pos, y_pos, color, font_size);
            }else{
                bsp_lcd_draw_font_pixel(x_pos, y_pos, b_color, font_size);
            }

            y_pos = y_pos + font_size;
        }

        y_pos -= (font_size << 0x03);
        x_pos += font_size;
    }

    x_pos += font_size;

    if (x_pos > ST7735_C_VALUE)
    {
        x_pos = (font_size + 0x01);
        y_pos += (font_size << 0x03);
    }
}

void bsp_lcd_print_str(uint8_t x_pos, uint8_t y_pos, uint8_t font_size, uint16_t color, uint16_t b_color, char *str)
{
    int size = strlen(str);

    for (int i = 0; i < size; i++)
    {
        if (str[i] != '\0')
        {
            bsp_lcd_print_char(x_pos, y_pos, font_size, color, b_color, str[i]);
            x_pos += (font_size * 0x06);
        }
    }
}

void bsp_lcd_write(uint8_t *buffer, uint32_t nbytes)
{
    st7735_write(DATA_MODE, buffer, nbytes);
    return;
}

void bsp_lcd_set_background_color(uint16_t rgb568)
{
    bsp_lcd_fill_rect(rgb568,0,(BSP_LCD_ACTIVE_WIDTH),0,(BSP_LCD_ACTIVE_HEIGHT));
}

void bsp_lcd_fill_rect(uint16_t rgb568, uint32_t x_start, uint32_t x_width,uint32_t y_start,uint32_t y_height)
{
	if ((x_start < 0) || (y_start < 0)) 	return;

    if((x_start+x_width) > BSP_LCD_ACTIVE_WIDTH) return;
	if((y_start+y_height) > BSP_LCD_ACTIVE_HEIGHT) return;

	bsp_lcd_set_display_area((uint16_t)x_start, (uint16_t)(x_start + x_width - 1), (uint16_t)y_start, (uint16_t)(y_start + y_height - 1));
    
	uint8_t params[2];
	params[1] = (~(0xFF00))&(rgb568);
	params[0] = ((~(0x00FF))&(rgb568)) >> 8;
    uint8_t data_trans[(x_start + x_width)*(y_start + y_height)*2];

	for (uint32_t i = 0; i < (x_start + x_width)*(y_start + y_height); i++)
	{
		data_trans[2*i] = params[0];
        data_trans[2*i + 1] = params[1];
	}

    lcd_write_data(data_trans, (x_start + x_width)*(y_start + y_height)*2);
}


static void delay_50ms(void){
	for(uint32_t i = 0 ; i<(0xFFFFU*10U);i++);
}

void lcd_config(void)
{
    //LCD_CSX_LOW();
    lcd_write_cmd(ST7735_SW_RESET);
    for(uint32_t i = 0 ; i<(0xFFFFU * 10U);i++);
    lcd_write_cmd(ST7735_SLEEP_OUT);
    for (uint32_t i = 0; i < (0xFFFFU * 10U); i++);

    lcd_write_cmd(ST7735_FRAME_RATE_CTRL1);
    uint8_t params[16];
    params[0] = 0x01;
    params[1] = 0x2C;
    params[2] = 0x2D;
    lcd_write_data(params, 3);

    lcd_write_cmd(ST7735_FRAME_RATE_CTRL2);
    lcd_write_data(params, 3);

    lcd_write_cmd(ST7735_FRAME_RATE_CTRL3);
    lcd_write_data(params, 3);
    lcd_write_data(params, 3);

    lcd_write_cmd(ST7735_FRAME_INVERSION_CTRL);
    params[0] = 0x07;
    lcd_write_data(params, 1);

    lcd_write_cmd(ST7735_PWR_CTRL1);
    params[0] = 0xA2;
    params[1] = 0x02;
    params[2] = 0x84;
    lcd_write_data(params, 3);

    lcd_write_cmd(ST7735_PWR_CTRL2);
    params[0] = 0xC5;
    lcd_write_data(params, 1);

    lcd_write_cmd(ST7735_PWR_CTRL3);
    params[0] = 0x0A;
    params[1] = 0x00;
    lcd_write_data(params, 2);

    lcd_write_cmd(ST7735_PWR_CTRL4);
    params[0] = 0x8A;
    params[1] = 0x2A;
    lcd_write_data(params, 2);

    lcd_write_cmd(ST7735_PWR_CTRL5);
    params[1] = 0xEE;
    lcd_write_data(params, 2);

    lcd_write_cmd(ST7735_VCOMH_VCOML_CTRL1);
    params[0] = 0x0E;
    lcd_write_data(params, 1);

    lcd_write_cmd(ST7735_DISPLAY_INVERSION_OFF);

    lcd_write_cmd(ST7735_MADCTL);
    params[0] = 0xA0;
    lcd_write_data(params, 1);

    lcd_write_cmd(ST7735_COLOR_MODE);
    params[0] = 0x05;
    lcd_write_data(params, 1);

    lcd_write_cmd(ST7735_CASET);
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    params[3] = 160;
    lcd_write_data(params, 4);

    lcd_write_cmd(ST7735_RASET);
    params[3] = 128;
    lcd_write_data(params, 4);

    lcd_write_cmd(ST7735_PV_GAMMA_CTRL);
    params[0] = 0x02;
    params[1] = 0x1C;
    params[2] = 0x07;
    params[3] = 0x12;
    params[4] = 0x37;
    params[5] = 0x32;
    params[6] = 0x29;
    params[7] = 0x2D;
    params[8] = 0x29;
    params[9] = 0x25;
    params[10] = 0x2B;
    params[11] = 0x39;
    params[12] = 0x00;
    params[13] = 0x01;
    params[14] = 0x03;
    params[15] = 0x10;
    lcd_write_data(params, 16);

    lcd_write_cmd(ST7735_NV_GAMMA_CTRL);
    params[0] = 0x03;
    params[1] = 0x1D;
    params[2] = 0x07;
    params[3] = 0x06;
    params[4] = 0x2E;
    params[5] = 0x2C;
    params[6] = 0x29;
    params[7] = 0x2D;
    params[8] = 0x2E;
    params[9] = 0x2E;
    params[10] = 0x37;
    params[11] = 0x3F;
    params[12] = 0x00;
    params[13] = 0x00;
    params[14] = 0x02;
    params[15] = 0x10;
    lcd_write_data(params, 16);

    lcd_write_cmd(ST7735_NORMAL_DISPLAY_OFF);
    delay_50ms();
    delay_50ms();
    lcd_write_cmd(ST7735_DISPLAY_ON);
}



 void lcd_set_orientation(uint8_t orientation)
{
	uint8_t param;

	if(orientation == LANDSCAPE){
		param = MADCTL_MV | MADCTL_MY | MADCTL_BGR; /*Memory Access Control <Landscape setting>*/
	}else if(orientation == PORTRAIT){
		param = MADCTL_MY| MADCTL_MX| MADCTL_BGR;  /* Memory Access Control <portrait setting> */
	}

	lcd_write_cmd(ST7735_MADCTL);    // Memory Access Control command
	lcd_write_data(&param, 1);
}

void lcd_write_cmd(uint8_t cmd)
{
    st7735_write(CMD_MODE, &cmd, 1);
    return;
}

void lcd_write_data(uint8_t *buffer,uint32_t len)
{
	st7735_write(DATA_MODE, buffer, len);
}


void bsp_lcd_set_display_area(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
    lcd_area_t area;
    area.x1 = x1;
    area.x2 = x2;
    area.y1 = y1;
    area.y2 = y2;
    lcd_set_display_area(&area);
}

void lcd_set_display_area(lcd_area_t *area)
{
    uint8_t params[4];
    /*Column address set(2Ah) */
    params[0] = 0x00;
    params[1] = area->x1;
    params[2] = 0x00;
    params[3] = area->x2;
    lcd_write_cmd(ST7735_CASET);
    lcd_write_data(params, 4);

    params[0] = 0x00;
    params[1] = area->y1;
    params[2] = 0x00;
    params[3] = area->y2;
    lcd_write_cmd(ST7735_RASET);
    lcd_write_data(params, 4);

    lcd_write_cmd(ST7735_WRITE_RAM);
}

void lcd_buffer_init(bsp_lcd_t *lcd)
{
	lcd->draw_buffer1 = bsp_db;
	lcd->draw_buffer2 = bsp_wb;
	lcd->buff_to_draw = NULL;
	lcd->buff_to_flush = NULL;
}


void make_area(lcd_area_t *area,uint32_t x_start, uint32_t x_width,uint32_t y_start,uint32_t y_height)
{
	uint16_t lcd_total_width,lcd_total_height;

	lcd_total_width = BSP_LCD_ACTIVE_WIDTH - 1;
	lcd_total_height = BSP_LCD_ACTIVE_HEIGHT - 1;

	area->x1 = x_start;
	area->x2 = x_start + x_width -1;
	area->y1 = y_start;
	area->y2 = y_start + y_height -1;

	area->x2 = (area->x2 > lcd_total_width) ? lcd_total_width :area->x2;
	area->y2 = (area->y2 > lcd_total_height) ? lcd_total_height : area->y2;

}


void lcd_word_write(uint16_t value)
{
    uint8_t params[2];
    params[0] = (value & 0xFF00) >> 0x08;
    params[1] = value & 0x00FF;
    lcd_write_data(params, 2);
    return;
}