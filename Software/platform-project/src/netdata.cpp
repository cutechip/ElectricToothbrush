#include<netdata.h>
#include <HTTPClient.h>
#include <wifi.h>
#include <ArduinoJson.h>
#include <ota.h>
#include <config.h>
#include "LittleFS.h"

// 下面需要更改成你的服务器地址

#define SERVER_CONFIG_URL   "http://XXX/config.json"
#define SERVER_IMAGE_URL   "http://XXX/image.png"

WiFiClient wifiClient;
HTTPClient httpClient;

uint8_t download_buff[512];

uint8_t net_version = 0; 
uint8_t net_img_id = 0; 

/**
 * @brief 联网更新图片
 * 
 */
void download_launcher_image()
{
	httpClient.begin(SERVER_IMAGE_URL);
	int httpCode = httpClient.GET();
	if (httpCode > 0)
	{
		Serial.printf("[HTTP] GET... code: %d\n", httpCode);
		if (httpCode == HTTP_CODE_OK)
		{
			int len = httpClient.getSize();
            File file = LittleFS.open("/launcher.png", "w");
			WiFiClient *stream = httpClient.getStreamPtr();
			while (httpClient.connected() && (len > 0 || len == -1))
			{
				size_t size = stream->available();
				if (size)
				{
					size_t read_size = ((size > sizeof(download_buff)) ? sizeof(download_buff) : size);
					int c = stream->readBytes(download_buff, read_size);
					download_buff[read_size] = 0;
                    file.write(download_buff, read_size);
					if (len > 0)
					{
						len -= c;
					}
				}
			}
            file.close();
		}
	}
	else
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n",
        httpClient.errorToString(httpCode).c_str());
	}

	httpClient.end();
}



/**
 * @brief 获取网络最新配置
 * 
 */
void get_net_last_config()
{
    httpClient.begin(wifiClient, SERVER_CONFIG_URL);
    int httpResponseCode = httpClient.GET();
    if(httpResponseCode > 0) {
        String payload = httpClient.getString();
        Serial1.println(payload);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);
        net_version = root.get<unsigned char>("version");
        net_img_id = root.get<unsigned char>("img_id");
    }
    httpClient.end();

}



/**
 * @brief 更新网络数据
 * 
 */
void update_net_data()
{
    DynamicJsonBuffer jsonBuffer;
    File file = LittleFS.open("/config.json", "r");
   
    JsonObject &config = jsonBuffer.parseObject(file.readString());
    file.close();
    int config_img_id = config.get<unsigned char>("img_id");

    // 联网获取信息
    get_net_last_config();

    // 判断是否需要更新启动图
    if (net_img_id != 0 && net_img_id != config_img_id)
    {
        download_launcher_image();
        file = LittleFS.open("/config.json", "w");
        
        JsonObject& imgConfig = jsonBuffer.createObject();
        config["img_id"] = net_img_id;
        config.prettyPrintTo(file);
        file.close();
    }
    
    // 判断是否需要更新固件
    if (net_version != 0 && net_version > FIRMWARE_VERSION)
    {
        // 开始固件升级
        start_ota();
    }
}


