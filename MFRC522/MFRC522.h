#ifndef __MFRC522_H
#define __MFRC522_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

extern SPI_HandleTypeDef hspi2;
extern uint8_t card_check_flag;
extern uint8_t INPUT_CARD;
extern volatile uint8_t Q_FLAG;
extern uint8_t print_cardID[50];
extern uint8_t NEW_INPUT_CARD;


extern uint8_t User_UID[4];

#define SPI_Sel(x) (x ? (GPIOB->ODR &=~ GPIO_PIN_3) : (GPIOB->ODR  |= GPIO_PIN_3))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//               MFRC522 register
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Page 0: Command and status
#define CommandReg				0x01	// starts and stops command execution
#define ComIEnReg				0x02	// enable and disable interrupt request control bits
#define DivIEnReg				0x03	// enable and disable interrupt request control bits
#define ComIrqReg				0x04	// interrupt request bits
#define DivIrqReg				0x05	// interrupt request bits
#define ErrorReg				0x06	// error bits showing the error status of the last command executed
#define Status1Reg				0x07	// communication status bits
#define Status2Reg				0x08	// receiver and transmitter status bits
#define FIFODataReg				0x09	// input and output of 64 byte FIFO buffer
#define FIFOLevelReg			0x0A	// number of bytes stored in the FIFO buffer
#define WaterLevelReg			0x0B	// level for FIFO underflow and overflow warning
#define ControlReg				0x0C	// miscellaneous control registers
#define BitFramingReg			0x0D	// adjustments for bit-oriented frames
#define CollReg					0x0E	// bit position of the first bit-collision detected on the RF interface
// Page 1: Command
#define ModeReg					0x11	// defines general modes for transmitting and receiving
#define TxModeReg				0x12	// defines transmission data rate and framing
#define RxModeReg				0x13	// defines reception data rate and framing
#define TxControlReg			0x14	// controls the logical behavior of the antenna driver pins TX1 and TX2
#define TxASKReg				0x15	// controls the setting of the transmission modulation
#define TxSelReg				0x16	// selects the internal sources for the antenna driver
#define RxSelReg				0x17	// selects internal receiver settings
#define RxThresholdReg			0x18	// selects thresholds for the bit decoder
#define DemodReg				0x19	// defines demodulator settings
#define MfTxReg					0x1C	// controls some MIFARE communication transmit parameters
#define MfRxReg					0x1D	// controls some MIFARE communication receive parameters
#define SerialSpeedReg			0x1F	// selects the speed of the serial UART interface
// Page 2: Configuration
#define CRCResultRegH			0x21	// shows the MSB and LSB values of the CRC calculation
#define CRCResultRegL			0x22
#define ModWidthReg				0x24	// controls the ModWidth setting?
#define RFCfgReg				0x26	// configures the receiver gain
#define GsNReg					0x27	// selects the conductance of the antenna driver pins TX1 and TX2 for modulation
#define CWGsPReg				0x28	// defines the conductance of the p-driver output during periods of no modulation
#define ModGsPReg				0x29	// defines the conductance of the p-driver output during periods of modulation
#define TModeReg				0x2A	// defines settings for the internal timer
#define TPrescalerReg			0x2B	// the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
#define TReloadRegH				0x2C	// defines the 16-bit timer reload value
#define TReloadRegL				0x2D
#define TCounterValueRegH		0x2E	// shows the 16-bit timer value
#define TCounterValueRegL		0x2F
// Page 3: Test Registers
#define TestSel1Reg				0x31	// general test signal configuration
#define TestSel2Reg				0x32	// general test signal configuration
#define TestPinEnReg			0x33	// enables pin output driver on pins D1 to D7
#define TestPinValueReg			0x34	// defines the values for D1 to D7 when it is used as an I/O bus
#define TestBusReg				0x35	// shows the status of the internal test bus
#define AutoTestReg				0x36	// controls the digital self-test
#define VersionReg				0x37	// shows the software version
#define AnalogTestReg			0x38	// controls the pins AUX1 and AUX2
#define TestDAC1Reg				0x39	// defines the test value for TestDAC1
#define TestDAC2Reg				0x3A	// defines the test value for TestDAC2
#define TestADCReg				0x3B    // shows the value of ADC I and Q channels

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//               Command
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PCD_CMD_Idle 				0x00
#define PCD_CMD_Mem					0x01
#define PCD_CMD_GenerateRandomId	0x02
#define PCD_CMD_CalcCRC				0x03
#define PCD_CMD_Transmit			0x04
#define PCD_CMD_NoCmdChange			0x07
#define PCD_CMD_Receive				0x08
#define PCD_CMD_Transceive			0x0C
#define PCD_CMD_MFAuthent			0x0E
#define PCD_CMD_SoftReset			0x0F


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 					Mifare_One card command word
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
#define PICC_CMD_REQA			0x26		// REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
#define PICC_CMD_WUPA			0x52		// Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
#define PICC_CMD_CT				0x88		// Cascade Tag. Not really a command, but used during anti collision.
#define PICC_CMD_SEL_CL1			0x93		// Anti collision/Select, Cascade Level 1
#define PICC_CMD_SEL_CL2			0x95		// Anti collision/Select, Cascade Level 2
#define PICC_CMD_SEL_CL3			0x97		// Anti collision/Select, Cascade Level 3
#define PICC_CMD_HLTA				0x50		// HaLT(Sleep) command, Type A. Instructs an ACTIVE PICC to go to state HALT.
#define PICC_CMD_RATS         		0xE0    	// Request command for Answer To Reset.
// The commands used for MIFARE Classic (from http://www.mouser.com/ds/2/302/MF1S503x-89574.pdf, Section 9)
// Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
// The read/write commands can also be used for MIFARE Ultralight.
#define MF_CMD_AUTH_KEY_A		0x60		// Perform authentication with Key A
#define MF_CMD_AUTH_KEY_B		0x61		// Perform authentication with Key B
#define MF_CMD_READ				0x30		// Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
#define MF_CMD_WRITE				0xA0		// Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
#define MF_CMD_DECREMENT			0xC0		// Decrements the contents of a block and stores the result in the internal data register.
#define MF_CMD_INCREMENT			0xC1		// Increments the contents of a block and stores the result in the internal data register.
#define MF_CMD_RESTORE			0xC2		// Reads the contents of a block into the internal data register.
#define MF_CMD_TRANSFER			0xB0		// Writes the contents of the internal data register to a block.
// The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
// The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
#define M_CMD_UL_WRITE		 0xA2		// Writes one 4 byte page to the PICC.


