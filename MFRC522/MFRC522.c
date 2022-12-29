#include "MFRC522.h"
#include "math.h"

uint8_t Temp[4];
uint8_t UID[10];
uint8_t SAK[1];
uint8_t KEY[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t TAGDATA[16];
uint8_t INFDATA[128];
uint8_t User_UID[4];
uint8_t INPUT_CARD;
uint8_t NEW_INPUT_CARD;

uint8_t card_check_flag;
volatile uint8_t Q_FLAG;
uint8_t print_cardID[50];

PCD_STATUS PSTATUS;
BF PICC;

SPI_HandleTypeDef hspi2;



uint8_t Read_RFID(int ADR){
	uint8_t TX_BF[1]={128};
	uint8_t RX_BF[1]={0};
	TX_BF[0] +=(ADR<<1)&0x7E;
	SPI_Sel(1);


	HAL_SPI_Transmit(&hspi2,TX_BF,1,100);
	HAL_SPI_Receive(&hspi2,RX_BF,1,100);



	SPI_Sel(0);
	return RX_BF[0];
}

void Write_RFID(int ADR,int cmd)
{
	uint8_t RX_BF[1]={0};

	uint8_t TX_BF[2];

	TX_BF[0] = (ADR<<1)&0x7E;
	TX_BF[1] = cmd;
	SPI_Sel(1);
	HAL_SPI_Transmit(&hspi2,TX_BF,2,100);
	SPI_Sel(0);
}

void SetAntennaGain(uint8_t mask) {
	if ((Read_RFID(RFCfgReg) & 0x70) != mask) {						// only bother if there is a change
		Write_RFID(RFCfgReg, mask);	// only set RxGain[2:0] bits
	}
}

void Init_RFID()
{
	printf("-------------------------RFID Initial Start-------------------------\r\n");
	Reset_RFID();

	uint8_t ReadRegister=0;
	uint16_t Prescale=0;
	uint16_t Reload=0;

	ReadRegister = Read_RFID(VersionReg);
	if((ReadRegister&0xF0)==0x90)	printf("MFRC522 version %d.0\r\n",ReadRegister&0x0F);
	else printf("RC522 Read Failed, data : %x\r\n",ReadRegister);
	printf("--------------------------------------------------------------------\r\n");
	Write_RFID(TModeReg,0x80);  		//Timer mode setting

	// Timer auto Start, restart initial : disable
	ReadRegister = Read_RFID(TModeReg);
	if((ReadRegister&0x80)==0x80) printf("Timer Auto Start Mode    : Enable\r\n");
	else 	printf("Timer Auto Start Mode    : Disable\r\n");
	printf("Timer Gated modes        : %d\r\n",(ReadRegister>>5)&0x03); // 0 : non-gated mode
	if((ReadRegister&0x10)==0x10) printf("Timer Auto Restart Mode  : Enable\r\n");
	else	printf("Timer Auto Restart Mode  : Disable\r\n");
	Prescale = ((uint16_t)(ReadRegister&0x0F))<<8;

	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].

	// Timer Prescale initial = 3328
	Write_RFID(TPrescalerReg, 0xA9);  //Timer Prescaler_low
	ReadRegister = Read_RFID(TPrescalerReg);
	Prescale += ReadRegister;
	printf("Timer Prescale           : %d\r\n",Prescale);

	// t_delay = (2*TPreScaler+1) * (TReload+1)/ 13.56 MHz

	// Timer Reload initial =0
	Write_RFID(TReloadRegH, 0x03);   	//Timer Reload Register 1
	Write_RFID(TReloadRegL, 0xE7);		//Timer Reload Register 2
	ReadRegister = Read_RFID(TReloadRegH);
	Reload = ((uint16_t)ReadRegister<<8);
	ReadRegister = Read_RFID(TReloadRegL);
	Reload += ReadRegister;
	printf("Timer Reload Value       : %d\r\n",Reload);
	printf("Total delay time         : (2*%d+1)*(%d+1)/13.56Mhz = %dms\r\n",Prescale,Reload,(Prescale*2+1)*(Reload+1)/13560);
	// TxASKReg Default  = 0
	Write_RFID(TxASKReg, 0x40);		//Reader to card 100 % ASK, Miller encoded, transfer speed 106 kBd to 848 kBd.
	ReadRegister = Read_RFID(TxASKReg);
	if(ReadRegister==0) printf("Control Transmit Setting : Disable\r\n");
	else printf("Control Transmit Setting : forces a 100 Percent ASK modulation\r\n");

	// ModeReg Default = 3F
	Write_RFID(ModeReg, 0x3D); //Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
	ReadRegister = Read_RFID(ModeReg);
	if((ReadRegister&0x03)==0x01) printf("CRC Preset Value         : 6363h\r\n");
	else if((ReadRegister&0x03)==0x02) printf("CRC Preset Value         : A671h\r\n");
	else if((ReadRegister&0x03)==0x03) printf("CRC Preset Value         : FFFFh\r\n");
	else if((ReadRegister&0x03)==0x00) printf("CRC Preset Value         : 0000h\r\n");
	printf("--------------------------------------------------------------------\r\n");

	SetAntennaGain(RxGain_48dB);
	AntennaOn_RFID();
	ReadRegister=Read_RFID(RFCfgReg);
	if((ReadRegister&0x70)==0x00) 	 printf("Rx Gain              : 18dB\r\n");
	else if((ReadRegister&0x70)==0x10) printf("Rx Gain              : 23dB\r\n");
	else if((ReadRegister&0x70)==0x20) printf("Rx Gain              : 18dB\r\n");
	else if((ReadRegister&0x70)==0x30) printf("Rx Gain              : 23dB\r\n");
	else if((ReadRegister&0x70)==0x40) printf("Rx Gain              : 33dB\r\n");
	else if((ReadRegister&0x70)==0x50) printf("Rx Gain              : 38dB\r\n");
	else if((ReadRegister&0x70)==0x60) printf("Rx Gain              : 43dB\r\n");
	else if((ReadRegister&0x70)==0x70) printf("Rx Gain              : 48dB\r\n");

	ReadRegister = Read_RFID(TxControlReg);
	if((ReadRegister&0x03)== 0x03) printf("Antenna is on\r\n");
	printf("--------------------------------------------------------------------\r\n");
}

