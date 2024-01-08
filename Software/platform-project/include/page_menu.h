#ifndef _PAGE_MENU_H
#define _PAGE_MENU_H

void page_menu_init(void (*menu_selected_callback)(uint8_t));
void add_menu_item(uint8_t menu_id, const char *title, const char *src);
void switch_pages();
uint8_t get_current_menu_id();
void page_menu_process();
void hide_menu_page();
void show_menu_page();
#endif