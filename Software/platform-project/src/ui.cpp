#include "Arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <input.h>
#include "lvgl.h"
#include <ui.h>
#include<page_menu.h>
#include<motor.h>
#include<sys_power.h>
#include <mpu6050.h>
#include <PlayAudio.h>

#include<charge_dec.h>
#define TFT_CS      5        //管脚定义，左边的屏幕
#define TFT_RST     4
#define TFT_DC      2
#define TFT_SCLK    18
#define TFT_MOSI    23

LV_FONT_DECLARE(myfont);

uint8_t screenWidth = 80;
uint8_t screenHeight = 160;

static lv_disp_draw_buf_t draw_buf; //初始化显示缓冲区
static lv_color_t buf_1[12800];   //用于LVGL使用的缓冲区
lv_group_t *menu_group;
lv_indev_t *indev_encoder;

static lv_obj_t* current_page_content;             
static lv_style_t common_page_style;
static lv_obj_t* title_text;
static lv_obj_t* icon_img;

uint8_t use_time_second = 0;
uint8_t run_status_flag = 0;
static uint32_t page_tick = 0;           // 新页面停留计时
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);



void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.drawRGBBitmap(area->x1, area->y1,(uint16_t *)&(color_p->full), w, h);
    lv_disp_flush_ready(disp);
}


// 按键状态更新函数
static void my_key_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
	static uint32_t last_key = 0;
	uint8_t act_enc = get_key_value();
	
	if(act_enc != 0) {
	    switch(act_enc) {
	        case 1:
	            act_enc = LV_KEY_ENTER;
	            data->state = LV_INDEV_STATE_PRESSED;	
	            break;
	        case 2:
	            act_enc = LV_KEY_RIGHT;
	            data->state = LV_INDEV_STATE_RELEASED;
	            data->enc_diff++;
	            break;
	        case 3:
	            act_enc = LV_KEY_LEFT;
	            data->state = LV_INDEV_STATE_RELEASED;
	            data->enc_diff--;
	            break;
	    }
	    last_key = (uint32_t)act_enc;
	}
	data->key = last_key;
}

// 输入设备初识化函数
void lv_port_indev_init(void)
{
	static lv_indev_drv_t indev_drv;
	lv_indev_drv_init( &indev_drv );
	indev_drv.type = LV_INDEV_TYPE_ENCODER;
	indev_drv.read_cb = my_key_read;
	indev_encoder = lv_indev_drv_register(&indev_drv);
}

// void input_drv_init()
// {
//     menu_group = lv_group_create();
//     lv_group_set_default(menu_group);
//     lv_indev_set_group(indev_encoder, menu_group);
// }

void menu_selected(uint8_t menu_id)
{
    Serial.printf("menu_id:%d", menu_id);
    switch (menu_id)
    {
        case 1:
            set_motor_mode(MOTOR_MODE_IDEL);
            break;
        case 2:
            set_motor_mode(MOTOR_MODE_IDEL);
            set_audio_disable();
            break;
        case 3:
            set_motor_mode(MOTOR_CARE_MODE);
            set_audio_disable();
            break;
        default:
            set_motor_mode(MOTOR_MODE_IDEL);
            break;
    }
}



/**
 * @brief 通用样例
 * 
 */
void common_page_style_init()
{
    lv_style_init(&common_page_style);
    lv_style_set_bg_color(&common_page_style, lv_color_hex(0x000000));
    lv_style_set_pad_all(&common_page_style, 0);
    lv_style_set_border_width(&common_page_style, 0);
    lv_style_set_outline_width(&common_page_style, 0);
    lv_style_set_radius(&common_page_style, 0);
}