void Reset_RFID()
{
	Read_RFID(CommandReg); 		//reset
	Write_RFID(CommandReg,PCD_CMD_SoftReset); 		//reset
	for(int i=0;i<3;i++){
		if((Read_RFID(CommandReg)&0x10)==0x10)
			printf("SoftRest ing...\n\r");
		else {
			printf("SoftRest End\n\r");
			return;
		}

	}

	// Reset baud rates
	Write_RFID(TxModeReg, 0x00); // reset value: 00h
	Write_RFID(RxModeReg, 0x00); // reset value: 00h
	// Reset ModWidthReg
	Write_RFID(ModWidthReg, 0x26); // reset value: 26h
}

void AntennaOn_RFID()
{
	uint8_t value = Read_RFID(TxControlReg);
	if ((value & 0x03) != 0x03) {
		SetRegisterBit_RFID(TxControlReg,0x03);
	}
}

void AntennaOff_RFID()
{
	ClearRegisterBit_RFID(TxControlReg, 0x03);
}

void SetRegisterBit_RFID(uint8_t reg,uint8_t mask) {
	uint8_t tmp;
	tmp = Read_RFID(reg);
	Write_RFID(reg, tmp | mask);
}

void ClearRegisterBit_RFID(uint8_t reg,uint8_t mask) {
	uint8_t tmp;
	tmp = Read_RFID(reg);
	Write_RFID(reg, tmp & (~mask));
}



void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
{
	uint8_t i,n;
	ClearRegisterBit_RFID(DivIrqReg,0x04);
	Write_RFID(CommandReg,PCD_CMD_Idle);
	SetRegisterBit_RFID(FIFOLevelReg,0x80);//
	for (i=0; i<len; i++)
	{   Write_RFID(FIFODataReg, *(pIndata+i));   }
	Write_RFID(CommandReg, PCD_CMD_CalcCRC);
	i = 0xFF;
	do
	{
		n = Read_RFID(DivIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x04));
	pOutData[0] = Read_RFID(CRCResultRegL);
	pOutData[1] = Read_RFID(CRCResultRegH);
}

StatusCode HLTA_RFID(void)
{
	StatusCode status;
	uint8_t unLen;
	uint8_t MF522Buf[MAXRLEN];

	MF522Buf[0] = PICC_CMD_HLTA;
	MF522Buf[1] = 0;
	CalulateCRC(MF522Buf,2,&MF522Buf[2]);

	status = Comunicate_PCDPICC(PCD_CMD_Transceive,MF522Buf,4,MF522Buf,&unLen);
	return status;
}

void StopCrypto1_RFID() {
	ClearRegisterBit_RFID(Status2Reg, 0x08); // Status2Reg[7..0] bits are: TempSensClear I2CForceHS reserved reserved MFCrypto1On ModemState[2:0]
}


StatusCode Read_CardData(uint8_t blockAddr)
{
	StatusCode status;
	uint8_t MF522Buf[MAXRLEN]={0,};
	uint8_t unLen;

	MF522Buf[0] = MF_CMD_READ;
	MF522Buf[1] = blockAddr;
	CalulateCRC(MF522Buf,2,&MF522Buf[2]);
	status = Comunicate_PCDPICC(PCD_CMD_Transceive,MF522Buf,4,MF522Buf,&unLen);

	if(!(status==STATUS_OK) && (unLen == 0x90)) {
		printf("Card Data Read Failed\n\r");
		return status;
	}

	for(int i=0;i<16;i++) TAGDATA[i]=MF522Buf[i];
	return status;
}

