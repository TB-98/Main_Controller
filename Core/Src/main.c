/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "ili9341.h"
#include "fonts.h"
#include "XPT2046_touch.h"
#include "app.h"
#include "at24cxx.h"
#include "MFRC522.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//debug mode on ==1
#define debug_mode 0

#define testForLoop 0
#define modefortest 1

typedef struct{
	uint16_t flag_DHT11;
	uint16_t flag_MPU6050;
	uint16_t flag_CDS;
	uint16_t flag_ULTRA;
	uint16_t flag_BATTERY;
	uint16_t flag_FSR;
	uint16_t flag_GPIO;
}mainLoopFlag;


typedef enum{
	F_DHT11=1,
	F_MPU6050,
	F_CDS,
	F_ULTRA,
	F_BATTERY,
	F_FSR,
	F_GPIO
}activaitive_fun;

#define shutdown	HAL_NVIC_SystemReset()

#define test_motor 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart5;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

//maximum value for init
mainLoopFlag F_mainLoop={100,100,100,100,100,100,100};

activaitive_fun F_active=0;

uint16_t x=0,y=0;
//PIr
uint8_t PIR_flag=0;



uint8_t standing_loop_flag=0;


//DHT11
int Temperature = 0;
int Humidity = 0;
uint8_t DHT_11_print[20];

//////////////////////////////
//MPU6050
volatile uint16_t MPU_milliseconds=0;
uint8_t Angle_on ,flag_for_MPU_RESET=1;
//flag_for_MPU_RESET==0 -> reset
///////////////////////////

//AT24C256
uint16_t MEM_ADDR[4];

//FIRST_SET_UP 0==First Set 1==NOT   _ CARD SET
uint8_t FIRST_SET_UP=1;
uint8_t User_UID_1[20];
uint8_t User_UID_2[20];
uint8_t input_cardID[20];
uint8_t size_card_input=0,size_card1=0,size_card2=0;


//AT24C256
uint16_t MEM_ADDR[4];


//main_loop_counter_flag (1sec)
uint8_t main_loop_counter_flag=0;
uint8_t main_start_flag=1;
//activative time for rfid
uint8_t activative_time_PIR=20;


//light;
uint8_t light_on=0;

//speed
uint8_t share_speed_val=0;

uint8_t flag_loop_seat=0;
uint8_t rfid_flag_wrong=0;



//ultra
float ultra_distance1;
float ultra_distance2;
float ultra_distance3;

uint8_t beepforultra=0;
uint8_t beepforultra_1=0;
uint8_t beepforultra_2=0;


//share_standing_option_on==1 standing on
uint8_t share_standing_option_on=0;


//limit
uint8_t angle_Y_limt=15;
uint8_t flag_Waring_angle=0;


//output assist value
uint16_t adc_FSR=0;
uint16_t adc_CDS=0;
uint16_t adc_battery=0;
float battery_voltage=0;
float battery_percentage=0;

float use_battery_voltage=25.2;
char adc_str[20];

////adc control
//uint8_t main_loop_adc_Timing=0;
//uint16_t main_loop_adc_Count=main_loop_adc_Timing;
////start with conversion
//main_loop_adc_Count=3*4-1;


//new value for main timing_max =610 == 61.0sec
uint16_t loopForMain=1;


uint8_t FSR_Sensity=0;


//cds_ auto light
uint8_t flag_autoLight=0;

uint8_t flag_seat=0;
uint8_t flag_seat_off=0;
//1 value == 0.5sec 20->10sec after reset
uint8_t seat_off_value=15;

uint8_t flag_motor_seat_joy=0;

uint8_t flag_motor_Left=0;

uint8_t flag_motor_Right=0;

uint8_t flag_motor_B_F=0;

uint8_t flag_motor_stop=0;


uint8_t standing_selection_delay_flag=0;
uint8_t SET_standing_selection=3;

uint8_t de_sh_f;
uint16_t de_sh_f_1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM14_Init(void);
static void MX_FMC_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM11_Init(void);
static void MX_UART5_Init(void);
static void MX_TIM9_Init(void);
/* USER CODE BEGIN PFP */

//DHT11
int dht11_read (void);
void dht11_print(void);
int wait_pulse(int state);
void delay_us(uint16_t time);


//AT24C256
void Memory_write(uint16_t wr_MEM_ADDR,uint8_t wData[]);
void Memory_Reset(void);
void Memory_read(uint16_t re_MEM_ADDR);

void rfid_loop_setup_card(void);
void rfid_loop_init_set(void);
void check_rfid_UID(void);

//BEEP
void BEEP(uint8_t rep, uint8_t rep_timing);
void Ultra_Distance_Data();

//mode 1-> for timer int   mode 2-> for main
void main_loop_control(uint8_t mode);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)								/* for printf */
#endif /* __GNUC__ */
// Standard Output String


PUTCHAR_PROTOTYPE			// For printf Function
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart5, (uint8_t *)&ch, 1, 0xFFFF);   // USB
	return ch;
}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
	MX_TIM6_Init();
	MX_TIM2_Init();
	MX_I2C2_Init();
	MX_I2C1_Init();
	MX_ADC1_Init();
	MX_SPI2_Init();
	MX_TIM14_Init();
	MX_FMC_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
	MX_TIM3_Init();
	MX_TIM7_Init();
	MX_TIM10_Init();
	MX_TIM11_Init();
	MX_UART5_Init();
	MX_TIM9_Init();
	/* USER CODE BEGIN 2 */


#if !testForLoop

	/*	LCD(BANK 1 NE1 A18 16Bit) _ while use lcd, usart3 exhibit
	 * */
	HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin,SET);
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();

	Init_RFID();
	lcdInit();

	HAL_Delay(10);
	// recommand == 1 (for touch)
	lcdSetOrientation(1);
	lcdFillRGB(COLOR_WHITE);

#if debug_mode ==1
	lcdSetTextFont(&Font20);
	lcdSetTextColor(COLOR_BLACK, COLOR_WHITE);
	lcdSetCursor(0, 0);
	lcdPrintf((char*)"debugging mode_start");
#else
	lcdSetTextFont(&Font20);
	lcdSetCursor(5, 10);
	lcdSetTextColor(COLOR_BLACK, COLOR_WHITE);
	lcdPrintf("S.A WheelChair");

	lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_ORANGE);
	lcdSetTextFont(&Font24);
	lcdSetTextColor(COLOR_BLACK, COLOR_ORANGE);
	lcdSetCursor((320-170)/2, 5*24);
	lcdPrintf("TOUCH CARD");



#endif