typedef enum {
	STATUS_OK 				,	// Success
	STATUS_ERROR			,	// Error in communication
	STATUS_COLLISION		,	// Collission detected
	STATUS_TIMEOUT			,	// Timeout in communication.
	STATUS_NO_ROOM			,	// A buffer is not big enough.
	STATUS_INTERNAL_ERROR	,	// Internal error in the code. Should not happen ;-)
	STATUS_INVALID			,	// Invalid argument.
	STATUS_KEY_ERROR		,	//
	STATUS_BCC_WRONG		,	// The BCC does not match
	STATUS_CRC_WRONG		,	// The CRC_A does not match
	STATUS_CMD_WRONG		,	// Command does not match
	STATUS_MIFARE_NACK		= 0xff	// A MIFARE PICC responded with NAK.
} StatusCode;

typedef enum{
	IDLE,
	READY,
	ACTIVE,
	READY2,
	ACTIVE2,
	HALT,
	I4_RATS,
	I4_PPS,
	I4_ACTIVE
}PCD_STATUS;

typedef struct{
	uint16_t ATQA16;
	uint8_t ATQA[2];
	uint8_t UID[10];
	uint8_t SAK;
	uint8_t UIDSize;
	uint8_t bit_frame_anticol;
}BF;

typedef struct{
	uint8_t sameD;
	uint8_t DS;
	uint8_t	DR;
}_TA;

typedef struct{
	uint8_t SFGI;
	uint8_t FWI;
}_TB;

typedef struct{
	uint8_t NAD;
	uint8_t CID;
}_TC;

typedef struct{
	uint8_t TL;
	uint8_t T0;
	uint8_t FSCI;
	_TA TA;
	_TB TB;
	_TC TC;
	uint8_t T1;
	uint8_t Tk;
	uint8_t CID;
}ATS;

//PCB Block
#define	IBlock					0x02  //00xxxx1x
#define	RBlock					0x82 //10xxxx1x
#define SBlock					0xC2 //11xxxx1x

#define Block_Num0				0x00
#define Block_Num1				0x01
#define NAD_Set					0x04
#define CID_Set					0x08
#define Chaning_Set				0x10
#define ACK_Set					0x10
#define Deselect				0x00
#define WTX						0x30

//Card types
#define Mifare_UltraLight 		0x0044
#define Mifare_One_S50			0x0004
#define Mifare_One_S70			0x0002
#define Mifare_Pro_X			0x0008
#define Mifare_DESFire			0x0344


#define MAXRLEN        			18
#define FIFO_SIZE		        64           //FIFO size=64byte
//#define MIN_STRENGTH  228

//PCD_RxGain
#define RxGain_18dB				 0x00 << 4	// 000b - 18 dB, minimum
#define RxGain_23dB				 0x01 << 4	// 001b - 23 dB
#define RxGain_18dB_2			 0x02 << 4	// 010b - 18 dB, it seems 010b is a duplicate for 000b
#define RxGain_23dB_2			 0x03 << 4	// 011b - 23 dB, it seems 011b is a duplicate for 001b
#define RxGain_33dB				 0x04 << 4	// 100b - 33 dB, average, and typical default
#define RxGain_38dB				 0x05 << 4	// 101b - 38 dB
#define RxGain_43dB				 0x06 << 4	// 110b - 43 dB
#define RxGain_48dB				 0x07 << 4	// 111b - 48 dB, maximum








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//               Function
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t Read_RFID(int ADR);
void Write_RFID(int ADR,int cmd);
void SetAntennaGain(uint8_t mask);
void Init_RFID();
void Reset_RFID();
void AntennaOn_RFID();
void AntennaOff_RFID();
void SetRegisterBit_RFID(uint8_t reg,uint8_t mask);
void ClearRegisterBit_RFID(uint8_t reg,uint8_t mask);
void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData);
StatusCode HLTA_RFID(void);
void StopCrypto1_RFID();
StatusCode Read_CardData(uint8_t blockAddr);
StatusCode Write_CardData(uint8_t blockAddr,uint8_t *data);
StatusCode Comunicate_PCDPICC(uint8_t Command ,uint8_t *pInData ,uint8_t InLenByte,uint8_t *pOutData,uint8_t *pOutLenBit);
void Check_ATQID(uint8_t Casecadeflg);
void Check_ATQA();
void REQA_t();
void REQ_ID();

StatusCode Authenticate_PICC(uint8_t req_code, uint8_t blockAddr); //req_code = M_CMD_AUTH_KEY_A or M_CMD_AUTH_KEY_B
void Active();
void RATS_I4();
void PPS_I4();
void Deslelect_I4(); //SBloc
void IBLOCK_I4(uint8_t *pInData ,uint8_t InLenByte, uint8_t *pOutData);
void PICC_Sequence();
StatusCode Error_write(StatusCode status);
#endif