StatusCode Write_CardData(uint8_t blockAddr,uint8_t *data)
{
	StatusCode status;
	uint8_t MF522Buf[MAXRLEN]={0,};
	uint8_t unLen;

	MF522Buf[0] = MF_CMD_WRITE;
	MF522Buf[1] = blockAddr;
	CalulateCRC(MF522Buf,2,&MF522Buf[2]);
	status = Comunicate_PCDPICC(PCD_CMD_Transceive,MF522Buf,4,MF522Buf,&unLen);

	if(status!=STATUS_OK||(unLen != 4 )||((MF522Buf[0]&0x0F)!= 0x0A)) {
		printf("Card Data Write CMD Failed\n\r");
		return status;
	}

	for (int i=0; i<16; i++)	MF522Buf[i] = *(data+i);

	CalulateCRC(MF522Buf,16,&MF522Buf[16]);
	status = Comunicate_PCDPICC(PCD_CMD_Transceive,MF522Buf,18,MF522Buf,&unLen);

	if((status != STATUS_OK)||(unLen != 4 )||((MF522Buf[0]&0x0F)!= 0x0A)){
		status = STATUS_ERROR;
		return status;
	}

	return status;
}

StatusCode Comunicate_PCDPICC(uint8_t Command ,uint8_t *pInData ,uint8_t InLenByte,uint8_t *pOutData,uint8_t *pOutLenBit)
{
	StatusCode status = STATUS_ERROR;
	//	uint8_t irqEn   = 0x00;
	uint8_t waitFor = 0x00;
	uint8_t lastBits= 0;
	uint8_t n=0;
	uint16_t i=0;

	switch (Command){
	case PCD_CMD_MFAuthent:
		//			irqEn   = 0x12;
		waitFor = 0x10;	//IdleIRQ
		break;
	case PCD_CMD_Transceive:
		//			irqEn   = 0x77;
		waitFor = 0x30; //IdleIRQ,RXIRQ
		break;
	default:
		return STATUS_CMD_WRONG;
		break;
	}

	Write_RFID(CommandReg,PCD_CMD_Idle);							// Stop any active command.
	ClearRegisterBit_RFID(ComIrqReg, 0x80);						    // Clear all seven interrupt request bits
	SetRegisterBit_RFID(FIFOLevelReg,0x80);	 					// FlushBuffer = 1, FIFO initialization
	for(i=0; i<InLenByte; i++)	Write_RFID(FIFODataReg,pInData[i]); // Write sendData to the FIFO

	//Write_RFID(ComIEnReg,irqEn|0x80); //�������� ���� Ȯ�� �ʿ�

	Write_RFID(CommandReg, Command);								//Command execute

	if(Command == PCD_CMD_Transceive) SetRegisterBit_RFID(BitFramingReg,0x80);
	//Each transmit process must be started by setting the BitFramingReg register��s StartSend bit(7bit) to logic 1

	i = 2000; //
	do
	{
		n = Read_RFID(ComIrqReg);
		i--;
	}
	while((i!=0)&&!(n&0x01)&&!(n&waitFor));							//  WaitFor is success...

	uint8_t err =Read_RFID(ErrorReg);

	if (n&0x01)
		return STATUS_TIMEOUT;								// TimerIRQ Flg Timeout
	if (i==0)
		return STATUS_TIMEOUT;								// do ~ while Timeout
	else{
		if(!(err&0x1B))	{
			status = STATUS_OK;

			if(Command == PCD_CMD_Transceive)  {
				n = Read_RFID(FIFOLevelReg);						// number of Bytes in FIFO Buffer
				lastBits = Read_RFID(ControlReg)&0x07; 				// RxLastBits[2:0]

				if(n==0 && lastBits==0) return STATUS_NO_ROOM;		// FIFO Buffer is zero.

				if(lastBits) *pOutLenBit = (n-1)*8 + lastBits;
				else	  	 *pOutLenBit = n*8;

				if(n==0) n=1;
				if(n>MAXRLEN) n = MAXRLEN;

				for (i=0; i<n; i++)
					pOutData[i] = Read_RFID(FIFODataReg);
			}
		}else {
			if(err&0x01) printf("ErrorReg : Protocol Error\n\r");
			if(err&0x02) printf("ErrorReg : Parity check failed\n\r");
			if(err&0x08) printf("ErrorReg : Collision Detected\n\r");
			if(err&0x10) printf("ErrorReg : FIFO buffer is already full\n\r");
			return STATUS_ERROR;
		}
	}
	return status;
}

