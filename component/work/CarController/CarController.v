//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Tue Dec 10 00:12:44 2019
// Version: v11.9 11.9.0.4
//////////////////////////////////////////////////////////////////////

`timescale 1ns / 100ps

// CarController
module CarController(
    // Inputs
    MSS_RESET_N,
    SPI_1_DI,
    UART_0_RXD,
    UART_1_RXD,
    // Outputs
    SPI_1_DO,
    SPI_1_FAB_SS,
    UART_0_TXD,
    UART_1_TXD,
    motor1_ctrl,
    motor1_pwm,
    motor2_ctrl,
    motor2_pwm,
    // Inouts
    SPI_1_CLK,
    SPI_1_SS
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input        MSS_RESET_N;
input        SPI_1_DI;
input        UART_0_RXD;
input        UART_1_RXD;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output       SPI_1_DO;
output [1:1] SPI_1_FAB_SS;
output       UART_0_TXD;
output       UART_1_TXD;
output [1:0] motor1_ctrl;
output       motor1_pwm;
output [1:0] motor2_ctrl;
output       motor2_pwm;
//--------------------------------------------------------------------
// Inout
//--------------------------------------------------------------------
inout        SPI_1_CLK;
inout        SPI_1_SS;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire   [31:0] CoreAPB3_0_APBmslave0_PADDR;
wire          CoreAPB3_0_APBmslave0_PENABLE;
wire   [31:0] CoreAPB3_0_APBmslave0_PRDATA;
wire          CoreAPB3_0_APBmslave0_PREADY;
wire          CoreAPB3_0_APBmslave0_PSELx;
wire          CoreAPB3_0_APBmslave0_PSLVERR;
wire   [31:0] CoreAPB3_0_APBmslave0_PWDATA;
wire          CoreAPB3_0_APBmslave0_PWRITE;
wire   [1:0]  motor1_ctrl_net_0;
wire          motor1_pwm_net_0;
wire   [1:0]  motor2_ctrl_net_0;
wire          motor2_pwm_net_0;
wire          MSS_RESET_N;
wire          smartfusionmss_0_FAB_CLK;
wire          smartfusionmss_0_M2F_RESET_N;
wire          smartfusionmss_0_MSS_MASTER_APB_PENABLE;
wire   [31:0] smartfusionmss_0_MSS_MASTER_APB_PRDATA;
wire          smartfusionmss_0_MSS_MASTER_APB_PREADY;
wire          smartfusionmss_0_MSS_MASTER_APB_PSELx;
wire          smartfusionmss_0_MSS_MASTER_APB_PSLVERR;
wire   [31:0] smartfusionmss_0_MSS_MASTER_APB_PWDATA;
wire          smartfusionmss_0_MSS_MASTER_APB_PWRITE;
wire          SPI_1_CLK;
wire          SPI_1_DI;
wire          SPI_1_DO_net_0;
wire   [1:1]  SPI_1_FAB_SS_net_0;
wire          SPI_1_SS;
wire          UART_0_RXD;
wire          UART_0_TXD_1;
wire          UART_1_RXD;
wire          UART_1_TXD_net_0;
wire          motor1_pwm_net_1;
wire          motor2_pwm_net_1;
wire          UART_1_TXD_net_1;
wire          UART_0_TXD_1_net_0;
wire   [1:0]  motor2_ctrl_net_1;
wire   [1:0]  motor1_ctrl_net_1;
wire          SPI_1_DO_net_1;
wire   [1:1]  SPI_1_FAB_SS_net_1;
//--------------------------------------------------------------------
// TiedOff Nets
//--------------------------------------------------------------------
wire          GND_net;
wire          VCC_net;
wire   [31:0] IADDR_const_net_0;
wire   [31:0] PRDATAS1_const_net_0;
wire   [31:0] PRDATAS2_const_net_0;
wire   [31:0] PRDATAS3_const_net_0;
wire   [31:0] PRDATAS4_const_net_0;
wire   [31:0] PRDATAS5_const_net_0;
wire   [31:0] PRDATAS6_const_net_0;
wire   [31:0] PRDATAS7_const_net_0;
wire   [31:0] PRDATAS8_const_net_0;
wire   [31:0] PRDATAS9_const_net_0;
wire   [31:0] PRDATAS10_const_net_0;
wire   [31:0] PRDATAS11_const_net_0;
wire   [31:0] PRDATAS12_const_net_0;
wire   [31:0] PRDATAS13_const_net_0;
wire   [31:0] PRDATAS14_const_net_0;
wire   [31:0] PRDATAS15_const_net_0;
wire   [31:0] PRDATAS16_const_net_0;
//--------------------------------------------------------------------
// Bus Interface Nets Declarations - Unequal Pin Widths
//--------------------------------------------------------------------
wire   [19:0] smartfusionmss_0_MSS_MASTER_APB_PADDR;
wire   [31:20]smartfusionmss_0_MSS_MASTER_APB_PADDR_0_31to20;
wire   [19:0] smartfusionmss_0_MSS_MASTER_APB_PADDR_0_19to0;
wire   [31:0] smartfusionmss_0_MSS_MASTER_APB_PADDR_0;
//--------------------------------------------------------------------
// Constant assignments
//--------------------------------------------------------------------
assign GND_net               = 1'b0;
assign VCC_net               = 1'b1;
assign IADDR_const_net_0     = 32'h00000000;
assign PRDATAS1_const_net_0  = 32'h00000000;
assign PRDATAS2_const_net_0  = 32'h00000000;
assign PRDATAS3_const_net_0  = 32'h00000000;
assign PRDATAS4_const_net_0  = 32'h00000000;
assign PRDATAS5_const_net_0  = 32'h00000000;
assign PRDATAS6_const_net_0  = 32'h00000000;
assign PRDATAS7_const_net_0  = 32'h00000000;
assign PRDATAS8_const_net_0  = 32'h00000000;
assign PRDATAS9_const_net_0  = 32'h00000000;
assign PRDATAS10_const_net_0 = 32'h00000000;
assign PRDATAS11_const_net_0 = 32'h00000000;
assign PRDATAS12_const_net_0 = 32'h00000000;
assign PRDATAS13_const_net_0 = 32'h00000000;
assign PRDATAS14_const_net_0 = 32'h00000000;
assign PRDATAS15_const_net_0 = 32'h00000000;
assign PRDATAS16_const_net_0 = 32'h00000000;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign motor1_pwm_net_1      = motor1_pwm_net_0;
assign motor1_pwm            = motor1_pwm_net_1;
assign motor2_pwm_net_1      = motor2_pwm_net_0;
assign motor2_pwm            = motor2_pwm_net_1;
assign UART_1_TXD_net_1      = UART_1_TXD_net_0;
assign UART_1_TXD            = UART_1_TXD_net_1;
assign UART_0_TXD_1_net_0    = UART_0_TXD_1;
assign UART_0_TXD            = UART_0_TXD_1_net_0;
assign motor2_ctrl_net_1     = motor2_ctrl_net_0;
assign motor2_ctrl[1:0]      = motor2_ctrl_net_1;
assign motor1_ctrl_net_1     = motor1_ctrl_net_0;
assign motor1_ctrl[1:0]      = motor1_ctrl_net_1;
assign SPI_1_DO_net_1        = SPI_1_DO_net_0;
assign SPI_1_DO              = SPI_1_DO_net_1;
assign SPI_1_FAB_SS_net_1[1] = SPI_1_FAB_SS_net_0[1];
assign SPI_1_FAB_SS[1:1]     = SPI_1_FAB_SS_net_1[1];
//--------------------------------------------------------------------
// Bus Interface Nets Assignments - Unequal Pin Widths
//--------------------------------------------------------------------
assign smartfusionmss_0_MSS_MASTER_APB_PADDR_0_31to20 = 12'h0;
assign smartfusionmss_0_MSS_MASTER_APB_PADDR_0_19to0 = smartfusionmss_0_MSS_MASTER_APB_PADDR[19:0];
assign smartfusionmss_0_MSS_MASTER_APB_PADDR_0 = { smartfusionmss_0_MSS_MASTER_APB_PADDR_0_31to20, smartfusionmss_0_MSS_MASTER_APB_PADDR_0_19to0 };

//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------CoreAPB3   -   Actel:DirectCore:CoreAPB3:4.1.100
CoreAPB3 #( 
        .APB_DWIDTH      ( 32 ),
        .APBSLOT0ENABLE  ( 1 ),
        .APBSLOT1ENABLE  ( 0 ),
        .APBSLOT2ENABLE  ( 0 ),
        .APBSLOT3ENABLE  ( 0 ),
        .APBSLOT4ENABLE  ( 0 ),
        .APBSLOT5ENABLE  ( 0 ),
        .APBSLOT6ENABLE  ( 0 ),
        .APBSLOT7ENABLE  ( 0 ),
        .APBSLOT8ENABLE  ( 0 ),
        .APBSLOT9ENABLE  ( 0 ),
        .APBSLOT10ENABLE ( 0 ),
        .APBSLOT11ENABLE ( 0 ),
        .APBSLOT12ENABLE ( 0 ),
        .APBSLOT13ENABLE ( 0 ),
        .APBSLOT14ENABLE ( 0 ),
        .APBSLOT15ENABLE ( 0 ),
        .FAMILY          ( 18 ),
        .IADDR_OPTION    ( 0 ),
        .MADDR_BITS      ( 12 ),
        .SC_0            ( 0 ),
        .SC_1            ( 0 ),
        .SC_2            ( 0 ),
        .SC_3            ( 0 ),
        .SC_4            ( 0 ),
        .SC_5            ( 0 ),
        .SC_6            ( 0 ),
        .SC_7            ( 0 ),
        .SC_8            ( 0 ),
        .SC_9            ( 0 ),
        .SC_10           ( 0 ),
        .SC_11           ( 0 ),
        .SC_12           ( 0 ),
        .SC_13           ( 0 ),
        .SC_14           ( 0 ),
        .SC_15           ( 0 ),
        .UPR_NIBBLE_POSN ( 2 ) )
CoreAPB3_0(
        // Inputs
        .PRESETN    ( GND_net ), // tied to 1'b0 from definition
        .PCLK       ( GND_net ), // tied to 1'b0 from definition
        .PWRITE     ( smartfusionmss_0_MSS_MASTER_APB_PWRITE ),
        .PENABLE    ( smartfusionmss_0_MSS_MASTER_APB_PENABLE ),
        .PSEL       ( smartfusionmss_0_MSS_MASTER_APB_PSELx ),
        .PREADYS0   ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERRS0  ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .PREADYS1   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS1  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS2   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS2  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS3   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS3  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS4   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS4  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS5   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS5  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS6   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS6  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS7   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS7  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS8   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS8  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS9   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS9  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS10  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS10 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS11  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS11 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS12  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS12 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS13  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS13 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS14  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS14 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS15  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS15 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS16  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS16 ( GND_net ), // tied to 1'b0 from definition
        .PADDR      ( smartfusionmss_0_MSS_MASTER_APB_PADDR_0 ),
        .PWDATA     ( smartfusionmss_0_MSS_MASTER_APB_PWDATA ),
        .PRDATAS0   ( CoreAPB3_0_APBmslave0_PRDATA ),
        .PRDATAS1   ( PRDATAS1_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS2   ( PRDATAS2_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS3   ( PRDATAS3_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS4   ( PRDATAS4_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS5   ( PRDATAS5_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS6   ( PRDATAS6_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS7   ( PRDATAS7_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS8   ( PRDATAS8_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS9   ( PRDATAS9_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS10  ( PRDATAS10_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS11  ( PRDATAS11_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS12  ( PRDATAS12_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS13  ( PRDATAS13_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS14  ( PRDATAS14_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS15  ( PRDATAS15_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS16  ( PRDATAS16_const_net_0 ), // tied to 32'h00000000 from definition
        .IADDR      ( IADDR_const_net_0 ), // tied to 32'h00000000 from definition
        // Outputs
        .PREADY     ( smartfusionmss_0_MSS_MASTER_APB_PREADY ),
        .PSLVERR    ( smartfusionmss_0_MSS_MASTER_APB_PSLVERR ),
        .PWRITES    ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PENABLES   ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSELS0     ( CoreAPB3_0_APBmslave0_PSELx ),
        .PSELS1     (  ),
        .PSELS2     (  ),
        .PSELS3     (  ),
        .PSELS4     (  ),
        .PSELS5     (  ),
        .PSELS6     (  ),
        .PSELS7     (  ),
        .PSELS8     (  ),
        .PSELS9     (  ),
        .PSELS10    (  ),
        .PSELS11    (  ),
        .PSELS12    (  ),
        .PSELS13    (  ),
        .PSELS14    (  ),
        .PSELS15    (  ),
        .PSELS16    (  ),
        .PRDATA     ( smartfusionmss_0_MSS_MASTER_APB_PRDATA ),
        .PADDRS     ( CoreAPB3_0_APBmslave0_PADDR ),
        .PWDATAS    ( CoreAPB3_0_APBmslave0_PWDATA ) 
        );

//--------hbridgecontroller
hbridgecontroller #( 
        .PWM_PERIOD ( 200000 ) )
hbridgecontroller_0(
        // Inputs
        .PCLK        ( smartfusionmss_0_FAB_CLK ),
        .PRESERN     ( smartfusionmss_0_M2F_RESET_N ),
        .PSEL        ( CoreAPB3_0_APBmslave0_PSELx ),
        .PENABLE     ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PWRITE      ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PADDR       ( CoreAPB3_0_APBmslave0_PADDR ),
        .PWDATA      ( CoreAPB3_0_APBmslave0_PWDATA ),
        // Outputs
        .PREADY      ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERR     ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .motor1_pwm  ( motor1_pwm_net_0 ),
        .motor2_pwm  ( motor2_pwm_net_0 ),
        .PRDATA      ( CoreAPB3_0_APBmslave0_PRDATA ),
        .motor1_ctrl ( motor1_ctrl_net_0 ),
        .motor2_ctrl ( motor2_ctrl_net_0 ) 
        );

//--------smartfusionmss
smartfusionmss smartfusionmss_0(
        // Inputs
        .MSS_RESET_N  ( MSS_RESET_N ),
        .MSSPREADY    ( smartfusionmss_0_MSS_MASTER_APB_PREADY ),
        .MSSPSLVERR   ( smartfusionmss_0_MSS_MASTER_APB_PSLVERR ),
        .UART_1_RXD   ( UART_1_RXD ),
        .UART_0_RXD   ( UART_0_RXD ),
        .MSSPRDATA    ( smartfusionmss_0_MSS_MASTER_APB_PRDATA ),
        .SPI_1_DI     ( SPI_1_DI ),
        // Outputs
        .MSSPSEL      ( smartfusionmss_0_MSS_MASTER_APB_PSELx ),
        .MSSPENABLE   ( smartfusionmss_0_MSS_MASTER_APB_PENABLE ),
        .MSSPWRITE    ( smartfusionmss_0_MSS_MASTER_APB_PWRITE ),
        .FAB_CLK      ( smartfusionmss_0_FAB_CLK ),
        .M2F_RESET_N  ( smartfusionmss_0_M2F_RESET_N ),
        .UART_1_TXD   ( UART_1_TXD_net_0 ),
        .UART_0_TXD   ( UART_0_TXD_1 ),
        .MSSPADDR     ( smartfusionmss_0_MSS_MASTER_APB_PADDR ),
        .MSSPWDATA    ( smartfusionmss_0_MSS_MASTER_APB_PWDATA ),
        .SPI_1_DO     ( SPI_1_DO_net_0 ),
        .SPI_1_FAB_SS ( SPI_1_FAB_SS_net_0 ),
        // Inouts
        .SPI_1_CLK    ( SPI_1_CLK ),
        .SPI_1_SS     ( SPI_1_SS ) 
        );


endmodule
