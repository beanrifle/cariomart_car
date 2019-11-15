///////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: hbridgecontroller.v
//
// Description: 
//
// Controls the H-Bridge to drive the motors on the cars for Cario Mart.
//
// Targeted device: <Family::SmartFusion> <Die::A2F200M3F> <Package::484 FBGA>
// Author: Benjamin Young
//
/////////////////////////////////////////////////////////////////////////////////////////////////// 

/* 

APB3 Interface:

Address 0x40050000  |   W/R |   Motor 1 Control Register    |   Write a 0x0 to set direction of motor 1 forward and 0x1 to set direction reverse
Address 0x40050004  |   W/R |   Motor 2 Control Register    |   Write a 0x0 to set direction of motor 2 forward and 0x1 to set direction reverse
Address 0x40050008  |   W/R |   Motor 1 Speed Control       |   Write a value between 0 and 255 to control the speed of motor 1
Address 0x4005000C  |   W/R |   Motor 2 Speed Control       |   Write a value between 0 and 255 to control the speed of motor 2

*/

//`timescale <time_units> / <precision>

module hbridgecontroller(
/*** APB3 BUS INTERFACE ***/
input PCLK, 				// clock
input PRESERN, 				// system reset
input PSEL, 				// peripheral select
input PENABLE, 				// distinguishes access phase
output wire PREADY, 		// peripheral ready signal
output wire PSLVERR,		// error signal
input PWRITE,				// distinguishes read and write cycles
input [31:0] PADDR,			// I/O address
input wire [31:0] PWDATA,	// data from processor to I/O device (32 bits)
output reg [31:0] PRDATA,	// data to processor from I/O device (32-bits)

/*** I/O PORTS DECLARATION ***/	
output reg motor1_pwm,
output reg motor2_pwm,
output reg [1:0] motor1_ctrl,
output reg [1:0] motor2_ctrl
); 

assign PSLVERR = 0;
assign PREADY = 1;

wire write_enable = (PENABLE && PWRITE && PSEL);
wire read_enable = (!PWRITE && PSEL); 

wire M1CTRL_WRITE = write_enable && (PADDR[15:0] == 16'h0000);
wire M1CTRL_READ = read_enable && (PADDR[15:0] == 16'h0000);
wire M2CTRL_WRITE = write_enable && (PADDR[15:0] == 16'h0004);
wire M2CTRL_READ = read_enable && (PADDR[15:0] == 16'h0004);
wire M1SPEED_WRITE = write_enable && (PADDR[15:0] == 16'h0008);
wire M1SPEED_READ = read_enable && (PADDR[15:0] == 16'h0008);
wire M2SPEED_WRITE = write_enable && (PADDR[15:0] == 16'h000c);
wire M2SPEED_READ = read_enable && (PADDR[15:0] == 16'h000c);

//////////////////////////////////
// end of APB3 interface header //
//////////////////////////////////

`define DIRECTION_FORWARD 2'b10
`define DIRECTION_REVERSE 2'b01

reg [31:0] counter;
parameter PWM_PERIOD = 200000;  // Default PWM period of 2ms, same as Arduino
reg [31:0] M1_duty, M2_duty;

// PWM Control
always @(posedge PCLK)
begin
    // Counter control
    if (counter >= PWM_PERIOD) counter = 0;
    else counter = counter + 1;

    // Motor 1 PWM control
    if (counter < M1_duty && M1_duty > (PWM_PERIOD >> 4)) motor1_pwm <= 1;
    else motor1_pwm <= 0;

    // Motor 2 PWM control
    if (counter < M2_duty && M2_duty > (PWM_PERIOD >> 4)) motor2_pwm <= 1;
    else motor2_pwm <= 0;
end


// APB3 Interface
always @(posedge PCLK)
begin
	if(!PRESERN)
	begin
        M1_duty = 0;
        M2_duty = 0;
        motor1_ctrl = `DIRECTION_FORWARD;
        motor2_ctrl = `DIRECTION_FORWARD;
    end

    if (M1CTRL_WRITE)
    begin
        if (PWDATA != 0) motor1_ctrl = `DIRECTION_REVERSE;
        else motor1_ctrl = `DIRECTION_FORWARD;
    end
    
    else if (M1CTRL_READ)
    begin
        if (motor1_ctrl == `DIRECTION_FORWARD) PRDATA = 1;
        else PRDATA = 0;
    end

    if (M2CTRL_WRITE)
    begin
        if (PWDATA != 0) motor2_ctrl = `DIRECTION_REVERSE;
        else motor2_ctrl = `DIRECTION_FORWARD;
    end
    
    else if (M2CTRL_READ)
    begin
        if (motor2_ctrl == `DIRECTION_FORWARD) PRDATA = 1;
        else PRDATA = 0;
    end

    else if (M1SPEED_WRITE) 
    begin
        M1_duty = (PWM_PERIOD * PWDATA[15:0]) >> 8;
        if (M1_duty > PWM_PERIOD) M1_duty = PWM_PERIOD;
    end

    else if (M1SPEED_READ) PRDATA = ( M1_duty / PWM_PERIOD );

    else if (M2SPEED_WRITE) 
    begin
        M2_duty = (PWM_PERIOD * PWDATA[15:0]) >> 8;
        if (M2_duty > PWM_PERIOD) M2_duty = PWM_PERIOD;
    end

    else if (M2SPEED_READ) PRDATA = ( M2_duty / PWM_PERIOD );
end

endmodule