#if debug_mode ==0
	while(main_start_flag){
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin,SET);
		HAL_SuspendTick();
		HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		HAL_ResumeTick();
		if(PIR_flag == 1){
			HAL_Delay(250);
			for(int i=0;i<2;i++){
				HAL_GPIO_WritePin(Relay_Back_Light_GPIO_Port, Relay_Back_Light_Pin, SET);
				HAL_GPIO_WritePin(Relay_Front_Light_GPIO_Port, Relay_Front_Light_Pin , SET);
				HAL_GPIO_WritePin(Relay_Left_Light_GPIO_Port, Relay_Left_Light_Pin, SET);
				HAL_GPIO_WritePin(Relay_Right_Light_GPIO_Port, Relay_Right_Light_Pin, SET);
				HAL_Delay(150);
				HAL_GPIO_WritePin(Relay_Back_Light_GPIO_Port, Relay_Back_Light_Pin, RESET);
				HAL_GPIO_WritePin(Relay_Front_Light_GPIO_Port, Relay_Front_Light_Pin , RESET);
				HAL_GPIO_WritePin(Relay_Left_Light_GPIO_Port, Relay_Left_Light_Pin, RESET);
				HAL_GPIO_WritePin(Relay_Right_Light_GPIO_Port, Relay_Right_Light_Pin, RESET);
				HAL_Delay(250);
			}
			HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin,RESET);
			main_loop_counter_flag=0;
			__HAL_TIM_SET_COUNTER(&htim14,0);
			HAL_TIM_Base_Start_IT(&htim14);
			// RFID Check
			rfid_loop_init_set();
			HAL_GPIO_WritePin(Alarm_GPIO_Port, Alarm_Pin, RESET);
			PIR_flag=0;
		}

		HAL_TIM_Base_Stop_IT(&htim14);
	}
	rfid_flag_wrong=0;
	BEEP(3,7);
	HAL_Delay(3000);
	rfid_loop_setup_card();
	BEEP(3,20);
	HAL_GPIO_WritePin(state_ThisBoard_GPIO_Port, state_ThisBoard_Pin, SET);
#endif

#if !test_motor
	HAL_GPIO_WritePin(share_default_set_GPIO_Port, share_default_set_Pin, SET);

	de_sh_f=1;
	de_sh_f_1=0;
	lcdFillRGB(COLOR_WHITE);
	lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_LIGHTGREY);
	lcdSetTextFont(&Font24);
	lcdSetTextColor(COLOR_BLACK, COLOR_LIGHTGREY);
	lcdSetCursor((320-170)/2, 5*24);
	lcdPrintf("Init System");

	lcdFillRGB(COLOR_WHITE);
	lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_ORANGE);
	lcdSetTextFont(&Font24);
	lcdSetTextColor(COLOR_BLACK, COLOR_ORANGE);
	lcdSetCursor((320-170)/2, 5*24);
	lcdPrintf("Put Walker");

	de_sh_f=1;

	uint8_t modesets_flag=0;
	while(de_sh_f){
#if !modefortest
		if(HAL_GPIO_ReadPin(share_default_state_GPIO_Port, share_default_state_Pin)==GPIO_PIN_SET){
			HAL_GPIO_WritePin(share_default_set_GPIO_Port, share_default_set_Pin, RESET);
			de_sh_f=0;
			lcdFillRGB(COLOR_WHITE);
			lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_LIGHTGREY);
			lcdSetTextFont(&Font24);
			lcdSetTextColor(COLOR_BLACK, COLOR_LIGHTGREY);
			lcdSetCursor((320-170)/2, 5*24);
			lcdPrintf("Working...");

		}
		else{
			//50sec wait(setup + walker ready)
			de_sh_f_1++;
			HAL_Delay(100);
		}
		if(de_sh_f_1>1000){
			shutdown;
		}
#endif
#if modefortest
		HAL_Delay(1000);
		modesets_flag+=1;

		if(modesets_flag>5){
			de_sh_f=0;
		}
#endif
	}
#if modefortest
	shutdown;
#endif


#endif
	HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin,RESET);

	//initialize lcd
#if debug_mode ==1
	lcdFillRGB(COLOR_WHITE);
	lcdSetCursor(0, 0);
	lcdPrintf((char*)"debugging mode_Home");
#endif

	lcdSetTextFont(&Font20);
	lcdSetCursor(5, 10);
	lcdSetTextColor(COLOR_BLACK, COLOR_WHITE);
	lcdPrintf("S.A WheelChair");


#if debug_mode ==0
	lcdFillRect(0, 30, 320, 240, COLOR_WHITE);
	lcdFillRoundRect(30, 40, 14*18-14, 30, 5, COLOR_GRAY_225);
	lcdSetTextFont(&Font20);
	lcdSetTextColor(COLOR_BLACK,COLOR_GRAY_225);
	lcdSetCursor(20, 45);
	lcdPrintf(" Temp:    Hum:    ");

	lcdFillRoundRect(30, 80, 14*8 , 30, 5, COLOR_THEME_SKYBLUE_BASE);
	lcdSetTextColor(COLOR_BLACK,COLOR_THEME_SKYBLUE_BASE);
	lcdSetCursor(30, 85);
	lcdPrintf("Speed:  ");

	lcdFillRoundRect(152, 80, 14*8 , 30, 5, COLOR_THEME_SKYBLUE_LIGHTER);
	lcdSetTextColor(COLOR_BLACK,COLOR_THEME_SKYBLUE_LIGHTER);
	lcdSetCursor(152, 85);
	lcdPrintf("Seat:   ");

	lcdFillRoundRect(30, 120, 240 , 30, 5, COLOR_THEME_LIMEGREEN_BASE);
	lcdSetTextColor(COLOR_BLACK,COLOR_THEME_LIMEGREEN_BASE);
	lcdSetCursor(30, 125);
	lcdPrintf("Power:   ");


#endif


	//	lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_ORANGE);
	//	lcdSetTextFont(&Font24);
	//	lcdSetTextColor(COLOR_BLACK, COLOR_ORANGE);
	//	lcdSetCursor((320-170)/2, 5*24);
	//	lcdPrintf("TOUCH CARD");



	//MPU6050
	HAL_TIM_Base_Start_IT(&htim2);
	MPU_milliseconds=0;
	flag_for_MPU_RESET=0;
	Angle_on=1;

#endif

	//Light Int 0.5sec
	HAL_TIM_Base_Start_IT(&htim7);

	//Beep Timing Tim _ 1sec default
	HAL_TIM_Base_Start_IT(&htim10);

	//main_loop_timing_0.1sec
	HAL_TIM_Base_Start_IT(&htim11);

	//mpu6050 int1
	HAL_TIM_Base_Start_IT(&htim9);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		F_active=0;
		main_loop_control(2);

#if testForLoop
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		HAL_Delay(250);

		flag_motor_B_F=1;
		printf("active %d \r\n",F_active);

		printf("1.DHT %d \r\n",F_mainLoop.flag_DHT11);
		printf("2.MPU %d \r\n",F_mainLoop.flag_MPU6050);
		printf("3.CDS %d \r\n",F_mainLoop.flag_CDS);
		printf("4.Ultra %d \r\n",F_mainLoop.flag_ULTRA);
		printf("5.BAT %d \r\n",F_mainLoop.flag_BATTERY);
		printf("6.FSR %d \r\n",F_mainLoop.flag_FSR);
		printf("7.GPIO %d \r\n",F_mainLoop.flag_GPIO);
#endif

