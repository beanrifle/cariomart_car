#include "drivers/mss_spi/drivers/mss_spi/mss_spi.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "mfrc522.h"

const uint8_t frame_size = 16;

void GPIO2_IRQHandler( void ) {
	unsigned char status, check, str[BUFFER_SIZE];

	// Find cards
	status = MFRC522_Request(PICC_REQIDL, str);
	if (status == MI_OK) {
	  printf("Card detected!\t%x , %x\r\n", str[0], str[1]);
	}

	// Anti-collision, return card serial number == 4 bytes
	status = MFRC522_Anticoll(str);
	if (status == MI_OK) {
	  check = str[0] ^ str[1] ^ str[2] ^ str[3];

	  printf("Card uid: %x %x %x %x %x\t checksum: %x\r\n", str[0], str[1], str[2], str[3], str[4], check);
	}
	/* Clear the interrupt pin on the mfrc */
	//MFRC522_WriteRegister(ComIrqReg, 0x00);
	MFRC522_WriteRegister(ComIrqReg, 0x00);
	MFRC522_WriteRegister(DivIrqReg, 0x00);
	/* Clear the gpio interrupt */
	MSS_GPIO_clear_irq( MSS_GPIO_2 );
}


//4 bytes card serial number, the first 5 bytes for the checksum byte
unsigned char serNum[5];

void PCD_init() {

  /* Initialize SPI 1 */
  MSS_SPI_init( &g_mss_spi1 );
  MSS_SPI_configure_master_mode
  (
    &g_mss_spi1,
    MSS_SPI_SLAVE_0,
    MSS_SPI_MODE0,          // clock starts low, data read on clock rising edge, data changes on clock falling edge
    MSS_SPI_PCLK_DIV_256,
    frame_size
  );

  /* Initialize GPIO Pin 1 for setting the RST line */
  MSS_GPIO_init();
  MSS_GPIO_config( MSS_GPIO_1, MSS_GPIO_IRQ_LEVEL_HIGH );


  MSS_GPIO_set_output( MSS_GPIO_1, 1 );


  MFRC522_Init();
  NVIC_EnableIRQ(GPIO2_IRQn);
  MSS_GPIO_config( MSS_GPIO_2, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_BOTH );
  MSS_GPIO_enable_irq( MSS_GPIO_2 );
}


/*
 * Function Name：Write_MFRC5200
 * Function Description: To a certain MFRC522 register to write a byte of data
 * Input Parameters：addr - register address; val - the value to be written
 * Return value: None
 */
void MFRC522_WriteRegister(unsigned char reg, unsigned char val) {
  uint32_t rx_bits;


  MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

  //write the value to the specified register
  rx_bits = MSS_SPI_transfer_frame( &g_mss_spi1, (((reg) & 0x7E) << 8) |  val );


  MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

}


/*
 * Function Name： MFRC522_ReadRegister
 * Description: From a certain MFRC522 read a byte of data register
 * Input Parameters: addr - register address
 * Returns: a byte of data read from the MFRC522
 */
unsigned char MFRC522_ReadRegister(unsigned char reg) {
 uint32_t rx_bits;


  MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

  /* Transfer the address and read from the register */
  rx_bits = MSS_SPI_transfer_frame( &g_mss_spi1, ((((reg ) & 0x7E) | 0x80) << 8) | 0x00 );


  MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );


	return (unsigned char) rx_bits;
}

/*
 * Function Name：SetBitMask
 * Description: Set RC522 register bit
 * Input parameters: reg - register address; mask - set value
 * Return value: None
 */
void SetBitMask(unsigned char reg, unsigned char mask)
{
    unsigned char tmp;
    tmp =  MFRC522_ReadRegister(reg);
     MFRC522_WriteRegister(reg, tmp | mask);  // set bit mask
}


/*
 * Function Name: ClearBitMask
 * Description: clear RC522 register bit
 * Input parameters: reg - register address; mask - clear bit value
 * Return value: None
*/
void ClearBitMask(unsigned char reg, unsigned char mask)
{
    unsigned char tmp;
    tmp =  MFRC522_ReadRegister(reg);
     MFRC522_WriteRegister(reg, tmp & (~mask));  // clear bit mask
}


