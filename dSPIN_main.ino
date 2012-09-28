//dSPIN_main.ino - Contains the setup() and loop() functions.

//float testSpeed = 10;
//int device = MOTOR_Y;
void setup() 
{
  // Standard serial port initialization for debugging.
  Serial.begin(9600);
  
  
  // dSPIN_init() is implemented in the dSPIN_support.ino file. It includes
  //  all the necessary port setup and SPI setup to allow the Arduino to
  //  control the dSPIN chip and relies entirely upon the pin redefinitions
  //  in dSPIN_example.ino
  dSPIN_init();
  
  // First things first: let's check communications. The CONFIG register should
  //  power up to 0x2E88, so we can use that to check the communications.
  //  On the test jig, this causes an LED to light up.
  //if (dSPIN_GetParam(MOTOR_X, dSPIN_CONFIG) == 0x2E88) digitalWrite(STAT1, HIGH);
  //if (dSPIN_GetParam(MOTOR_Y, dSPIN_CONFIG) == 0x2E88) digitalWrite(STAT1, HIGH);
  Serial.println(dSPIN_GetParam(MOTOR_X, dSPIN_CONFIG), HEX);
  Serial.println(dSPIN_GetParam(MOTOR_Y, dSPIN_CONFIG), HEX);
  Serial.println(dSPIN_GetParam(MOTOR_Z, dSPIN_CONFIG), HEX);
  Serial.println(dSPIN_GetParam(MOTOR_E, dSPIN_CONFIG), HEX);
  
  // The following function calls are for this demo application- you will
  //  need to adjust them for your particular application, and you may need
  //  to configure additional registers.
  
  // First, let's set the step mode register:
  //   - dSPIN_SYNC_EN controls whether the BUSY/SYNC pin reflects the step
  //     frequency or the BUSY status of the chip. We want it to be the BUSY
  //     status.
  //   - dSPIN_STEP_SEL_x is the microstepping rate- we'll go full step.
  //   - dSPIN_SYNC_SEL_x is the ratio of (micro)steps to toggles on the
  //     BUSY/SYNC pin (when that pin is used for SYNC). Make it 1:1, despite
  //     not using that pin.
  dSPIN_SetParam(MOTOR_X, dSPIN_STEP_MODE, dSPIN_SYNC_EN | dSPIN_STEP_SEL_1_4 | dSPIN_SYNC_SEL_16);
  dSPIN_SetParam(MOTOR_Y, dSPIN_STEP_MODE, dSPIN_SYNC_EN | dSPIN_STEP_SEL_1_16 | dSPIN_SYNC_SEL_16);
  dSPIN_SetParam(MOTOR_Z, dSPIN_STEP_MODE, dSPIN_SYNC_EN | dSPIN_STEP_SEL_1_16 | dSPIN_SYNC_SEL_16);
  dSPIN_SetParam(MOTOR_E, dSPIN_STEP_MODE, dSPIN_SYNC_EN | dSPIN_STEP_SEL_1_16 | dSPIN_SYNC_SEL_16);

  dSPIN_SetParam(MOTOR_X, dSPIN_STALL_TH, 0x7F);
  dSPIN_SetParam(MOTOR_Y, dSPIN_STALL_TH, 0x7F);
  dSPIN_SetParam(MOTOR_Z, dSPIN_STALL_TH, 0x7F); 
  dSPIN_SetParam(MOTOR_E, dSPIN_STALL_TH, 0x7F); 
  // Configure the MAX_SPEED register- this is the maximum number of (micro)steps per
  //  second allowed. You'll want to mess around with your desired application to see
  //  how far you can push it before the motor starts to slip. The ACTUAL parameter
  //  passed to this function is in steps/tick; MaxSpdCalc() will convert a number of
  //  steps/s into an appropriate value for this function. Note that for any move or
  //  goto type function where no speed is specified, this value will be used.
  dSPIN_SetParam(MOTOR_X, dSPIN_MAX_SPEED, MaxSpdCalc(100));
  dSPIN_SetParam(MOTOR_Y, dSPIN_MAX_SPEED, MaxSpdCalc(100));
  dSPIN_SetParam(MOTOR_Z, dSPIN_MAX_SPEED, MaxSpdCalc(100));
  dSPIN_SetParam(MOTOR_E, dSPIN_MAX_SPEED, MaxSpdCalc(100));

  dSPIN_SetParam(MOTOR_X, dSPIN_MIN_SPEED, MinSpdCalc(0));
  dSPIN_SetParam(MOTOR_Y, dSPIN_MIN_SPEED, MinSpdCalc(0));
  dSPIN_SetParam(MOTOR_Z, dSPIN_MIN_SPEED, MinSpdCalc(0));
  dSPIN_SetParam(MOTOR_E, dSPIN_MIN_SPEED, MinSpdCalc(0));
  // Configure the FS_SPD register- this is the speed at which the driver ceases
  //  microstepping and goes to full stepping. FSCalc() converts a value in steps/s
  //  to a value suitable for this register; to disable full-step switching, you
  //  can pass 0x3FF to this register.
  dSPIN_SetParam(MOTOR_X, dSPIN_FS_SPD, FSCalc(5000));
  dSPIN_SetParam(MOTOR_Y, dSPIN_FS_SPD, FSCalc(5000));
  dSPIN_SetParam(MOTOR_Z, dSPIN_FS_SPD, FSCalc(5000));
  dSPIN_SetParam(MOTOR_E, dSPIN_FS_SPD, FSCalc(5000));
//  dSPIN_SetParam(dSPIN_FS_SPD, 0x3ff);
  // Configure the acceleration rate, in steps/tick/tick. There is also a DEC register;
  //  both of them have a function (AccCalc() and DecCalc() respectively) that convert
  //  from steps/s/s into the appropriate value for the register. Writing ACC to 0xfff
  //  sets the acceleration and deceleration to 'infinite' (or as near as the driver can
  //  manage). If ACC is set to 0xfff, DEC is ignored. To get infinite deceleration
  //  without infinite acceleration, only hard stop will work.
//  dSPIN_SetParam(dSPIN_ACC, 0xfff);
  dSPIN_SetParam(MOTOR_X, dSPIN_ACC, 0xf0);
  dSPIN_SetParam(MOTOR_Y, dSPIN_ACC, 0xf0);
  dSPIN_SetParam(MOTOR_Z, dSPIN_ACC, 0xf0);
  dSPIN_SetParam(MOTOR_E, dSPIN_ACC, 0xf0);
  
  dSPIN_SetParam(MOTOR_X, dSPIN_DEC, 0xf0);  
  dSPIN_SetParam(MOTOR_Y, dSPIN_DEC, 0xf0);  
  dSPIN_SetParam(MOTOR_Z, dSPIN_DEC, 0xf0);  
  dSPIN_SetParam(MOTOR_E, dSPIN_DEC, 0xf0);  
  // Configure the overcurrent detection threshold. The constants for this are defined
  //  in the dSPIN_example.ino file.
  dSPIN_SetParam(MOTOR_X, dSPIN_OCD_TH, dSPIN_OCD_TH_6000mA);
  dSPIN_SetParam(MOTOR_Y, dSPIN_OCD_TH, dSPIN_OCD_TH_6000mA);
  dSPIN_SetParam(MOTOR_Z, dSPIN_OCD_TH, dSPIN_OCD_TH_6000mA);
  dSPIN_SetParam(MOTOR_E, dSPIN_OCD_TH, dSPIN_OCD_TH_6000mA);
  // Set up the CONFIG register as follows:
  //  PWM frequency divisor = 1
  //  PWM frequency multiplier = 2 (62.5kHz PWM frequency)
  //  Slew rate is 290V/us
  //  Do NOT shut down bridges on overcurrent
  //  Disable motor voltage compensation
  //  Hard stop on switch low
  //  16MHz internal oscillator, nothing on output
  dSPIN_SetParam(MOTOR_X, dSPIN_CONFIG, 
                   dSPIN_CONFIG_PWM_DIV_1 | dSPIN_CONFIG_PWM_MUL_2 | dSPIN_CONFIG_SR_290V_us
                 | dSPIN_CONFIG_OC_SD_DISABLE | dSPIN_CONFIG_VS_COMP_DISABLE 
                 | dSPIN_CONFIG_SW_HARD_STOP | dSPIN_CONFIG_INT_16MHZ);
  dSPIN_SetParam(MOTOR_Y, dSPIN_CONFIG, 
                   dSPIN_CONFIG_PWM_DIV_1 | dSPIN_CONFIG_PWM_MUL_2 | dSPIN_CONFIG_SR_290V_us
                 | dSPIN_CONFIG_OC_SD_DISABLE | dSPIN_CONFIG_VS_COMP_DISABLE 
                 | dSPIN_CONFIG_SW_HARD_STOP | dSPIN_CONFIG_INT_16MHZ);
  dSPIN_SetParam(MOTOR_Z, dSPIN_CONFIG, 
                   dSPIN_CONFIG_PWM_DIV_1 | dSPIN_CONFIG_PWM_MUL_2 | dSPIN_CONFIG_SR_290V_us                 
                 | dSPIN_CONFIG_OC_SD_DISABLE | dSPIN_CONFIG_VS_COMP_DISABLE 
                 | dSPIN_CONFIG_SW_HARD_STOP | dSPIN_CONFIG_INT_16MHZ);
  dSPIN_SetParam(MOTOR_E, dSPIN_CONFIG, 
                   dSPIN_CONFIG_PWM_DIV_1 | dSPIN_CONFIG_PWM_MUL_2 | dSPIN_CONFIG_SR_290V_us                 
                 | dSPIN_CONFIG_OC_SD_DISABLE | dSPIN_CONFIG_VS_COMP_DISABLE 
                 | dSPIN_CONFIG_SW_HARD_STOP | dSPIN_CONFIG_INT_16MHZ);

  // Configure the RUN KVAL. This defines the duty cycle of the PWM of the bridges
  //  during running. 0xFF means that they are essentially NOT PWMed during run; this
  //  MAY result in more power being dissipated than you actually need for the task.
  //  Setting this value too low may result in failure to turn.
  //  There are ACC, DEC, and HOLD KVAL registers as well; you may need to play with
  //  those values to get acceptable performance for a given application.
//  dSPIN_SetParam(dSPIN_KVAL_RUN, 0xFF);
  // Calling GetStatus() clears the UVLO bit in the status register, which is set by
  //  default on power-up. The driver may not run without that bit cleared by this
  //  read operation.
  
//  dSPIN_SetParam(dSPIN_KVAL_HOLD, 0xa);  //0xa
//  dSPIN_SetParam(dSPIN_KVAL_ACC, 0x10);  // 0xb
//  dSPIN_SetParam(dSPIN_KVAL_DEC, 0x10);  // 0xb
//  dSPIN_SetParam(dSPIN_KVAL_RUN, 0x10);  // 0xb
//  dSPIN_SetParam(dSPIN_ST_SLP, 0x5);     // 0x5
//  dSPIN_SetParam(dSPIN_INT_SPD, 0x1ad2);  // this should be 0x1c50 according to bemf cacl
//  dSPIN_SetParam(dSPIN_FN_SLP_ACC, 0xb);  //0xb
//  dSPIN_SetParam(dSPIN_FN_SLP_DEC, 0xb);  //0xb

  dSPIN_SetParam(MOTOR_X, dSPIN_KVAL_HOLD, 0x40);  //0xa
  dSPIN_SetParam(MOTOR_X, dSPIN_KVAL_ACC, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_X, dSPIN_KVAL_DEC, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_X, dSPIN_KVAL_RUN, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_X, dSPIN_ST_SLP, 0x6);     // 0x5
  dSPIN_SetParam(MOTOR_X, dSPIN_INT_SPD, 0x165a);  // this should be 0x1c50 according to bemf cacl
  dSPIN_SetParam(MOTOR_X, dSPIN_FN_SLP_ACC, 0x1d);  //0xb
  dSPIN_SetParam(MOTOR_X, dSPIN_FN_SLP_DEC, 0x1d);  //0xb


  dSPIN_SetParam(MOTOR_Y, dSPIN_KVAL_HOLD, 0x5);  //0xa
  dSPIN_SetParam(MOTOR_Y, dSPIN_KVAL_ACC, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_Y, dSPIN_KVAL_DEC, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_Y, dSPIN_KVAL_RUN, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_Y, dSPIN_ST_SLP, 0x6);     // 0x5
  dSPIN_SetParam(MOTOR_Y, dSPIN_INT_SPD, 0x165a);  // this should be 0x1c50 according to bemf cacl
  dSPIN_SetParam(MOTOR_Y, dSPIN_FN_SLP_ACC, 0x1d);  //0xb
  dSPIN_SetParam(MOTOR_Y, dSPIN_FN_SLP_DEC, 0x1d);  //0xb

  dSPIN_SetParam(MOTOR_Z, dSPIN_KVAL_HOLD, 0x5);  //0xa
  dSPIN_SetParam(MOTOR_Z, dSPIN_KVAL_ACC, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_Z, dSPIN_KVAL_DEC, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_Z, dSPIN_KVAL_RUN, 0x40);  // 0xb
  dSPIN_SetParam(MOTOR_Z, dSPIN_ST_SLP, 0x6);     // 0x5
  dSPIN_SetParam(MOTOR_Z, dSPIN_INT_SPD, 0x165a);  // this should be 0x1c50 according to bemf cacl
  dSPIN_SetParam(MOTOR_Z, dSPIN_FN_SLP_ACC, 0x1d);  //0xb
  dSPIN_SetParam(MOTOR_Z, dSPIN_FN_SLP_DEC, 0x1d);  //0xb

  dSPIN_SetParam(MOTOR_E, dSPIN_KVAL_HOLD, 0x23);  // 0x23
  dSPIN_SetParam(MOTOR_E, dSPIN_KVAL_ACC, 0xc5);  // 0xc5
  dSPIN_SetParam(MOTOR_E, dSPIN_KVAL_DEC, 0xc5);  // 0xc5
  dSPIN_SetParam(MOTOR_E, dSPIN_KVAL_RUN, 0xc5);  // 0xc5
  dSPIN_SetParam(MOTOR_E, dSPIN_ST_SLP, 0x5);     // 0x5
  dSPIN_SetParam(MOTOR_E, dSPIN_INT_SPD, 0x1c56);  // this should be 0x1c56 according to bemf cacl
  dSPIN_SetParam(MOTOR_E, dSPIN_FN_SLP_ACC, 0x79);  //0x79
  dSPIN_SetParam(MOTOR_E, dSPIN_FN_SLP_DEC, 0x79);  //0x79

  // Thermal compensation for motor heating
  dSPIN_SetParam(MOTOR_X, dSPIN_K_THERM, 0x2);  
  dSPIN_SetParam(MOTOR_Y, dSPIN_K_THERM, 0x2);  
  dSPIN_SetParam(MOTOR_Z, dSPIN_K_THERM, 0x2);  
  dSPIN_SetParam(MOTOR_E, dSPIN_K_THERM, 0x2);  
  
  dSPIN_GetStatus(MOTOR_X);
  dSPIN_GetStatus(MOTOR_Y);
  dSPIN_GetStatus(MOTOR_Z);
  dSPIN_GetStatus(MOTOR_E);
  
  Serial.println(dSPIN_GetParam(MOTOR_X, dSPIN_CONFIG), HEX);
  Serial.println(dSPIN_GetParam(MOTOR_Y, dSPIN_CONFIG), HEX);
  Serial.println(dSPIN_GetParam(MOTOR_Z, dSPIN_CONFIG), HEX);
  Serial.println(dSPIN_GetParam(MOTOR_E, dSPIN_CONFIG), HEX);

  Serial.println(dSPIN_GetStatus(MOTOR_X), BIN);
  Serial.println(dSPIN_GetStatus(MOTOR_Y), BIN);
  Serial.println(dSPIN_GetStatus(MOTOR_Z), BIN);
  Serial.println(dSPIN_GetStatus(MOTOR_E), BIN);

}

