#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "oled.h"
#include "SHT30.h"
#include "wifi.h"
#include "key.h"
#include "mcu_api.h"
#include "rtc.h"

extern float Temperature;
extern float Humidity;
extern u16 tcnt;


 int main(void)
 {	
	 unsigned char flag = 2;   
	 u8 buff[10];
	 unsigned char i = 0;
	 u8 j;
 
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	OLED_Init();       //OLED初始化
	Sht30_Init();				//温湿度传感器初始化
	wifi_protocol_init();		//wifi初始化
	KEY_Init();		    			 //按键初始化			
	OLED_Clear();           //清屏
	 for(j = 0;j < 6;j++)    //显示  “温湿度传感器”
	 {
		 OLED_ShowCHinese(15+16*j,0,j);
	 }
	while(RTC_Init())		//RTC初始化   计时，每5分钟触发一次上传云端
	{
		OLED_ShowString(0,2,"RTC ERROR!   ",16);	   //初始化失败
		delay_ms(800);
		OLED_ShowString(0,2,"RTC Trying...",16);	    //继续尝试
	}
	while(1)
	{
		wifi_uart_service();     //监听串口
		if(tcnt >= 297)          //每5分钟读取一次数据更新一次屏幕，每次读取数据有3s时间，以减少相对误差
		{
			Sht30_ReadData();     //读取SHT30数据
			Convert_sht30();      //转换为温度、湿度
			sprintf(buff,"%.2f",Temperature);
			OLED_ShowCHinese(0,2,0);
			OLED_ShowCHinese(16,2,2);
			OLED_ShowChar(32,2,':',16);
			OLED_ShowString(48,2,buff,16);
			sprintf(buff,"%.2f",Humidity);
			OLED_ShowCHinese(0,4,1);
			OLED_ShowCHinese(16,4,2);
			OLED_ShowChar(32,4,':',16);
			OLED_ShowString(48,4,buff,16);
		}
		i = KEY_Scan(1);    //扫描按键
		switch(i)     //判断选择的模式
		{
			case 1:mcu_set_wifi_mode(SMART_CONFIG);flag = 0;break;    //SMART配网
			case 2:mcu_set_wifi_mode(AP_CONFIG);flag = 1;break;       //AP配网
			default:break;
		}
		switch(flag)   //不同配网模式指示灯不同情况
		{
			case 0:delay_ms(20);LED = !LED;break;
			case 1:delay_ms(200);LED = !LED;break;
			default:break;
		}
		switch(mcu_get_wifi_work_state())    //判断WiFi情况
		{
			case SMART_CONFIG_STATE:flag = 0;OLED_ShowString(0,6,"SMART MODE     ",8);break;
			case AP_STATE:flag = 1;OLED_ShowString(0,6,"AP MODE        ",8);break;
			case WIFI_NOT_CONNECTED:LED = 1;flag = 2;OLED_ShowString(0,6,"CONNECTING     ",8);break;
			case WIFI_CONNECTED:LED = 0;flag = 2;OLED_ShowString(0,6,"CONNECTED      ",8);break;
			case WIFI_CONN_CLOUD:LED = 1;flag = 2;OLED_ShowString(0,6,"CONNECTED CLOUD",8);break;
			default:break;
		}
	}	 
}


