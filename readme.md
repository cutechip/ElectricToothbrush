一款带屏幕的电动牙刷，可骨传导播放音乐。

主控采用：ESP32

* 音乐采用内部软件MP3解码通过I2S传输到DAC转换芯片再通过功放放大目前只用于传输网络歌曲

* 软件使用platform开发 编译时记得构建文件系统以及烧写文件系统
* 牙刷识别位置振动智能调控算法还在研究中，欢迎各位大佬去研究。
* 显示屏幕使用的是0.96寸的ST7735的SPI屏幕，显示框架采用LVGL

网络远程JSON配置文件

```
{
    "launcher":"launcher.png",
    "img_id": 22,
    "version": 106
}
```

