#ifndef __CAMERA_SCCB_H
#define __CAMERA_SCCB_H

#include "stm32h7xx_hal.h"

#define OV2640_DEVICE_ADDRESS     0x60    // OV2640��ַ
#define OV5640_DEVICE_ADDRESS     0X78		// OV5640��ַ

/*----------------------------------------- IIIC �������ú� -----------------------------------------------*/

#define SCCB_SCL_CLK_ENABLE     	__HAL_RCC_GPIOF_CLK_ENABLE()		// SCL ����ʱ��
#define SCCB_SCL_PORT   			 GPIOF                 				// SCL ���Ŷ˿�
#define SCCB_SCL_PIN     			 GPIO_PIN_14  							// SCL ����
         
#define SCCB_SDA_CLK_ENABLE     	 __HAL_RCC_GPIOF_CLK_ENABLE() 	// SDA ����ʱ��
#define SCCB_SDA_PORT   			 GPIOF                   			// SDA ���Ŷ˿�
#define SCCB_SDA_PIN    			 GPIO_PIN_15              			// SDA ����

/*------------------------------------------ IIC��ض��� -------------------------------------------------*/

#define ACK_OK  	1  			// ��Ӧ����
#define ACK_ERR 	0				// ��Ӧ����

// IICͨ����ʱ��Touch_IIC_Delay()����ʹ�ã�
#define SCCB_DelayVaule  15  	

/*-------------------------------------------- IO�ڲ��� ---------------------------------------------------*/   

#define SCCB_SCL(a)	if (a)	\
										HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET); \
									else		\
										HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_RESET)	

#define SCCB_SDA(a)	if (a)	\
										HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET); \
									else		\
										HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_RESET)		

/*--------------------------------------------- �������� --------------------------------------------------*/  		
					
void 		SCCB_GPIO_Config (void);				// IIC���ų�ʼ��
void 		SCCB_Delay(uint32_t a);					// IIC��ʱ����						
void 		SCCB_Start(void);							// ����IICͨ��
void 		SCCB_Stop(void);							// IICֹͣ�ź�
void 		SCCB_ACK(void);							//	������Ӧ�ź�
void 		SCCB_NoACK(void);							// ���ͷ�Ӧ���ź�
uint8_t 	SCCB_WaitACK(void);						//	�ȴ�Ӧ���ź�
uint8_t	SCCB_WriteByte(uint8_t IIC_Data); 	// д�ֽں���
uint8_t 	SCCB_ReadByte(uint8_t ACK_Mode);		// ���ֽں���
		
uint8_t  SCCB_WriteReg (uint8_t addr,uint8_t value);     	// ��ָ���ļĴ���(8λ��ַ)дһ�ֽ����ݣ�OV2640�õ�
uint8_t  SCCB_ReadReg (uint8_t addr);                    	// ��ָ���ļĴ���(8λ��ַ)��һ�ֽ����ݣ�OV2640�õ�
									
uint8_t 	SCCB_WriteReg_16Bit(uint16_t addr,uint8_t value);	// ��ָ���ļĴ���(16λ��ַ)дһ�ֽ����ݣ�OV5640�õ�									
uint8_t 	SCCB_ReadReg_16Bit (uint16_t addr);						// ��ָ���ļĴ���(16λ��ַ)��һ�ֽ����ݣ�OV5640�õ�
uint8_t 	SCCB_WriteBuffer_16Bit(uint16_t addr,uint8_t *pData, uint32_t size);	// ��ָ���ļĴ���(16λ��ַ)����д���ݣ�OV5640 д���Զ��Խ��̼�ʱ�õ�		
									
#endif //__CAMERA_SCCB_H