void Check_ATQID(uint8_t Casecadeflg)
{
	Q_FLAG=1;

	printf("\nUID : ");
	card_check_flag=0;

	for(int i=0;i<Casecadeflg*3+1;i++){

		User_UID[i]=PICC.UID[i];
		printf("%d ",User_UID[i]);
	}

	printf("\n\rSAK : %#x\n\r",PICC.SAK);

	switch(PICC.SAK){
	case 0x04:	printf("PICC type : SAK indicates UID is not complete\n\r");
	break;
	case 0x09:	printf("PICC type : MIFARE Mini, 320 bytes\n\r");
	break;
	case 0x08:	printf("PICC type : MIFARE 1KB\n\r");
	break;
	case 0x88:	printf("PICC type : MIFARE 1KB by infineon\n\r");
	break;
	case 0x18:	printf("PICC type : MIFARE 4KB\n\r");
	break;
	case 0x00:	printf("PICC type : MIFARE Ultralight or Ultralight C\n\r");
	break;
	case 0x10:  printf("PICC type : Unknown type\n\r");
	break;
	case 0x11:	printf("PICC type : MIFARE Plus\n\r");
	break;
	case 0x01:	printf("PICC type : MIFARE TNP3XXX\n\r");
	break;
	case 0x20:	printf("PICC type : PICC compliant with ISO/IEC 14443-4\n\r");
	if (PICC.ATQA16 == Mifare_DESFire) printf("PICC type : MIFARE DESFire\n\r");
	if (PICC.ATQA16 == Mifare_UltraLight) printf("PICC type : Mifare_UltraLight\n\r");
	break;
	case 0x40:	printf("PICC type : PICC compliant with ISO/IEC 18092 (NFC)\n\r");
	break;
	default:	printf("PICC type : Unknown type\n\r");
	break;

	}
}
void Check_ATQA()
{
	PICC.ATQA16 = (PICC.ATQA[1]<<8) + PICC.ATQA[0];
	PICC.UIDSize = (PICC.ATQA[0]>>6)&0x03;
	PICC.bit_frame_anticol= PICC.ATQA[0]&0x1F;
	switch(PICC.UIDSize){
	case 0:
		printf("UID Size : Single\n\r");
		break;
	case 1:
		printf("UID Size : double\n\r");
		break;
	case 2:
		printf("UID Size : triple\n\r");
		break;
	case 3:
		printf("UID Size : Reserved\n\r");
		break;
	default:
		printf("UID Size : ATQA ERR\n\r");
		break;
	}

	switch(PICC.bit_frame_anticol){
	case 1:
		printf("1st bit frame anticollision\n\r");
		break;
	case 2:
		printf("2nd bit frame anticollision\n\r");
		break;
	case 4:
		printf("3rd bit frame anticollision\n\r");
		break;
	case 8:
		printf("4th bit frame anticollision\n\r");
		break;
	case 16:
		printf("5th bit frame anticollision\n\r");
		break;
	default:
		printf("bit frame anticollision ERR\n\r");
		break;
	}

	switch(PICC.ATQA16){
	case Mifare_UltraLight :
		printf("Card type : Mifare_UltraLight\n\r");
		break;
	case Mifare_One_S50 :
		printf("Card type : Mifare_One_S50\n\r");
		break;
	case Mifare_One_S70 :
		printf("Card type : Mifare_One_S70\n\r");
		break;
	case Mifare_Pro_X :
		printf("Card type : Mifare_Pro_X\n\r");
		break;
	case Mifare_DESFire :
		printf("Card type : Mifare_DESFire\n\r");
		break;
	default:
		printf("Card type : Unknown\n\r");
		break;
	}
}
void REQA_t()
{
	StatusCode status;
	uint8_t RxbitLen;
	uint8_t TransceiveBFF[MAXRLEN];

	// Reset baud rates
	Write_RFID(TxModeReg, 0x00); // reset value: 00h
	Write_RFID(RxModeReg, 0x00); // reset value: 00h
	// Reset ModWidthReg
	Write_RFID(ModWidthReg, 0x26); // reset value: 26h
	ClearRegisterBit_RFID(CollReg, 0x80); // ValuesAfterColl=1 => Bits received after collision are cleared.

	//REQA&WUPA Command : Short frame 7bit ISO-14443-3 6.1.5.1 / 6.3.1
	uint8_t bit_oriented_frames = 7;
	Write_RFID(BitFramingReg,bit_oriented_frames); //TXLastbit write

	TransceiveBFF[0] = PICC_CMD_REQA; // PICC_CMD_REQA//PICC_CMD_WUPA

	status = Comunicate_PCDPICC(PCD_CMD_Transceive,TransceiveBFF,1,TransceiveBFF,&RxbitLen);

	if(Error_write(status) != STATUS_OK) {
		PSTATUS = IDLE;
		return;
	}

	if(RxbitLen == 16)  //ATQA : 16bit
	{
		PICC.ATQA[0] = TransceiveBFF[0];
		PICC.ATQA[1] = TransceiveBFF[1];
		PSTATUS = READY;
		printf("PICC Sequence IDLE to Ready\n\r");
		Check_ATQA();
	}else {
		printf("Error in communication in REQA_t/RxbitLen\n\r");
		PSTATUS = IDLE;
	}
	if(PSTATUS==READY){
		REQ_ID();
	}
}