void common_page_content_init()
{
    current_page_content = lv_obj_create(lv_scr_act());
    lv_obj_set_size(current_page_content, 80, 160);
    lv_obj_add_style(current_page_content, &common_page_style, 0);

    icon_img = lv_img_create(current_page_content);
    lv_obj_align(icon_img, LV_ALIGN_TOP_MID, 0, 20);

    title_text = lv_label_create(current_page_content);
    lv_obj_set_style_text_font(title_text, &myfont, 0);
    lv_label_set_recolor(title_text, true);
    lv_obj_align(title_text, LV_ALIGN_TOP_MID, 0, 90);
    lv_label_set_text(title_text, " ");
    lv_obj_add_flag(current_page_content, LV_OBJ_FLAG_HIDDEN); 
    lv_obj_set_style_text_color(title_text, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
}



// 隐藏页面
void common_page_content_hide()
{
    lv_obj_add_flag(current_page_content, LV_OBJ_FLAG_HIDDEN);   
}


/**
 * @brief 显示页面
 * 
 */
void common_page_content_show()
{
    lv_obj_clear_flag(current_page_content, LV_OBJ_FLAG_HIDDEN);   
}



/**
 * @brief UI页面初始化
 * 
 */
void ui_init()
{
    
    /* TFT init */
    tft.initR(INITR_MINI160x80_PLUGIN);      // Init ST7735S chip, black tab
    tft.setRotation(4);                //旋转屏幕
    tft.invertDisplay(true);        


    /*初始化显示*/
    lv_init();  //初始化lvgl库
    lv_disp_draw_buf_init(&draw_buf, buf_1, NULL, screenWidth * screenHeight); //开启双缓冲

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;  
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;  /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;      /*Assign the buffer to the display*/
    lv_disp_drv_register(&disp_drv);    //注册显示屏

    // lv_port_indev_init();
    // input_drv_init();
    common_page_style_init();
    common_page_content_init();


    page_menu_init(menu_selected);
    add_menu_item(1, "#ffffff 萌了个芯#", "S:launcher.png");
    add_menu_item(2, "#ffffff 智能模式#", "S:shuaya.png");
    add_menu_item(3, "#ffffff 呵护模式#", "S:baohu.png");
    add_menu_item(4, "#ffffff 听音乐#", "S:music.png");
    add_menu_item(5, "#ffffff      ", "S:chat.png");
    add_menu_item(6, "#ffffff 网络配置#", "S:wifi.png");
    
}


/**
 * @brief 显示刷牙时长
 * 
 */
void show_use_time()
{
    static uint32_t tick = 0;
    static uint8_t  time_cnt = 0;
    char time_str[8];
    static uint8_t an = 0;
    if (millis() - tick >= 100)
    {
        tick = millis();
        if (++time_cnt >= 10)
        {
            time_cnt = 0;
            use_time_second++;
            sprintf(time_str, "%02d:%02d", use_time_second/60, use_time_second%60);
            lv_obj_set_style_bg_color(title_text, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
            lv_label_set_text(title_text, time_str);
        }
        switch (get_position_state())
        {
        case POSITION_LEFT_TOP:
            lv_img_set_angle(icon_img, 3150);
            break;
        case POSITION_LEFT_DOWN:
           lv_img_set_angle(icon_img, 2250);
        break;
        case POSITION_TOP:
            lv_img_set_angle(icon_img, 0);
        break;
        case POSITION_RIGH_TOP:
            lv_img_set_angle(icon_img, 450);
        break;
        case POSITION_RIGH_DOWN:
           lv_img_set_angle(icon_img, 1350);
        break;
        default:
            break;
        } 
    }
}


/**
 * @brief 显示总数
 * 
 */
void show_total_time()
{
    char time_str[8];
    lv_img_set_src(icon_img, "S:tocal_time.png");
    sprintf(time_str, "%02d:%02d", use_time_second/60, use_time_second%60);
    lv_img_set_angle(icon_img, 0);
    lv_label_set_text(title_text, time_str);
}

void persistent_menu(uint8_t menu_id)
{
    if (menu_id == 2 || menu_id == 3)
    {
        hide_menu_page();
        common_page_content_show();
        lv_obj_set_style_bg_color(title_text, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
        lv_img_set_src(icon_img, "S:direction.png");
        run_status_flag = 1;
        use_time_second = 0;
    }
}


/**
 * @brief 返回菜单页面
 * 
 */
void back_menu_page()
{
    run_status_flag = 0;
    page_tick = millis();
    common_page_content_hide();
    show_menu_page();
}


void long_press_menu(uint8_t menu_id)
{
    if (menu_id == 4)
    {
        hide_menu_page();
        common_page_content_show();
        lv_img_set_src(icon_img, "S:play.png");
        lv_label_set_text(title_text, "播放中");
        play_audio_music();
        run_status_flag = 3;
    }

    if (menu_id == 5)
    {
        hide_menu_page();
        common_page_content_show();
        lv_img_set_src(icon_img, "S:select-chat.png");
        lv_label_set_text(title_text, "chat");
        play_audio();
        run_status_flag = 4;
    }
}


/**
 * @brief 音乐播放与停止
 * 
 * @param status 
 */
void audio_play_and_stop(uint8_t status)
{
    if (status)
    {
        lv_label_set_text(title_text, "已暂停");
        audio_stop();
    } else {
        audio_resume();
        lv_label_set_text(title_text, "播放中");
    }
}



/**
 * @brief UI处理
 * 
 */
void ui_process()
{
    uint8_t key = get_key_value();
    uint8_t current_menu_id = get_current_menu_id();
    
    static uint8_t last_menu_id = 0;
    static uint8_t menu_stay_flag = 0;

    static uint8_t last_charge_flag = 0;

    if (last_charge_flag != get_charge_flag())
    {
        last_charge_flag = get_charge_flag();
        if (last_charge_flag)
        {
            hide_menu_page();
            common_page_content_show();
            lv_img_set_src(icon_img, "S:charge.png");
            lv_label_set_text(title_text, "充电中");
        } else {
            page_tick = millis();
            back_menu_page(); 
        }
    }
    last_charge_flag = get_charge_flag();
    if(last_charge_flag) return ;

    if (current_menu_id != last_menu_id)
    {
        menu_stay_flag = 1;
        page_tick = millis();
        run_status_flag = 0;
        Serial.printf("current_menu_id:%d\n", current_menu_id);
        last_menu_id = current_menu_id;
    }

    if (menu_stay_flag && (millis() - page_tick) > 2000)
    {
        menu_stay_flag = 0;
        persistent_menu(current_menu_id);
        Serial.printf("persistent_menu:%d\n", current_menu_id);
    }
    

    if (run_status_flag == 0 && millis() - page_tick > 5000)
    {   
        sys_power_disable();
    }

    switch (run_status_flag)
    {
    case 0:
        if (key == 1)
        {
            page_tick = millis();
            switch_pages();
        }
        else if (key == 2)
        {
            long_press_menu(current_menu_id);
        }
        page_menu_process();
        break;
    case 1:
        if (key == 1)
        { 
           run_status_flag = 2;  
           set_motor_mode(MOTOR_MODE_IDEL);
           page_tick = millis();

        }
        show_use_time();
        break;
    case 2:
        if (millis() - page_tick > 2000)
        {
            back_menu_page(); 
        }
        show_total_time();
        break;
    case 3:
        // 音乐模式
        if (key == 1)
        {
            // 暂停或停止
            audio_play_and_stop(get_playflag());
        }
        else if (key == 2)
        {
            audio_stop();
            back_menu_page(); 
        }
        break;
    case 4:
        // 唠嗑模式
        if (key == 1)
        {
            // 切换
            change_over_audio();
        }
        else if (key == 2)
        {
            back_menu_page(); 
        }
        break;
    default:
        break;
    }
    
}