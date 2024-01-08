#ifndef _PLAYAUDIO_H
#define _PLAYAUDIO_H


void audio_init();
void play_audio_music();
void play_audio();
void audio_task();
void change_over_audio();
uint8_t get_playflag();
void audio_stop();
void audio_resume();
#endif