#if !testForLoop
		switch(F_active){

		case F_GPIO:
			//GPIO
			/////////////////////////////////
			if(HAL_GPIO_ReadPin(share_Left_in_GPIO_Port, share_Left_in_Pin)==GPIO_PIN_SET)
			{
				flag_motor_Left=1;

			}
			else{
				flag_motor_Left=0;
			}

			if(HAL_GPIO_ReadPin(share_Right_in_GPIO_Port, share_Right_in_Pin)==GPIO_PIN_SET){
				flag_motor_Right=1;
			}
			else{
				flag_motor_Right=0;
			}
			if(HAL_GPIO_ReadPin(share_Back_in_GPIO_Port, share_Back_in_Pin)==GPIO_PIN_SET){
				flag_motor_B_F=1;
			}
			else{
				flag_motor_B_F=0;
			}

			if(HAL_GPIO_ReadPin(share_stop_in_GPIO_Port, share_stop_in_Pin)==GPIO_PIN_SET){
				flag_motor_stop=1;
			}
			else{
				flag_motor_stop=0;
			}

			//2.4sec  -> 0.8sec *3
			if(HAL_GPIO_ReadPin(SW_standing_method_GPIO_Port, SW_standing_method_Pin)==GPIO_PIN_SET)
			{
				share_standing_option_on=1;

			}
			if(standing_loop_flag){
				share_standing_option_on=0;
				standing_loop_flag=0;
				HAL_GPIO_WritePin(share_stand_output_GPIO_Port, share_stand_output_Pin, RESET);
			}

			if(HAL_GPIO_ReadPin(share_standing_complete_GPIO_Port, share_standing_complete_Pin) == GPIO_PIN_SET ){
				shutdown;

			}
			if(share_standing_option_on==1){
				if(flag_motor_stop==1){
					HAL_GPIO_WritePin(share_stand_output_GPIO_Port, share_stand_output_Pin, SET);
					HAL_TIM_Base_Stop_IT(&htim11);
					standing_loop_flag=1;
					HAL_Delay(3000);
					HAL_TIM_Base_Start_IT(&htim11);


				}
			}


			if(HAL_GPIO_ReadPin(share_seat_joy_in_GPIO_Port, share_seat_joy_in_Pin)==GPIO_PIN_SET)
			{
				flag_motor_seat_joy=1;
			}
			else{
				flag_motor_seat_joy=0;
			}

			share_speed_val=0;

			if(HAL_GPIO_ReadPin(share_speed_in_GPIO_Port, share_speed_in_Pin) == GPIO_PIN_SET){
				share_speed_val+=1;
			}
			if(HAL_GPIO_ReadPin(share_speed_in_1_GPIO_Port, share_speed_in_1_Pin) == GPIO_PIN_SET){
				share_speed_val+=2;
			}


#if debug_mode ==1
			lcdSetCursor(0, 40);
			lcdPrintf("%d",share_speed_val);
#endif
#if debug_mode == 0
			lcdSetTextColor(COLOR_BLACK,COLOR_THEME_SKYBLUE_BASE);
			lcdSetCursor(30+14*6, 85);
			lcdPrintf("%d",share_speed_val);
#endif
			break;

		case F_FSR:
			/////////////////////////////////////////////////////
			//FSR
			HAL_ADC_Start(&hadc1);
			if(HAL_ADC_PollForConversion(&hadc1, 100)==HAL_OK)
			{
				adc_FSR=HAL_ADC_GetValue(&hadc1);
				if(adc_FSR>FSR_Sensity){
					if(flag_seat<9)
						flag_seat+=3;

					if(flag_seat>=9)
						flag_seat=9;

					if(flag_motor_stop && flag_seat_off>0){

						flag_seat_off-=1;
					}
					else if(flag_motor_stop == 0){
						flag_seat_off=0;
					}

				}


				else{
					if(flag_motor_stop && flag_seat_off<seat_off_value){
						flag_seat_off+=1;
					}
					else if(flag_motor_stop == 0){
						flag_seat_off=0;
					}
					if(flag_seat>=1){
						flag_seat-=1;
					}
					if(flag_seat<0){
						flag_seat=0;
					}
				}

				if(flag_seat_off>=seat_off_value){
					shutdown;
				}
#if debug_mode==1
				sprintf(adc_str,"adc_FSR: %05d  ",adc_FSR);
				lcdSetCursor(0, 20);
				lcdPrintf(adc_str);
#endif
			}
			if(flag_seat>=9){
				HAL_GPIO_WritePin(share_seat_out_GPIO_Port, share_seat_out_Pin, SET);
#if debug_mode == 0
				if(flag_motor_seat_joy && flag_loop_seat!=1){
					lcdFillRoundRect(152, 80, 14*8 , 30, 5, COLOR_THEME_SKYBLUE_LIGHTER);
					lcdSetTextColor(COLOR_BLACK,COLOR_THEME_SKYBLUE_LIGHTER);
					lcdSetCursor(152, 85);
					lcdPrintf("Seat:   ");
					lcdSetCursor(222, 85);
					lcdPrintf("on ");
					flag_loop_seat=1;
				}
				else if(flag_motor_seat_joy==0 && flag_loop_seat!=2){
					BEEP(2,10);
					lcdFillRoundRect(152, 80, 14*8 , 30, 5, COLOR_THEME_VIOLET_LIGHTER);
					lcdSetTextColor(COLOR_BLACK,COLOR_THEME_VIOLET_LIGHTER);
					lcdSetCursor(152, 85);
					lcdPrintf("Seat:   ");
					lcdSetCursor(222, 85);
					lcdPrintf("on ");
					flag_loop_seat=2;
				}

#endif
			}
			else if(flag_seat==0){
				HAL_GPIO_WritePin(share_seat_out_GPIO_Port, share_seat_out_Pin, RESET);
#if debug_mode == 0
				if(flag_motor_seat_joy && flag_loop_seat!=3){
					lcdFillRoundRect(152, 80, 14*8 , 30, 5, COLOR_THEME_SKYBLUE_LIGHTER);
					lcdSetTextColor(COLOR_BLACK,COLOR_THEME_SKYBLUE_LIGHTER);
					lcdSetCursor(152, 85);
					lcdPrintf("Seat:   ");
					lcdSetCursor(222, 85);
					lcdPrintf("off");
					flag_loop_seat=3;
				}
				else if(flag_motor_seat_joy==0 && flag_loop_seat!=4){
					BEEP(2,10);
					lcdFillRoundRect(152, 80, 14*8 , 30, 5, COLOR_THEME_VIOLET_LIGHTER);
					lcdSetTextColor(COLOR_BLACK,COLOR_THEME_VIOLET_LIGHTER);
					lcdSetCursor(152, 85);
					lcdPrintf("Seat:   ");
					lcdSetCursor(222, 85);
					lcdPrintf("off");
					flag_loop_seat=4;
				}

#endif
			}

			break;

		case F_BATTERY:

			//battery
			/////////////////////////////////////////////
			HAL_ADC_Start(&hadc3);
			if(HAL_ADC_PollForConversion(&hadc3, 100)==HAL_OK){
				adc_battery=HAL_ADC_GetValue(&hadc3);
				battery_voltage=(float)adc_battery/4096.0*3.23*10.0;
				battery_percentage=(battery_voltage-16.5)/(use_battery_voltage-16.5)*100.0;
			}


#if debug_mode == 0
			lcdSetTextColor(COLOR_BLACK,COLOR_THEME_LIMEGREEN_BASE);
			lcdSetCursor(30+14*6, 125);
			if(battery_voltage<30 && battery_voltage>=16.5){
				lcdPrintf("%03.0f%%(%02.0fV)",battery_percentage,battery_voltage);
			}
#endif
			break;

		case F_ULTRA:
			//ultra_
			//////////////////////////////////////////
			if(flag_motor_B_F==1){
				Ultra_Distance_Data();
				beepforultra_1=0;
				if(ultra_distance1>=19){
					if(ultra_distance1<45 )	beepforultra_1=5;
					else if(ultra_distance1<55 )	beepforultra_1=4;
					else if(ultra_distance1<85 )	beepforultra_1=3;
					else if(ultra_distance1<125 )	beepforultra_1=2;
					else if(ultra_distance1<200 )	beepforultra_1=1;
					else	beepforultra_1=0;
				}
				beepforultra_2=0;
				if(ultra_distance2>=19){
					if(ultra_distance2<45 )	beepforultra_2=5;
					else if(ultra_distance2<55 )	beepforultra_2=4;
					else if(ultra_distance2<85 )	beepforultra_2=3;
					else if(ultra_distance2<125)	beepforultra_2=2;
					else if(ultra_distance2<200)	beepforultra_2=1;
					else	beepforultra_2=0;
				}
			}

			if(beepforultra_1<beepforultra_2){
				beepforultra=beepforultra_1;
			}
			else{
				beepforultra=beepforultra_2;
			}

			break;



		case F_CDS:
			////////////////////////////////////////////
			//CDS
			HAL_ADC_Start(&hadc2);
			if(HAL_ADC_PollForConversion(&hadc2, 100)==HAL_OK)
			{
				adc_CDS=HAL_ADC_GetValue(&hadc2);
				if(adc_CDS<1200){
					flag_autoLight+=3;
					if(flag_autoLight>9)
						flag_autoLight=9;
				}
				else{
					if(flag_autoLight>0)
						flag_autoLight-=1;
				}

#if debug_mode==1
				sprintf(adc_str,"adc_CDS: %05d  ",adc_CDS);
				lcdSetCursor(0, 120);
				lcdPrintf(adc_str);
#endif
			}
			if(flag_autoLight>=9){
				HAL_GPIO_WritePin(Relay_Front_Light_GPIO_Port, Relay_Front_Light_Pin, SET);
				HAL_GPIO_WritePin(Relay_Back_Light_GPIO_Port, Relay_Back_Light_Pin, SET);
			}
			else if(flag_autoLight==0){
				HAL_GPIO_WritePin(Relay_Front_Light_GPIO_Port, Relay_Front_Light_Pin, RESET);
				HAL_GPIO_WritePin(Relay_Back_Light_GPIO_Port, Relay_Back_Light_Pin, RESET);
			}
			break;

		case F_MPU6050:
			///////////////////////////////////////////
			//MPU6050
			if(angleY >0){
				if(angleY > angle_Y_limt){
					if(flag_Waring_angle<5){
						flag_Waring_angle+=1;
					}
					else{
						if(flag_Waring_angle>0)
							flag_Waring_angle-=1;
					}
					if(flag_Waring_angle == 5)
						BEEP(1,7);
				}
			}

			else{
				if(-angleY > angle_Y_limt){
					if(flag_Waring_angle < 5){
						flag_Waring_angle+=1;
					}
					else{
						if(flag_Waring_angle > 0)
							flag_Waring_angle-=1;
					}
					if(flag_Waring_angle == 5)
						BEEP(1,7);
				}
			}
			break;

		case F_DHT11:
			///////////////////////////////////////////////////////
			//DHT 11

#if debug_mode==1
			//DHT11
			dht11_print();
			//MPU6050
			MPU6050_printAngle(MPU_milliseconds);
#else
			//DHT11
			if(dht11_read()==1){
				sprintf(DHT_11_print,"%02dC",Temperature);
				lcdSetTextColor(COLOR_BLACK,COLOR_GRAY_225);
				lcdSetCursor(104, 45);
				lcdPrintf(DHT_11_print);

				sprintf(DHT_11_print,"%02d %%",Humidity);
				lcdSetTextColor(COLOR_BLACK,COLOR_GRAY_225);
				lcdSetCursor(216, 45);
				lcdPrintf(DHT_11_print);
			}
#endif
			break;
		}

		//end
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif





		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief ADC2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC2_Init(void)
{

	/* USER CODE BEGIN ADC2_Init 0 */

	/* USER CODE END ADC2_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC2_Init 1 */

	/* USER CODE END ADC2_Init 1 */
	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc2.Init.Resolution = ADC_RESOLUTION_12B;
	hadc2.Init.ScanConvMode = DISABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion = 1;
	hadc2.Init.DMAContinuousRequests = DISABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc2) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC2_Init 2 */

	/* USER CODE END ADC2_Init 2 */

}

