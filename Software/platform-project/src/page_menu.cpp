#include "Arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <input.h>
#include "lvgl.h"
#include <page_menu.h>
#include <input.h>





LV_FONT_DECLARE(myfont);

typedef struct 
{
    uint8_t menu_id;
    const char *title;
    const char *src;
}page_menu_t;

page_menu_t pahe_menu[10];
uint8_t page_menu_len = 0;
uint8_t refresh_flag = 1;
uint8_t menu_index = 0;
static lv_obj_t* current_page_content;             
static lv_style_t page_style;
lv_obj_t* title;
lv_obj_t* icon_img;
void (*menu_selected)(uint8_t);


/**
 * @brief 菜单页面初始化
 * 
 */
void page_menu_init(void (*menu_selected_callback)(uint8_t))
{
    lv_style_init(&page_style);
    lv_style_set_bg_color(&page_style, lv_color_hex(0x000000));
    lv_style_set_pad_all(&page_style, 0);
    lv_style_set_border_width(&page_style, 0);
    lv_style_set_outline_width(&page_style, 0);
    lv_style_set_radius(&page_style, 0);
    

    current_page_content = lv_obj_create(lv_scr_act());
    lv_obj_set_size(current_page_content, 80, 160);
    lv_obj_add_style(current_page_content, &page_style, 0);

    

    icon_img = lv_img_create(current_page_content);
    
    lv_obj_align(icon_img, LV_ALIGN_TOP_MID, 0, 20);

    title = lv_label_create(current_page_content);
    // lv_obj_set_width(title, 40);
    lv_obj_set_style_text_font(title, &myfont, 0);
    lv_label_set_recolor(title, true);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 90);
    lv_label_set_text(title, " ");
    // lv_obj_add_flag(current_page->page_src_obj, LV_OBJ_FLAG_HIDDEN);   

     menu_selected = menu_selected_callback;
}

/**
 * @brief 添加菜单选项
 * 
 * @param title 
 * @param src 
 */
void add_menu_item(uint8_t menu_id, const char *title, const char *src)
{
    if (page_menu_len >= 10) return ;
    pahe_menu[page_menu_len].menu_id = menu_id;
    pahe_menu[page_menu_len].title = title;
    pahe_menu[page_menu_len].src = src;
    page_menu_len++;
}


/**
 * @brief 切换显示页面
 * 
 */
void switch_pages()
{
    refresh_flag = 1;
    menu_index++;
    if (menu_index >= page_menu_len) menu_index = 0;
}


/**
 * @brief 获取当前页面的ID
 * 
 * @return uint8_t 
 */
uint8_t get_current_menu_id()
{
    return pahe_menu[menu_index].menu_id;
}

/**
 * @brief 隐藏菜单
 * 
 */
void hide_menu_page()
{
    lv_obj_add_flag(current_page_content, LV_OBJ_FLAG_HIDDEN);   
}

/**
 * @brief 显示菜单
 * 
 */
void show_menu_page()
{
   lv_obj_clear_flag(current_page_content, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief 菜单页面处理
 * 
 */
void page_menu_process()
{
    if (!refresh_flag) return ;
    lv_img_set_src(icon_img, pahe_menu[menu_index].src);
    lv_label_set_text(title, pahe_menu[menu_index].title);
    if (menu_selected != NULL)
    {
        menu_selected(pahe_menu[menu_index].menu_id);
    }
    refresh_flag = 0;
}