void REQ_ID()
{
	StatusCode status;
	uint8_t i,BCC_check=0;
	uint8_t RxbitLen;
	uint8_t TransceiveBFF[MAXRLEN];
	uint8_t temp[5];
	uint8_t Casecadelevel;
	uint8_t Casecadeflg=0;


	//all received bits will be cleared after a collision only used during bitwise anticollision at 106 kBd
	ClearRegisterBit_RFID(CollReg,0x80);

	Write_RFID(BitFramingReg,0x00); 						 //TXLastbit write = 0


	do{ //Casecade level 1 to 3
		BCC_check=0;
		Casecadeflg+=1;

		if(Casecadeflg>1){
			for(i=0;i<3;i++)	PICC.UID[(Casecadeflg-2)*3+i]= temp[i+1];
		}
		switch (Casecadeflg){//SEL
		case 1:
			Casecadelevel = PICC_CMD_SEL_CL1;
			break;
		case 2:
			Casecadelevel = PICC_CMD_SEL_CL2;
			break;
		case 3:
			Casecadelevel = PICC_CMD_SEL_CL3;
			break;
		default:
			Error_write(STATUS_INTERNAL_ERROR);
			return;
			break;
		}
		TransceiveBFF[0] = Casecadelevel;
		TransceiveBFF[1] = 0x20;	//NVB 2Byte

		status = Comunicate_PCDPICC(PCD_CMD_Transceive,TransceiveBFF,2,TransceiveBFF,&RxbitLen);
		if(Error_write(status) != STATUS_OK) {
			PSTATUS = IDLE;
			return;
		} else{
			for (i=0; i<4; i++)
			{
				temp[i]  = TransceiveBFF[i];
				BCC_check ^= TransceiveBFF[i];
			}
			if (BCC_check != TransceiveBFF[i]) status = Error_write(STATUS_BCC_WRONG);


			if (status == STATUS_OK){
				for(i=0;i<5;i++) 	TransceiveBFF[6-i]=TransceiveBFF[4-i];

				TransceiveBFF[0] = Casecadelevel; //SEL
				TransceiveBFF[1] = 0x70;	//NVB 2Byte
				CalulateCRC(TransceiveBFF,7,&TransceiveBFF[7]);

				status = Comunicate_PCDPICC(PCD_CMD_Transceive,TransceiveBFF,9,TransceiveBFF,&RxbitLen);

				PICC.SAK = TransceiveBFF[0];
			}
		}

	}while((PICC.SAK&0x04)==0x04);

	printf("Casecade level %d Completed\n\r",Casecadeflg);

	if(Casecadeflg != PICC.UIDSize+1) printf("Check the cascade level\n\r");

	for(i=0;i<4;i++)	PICC.UID[(Casecadeflg-1)*3+i]= temp[i];
	Check_ATQID(Casecadeflg);
	PSTATUS = ACTIVE;
	printf("PICC Sequence Ready to ACTIVE\n\r");
	SetRegisterBit_RFID(CollReg,0x80);//CollRegCollReg �106kbps��
}

StatusCode Authenticate_PICC(uint8_t req_code, uint8_t blockAddr) //req_code = M_CMD_AUTH_KEY_A or M_CMD_AUTH_KEY_B
{
	StatusCode status;
	uint8_t RxbitLen;
	// Build command buffer
	uint8_t TransceiveBFF[MAXRLEN]={0,};
	//uint8_t sendData[12];
	TransceiveBFF[0] = req_code;
	TransceiveBFF[1] = blockAddr;
	for (uint8_t i = 0; i < 6; i++) {				// key 6bytes
		TransceiveBFF[2+i] = KEY[i];
	}
	for (uint8_t i = 0; i < 4; i++) {				// UID 4bytes
		TransceiveBFF[8+i] = PICC.UID[i];
	}

	//Write_RFID(BitFramingReg,0x00);
	// Start the authentication.
	status = Comunicate_PCDPICC(PCD_CMD_MFAuthent,TransceiveBFF,12,TransceiveBFF,&RxbitLen);

	//printf("%x\n\r",Read_RFID(Status2Reg));
	if(status == STATUS_OK){
		if(!(Read_RFID(Status2Reg) & 0x08)) status = STATUS_KEY_ERROR;
	}

	return status;
}