/*
 * Function Name：AntennaOn
 * Description: Open antennas, each time you start or shut down the natural barrier between the transmitter should be at least 1ms interval
 * Input: None
 * Return value: None
 */
void AntennaOn(void)
{
  SetBitMask(TxControlReg, 0x03);
}


/*
  * Function Name: AntennaOff
  * Description: Close antennas, each time you start or shut down the natural barrier between the transmitter should be at least 1ms interval
  * Input: None
  * Return value: None
 */
void AntennaOff(void)
{
  ClearBitMask(TxControlReg, 0x03);
}


/*
 * Function Name: ResetMFRC522
 * Description: Reset RC522
 * Input: None
 * Return value: None
 */
void MFRC522_Reset(void)
{
   MFRC522_WriteRegister(CommandReg, PCD_SoftReset);
}


/*
 * Function Name：InitMFRC522
 * Description: Initialize RC522
 * Input: None
 * Return value: None
*/
void MFRC522_Init(void)
{
    MSS_GPIO_set_output( MSS_GPIO_1, 1 );
    MFRC522_Reset();


    MFRC522_WriteRegister(TModeReg, 0x8D);      // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    MFRC522_WriteRegister(TPrescalerReg, 0x3E); // TModeReg[3..0] + TPrescalerReg
  	MFRC522_WriteRegister(TReloadRegL, 30);
  	MFRC522_WriteRegister(TReloadRegH, 0);
  	MFRC522_WriteRegister(TxASKReg, 0x40);     // force 100% ASK modulation
  	MFRC522_WriteRegister(ModeReg, 0x3D);       // CRC Initial value 0x6363

  	// interrupt enable'
  	MFRC522_WriteRegister(ComIEnReg, 0x84);
  	//MFRC522_WriteRegister(ComIEnReg, 0xA0);

  	// turn antenna on
  	AntennaOn();
}


/*
 * Function Name：MFRC522_Request
 * Description: Find cards, read the card type number
 * Input parameters: reqMode - find cards way
 *   TagType - Return Card Type
 *    0x4400 = Mifare_UltraLight
 *    0x0400 = Mifare_One(S50)
 *    0x0200 = Mifare_One(S70)
 *    0x0800 = Mifare_Pro(X)
 *    0x4403 = Mifare_DESFire
 * Return value: the successful return MI_OK
 */
unsigned char MFRC522_Request(unsigned char reqMode, unsigned char *TagType)
{
  unsigned char status;
  unsigned int backBits; // The received data bits

   MFRC522_WriteRegister(BitFramingReg, 0x07);   // TxLastBists = BitFramingReg[2..0]

  TagType[0] = reqMode;

  status = MFRC522_ToCard(PCD_Transceive, TagType, 1, TagType, &backBits);
  if ((status != MI_OK) || (backBits != 0x10)) {
    status = MI_ERR;
  }

  return status;
}


/*
 * Function Name: MFRC522_ToCard
 * Description: RC522 and ISO14443 card communication
 * Input Parameters: command - MF522 command word,
 *			 sendData--RC522 sent to the card by the data
 *			 sendLen--Length of data sent
 *			 backData--Received the card returns data,
 *			 backLen--Return data bit length
 * Return value: the successful return MI_OK
 */
