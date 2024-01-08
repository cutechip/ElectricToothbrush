#include "Audio.h"
#include "LittleFS.h"
#include <PlayAudio.h>
#include <sys_power.h>

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26
#define I2S_MCLK      0
Audio audio;

uint8_t play_flag = 0; 

void audio_init()
{
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.i2s_mclk_pin_select(I2S_MCLK);
    audio.setVolume(21); // 0...21
    // audio.connecttoFS(LittleFS, "vocal.mp3");
    // audio.connecttohost("http://139.9.65.29:8989/1.mp3"); //  128k mp3
}



void audio_info(const char *info){

    Serial.print("info        "); 
    Serial.println(info);

}



void play_audio_music()
{
    audio.connecttohost("http://139.9.65.29:8989/1.mp3"); //  128k mp3
    set_audio_enable();
    play_flag = 1;
}


void play_audio()
{
    set_audio_enable();
    play_flag = 1;
    change_over_audio();
}



/**
 * @brief 切换语音
 * 
 */
void change_over_audio()
{
    static uint8_t index = 0;
    switch (index)
    {
    case 0:
        audio.connecttoFS(LittleFS, "querun.mp3");
        index++;
        break;
    case 1:
        audio.connecttoFS(LittleFS, "shuijiao.mp3");
        index++;
        break;
    case 2:
        audio.connecttoFS(LittleFS, "vocal.mp3");
        index++;
        break;
    case 3:
        audio.connecttoFS(LittleFS, "zdjd.mp3");
        index++;
        break;
    case 4:
        audio.connecttoFS(LittleFS, "woele.mp3");
        index++;
        break;
     case 5:
        index = 0;
        break;
    default:
        break;
    }
}

/**
 * @brief 停止
 * 
 */
void audio_stop()
{
    audio.stopSong();
    play_flag = 0;
    set_audio_disable();
}



/**
 * @brief 获取播放标志位
 * 
 * @return uint8_t 
 */
uint8_t get_playflag()
{
    return play_flag;
}

void audio_resume()
{
    audio.pauseResume();
    play_flag = 1;
    set_audio_enable();
}


void audio_task()
{
    audio.loop();
}