void Active()
{
	StatusCode status=0;

	switch(PICC.SAK)
	{
	case 0x20:
		//ISO14443-4 Sequence
		PSTATUS = I4_RATS;
		printf("PICC Sequence ACTIVE to RATS by ISO14443-4 Sequence\n\r");
		return;
		break;
	case 0x40:
		PSTATUS = IDLE;
		printf("PICC Sequence ACTIVE to IDLE by NTAG Sequence\n\r");
		return;
		//NTAG Sequence
		break;
	default:
		status = Authenticate_PICC(MF_CMD_AUTH_KEY_A,3);
		break;
	}

	if(Error_write(status)!=STATUS_OK){
		PSTATUS = HALT;
		printf("PICC Sequence Active to HALT, check the Error\n\r");
	}
	else{
		printf("Authenticating using key A [0xff 0xff 0xff 0xff 0xff 0xff] : OK\n\r\n\r");
		printf("PICC Data Reading Start\n\r");
		printf("--------------------------------------------------------------------------------------------\r\n");
		printf("Sector Block DATA [ 0 ... 15 Bytes]                                                         \r\n");
		printf("             00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15\n\r");
		printf("--------------------------------------------------------------------------------------------\r\n");

		for(int Sector=0;Sector<16;Sector++){
			status = Authenticate_PICC(MF_CMD_AUTH_KEY_A,4*Sector+3);
			if(status==STATUS_OK){

				for(int Block=0;Block<4;Block++){
					printf("  %2d     %d   ",Sector,Block);
					status = Read_CardData(4*Sector+Block);
					if(status == STATUS_OK){
						for(int i=0;i<16;i++) printf("0x%2X ",TAGDATA[i]);
						printf("\n\r");

					}else{
						printf("Sector %d,Block %d Data Read NG : ",Sector,Block);
						Error_write(status);
					}
				}
			}else {
				printf("   %d Sector : ",Sector);
				Error_write(status);
			}
		}
		PSTATUS = HALT;
		printf("PICC Sequence Active to HALT\n\r");
	}
}

ATS RATS;

void RATS_I4()
{
	StatusCode status;
	uint8_t i=0,RxbitLen;
	uint8_t TransceiveBFF[FIFO_SIZE]={0,};
	uint8_t TA=0,TB=0,TC=0;
	RATS.CID=0x00;
	TransceiveBFF[0] = PICC_CMD_RATS;							// START BYTE(E0)
	TransceiveBFF[1] = 0X50+(RATS.CID&0xf);						// 0x XY , X = FSDI Y= CID
	//FSDI : 5 , FIFO Buffer size 64,
	//CID  : 0 , it defines the logical number of the addressed PICC in the range from 0 to 14. The value 15 is RFU. The CID is specified by the PCD and shall be unique for all PICCs,
	CalulateCRC(TransceiveBFF,2,&TransceiveBFF[2]);

	status = Comunicate_PCDPICC(PCD_CMD_Transceive,TransceiveBFF,4,TransceiveBFF,&RxbitLen);

	if(Error_write(status) != STATUS_OK) {
		PSTATUS = IDLE;
		return;
	} else{
		RATS.TL = TransceiveBFF[i++];
		RATS.T0 = TransceiveBFF[i++];
		RATS.FSCI = RATS.T0&0x0f;

		if(RATS.FSCI<5) RATS.FSCI = (RATS.FSCI+1)*8;
		else RATS.FSCI=64;					//FIFO =64 Upper 64byte can not use.

		if((RATS.T0&0x10)==0x10){
			TA = TransceiveBFF[i++];
			RATS.TA.DR = (TA&0x07)*2;
			RATS.TA.DS = ((TA>>4)&0x07)*2;
			RATS.TA.sameD = (TA>>7)&0x01;
		}else{
			RATS.TA.DR = 0;
			RATS.TA.DS = 0;
		}
		if((RATS.T0&0x20)==0x20){
			TB = TransceiveBFF[i++];
			RATS.TB.SFGI = TB&0x0f;
			RATS.TB.FWI = (TB>>4)&0x0f;
		}else{
			RATS.TB.SFGI=0;
			RATS.TB.FWI=0;
		}
		if((RATS.T0&0x40)==0x40){
			TC = TransceiveBFF[i++];
			RATS.TC.CID = (TC>>1)&&0x01;
			RATS.TC.NAD = TC&0x01;
		}else{
			RATS.TC.CID=1;
			RATS.TC.NAD=0;
		}

		printf("ATS Result\n\r");
		printf("--------------------------------------------------------------------\r\n");
		printf("TL : %d",RATS.TL);
		printf("         FSCI : %d\n\r",RATS.FSCI);
		if(TA !=0) printf("TA Transmit : true  ");
		else printf("TA Transmit : false ");
		if(TB !=0) printf(",TB Transmit : true  ");
		else printf(",TB Transmit : false ");
		if(TC !=0) printf(",TC Transmit : true\n\r");
		else printf(",TC Transmit : false\n\r");
		printf("TA_DR       : %d     ",RATS.TA.DR);
		printf(",TA_DS       : %d     ",RATS.TA.DS);
		printf(",TA_sameD    : %d\n\r",RATS.TA.sameD);
		printf("TB_FWI      : %d     ",RATS.TB.FWI);
		printf(",TB_SFGI     : %d\n\r",RATS.TB.SFGI);
		printf("FWT         : %.3f ms, SFGT     : %.3f ms\n\r",(double)256*16/13560*pow(2,RATS.TB.FWI),(double)256*16/13560*pow(2,RATS.TB.SFGI));
		if(RATS.TC.CID !=0) printf("TC_CID      : true  ");
		else printf("TC_CID      : false ");
		if(RATS.TC.NAD !=0) printf(",TC_NAD      : true\n\r");
		else printf(",TC_NAD      : false\n\r");
		printf("--------------------------------------------------------------------\r\n");
		PSTATUS = I4_PPS;
		printf("PICC Sequence RATS to PPS in ISO14443-4\n\r");
	}
}