unsigned char MFRC522_ToCard(unsigned char command, unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned int *backLen)
{
  unsigned char status = MI_ERR;
  unsigned char irqEn = 0x00;
  unsigned char waitIRq = 0x00;
  unsigned char lastBits;
  unsigned char n;
  unsigned int i;

  switch (command)
  {
    case PCD_MFAuthent:     // Certification cards close
      {
        irqEn = 0x12;
        waitIRq = 0x10;
        break;
      }
    case PCD_Transceive:  // Transmit FIFO data
      {
        irqEn = 0x77;
        waitIRq = 0x30;
        break;
      }
    default:
      break;
  }

   MFRC522_WriteRegister(ComIEnReg, irqEn|0x80);  // Interrupt request
  ClearBitMask(ComIrqReg, 0x80);         // Clear all interrupt request bit
  SetBitMask(FIFOLevelReg, 0x80);         // FlushBuffer=1, FIFO Initialization

   MFRC522_WriteRegister(CommandReg, PCD_Idle);    // NO action; Cancel the current command

  // Writing data to the FIFO
  for (i=0; i<sendLen; i++)
  {
     MFRC522_WriteRegister(FIFODataReg, sendData[i]);
  }

  // Execute the command
   MFRC522_WriteRegister(CommandReg, command);
  if (command == PCD_Transceive)
  {
    SetBitMask(BitFramingReg, 0x80);      // StartSend=1,transmission of data starts
  }

  // Waiting to receive data to complete
  i = 2000;	// i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
  do
  {
    // CommIrqReg[7..0]
    // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n =  MFRC522_ReadRegister(ComIrqReg);
    i--;
  }
  while ((i!=0) && !(n&0x01) && !(n&waitIRq));

  ClearBitMask(BitFramingReg, 0x80);      // StartSend=0

  if (i != 0)
  {
    if(!( MFRC522_ReadRegister(ErrorReg) & 0x1B))  // BufferOvfl Collerr CRCErr ProtecolErr
    {
      status = MI_OK;
      if (n & irqEn & 0x01)
      {
        status = MI_NOTAGERR;             // ??
      }

      if (command == PCD_Transceive)
      {
        n =  MFRC522_ReadRegister(FIFOLevelReg);
        lastBits =  MFRC522_ReadRegister(ControlReg) & 0x07;
        if (lastBits)
        {
          *backLen = (n-1)*8 + lastBits;
        }
        else
        {
          *backLen = n*8;
        }

        if (n == 0)
        {
          n = 1;
        }
        if (n > BUFFER_SIZE)
        {
          n = BUFFER_SIZE;
        }

        // Reading the received data in FIFO
        for (i=0; i<n; i++)
        {
          backData[i] =  MFRC522_ReadRegister(FIFODataReg);
        }
      }
    }
    else {
      //printf("buffer overflow, collerr, crcerr, or protecolerr\r\n");
      status = MI_ERR;
    }
  }
  else {
    printf("timed out\r\n");
  }

  return status;
}


/*
 * Function Name: MFRC522_Anticoll
 * Description: Anti-collision detection, reading selected card serial number card
 * Input parameters: serNum - returns 4 bytes card serial number, the first 5 bytes for the checksum byte
 * Return value: the successful return MI_OK
 */
unsigned char MFRC522_Anticoll(unsigned char *serNum)
{
  unsigned char status;
  unsigned char i;
  unsigned char serNumCheck=0;
  unsigned int unLen;


  //ClearBitMask(Status2Reg, 0x08);		//TempSensclear
  //ClearBitMask(CollReg,0x80);			//ValuesAfterColl
   MFRC522_WriteRegister(BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]

  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  status = MFRC522_ToCard(PCD_Transceive, serNum, 2, serNum, &unLen);

  if (status == MI_OK)
  {
    //Check card serial number
    for (i=0; i<4; i++)
    {
      serNumCheck ^= serNum[i];
    }
    if (serNumCheck != serNum[i])
    {
      status = MI_ERR;
    }
  }

  //SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1

  return status;
}


/*
 * Function Name: CalulateCRC
 * Description: CRC calculation with MF522
 * Input parameters: pIndata - To read the CRC data, len - the data length, pOutData - CRC calculation results
 * Return value: None
 */
void CalulateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
  unsigned char i, n;

  ClearBitMask(DivIrqReg, 0x04);			//CRCIrq = 0
  SetBitMask(FIFOLevelReg, 0x80);			//Clear the FIFO pointer
  // MFRC522_WriteRegister(CommandReg, PCD_Idle);

  //Writing data to the FIFO
  for (i=0; i<len; i++)
  {
     MFRC522_WriteRegister(FIFODataReg, *(pIndata+i));
  }
   MFRC522_WriteRegister(CommandReg, PCD_CalcCRC);

  //Wait CRC calculation is complete
  i = 0xFF;
  do
  {
    n =  MFRC522_ReadRegister(DivIrqReg);
    i--;
  }
  while ((i!=0) && !(n&0x04));			//CRCIrq = 1

  //Read CRC calculation result
  pOutData[0] =  MFRC522_ReadRegister(CRCResultRegL);
  pOutData[1] =  MFRC522_ReadRegister(CRCResultRegH);
}


/*
 * Function Name: MFRC522_SelectTag
 * Description: election card, read the card memory capacity
 * Input parameters: serNum - Incoming card serial number
 * Return value: the successful return of card capacity
 */
