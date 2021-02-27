#include "SHT30.h"   
#include "bsp_i2c_gpio.h"

u8 Buff[6]={0};


float Temperature ;
float Humidity ;

void Sht30_Init(void)
{	
	// 在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
while (i2c_CheckDevice(0x88+0));  //发送设备地址+写信号 
}


/**
  * @brief  SHT30写命令
  * @param  msb
            lsb
  * @retval None
  */
void Sht30_WriteCmd(uint8_t msb,uint8_t lsb)
	{
		i2c_Start();//起始信号
        i2c_SendByte(0x88+0); //发送设备地址+写信号
		while (i2c_WaitAck());//CPU产生一个时钟，并读取器件的ACK应答信号
		i2c_SendByte(msb); //发送命令高位
		while (i2c_WaitAck());//CPU产生一个时钟，并读取器件的ACK应答信号
		i2c_SendByte(lsb); //发送命令低位
		while (i2c_WaitAck());//CPU产生一个时钟，并读取器件的ACK应答信号
		i2c_Stop();  // CPU发起I2C总线停止信号

	 }
	
	 
 
/**
  * @brief  SHT30读数据
  * @param  None
  * @retval None
  */
void Sht30_ReadData(void)
	{   	
		Sht30_WriteCmd(0x21,0x26);	//数据采集频率1 m/s
		i2c_Start();//起始信号
		i2c_SendByte(0x88+1); //发送设备地址+读信号
		i2c_Ack();//CPU产生一个时钟，并读取器件的ACK应答信号
		Buff[0]=i2c_ReadByte();//读取温度高八位并且发送一个ack
		i2c_Ack();
		Buff[1]=i2c_ReadByte(); //低八位
		i2c_Ack();
		Buff[2]=i2c_ReadByte(); //校验位发送ACK8位
		i2c_Ack();
		Buff[3] = i2c_ReadByte();  //湿度发送ACK高8位
		i2c_Ack();
		Buff[4] = i2c_ReadByte();  //湿度低八位
        i2c_Ack();
        Buff[5] = i2c_ReadByte();  //校验位之后发送nack具体见数据手册
		i2c_NAck();
        i2c_Stop();  // CPU发起I2C总线停止信号	
	}	
	
/**
  * @brief  读取的数据进行公式转换
  * @param  None
  * @retval None
  */
void Convert_sht30(void)
{
    Temperature = (float)175*((Buff[0]<<8)+Buff[1])/65535-45;    //左移一位相当于乘2，左移8位*2^8。
    Humidity = (float)100*((Buff[3]<<8)+Buff[4])/65535;

    Buff[0] = 0;
    Buff[1] = 0;
    Buff[2] = 0;
    Buff[3] = 0;
    Buff[4] = 0;
    Buff[5] = 0;
}