/**
 * @brief ADC3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC3_Init(void)
{

	/* USER CODE BEGIN ADC3_Init 0 */

	/* USER CODE END ADC3_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC3_Init 1 */

	/* USER CODE END ADC3_Init 1 */
	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc3.Init.Resolution = ADC_RESOLUTION_12B;
	hadc3.Init.ScanConvMode = DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DMAContinuousRequests = DISABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc3) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC3_Init 2 */

	/* USER CODE END ADC3_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 400000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void)
{

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 8400-1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 10-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 84-1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void)
{

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 84-1;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 65535-1;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM6_Init 2 */

	/* USER CODE END TIM6_Init 2 */

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void)
{

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 8400-1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 5000-1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

}

/**
 * @brief TIM9 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM9_Init(void)
{

	/* USER CODE BEGIN TIM9_Init 0 */

	/* USER CODE END TIM9_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};

	/* USER CODE BEGIN TIM9_Init 1 */

	/* USER CODE END TIM9_Init 1 */
	htim9.Instance = TIM9;
	htim9.Init.Prescaler = 16800-1;
	htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim9.Init.Period = 10000-1;
	htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM9_Init 2 */

	/* USER CODE END TIM9_Init 2 */

}

/**
 * @brief TIM10 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM10_Init(void)
{

	/* USER CODE BEGIN TIM10_Init 0 */

	/* USER CODE END TIM10_Init 0 */

	/* USER CODE BEGIN TIM10_Init 1 */

	/* USER CODE END TIM10_Init 1 */
	htim10.Instance = TIM10;
	htim10.Init.Prescaler = 16800-1;
	htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim10.Init.Period = 10000-1;
	htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM10_Init 2 */

	/* USER CODE END TIM10_Init 2 */

}

/**
 * @brief TIM11 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM11_Init(void)
{

	/* USER CODE BEGIN TIM11_Init 0 */

	/* USER CODE END TIM11_Init 0 */

	/* USER CODE BEGIN TIM11_Init 1 */

	/* USER CODE END TIM11_Init 1 */
	htim11.Instance = TIM11;
	htim11.Init.Prescaler = 16800-1;
	htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim11.Init.Period = 1000-1;
	htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM11_Init 2 */

	/* USER CODE END TIM11_Init 2 */

}

/**
 * @brief TIM14 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM14_Init(void)
{

	/* USER CODE BEGIN TIM14_Init 0 */

	/* USER CODE END TIM14_Init 0 */

	/* USER CODE BEGIN TIM14_Init 1 */

	/* USER CODE END TIM14_Init 1 */
	htim14.Instance = TIM14;
	htim14.Init.Prescaler = 8400-1;
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim14.Init.Period = 10000-1;
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM14_Init 2 */

	/* USER CODE END TIM14_Init 2 */

}

