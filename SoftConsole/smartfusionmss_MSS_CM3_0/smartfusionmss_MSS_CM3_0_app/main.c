#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "CarControl.h"
#include "xbee.h"
#include "MFRC522.h"

void loop();

int main() {
	XBEE_init();
	CarControl_init();
	PCD_init();

	while(1){
		loop();
	}
}

void loop() {
  unsigned char status, check, str[BUFFER_SIZE];
	char * tx_msg;
  // Anti-collision, return card serial number == 4 bytes
	status = MFRC522_Request(PICC_REQIDL, str);
  status = MFRC522_Anticoll(str);
  if (status == MI_OK) {

		//send new lap packet
		if ((str[0]-'0')%16 == 12){
			tx_msg = "1,1,0\r\n";
		}

		//send power up packet
		else if ((str[0]-'0')%16 == 14){
			tx_msg = "1,1,1\r\n";
		}
		XBEE_send(tx_msg);


		delay(1000);
  }
}