unsigned char MFRC522_SelectTag(unsigned char *serNum)
{
  unsigned char i;
  unsigned char status;
  unsigned char size;
  unsigned int recvBits;
  unsigned char buffer[9];

  //ClearBitMask(Status2Reg, 0x08);			//MFCrypto1On=0

  buffer[0] = PICC_SElECTTAG;
  buffer[1] = 0x70;
  for (i=0; i<5; i++)
  {
    buffer[i+2] = *(serNum+i);
  }
  CalulateCRC(buffer, 7, &buffer[7]);		//??
  status = MFRC522_ToCard(PCD_Transceive, buffer, 9, buffer, &recvBits);

  if ((status == MI_OK) && (recvBits == 0x18))
  {
    size = buffer[0];
  }
  else
  {
    size = 0;
  }

  return size;
}


/*
 * Function Name: MFRC522_Auth
 * Description: Verify card password
 * Input parameters: authMode - Password Authentication Mode
                 0x60 = A key authentication
                 0x61 = Authentication Key B
             BlockAddr--Block address
             Sectorkey--Sector password
             serNum--Card serial number, 4-byte
 * Return value: the successful return MI_OK
 */
unsigned char MFRC522_Auth(unsigned char authMode, unsigned char BlockAddr, unsigned char *Sectorkey, unsigned char *serNum)
{
  unsigned char status;
  unsigned int recvBits;
  unsigned char i;
  unsigned char buff[12];

  //Verify the command block address + sector + password + card serial number
  buff[0] = authMode;
  buff[1] = BlockAddr;
  for (i=0; i<6; i++)
  {
    buff[i+2] = *(Sectorkey+i);
  }
  for (i=0; i<4; i++)
  {
    buff[i+8] = *(serNum+i);
  }
  status = MFRC522_ToCard(PCD_MFAuthent, buff, 12, buff, &recvBits);

  if ((status != MI_OK) || (!( MFRC522_ReadRegister(Status2Reg) & 0x08)))
  {
    status = MI_ERR;
  }

  return status;
}


/*
 * Function Name: MFRC522_Read
 * Description: Read block data
 * Input parameters: blockAddr - block address; recvData - read block data
 * Return value: the successful return MI_OK
 */
unsigned char MFRC522_Read(unsigned char blockAddr, unsigned char *recvData)
{
  unsigned char status;
  unsigned int unLen;

  recvData[0] = PICC_READ;
  recvData[1] = blockAddr;
  CalulateCRC(recvData,2, &recvData[2]);
  status = MFRC522_ToCard(PCD_Transceive, recvData, 4, recvData, &unLen);

  if ((status != MI_OK) || (unLen != 0x90))
  {
    status = MI_ERR;
  }

  return status;
}


/*
 * Function Name: MFRC522_Write
 * Description: Write block data
 * Input parameters: blockAddr - block address; writeData - to 16-byte data block write
 * Return value: the successful return MI_OK
 */
unsigned char MFRC522_Write(unsigned char blockAddr, unsigned char *writeData)
{
  unsigned char status;
  unsigned int recvBits;
  unsigned char i;
  unsigned char buff[18];

  buff[0] = PICC_WRITE;
  buff[1] = blockAddr;
  CalulateCRC(buff, 2, &buff[2]);
  status = MFRC522_ToCard(PCD_Transceive, buff, 4, buff, &recvBits);

  if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
  {
    status = MI_ERR;
  }

  if (status == MI_OK)
  {
    for (i=0; i<16; i++)		//Data to the FIFO write 16Byte
    {
      buff[i] = *(writeData+i);
    }
    CalulateCRC(buff, 16, &buff[16]);
    status = MFRC522_ToCard(PCD_Transceive, buff, 18, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {
      status = MI_ERR;
    }
  }

  return status;
}


/*
 * Function Name: MFRC522_Halt
 * Description: Command card into hibernation
 * Input: None
 * Return value: None
 */
void MFRC522_Halt(void)
{
  unsigned char status;
  unsigned int unLen;
  unsigned char buff[4];

  buff[0] = PICC_HALT;
  buff[1] = 0;
  CalulateCRC(buff, 2, &buff[2]);

  status = MFRC522_ToCard(PCD_Transceive, buff, 4, buff,&unLen);
}