/**
 * @brief UART5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART5_Init(void)
{

	/* USER CODE BEGIN UART5_Init 0 */

	/* USER CODE END UART5_Init 0 */

	/* USER CODE BEGIN UART5_Init 1 */

	/* USER CODE END UART5_Init 1 */
	huart5.Instance = UART5;
	huart5.Init.BaudRate = 115200;
	huart5.Init.WordLength = UART_WORDLENGTH_8B;
	huart5.Init.StopBits = UART_STOPBITS_1;
	huart5.Init.Parity = UART_PARITY_NONE;
	huart5.Init.Mode = UART_MODE_TX_RX;
	huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart5.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart5) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN UART5_Init 2 */

	/* USER CODE END UART5_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

	/* USER CODE BEGIN FMC_Init 0 */

	/* USER CODE END FMC_Init 0 */

	FMC_NORSRAM_TimingTypeDef Timing = {0};

	/* USER CODE BEGIN FMC_Init 1 */

	/* USER CODE END FMC_Init 1 */

	/** Perform the SRAM1 memory initialization sequence
	 */
	hsram1.Instance = FMC_NORSRAM_DEVICE;
	hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram1.Init */
	hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
	hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
	hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
	hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
	hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram1.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
	hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
	hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
	hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
	hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
	hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
	hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
	hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 15;
	Timing.AddressHoldTime = 15;
	Timing.DataSetupTime = 255;
	Timing.BusTurnAroundDuration = 15;
	Timing.CLKDivision = 16;
	Timing.DataLatency = 17;
	Timing.AccessMode = FMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
	{
		Error_Handler( );
	}

	/* USER CODE BEGIN FMC_Init 2 */

	/* USER CODE END FMC_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOF, DHT11_data_Pin|share_seat_out_Pin|Relay_Front_Light_Pin|share_stand_output_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOF, LCD_BL_Pin|T_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, ultra3_Trig_Pin|ultra2_Trig_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|ultra1_Trig_Pin|rc522_sel_Pin
			|LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, Buzzer_Pin|WP_Pin|share_default_set_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, Alarm_Pin|Relay_Back_Light_Pin|Relay_Left_Light_Pin|Relay_Right_Light_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(state_ThisBoard_GPIO_Port, state_ThisBoard_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : share_seat_joy_in_Pin */
	GPIO_InitStruct.Pin = share_seat_joy_in_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(share_seat_joy_in_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : DHT11_data_Pin LCD_BL_Pin share_seat_out_Pin Relay_Front_Light_Pin
                           share_stand_output_Pin */
	GPIO_InitStruct.Pin = DHT11_data_Pin|LCD_BL_Pin|share_seat_out_Pin|Relay_Front_Light_Pin
			|share_stand_output_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pin : T_CS_Pin */
	GPIO_InitStruct.Pin = T_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(T_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : share_Back_in_Pin share_speed_in_Pin share_speed_in_1_Pin */
	GPIO_InitStruct.Pin = share_Back_in_Pin|share_speed_in_Pin|share_speed_in_1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pin : T_PEN_Pin */
	GPIO_InitStruct.Pin = T_PEN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(T_PEN_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : share_Right_in_Pin ultra2_ECHO_Pin share_standing_complete_Pin */
	GPIO_InitStruct.Pin = share_Right_in_Pin|ultra2_ECHO_Pin|share_standing_complete_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : share_Left_in_Pin */
	GPIO_InitStruct.Pin = share_Left_in_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(share_Left_in_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ultra3_Trig_Pin ultra2_Trig_Pin */
	GPIO_InitStruct.Pin = ultra3_Trig_Pin|ultra2_Trig_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : LD1_Pin LD3_Pin ultra1_Trig_Pin rc522_sel_Pin
                           LD2_Pin */
	GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|ultra1_Trig_Pin|rc522_sel_Pin
			|LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : Buzzer_Pin WP_Pin share_default_set_Pin */
	GPIO_InitStruct.Pin = Buzzer_Pin|WP_Pin|share_default_set_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pins : ultra1_ECHO_Pin ultra3_ECHO_Pin share_stop_in_Pin */
	GPIO_InitStruct.Pin = ultra1_ECHO_Pin|ultra3_ECHO_Pin|share_stop_in_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : share_default_state_Pin */
	GPIO_InitStruct.Pin = share_default_state_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(share_default_state_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : Alarm_Pin Relay_Back_Light_Pin Relay_Left_Light_Pin Relay_Right_Light_Pin */
	GPIO_InitStruct.Pin = Alarm_Pin|Relay_Back_Light_Pin|Relay_Left_Light_Pin|Relay_Right_Light_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : state_ThisBoard_Pin */
	GPIO_InitStruct.Pin = state_ThisBoard_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(state_ThisBoard_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SW_standing_method_Pin */
	GPIO_InitStruct.Pin = SW_standing_method_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(SW_standing_method_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PIR_sensor_Pin */
	GPIO_InitStruct.Pin = PIR_sensor_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PIR_sensor_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
volatile uint32_t touch_delay_count=0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == T_PEN_Pin){
		if(XPT2046_TouchPressed())
		{
			if(XPT2046_TouchGetCoordinates(&x, &y))
			{

			}
		}
	}
	if(GPIO_Pin == PIR_sensor_Pin){
		PIR_flag=1;
	}

}


uint8_t preBeepForUltra=8;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance==TIM2)
	{
		if(Angle_on){
			if(MPU_milliseconds < 65531 && flag_for_MPU_RESET == 1){
				MPU_milliseconds+=1;
			}
			else{
				MPU_milliseconds=0;
				MPU6050_reset();
				flag_for_MPU_RESET=1;
			}
		}
	}
	if(htim->Instance==TIM14)
	{
		main_loop_counter_flag+=1;
	}

	//방향?��(Direction Light)_0.5sec int
	if(htim-> Instance ==TIM7){
		if(flag_motor_B_F==1){
			HAL_GPIO_TogglePin(Relay_Left_Light_GPIO_Port, Relay_Left_Light_Pin);
			HAL_GPIO_TogglePin(Relay_Right_Light_GPIO_Port, Relay_Right_Light_Pin);
		}

		else{
			if(flag_motor_Right==1){
				HAL_GPIO_TogglePin(Relay_Right_Light_GPIO_Port, Relay_Right_Light_Pin);
			}
			else if(flag_motor_Left==1){
				HAL_GPIO_TogglePin(Relay_Left_Light_GPIO_Port, Relay_Left_Light_Pin);
			}
			else{
				HAL_GPIO_WritePin(Relay_Right_Light_GPIO_Port, Relay_Right_Light_Pin,RESET);
				HAL_GPIO_WritePin(Relay_Left_Light_GPIO_Port, Relay_Left_Light_Pin,RESET);

			}
		}
	}
	if(htim->Instance == TIM9){
		MPU6050_load_Angle(MPU_milliseconds);
	}

	//default ARR = 10000 -> 1sec
	if(htim ->Instance == TIM10){

		switch(beepforultra){
		case 1:
			if(preBeepForUltra!=1)
				__HAL_TIM_SET_AUTORELOAD(&htim10,1000*10);
			HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
			preBeepForUltra=1;
			break;
		case 2:
			if(preBeepForUltra!=2)
				__HAL_TIM_SET_AUTORELOAD(&htim10,1000*5);
			HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
			preBeepForUltra=2;
			break;
		case 3:
			if(preBeepForUltra!=3)
				__HAL_TIM_SET_AUTORELOAD(&htim10,1000*3);
			HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
			preBeepForUltra=3;
			break;
		case 4:
			if(preBeepForUltra!=4)
				__HAL_TIM_SET_AUTORELOAD(&htim10,1000*2);
			HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
			preBeepForUltra=4;
			break;
		case 5:
			if(preBeepForUltra!=5)
				__HAL_TIM_SET_AUTORELOAD(&htim10,500);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin,SET);
			preBeepForUltra=5;
			break;
		default:
			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, RESET);
			if(preBeepForUltra!=8)
				__HAL_TIM_SET_AUTORELOAD(&htim10,1000*10);
			preBeepForUltra=8;
		}

	}
	if(htim -> Instance ==TIM11){
		loopForMain++;
		if(loopForMain==610){
			loopForMain=1;
		}
		main_loop_control(1);


	}
}

uint8_t act_m_ct=0;
void main_loop_control(uint8_t mode){
	switch(mode){
	case 1:
		if(loopForMain%5 == 0){
			F_mainLoop.flag_FSR++;
		}
		if(loopForMain%8 == 0){
			F_mainLoop.flag_MPU6050++;
		}
		if(loopForMain%8 == 0){
			F_mainLoop.flag_GPIO++;
		}
		if(loopForMain%8 == 0){
			F_mainLoop.flag_CDS++;
		}
		if(loopForMain%50 == 0){
			F_mainLoop.flag_BATTERY++;
		}
		if(loopForMain%30 == 0){
			F_mainLoop.flag_DHT11++;
		}
		if(flag_motor_B_F == 1){
			if(loopForMain % 2 == 0){
				F_mainLoop.flag_ULTRA+=4;
			}
		}
		else{
			F_mainLoop.flag_ULTRA=0;
			beepforultra=0;
		}
		break;

	case 2:
		act_m_ct=0;
		if(!act_m_ct){
			if(F_mainLoop.flag_BATTERY>=F_mainLoop.flag_CDS)
				if(F_mainLoop.flag_BATTERY>=F_mainLoop.flag_DHT11)
					if(F_mainLoop.flag_BATTERY>=F_mainLoop.flag_FSR)
						if(F_mainLoop.flag_BATTERY>=F_mainLoop.flag_GPIO)
							if(F_mainLoop.flag_BATTERY>=F_mainLoop.flag_MPU6050)
								if(F_mainLoop.flag_BATTERY>=F_mainLoop.flag_ULTRA){
									F_mainLoop.flag_BATTERY=0;
									F_active=F_BATTERY;
									act_m_ct=1;
									//배터리�?? ?��?��?��
								}
		}
		if(!act_m_ct){
			if(F_mainLoop.flag_CDS>=F_mainLoop.flag_BATTERY)
				if(F_mainLoop.flag_CDS>=F_mainLoop.flag_DHT11)
					if(F_mainLoop.flag_CDS>=F_mainLoop.flag_FSR)
						if(F_mainLoop.flag_CDS>=F_mainLoop.flag_GPIO)
							if(F_mainLoop.flag_CDS>=F_mainLoop.flag_MPU6050)
								if(F_mainLoop.flag_CDS>=F_mainLoop.flag_ULTRA){
									F_mainLoop.flag_CDS=0;
									F_active=F_CDS;
									act_m_ct=1;
									// ?��?��?��
								}
		}
		if(!act_m_ct){
			if(F_mainLoop.flag_DHT11>=F_mainLoop.flag_BATTERY)
				if(F_mainLoop.flag_DHT11>=F_mainLoop.flag_CDS)
					if(F_mainLoop.flag_DHT11>=F_mainLoop.flag_FSR)
						if(F_mainLoop.flag_DHT11>=F_mainLoop.flag_GPIO)
							if(F_mainLoop.flag_DHT11>=F_mainLoop.flag_MPU6050)
								if(F_mainLoop.flag_DHT11>=F_mainLoop.flag_ULTRA){
									F_mainLoop.flag_DHT11=0;
									F_active=F_DHT11;
									act_m_ct=1;
									// ?��?��?��
								}
		}
		if(!act_m_ct){
			if(F_mainLoop.flag_FSR>=F_mainLoop.flag_BATTERY)
				if(F_mainLoop.flag_FSR>=F_mainLoop.flag_CDS)
					if(F_mainLoop.flag_FSR>=F_mainLoop.flag_DHT11)
						if(F_mainLoop.flag_FSR>=F_mainLoop.flag_GPIO)
							if(F_mainLoop.flag_FSR>=F_mainLoop.flag_MPU6050)
								if(F_mainLoop.flag_FSR>=F_mainLoop.flag_ULTRA){
									F_mainLoop.flag_FSR=0;
									F_active=F_FSR;
									act_m_ct=1;
									// ?��?��?��
								}
		}
		if(!act_m_ct){
			if(F_mainLoop.flag_GPIO>=F_mainLoop.flag_BATTERY)
				if(F_mainLoop.flag_GPIO>=F_mainLoop.flag_CDS)
					if(F_mainLoop.flag_GPIO>=F_mainLoop.flag_DHT11)
						if(F_mainLoop.flag_GPIO>=F_mainLoop.flag_FSR)
							if(F_mainLoop.flag_GPIO>=F_mainLoop.flag_MPU6050)
								if(F_mainLoop.flag_GPIO>=F_mainLoop.flag_ULTRA){
									F_mainLoop.flag_GPIO=0;
									F_active=F_GPIO;
									act_m_ct=1;
									// ?��?��?��
								}
		}
		if(!act_m_ct){
			if(F_mainLoop.flag_MPU6050>=F_mainLoop.flag_BATTERY)
				if(F_mainLoop.flag_MPU6050>=F_mainLoop.flag_CDS)
					if(F_mainLoop.flag_MPU6050>=F_mainLoop.flag_DHT11)
						if(F_mainLoop.flag_MPU6050>=F_mainLoop.flag_FSR)
							if(F_mainLoop.flag_MPU6050>=F_mainLoop.flag_GPIO)
								if(F_mainLoop.flag_MPU6050>=F_mainLoop.flag_ULTRA){
									F_mainLoop.flag_MPU6050=0;
									F_active=F_MPU6050;
									act_m_ct=1;
									// ?��?��?��
								}
		}
		if(!act_m_ct){
			if(F_mainLoop.flag_ULTRA>=F_mainLoop.flag_BATTERY)
				if(F_mainLoop.flag_ULTRA>=F_mainLoop.flag_CDS)
					if(F_mainLoop.flag_ULTRA>=F_mainLoop.flag_DHT11)
						if(F_mainLoop.flag_ULTRA>=F_mainLoop.flag_FSR)
							if(F_mainLoop.flag_ULTRA>=F_mainLoop.flag_GPIO)
								if(F_mainLoop.flag_ULTRA>=F_mainLoop.flag_MPU6050){
									F_mainLoop.flag_ULTRA=0;
									F_active=F_ULTRA;
									act_m_ct=1;
									// ?��?��?��
								}
		}
		break;
	}
}





////////////////////////////////////////////////////////////////////////////////
//AT24C256
/////////////////////////////////memory save//////////
bool writeStatus = false;
bool readStatus = false;
bool eraseStatus = false;

void Memory_read(uint16_t re_MEM_ADDR)
{
	uint8_t rData[30];
	if(at24_isConnected()){
		at24_read(re_MEM_ADDR,rData,20, 100);
		//printf("\nMem read fun: %s\n",rData);
		HAL_Delay(10);
	}

}
void Memory_Reset(){
	if(at24_isConnected()){
		eraseStatus = at24_eraseChip();
	}
}
void Memory_write(uint16_t wr_MEM_ADDR,uint8_t wData[]){
	if(at24_isConnected()){
		at24_write(wr_MEM_ADDR,wData,20, 100);
		HAL_Delay(10);
	}
}
////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//DHT11
char buffer[25];
uint16_t dht_temp1=0,dht_temp2=0;
int dht11_read(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT11_data_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DHT11_data_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(DHT11_data_GPIO_Port, DHT11_data_Pin, RESET);

	delay_us(18000);

	HAL_GPIO_WritePin(DHT11_data_GPIO_Port, DHT11_data_Pin, SET);
	delay_us(20);

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_data_GPIO_Port, &GPIO_InitStruct);

	delay_us(40);
	if(!(HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin))) {
		delay_us(80);
		if(!(HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin)))
		{
			return -1;

		}
	}

	if(wait_pulse(GPIO_PIN_RESET) == 0)
	{
		return -1; // timeout

	}

	uint8_t out[5], i, j;
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 8; j++) {
			if(!wait_pulse(GPIO_PIN_SET))
			{
				return -1;
			}
			delay_us(40);
			if(!(HAL_GPIO_ReadPin (DHT11_data_GPIO_Port, DHT11_data_Pin)))
				out[i] &= ~(1<<(7-j));
			else
				out[i] |= (1<<(7-j));

			if(!wait_pulse(GPIO_PIN_RESET))
			{
				return -1;
			}
		}
	}


	if(out[4] != (out[0] + out[1] + out[2] + out[3]))
	{

		return -2;
	}
	Temperature = out[2];
	Humidity = out[0];
	HAL_TIM_Base_Stop_IT(&htim6);
	return 1;
}
void dht11_print(void)
{
	if(dht11_read() == 1) {
		sprintf(buffer, "%02dC, %02d%%", Temperature, Humidity);
		lcdSetCursor(0, 40);
		lcdPrintf(buffer);
	}
	//	 else {
	//		sprintf(buffer, "timeout");
	//		lcdSetCursor(20, 50);
	//		lcdPrintf(buffer);
	//	}
}
int wait_pulse(int state) {
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while (HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin) != state) {
		if(__HAL_TIM_GET_COUNTER(&htim6) >= 100) {
			return 0;
		}
	}
	return 1;
}

