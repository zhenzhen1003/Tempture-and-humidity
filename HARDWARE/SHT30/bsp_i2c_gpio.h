#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H


#include <inttypes.h>


#define SHT30_I2C_WR	0		/* 写控制bit */
#define SHT30_I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define SHT30_GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define SHT30_I2C_SCL_PIN		GPIO_Pin_10			/* 连接到SCL时钟线的GPIO */
#define SHT30_I2C_SDA_PIN		GPIO_Pin_11			/* 连接到SDA数据线的GPIO */
#define SHT30_RCC						RCC_APB2Periph_GPIOB	/* 时钟 */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define SHT30_I2C_SCL_1()  GPIO_SetBits(SHT30_GPIO_PORT_I2C, SHT30_I2C_SCL_PIN)		/* SCL = 1 */
	#define SHT30_I2C_SCL_0()  GPIO_ResetBits(SHT30_GPIO_PORT_I2C, SHT30_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define SHT30_I2C_SDA_1()  GPIO_SetBits(SHT30_GPIO_PORT_I2C, SHT30_I2C_SDA_PIN)		/* SDA = 1 */
	#define SHT30_I2C_SDA_0()  GPIO_ResetBits(SHT30_GPIO_PORT_I2C, SHT30_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define SHT30_I2C_SDA_READ()  GPIO_ReadInputDataBit(SHT30_GPIO_PORT_I2C, SHT30_I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define SHT30_I2C_SCL_1()  SHT30_GPIO_PORT_I2C->BSRR = SHT30_I2C_SCL_PIN				/* SCL = 1 */
	#define SHT30_I2C_SCL_0()  SHT30_GPIO_PORT_I2C->BRR = SHT30_I2C_SCL_PIN				/* SCL = 0 */
	
	#define SHT30_I2C_SDA_1()  SHT30_GPIO_PORT_I2C->BSRR = SHT30_I2C_SDA_PIN				/* SDA = 1 */
	#define SHT30_I2C_SDA_0()  SHT30_GPIO_PORT_I2C->BRR = SHT30_I2C_SDA_PIN				/* SDA = 0 */
	
	#define SHT30_I2C_SDA_READ()  ((SHT30_GPIO_PORT_I2C->IDR & SHT30_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif


void i2c_Start(void); //CPU发起I2C总线启动信号
void i2c_Stop(void);  // CPU发起I2C总线停止信号
void i2c_SendByte(uint8_t _ucByte);//CPU向I2C总线设备发送8bit数据
uint8_t i2c_ReadByte(void);// CPU从I2C总线设备读取8bit数据
uint8_t i2c_WaitAck(void);//CPU产生一个时钟，并读取器件的ACK应答信号
void i2c_Ack(void);//CPU产生一个ACK信号
void i2c_NAck(void);//CPU产生一个NACK信号
uint8_t i2c_CheckDevice(uint8_t _Address);//检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在


#endif