void PPS_I4()
{
	StatusCode status;
	uint8_t RxbitLen;
	uint8_t TransceiveBFF[FIFO_SIZE]={0,};

	TransceiveBFF[0] = 0xD0+(RATS.CID&0xf);//+CID;				// START BYTE(D0)+CID(0x00)
	TransceiveBFF[1] = 0X11;					// Parameter 0, bit 5 = 1 with Parameter1

	TransceiveBFF[2] = ((RATS.TA.DS<<2)&0x0C) + RATS.TA.DR;				// Parameter 1

	CalulateCRC(TransceiveBFF,3,&TransceiveBFF[3]);

	status = Comunicate_PCDPICC(PCD_CMD_Transceive,TransceiveBFF,5,TransceiveBFF,&RxbitLen);

	if(Error_write(status) != STATUS_OK) {
		PSTATUS = IDLE;
		return;
	}{
		if(TransceiveBFF[0]==0xD0 && RxbitLen==24){
			printf("PICC Sequence PPS to ACTIVE in ISO14443-4\n\r");
			PSTATUS = I4_ACTIVE;
			uint8_t txReg = Read_RFID(TxModeReg) & 0x8F;
			uint8_t rxReg = Read_RFID(RxModeReg) & 0x8F;

			// Set bit rate and enable CRC for T=CL
			txReg = txReg  | ((RATS.TA.DR & 0x03) << 4) | 0x80;
			rxReg = rxReg  | ((RATS.TA.DS & 0x03) << 4) | 0x80;

			Write_RFID(TxModeReg, txReg);
			Write_RFID(RxModeReg, rxReg);

			if(RATS.TA.DS>0){
				switch(RATS.TA.DS){
				case 1:		Write_RFID(ModWidthReg, 0x15);
				break;
				case 2:		Write_RFID(ModWidthReg, 0x0A);
				break;
				case 3:		Write_RFID(ModWidthReg, 0x05);
				break;
				default:	Write_RFID(ModWidthReg, 0x26); // Default value
				break;
				}
			}
		}
	}
}

void Deslelect_I4() //SBlock
{
	StatusCode status;
	uint8_t RxbitLen;
	uint8_t TransceiveBFF[FIFO_SIZE]={0,};
	uint8_t BFF_Len;

	if(RATS.TC.CID==1){
		TransceiveBFF[0] = CID_Set+ Deselect + SBlock ; //SBLOCK
		TransceiveBFF[1] = RATS.CID;					//CID + Power level indication??
		BFF_Len=2;
	}else{
		TransceiveBFF[0] = Deselect + SBlock ; //SBLOCK
		BFF_Len=1;
	}
	status =  Comunicate_PCDPICC(PCD_CMD_Transceive,TransceiveBFF,BFF_Len,TransceiveBFF,&RxbitLen);

	if(Error_write(status) != STATUS_OK) {
		PSTATUS = IDLE;
		printf("PICC Sequence Deselect to IDLE by error\n\r");
		return;
	}else{
		PSTATUS = IDLE;
		printf("PICC Sequence Deselect to IDLE\n\r");
	}
}