void delay_us(uint16_t time) {
	__HAL_TIM_SetCounter(&htim6,0);
	while(__HAL_TIM_GET_COUNTER(&htim6)<time);
}
///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////
//RFID
uint8_t flag_rfid_lcd=1;
void rfid_loop_init_set(){
	if(FIRST_SET_UP){
		card_check_flag=1;
		while(card_check_flag == 1 && main_loop_counter_flag < activative_time_PIR){
			Q_FLAG=0;
			REQA_t();
#if debug_mode ==0
			if(flag_rfid_lcd){
				flag_rfid_lcd=0;
				lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_ORANGE);
				lcdSetTextFont(&Font24);
				lcdSetTextColor(COLOR_BLACK, COLOR_ORANGE);
				lcdSetCursor((320-170)/2, 5*24);
				lcdPrintf("TOUCH CARD");
			}
#endif

			if(Q_FLAG){
				rfid_flag_wrong+=1;
				check_rfid_UID();
				flag_rfid_lcd=1;
				if(card_check_flag == 0){
#if debug_mode ==0
					lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_GREEN);
					lcdSetTextFont(&Font24);
					lcdSetTextColor(COLOR_BLACK, COLOR_GREEN);
					lcdSetCursor((320-126)/2, 5*24);
					lcdPrintf("Reg CARD");
					BEEP(1,7);
#endif
					main_start_flag=0;
				}
				else{
#if debug_mode ==0
					lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_RED);
					lcdSetTextFont(&Font24);
					lcdSetTextColor(COLOR_BLACK, COLOR_RED);
					lcdSetCursor((320-170)/2, 5*24);
					lcdPrintf("UnReg CARD");
					BEEP(2,7);
#endif

				}
			}
			while(rfid_flag_wrong >= 5 && main_loop_counter_flag < activative_time_PIR){
				HAL_GPIO_WritePin(Alarm_GPIO_Port, Alarm_Pin, SET);
			}
			if(rfid_flag_wrong >= 5){
				rfid_flag_wrong=0;
			}

		}
	}
	else{
		main_start_flag=0;
	}
}
void rfid_loop_setup_card(){

	if(FIRST_SET_UP){
		Q_FLAG=0;

		REQA_t();
		if(Q_FLAG){
			check_rfid_UID();

			//?���??????????????????????
			if(NEW_INPUT_CARD==1)
			{
				switch(INPUT_CARD){
				case 1:
					if(at24_isConnected()){
						at24_write(0x31,&size_card_input,1,100);
						at24_write(0x32,input_cardID,size_card_input,100);
						//printf("\nCard 2 Change\n");
#if debug_mode ==0
						lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_YELLOW);
						lcdSetTextFont(&Font24);
						lcdSetTextColor(COLOR_BLACK, COLOR_YELLOW);
						lcdSetCursor((320-221)/2, 5*24);
						lcdPrintf("CHANGE CARD 2");
#endif
						BEEP(3,7);
						HAL_Delay(3000);
					}
					break;
				case 2:
					if(at24_isConnected()){
						at24_write(0x01,&size_card_input,1,100);
						at24_write(0x02,input_cardID,size_card_input,100);
#if debug_mode ==0
						lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_YELLOW);
						lcdSetTextFont(&Font24);
						lcdSetTextColor(COLOR_BLACK, COLOR_YELLOW);
						lcdSetCursor((320-221)/2, 5*24);
						lcdPrintf("CHANGE CARD 1");
#endif

						//printf("\nCard 1 Change\n");
						BEEP(3,7);
						HAL_Delay(3000);
					}
					break;
				}
			}
		}
	}
	else{
		Q_FLAG=0;
		while(Q_FLAG==0){
			Q_FLAG=0;
			REQA_t();
			if(Q_FLAG){
				check_rfid_UID();
			}

			//?���??????????????????????
			if(Q_FLAG==1){
				if(at24_isConnected()){
					at24_write(0x01,&size_card_input,1,100);
					at24_write(0x02,input_cardID,size_card_input,100);
#if debug_mode ==0
					lcdFillRoundRect(20, 80, 280, 100, 5, COLOR_YELLOW);
					lcdSetTextFont(&Font24);
					lcdSetTextColor(COLOR_BLACK, COLOR_YELLOW);
					lcdSetCursor((320-170)/2, 5*24);
					lcdPrintf("REG CARD 1");
					HAL_Delay(3000);
#endif

					//printf("\nCard 1 Change\n");
					BEEP(3,7);
				}
			}
		}
	}
}




