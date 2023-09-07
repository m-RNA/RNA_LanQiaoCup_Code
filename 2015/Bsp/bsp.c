#include "bsp.h"
#include "bsp_iic.h"
#include "adc.h"

void LED_Disp(u8 ucLED)
{
  HAL_GPIO_WritePin(GPIOC, 0xFF00, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, ucLED<<8, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LED_LOCK_GPIO_Port, LED_LOCK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_LOCK_GPIO_Port, LED_LOCK_Pin, GPIO_PIN_RESET);
}


u8 KEY_Scan(void)
{
  if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
    return 1;
  if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
    return 2;
  if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
    return 3;  
  if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET)
    return 4;  
  
  return 0;  
}

u16 ADC_GetVal(void)
{
	HAL_ADC_Start(&hadc2); // ※ 这里忘了加上去
  HAL_ADC_PollForConversion(&hadc2, 50);
  return HAL_ADC_GetValue(&hadc2);
}

void Write_EEPROM(u8 * Data, u8 Size, u8 Addr)
{
  I2CStart();
  I2CSendByte(0xA0); // ※ 别忘了
  I2CWaitAck();
  
  I2CSendByte(Addr); // ※ 别忘了
  I2CWaitAck();  
  
  while(Size--)
  {
    I2CSendByte(*Data);
    I2CWaitAck();
  
    Data++;
  }
  I2CStop();
  
  HAL_Delay(4);  
}

void Read_EEPROM(u8 * Data, u8 Size, u8 Addr)
{
  I2CStart();
  I2CSendByte(0xA0);
  I2CWaitAck();

  I2CSendByte(Addr);
  I2CWaitAck();
  
  /* ※ 这里别搞错了 */  
  I2CStart();
  I2CSendByte(0xA1);
  I2CWaitAck();

  while(Size--)
  {
    *Data = I2CReceiveByte();
    
    /* ※ 这里别搞错了 */
    if(Size)
    {
      I2CSendAck();
      Data++;
    }
    else
    {
      I2CSendNotAck();
    }
  }
  I2CStop();
}







