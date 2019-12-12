#include "drivers/mss_spi/drivers/mss_spi/mss_spi.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "mfrc522.h"

const uint8_t frame_size = 16;

unsigned char serNum[5];

void PCD_init() {

  /* Initialize SPI 1 */
  MSS_SPI_init( &g_mss_spi1 );
  MSS_SPI_configure_master_mode
  (
    &g_mss_spi1,
    MSS_SPI_SLAVE_0,
    MSS_SPI_MODE0,          
    MSS_SPI_PCLK_DIV_256,
    frame_size
  );

  /* Set reset line high */
  MSS_GPIO_init();
  MSS_GPIO_config( MSS_GPIO_1, MSS_GPIO_IRQ_LEVEL_HIGH );


  MSS_GPIO_set_output( MSS_GPIO_1, 1 );


  MFRC522_Init();
  NVIC_EnableIRQ(GPIO2_IRQn);
  MSS_GPIO_config( MSS_GPIO_2, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_BOTH );
  MSS_GPIO_enable_irq( MSS_GPIO_2 );
}



void MFRC522_WriteRegister(unsigned char reg, unsigned char val) {
  uint32_t rx_bits;


  MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

  //write the value to the specified register
  rx_bits = MSS_SPI_transfer_frame( &g_mss_spi1, (((reg) & 0x7E) << 8) |  val );


  MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

}



unsigned char MFRC522_ReadRegister(unsigned char reg) {
 uint32_t rx_bits;


  MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

  /* Transfer the address and read from the register */
  rx_bits = MSS_SPI_transfer_frame( &g_mss_spi1, ((((reg ) & 0x7E) | 0x80) << 8) | 0x00 );


  MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );


	return (unsigned char) rx_bits;
}


void SetBitMask(unsigned char reg, unsigned char mask)
{
    unsigned char tmp;
    tmp =  MFRC522_ReadRegister(reg);
     MFRC522_WriteRegister(reg, tmp | mask);  // set bit mask
}


void ClearBitMask(unsigned char reg, unsigned char mask)
{
    unsigned char tmp;
    tmp =  MFRC522_ReadRegister(reg);
     MFRC522_WriteRegister(reg, tmp & (~mask));  // clear bit mask
}



void AntennaOn(void)
{
  SetBitMask(TxControlReg, 0x03);
}


void AntennaOff(void)
{
  ClearBitMask(TxControlReg, 0x03);
}


void MFRC522_Reset(void)
{
   MFRC522_WriteRegister(CommandReg, PCD_SoftReset);
}


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


unsigned char MFRC522_Request(unsigned char reqMode, unsigned char *TagType)
{
  unsigned char status;
  unsigned int backBits; 

   MFRC522_WriteRegister(BitFramingReg, 0x07);  

  TagType[0] = reqMode;

  status = MFRC522_ToCard(PCD_Transceive, TagType, 1, TagType, &backBits);
  if ((status != MI_OK) || (backBits != 0x10)) {
    status = MI_ERR;
  }

  return status;
}


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
    case PCD_MFAuthent:    
      {
        irqEn = 0x12;
        waitIRq = 0x10;
        break;
      }
    case PCD_Transceive:  
      {
        irqEn = 0x77;
        waitIRq = 0x30;
        break;
      }
    default:
      break;
  }

   MFRC522_WriteRegister(ComIEnReg, irqEn|0x80);  /
  ClearBitMask(ComIrqReg, 0x80);        
  SetBitMask(FIFOLevelReg, 0x80);       

   MFRC522_WriteRegister(CommandReg, PCD_Idle);  


  for (i=0; i<sendLen; i++)
  {
     MFRC522_WriteRegister(FIFODataReg, sendData[i]);
  }

   MFRC522_WriteRegister(CommandReg, command);
  if (command == PCD_Transceive)
  {
    SetBitMask(BitFramingReg, 0x80); 
  }

 
  i = 2000;	
  do
  {

    n =  MFRC522_ReadRegister(ComIrqReg);
    i--;
  }
  while ((i!=0) && !(n&0x01) && !(n&waitIRq));

  ClearBitMask(BitFramingReg, 0x80);     

  if (i != 0)
  {
    if(!( MFRC522_ReadRegister(ErrorReg) & 0x1B)) 
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

        for (i=0; i<n; i++)
        {
          backData[i] =  MFRC522_ReadRegister(FIFODataReg);
        }
      }
    }
    else {

      status = MI_ERR;
    }
  }
  else {
    printf("timed out\r\n");
  }

  return status;
}

unsigned char MFRC522_Anticoll(unsigned char *serNum)
{
  unsigned char status;
  unsigned char i;
  unsigned char serNumCheck=0;
  unsigned int unLen;


  ClearBitMask(Status2Reg, 0x08);		
  ClearBitMask(CollReg,0x80);		
   MFRC522_WriteRegister(BitFramingReg, 0x00);

  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  status = MFRC522_ToCard(PCD_Transceive, serNum, 2, serNum, &unLen);

  if (status == MI_OK)
  {
    //Check serial number
    for (i=0; i<4; i++)
    {
      serNumCheck ^= serNum[i];
    }
    if (serNumCheck != serNum[i])
    {
      status = MI_ERR;
    }
  }

  SetBitMask(CollReg, 0x80);	

  return status;
}



void CalulateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
  unsigned char i, n;

  ClearBitMask(DivIrqReg, 0x04);		
  SetBitMask(FIFOLevelReg, 0x80);		
   MFRC522_WriteRegister(CommandReg, PCD_Idle);


  for (i=0; i<len; i++)
  {
     MFRC522_WriteRegister(FIFODataReg, *(pIndata+i));
  }
   MFRC522_WriteRegister(CommandReg, PCD_CalcCRC);

  i = 0xFF;
  do
  {
    n =  MFRC522_ReadRegister(DivIrqReg);
    i--;
  }
  while ((i!=0) && !(n&0x04));			

  //Read CRC 
  pOutData[0] =  MFRC522_ReadRegister(CRCResultRegL);
  pOutData[1] =  MFRC522_ReadRegister(CRCResultRegH);
}



unsigned char MFRC522_SelectTag(unsigned char *serNum)
{
  unsigned char i;
  unsigned char status;
  unsigned char size;
  unsigned int recvBits;
  unsigned char buffer[9];

  ClearBitMask(Status2Reg, 0x08);	

  buffer[0] = PICC_SElECTTAG;
  buffer[1] = 0x70;
  for (i=0; i<5; i++)
  {
    buffer[i+2] = *(serNum+i);
  }
  CalulateCRC(buffer, 7, &buffer[7]);		
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



unsigned char MFRC522_Auth(unsigned char authMode, unsigned char BlockAddr, unsigned char *Sectorkey, unsigned char *serNum)
{
  unsigned char status;
  unsigned int recvBits;
  unsigned char i;
  unsigned char buff[12];


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