//반복?�� card_check_flag=1
void check_rfid_UID(){
	uint8_t check_count=0;
	uint8_t flag_card = 0;

	NEW_INPUT_CARD=0;

	if(at24_isConnected()){
		at24_read(0x01,&size_card1,1, 200);
		HAL_Delay(10);
		at24_read(0x31,&size_card2,1, 200);
		HAL_Delay(10);

		if(size_card1<21){
			at24_read(0x02,User_UID_1,size_card1, 200);
			HAL_Delay(10);
		}
		if(size_card2<21){
			at24_read(0x32,User_UID_2,size_card2, 200);
			HAL_Delay(10);
		}
	}
	sprintf(input_cardID,"%d%d%d%d",User_UID[0],User_UID[1],User_UID[2],User_UID[3]);
	size_card_input=strlen(input_cardID);

#if debug_mode == 1
	lcdSetCursor(0, 40);
	lcdPrintf("card1:");
	lcdSetCursor(14*6, 40);
	lcdPrintf(User_UID_1);
	printf("card1 %s\r\n",User_UID_1);

	lcdSetCursor(0, 60);
	lcdPrintf("card2:");
	lcdSetCursor(14*6, 60);
	lcdPrintf(User_UID_2);
	printf("card2 %s\r\n",User_UID_2);

	lcdSetCursor(0, 80);
	lcdPrintf("input:");
	lcdSetCursor(14*6, 80);
	lcdPrintf(input_cardID);
	printf("card_in %s\r\n",input_cardID);
#endif

	check_count=size_card1+1;
	if(size_card_input == size_card1 &&  size_card1>0 && size_card1<21){
		for(int i=0;i<size_card_input;i++){
			if(User_UID_1[i]==input_cardID[i])
				check_count-=1;
		}
		if(check_count==1){
			flag_card = 1;
		}
	}
	check_count=size_card2+1;
	if(size_card_input == size_card2 &&  size_card2>0 && size_card2<21){
		for(int i=0;i<size_card_input;i++){
			if(User_UID_2[i]==input_cardID[i])
				check_count-=1;
		}
		if(check_count==1){
			flag_card = 2;
		}
	}

	if(flag_card == 1){
		INPUT_CARD=1;
		card_check_flag=0;
	}
	else if(flag_card == 2){
		INPUT_CARD=2;
		card_check_flag=0;
	}
	else{
		NEW_INPUT_CARD=1;
		card_check_flag=1;
	}

}
//uint8_t sys_beep=0;
void BEEP(uint8_t rep, uint8_t rep_timing){
	for(int i=0;i<rep;i++){
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, SET);
		//		sys_beep=HAL_GetTick();
		//		while(sys_beep+rep_timing*1000<HAL_GetTick());
		HAL_Delay(rep_timing);
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, RESET);
		if(rep!=1){
			HAL_Delay(150);
		}
	}
}