boolean dSPIN_IsBusy(int device) {
  unsigned int status = dSPIN_GetStatus(device);
  return ((status & dSPIN_STATUS_BUSY)  == 0);
}


// Test jig behavior- rotate one full revolution forward, then one full revolution
//  backwards, then slowly tick forwards until the hard stop button is pressed.
int counter = 0;
void loop()
{
  dSPIN_Step_Clock(MOTOR_X, 0);

  for(int x = 0; x < 500; x++){
    digitalWrite(MOTOR_X_STEP, HIGH);
    delay(1);
    digitalWrite(MOTOR_X_STEP, LOW);
  }  
  /*
  // Get Position
  // The abs pos is a 22 bit number in 2's complement format. This isn't being handled correctly here.
  unsigned long current_pos = dSPIN_GetParam(MOTOR_X, dSPIN_ABS_POS);
 // if (current_pos & 0x200000) { // Need to convert 22 bit 2s complement into a signed long. If this bit is set then the number is -ve and so set the higher bits to 1
//	current_pos |= 0xFC0000;
  //}
  //current_pos = current_pos + 0x100000;
  Serial.println(dSPIN_GetParam(MOTOR_X, dSPIN_ABS_POS));
  // Set Mark
  dSPIN_SetParam(MOTOR_X, dSPIN_MARK, current_pos + 1000);
  // Goto Mark
  dSPIN_GoMark(MOTOR_X);

  // Wait
  while(dSPIN_IsBusy(MOTOR_X));
  //delay(400);
  Serial.println(dSPIN_GetParam(MOTOR_X, dSPIN_ABS_POS));
  //digitalWrite(LED, HIGH);  
  dSPIN_SetParam(MOTOR_X, dSPIN_MARK, current_pos - 500 );  
  // Move Mark
  //dSPIN_GoMark(MOTOR_X);  
  while(dSPIN_IsBusy(MOTOR_X));

  //delay(2000);
  Serial.println(dSPIN_GetParam(MOTOR_X, dSPIN_ABS_POS));
  
  dSPIN_SoftStop(MOTOR_X);
  */
  while(1); //update_LED(); 
}