void IBLOCK_I4(uint8_t *pInData ,uint8_t InLenByte, uint8_t *pOutData)
{
	StatusCode status;
	uint8_t RxbitLen;
	uint8_t RxLen=0;
	uint8_t TransceiveBFF[FIFO_SIZE]={0,};
	uint8_t BFF_Len;
	uint8_t chain=0;
	uint8_t InLenTranseByte=0;
	uint8_t BlockN=Block_Num0;

	do{
		TransceiveBFF[0]= IBlock+BlockN;
		BFF_Len=1;

		if(RATS.TC.CID==1){
			TransceiveBFF[0] += CID_Set;
			TransceiveBFF[BFF_Len++] = RATS.CID;				//CID + Power level indication??
		}

		if(RATS.TC.NAD==1){
			TransceiveBFF[0] += NAD_Set;
			TransceiveBFF[BFF_Len++] = 0x77;					//unknown.
		}

		if(InLenByte>0){
			for(int i=0;i+InLenTranseByte<InLenByte;i++){
				if(BFF_Len==RATS.FSCI-2){						//INF Data lenth check, 62+ EDC(2bytes)
					if(i+InLenTranseByte==InLenByte) break;		//Data end check
					TransceiveBFF[0] += Chaning_Set;			//chain set
					InLenTranseByte+=i;
					chain=1;									//chainflg
					break;
				}
				TransceiveBFF[BFF_Len++]=pInData[i+InLenTranseByte];
				chain=0;
			}
		}

		CalulateCRC(TransceiveBFF,BFF_Len,&TransceiveBFF[BFF_Len]);
		if(chain==0)	printf("PICC Sequence IBlock Transmit Start\n\r");
		else			printf("PICC Sequence IBlock Chain Transmit Start\n\r");

		status =  Comunicate_PCDPICC(PCD_CMD_Transceive,TransceiveBFF,BFF_Len+2,TransceiveBFF,&RxbitLen);

		if(Error_write(status) != STATUS_OK) {
			PSTATUS = IDLE;
			printf("PICC Sequence IBlock_I4 to IDLE by error\n\r");
			return;
		}else{
			if(chain==0){
				if((TransceiveBFF[0]&IBlock)==IBlock && (TransceiveBFF[0]&BlockN)==BlockN){
					int Len=1;
					if((TransceiveBFF[0]&0x4)==0x04) Len++;
					if((TransceiveBFF[0]&0x8)==0x08) Len++;
					for(int i=0;i<(RxbitLen/8)-Len-2;i++){
						pOutData[RxLen+i]=TransceiveBFF[i+Len];
						RxLen++;
					}
					if(BlockN==Block_Num0) BlockN=Block_Num1;
					else BlockN=Block_Num0;
					printf("PICC Sequence IBlock End\n\r");
				}else{
					printf("needs add function\n\r");
				}
			}else{
				if((TransceiveBFF[0]&0xf0)==0xA0 && (TransceiveBFF[0]&BlockN)==BlockN){
					if(BlockN==Block_Num0) BlockN=Block_Num1;
					else BlockN=Block_Num0;
					printf("PICC Sequence RBlock Receive End\n\r");
				}else{
					printf("needs add function\n\r");
				}
			}
		}
	}while(chain>0);
}


void PICC_Sequence()
{
	StatusCode status;
	switch(PSTATUS){
	case IDLE:
		REQA_t();
		break;
	case READY:
		REQ_ID();
		break;
	case ACTIVE:
		Active();
		break;
	case READY2:
		break;
	case ACTIVE2:
		break;
	case I4_RATS:
		RATS_I4();
		break;
	case I4_PPS:
		PPS_I4();
		break;
	case I4_ACTIVE:
		//Active_I4();
		for(int i=0;i<128;i++)INFDATA[i]=i;
		IBLOCK_I4(INFDATA,64,INFDATA);
		Deslelect_I4();
		break;
	case HALT:
		status = HLTA_RFID();
		if(status == STATUS_OK)
		{
			printf("HALT Err\n\r");
		}
		else printf("HALT OK\n\r");

		StopCrypto1_RFID();
		PSTATUS=IDLE;
		printf("PICC Sequence HALT to IDLE\n\r");
		break;
	default:
		PSTATUS=IDLE;
		printf("PICC Sequence Err\n\r");
		break;
	}
}

StatusCode Error_write(StatusCode status)
{
	switch(status){
	case STATUS_OK:
		return status;
		break;
	case STATUS_ERROR:
		printf("Error in communication\n\r");
		break;
	case STATUS_COLLISION:
		printf("Collision detected\n\ r");
		break;
	case STATUS_TIMEOUT:
		if(PSTATUS!=IDLE) printf("Timeout in communication\n\r");
		break;
	case STATUS_NO_ROOM:
		printf("A buffer is not big enough.\n\r");
		break;
	case STATUS_INTERNAL_ERROR:
		printf("Internal error in the code. Should not happen\n\r");
		break;
	case STATUS_INVALID:
		printf("Invalid argument.\n\r");
		break;
	case STATUS_KEY_ERROR:
		printf("Error in KEY\n\r");
		break;
	case STATUS_BCC_WRONG:
		printf("The BCC does not match\n\r");
		break;
	case STATUS_CRC_WRONG:
		printf("The CRC_A does not match\n\r");
		break;
	case STATUS_CMD_WRONG:
		printf("Command does not match\n\r");
		break;
	case STATUS_MIFARE_NACK:
		printf("A MIFARE PICC responded with NAK.\n\r");
		break;
	default:
		break;
	}
	return status;
}