/////////////////////////////////////////////////////////////////
//ultrasonic
uint8_t ultra_str[100];
volatile uint32_t pMillis;
float Value1 = 0;
float Value2 = 0;
void Ultra_Distance_Data(){
	HAL_TIM_Base_Start(&htim3);

	/////////////////////////1
	HAL_GPIO_WritePin(ultra1_Trig_GPIO_Port, ultra1_Trig_Pin, SET);
	__HAL_TIM_SetCounter(&htim3,0);
	while(__HAL_TIM_GET_COUNTER(&htim3)<10);
	HAL_GPIO_WritePin(ultra1_Trig_GPIO_Port, ultra1_Trig_Pin, RESET);

	pMillis = HAL_GetTick();
	// wait for the echo pin to go high
	while (!(HAL_GPIO_ReadPin (ultra1_ECHO_GPIO_Port, ultra1_ECHO_Pin)) && pMillis + 10 >  HAL_GetTick());
	Value1 = __HAL_TIM_GET_COUNTER (&htim3);

	pMillis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
	// wait for the echo pin to go low
	while ((HAL_GPIO_ReadPin (ultra1_ECHO_GPIO_Port, ultra1_ECHO_Pin)) &&  pMillis + 50 > HAL_GetTick());
	Value2 = __HAL_TIM_GET_COUNTER (&htim3);

	ultra_distance1 = (Value2-Value1)* 0.034/2.0;

	////////////////////////2


	HAL_GPIO_WritePin(ultra2_Trig_GPIO_Port, ultra2_Trig_Pin, SET);
	__HAL_TIM_SetCounter(&htim3,0);
	while(__HAL_TIM_GET_COUNTER(&htim3)<10);
	HAL_GPIO_WritePin(ultra2_Trig_GPIO_Port, ultra2_Trig_Pin, RESET);

	pMillis = HAL_GetTick();
	// wait for the echo pin to go high
	while (!(HAL_GPIO_ReadPin (ultra2_ECHO_GPIO_Port, ultra2_ECHO_Pin)) && pMillis + 10 >  HAL_GetTick());
	Value1 = __HAL_TIM_GET_COUNTER (&htim3);

	pMillis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
	// wait for the echo pin to go low
	while ((HAL_GPIO_ReadPin (ultra2_ECHO_GPIO_Port, ultra2_ECHO_Pin)) &&  pMillis + 50 > HAL_GetTick());
	Value2 = __HAL_TIM_GET_COUNTER (&htim3);

	ultra_distance2 = (Value2-Value1)* 0.034/2.0;

	////////////////////////3
	//
	//
	//	HAL_GPIO_WritePin(ultra3_Trig_GPIO_Port, ultra3_Trig_Pin, SET);
	//	__HAL_TIM_SetCounter(&htim3,0);
	//	while(__HAL_TIM_GET_COUNTER(&htim3)<10);
	//	HAL_GPIO_WritePin(ultra3_Trig_GPIO_Port, ultra3_Trig_Pin, RESET);
	//
	//	pMillis = HAL_GetTick();
	//	// wait for the echo pin to go high
	//	while (!(HAL_GPIO_ReadPin (ultra3_ECHO_GPIO_Port, ultra3_ECHO_Pin)) && pMillis + 10 >  HAL_GetTick());
	//	Value1 = __HAL_TIM_GET_COUNTER (&htim3);
	//
	//	pMillis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
	//	// wait for the echo pin to go low
	//	while ((HAL_GPIO_ReadPin (ultra3_ECHO_GPIO_Port, ultra3_ECHO_Pin)) &&  pMillis + 50 > HAL_GetTick());
	//	Value2 = __HAL_TIM_GET_COUNTER (&htim3);
	//
	//	ultra_distance3 = (Value2-Value1)* 0.034/2.0;

	HAL_TIM_Base_Stop(&htim3);

}

void Ultra_Distance_Data_Print(void){
	sprintf(ultra_str,"1. %.2f cm		2.%.2f cm		3.%.2f cm",ultra_distance1,ultra_distance2,ultra_distance3);
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
