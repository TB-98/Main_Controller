/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define share_seat_joy_in_Pin GPIO_PIN_3
#define share_seat_joy_in_GPIO_Port GPIOE
#define DHT11_data_Pin GPIO_PIN_0
#define DHT11_data_GPIO_Port GPIOF
#define LCD_BL_Pin GPIO_PIN_1
#define LCD_BL_GPIO_Port GPIOF
#define T_CS_Pin GPIO_PIN_2
#define T_CS_GPIO_Port GPIOF
#define share_Back_in_Pin GPIO_PIN_3
#define share_Back_in_GPIO_Port GPIOF
#define T_PEN_Pin GPIO_PIN_5
#define T_PEN_GPIO_Port GPIOF
#define T_PEN_EXTI_IRQn EXTI9_5_IRQn
#define share_speed_in_Pin GPIO_PIN_7
#define share_speed_in_GPIO_Port GPIOF
#define share_seat_out_Pin GPIO_PIN_8
#define share_seat_out_GPIO_Port GPIOF
#define Relay_Front_Light_Pin GPIO_PIN_9
#define Relay_Front_Light_GPIO_Port GPIOF
#define share_Right_in_Pin GPIO_PIN_0
#define share_Right_in_GPIO_Port GPIOC
#define share_Left_in_Pin GPIO_PIN_3
#define share_Left_in_GPIO_Port GPIOA
#define ultra3_Trig_Pin GPIO_PIN_4
#define ultra3_Trig_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define share_stand_output_Pin GPIO_PIN_12
#define share_stand_output_GPIO_Port GPIOF
#define share_speed_in_1_Pin GPIO_PIN_13
#define share_speed_in_1_GPIO_Port GPIOF
#define Buzzer_Pin GPIO_PIN_0
#define Buzzer_GPIO_Port GPIOG
#define WP_Pin GPIO_PIN_1
#define WP_GPIO_Port GPIOG
#define ultra1_ECHO_Pin GPIO_PIN_12
#define ultra1_ECHO_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define ultra1_Trig_Pin GPIO_PIN_15
#define ultra1_Trig_GPIO_Port GPIOB
#define share_default_state_Pin GPIO_PIN_2
#define share_default_state_GPIO_Port GPIOG
#define share_default_set_Pin GPIO_PIN_3
#define share_default_set_GPIO_Port GPIOG
#define Alarm_Pin GPIO_PIN_6
#define Alarm_GPIO_Port GPIOC
#define ultra2_ECHO_Pin GPIO_PIN_7
#define ultra2_ECHO_GPIO_Port GPIOC
#define Relay_Back_Light_Pin GPIO_PIN_8
#define Relay_Back_Light_GPIO_Port GPIOC
#define Relay_Left_Light_Pin GPIO_PIN_9
#define Relay_Left_Light_GPIO_Port GPIOC
#define ultra2_Trig_Pin GPIO_PIN_15
#define ultra2_Trig_GPIO_Port GPIOA
#define Relay_Right_Light_Pin GPIO_PIN_10
#define Relay_Right_Light_GPIO_Port GPIOC
#define share_standing_complete_Pin GPIO_PIN_11
#define share_standing_complete_GPIO_Port GPIOC
#define state_ThisBoard_Pin GPIO_PIN_6
#define state_ThisBoard_GPIO_Port GPIOD
#define SW_standing_method_Pin GPIO_PIN_14
#define SW_standing_method_GPIO_Port GPIOG
#define rc522_sel_Pin GPIO_PIN_3
#define rc522_sel_GPIO_Port GPIOB
#define ultra3_ECHO_Pin GPIO_PIN_4
#define ultra3_ECHO_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define share_stop_in_Pin GPIO_PIN_8
#define share_stop_in_GPIO_Port GPIOB
#define PIR_sensor_Pin GPIO_PIN_0
#define PIR_sensor_GPIO_Port GPIOE
#define PIR_sensor_EXTI_IRQn EXTI0_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
