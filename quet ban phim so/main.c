#include <stdint.h>
#include<stdio.h>
void delay(void)
{
 for(uint32_t i =0 ; i < 300000 ; i++);
}
int main(void)
{
 uint32_t volatile *const pGPIOAModeReg  =  (uint32_t*)(0x40020000);
 uint32_t volatile *const pInPutDataReg  =  (uint32_t*)(0x40020000+0x10);
 uint32_t volatile *const pOutPutDataReg =  (uint32_t*)(0x40020000+0x14);
 uint32_t volatile *const pPullupDownReg =  (uint32_t*)(0x40020800 + 0x0C);
 uint32_t volatile *const pClockCtrlReg  =  (uint32_t*)(0x40023830);
 //1.Enable the peripheral clock of GPIOA peripheral
 *pClockCtrlReg |= ( 1 << 0);
 // 2.configure PA0,PA1,PA2,PA3 as output (rows)
 *pGPIOAModeReg &= ~(0xFF);//clear
 *pGPIOAModeReg |= 0x55;   //set
 // 3. configure PA12 , PA13, PA14, PA15 as input (columns)
  *pGPIOAModeReg &= ~(3 << 26);
  // 4.Enable internal pull-up resistors for PA12 PA13 PA14 PA15
      *pPullupDownReg &= ~(0xFF << 24);
      *pPullupDownReg |=  (0x55 << 24);
      while(1)
      {
          //make all rows HIGH
          *pOutPutDataReg |= 0x0f;
          //make R1 LOW(PA0)
          *pOutPutDataReg &= ~( 1 << 0);
          //scan the columns
          //check C1(PA12) low or high
          if(!(*pInPutDataReg & ( 1 << 12))){
           //key is pressed
           delay();
           printf("1\n");
          }
          //check C2(PA13) low or high
          if(!(*pInPutDataReg & ( 1 << 13))){
           //key is pressed
           delay();
           printf("2\n");
          }
          //check C3(PA14) low or high
          if(!(*pInPutDataReg & ( 1 << 14))){
           //key is pressed
           delay();
           printf("3\n");
          }
          //check C4(PA15) low or high
          if(!(*pInPutDataReg & ( 1 << 15))){
           //key is pressed
           delay();
           printf("4\n");
          }
          //make all rows HIGH
           *pOutPutDataReg |= 0x0f;
          //make R2 LOW(PA1)
          *pOutPutDataReg &= ~( 1 << 1);
          //scan the columns
          //check C1(PA12) low or high
          if(!(*pInPutDataReg & ( 1 << 12))){
 //key is pressed
 delay();
 printf("5\n");
  }
  //check C2(PA13) low or high
  if(!(*pInPutDataReg & ( 1 << 13))){
 //key is pressed
 delay();
 printf("6\n");
  }
  //check C3(PA14) low or high
  if(!(*pInPutDataReg & ( 1 << 14))){
 //key is pressed
 delay();
 printf("7\n");
  }
  //check C4(PA15) low or high
  if(!(*pInPutDataReg & ( 1 << 15))){
 //key is pressed
 delay();
 printf("8\n");
  }
          //make all rows HIGH
           *pOutPutDataReg |= 0x0f;
          //make R3 LOW(PA2)
          *pOutPutDataReg &= ~( 1 << 2);
          //scan the columns
  //check C1(PA12) low or high
          if(!(*pInPutDataReg & ( 1 << 12))){
 //key is pressed
 delay();
 printf("9\n");
  }
  //check C2(PA13) low or high
  if(!(*pInPutDataReg & ( 1 << 13))){
 //key is pressed
 delay();
 printf("10\n");
  }
  //check C3(PA14) low or high
  if(!(*pInPutDataReg & ( 1 << 14))){
 //key is pressed
 delay();
 printf("11\n");
  }
  //check C4(PA15) low or high
  if(!(*pInPutDataReg & ( 1 << 15))){
 //key is pressed
 delay();
 printf("12\n");
  }
          //make all rows HIGH
           *pOutPutDataReg |= 0x0f;
          //make R4 LOW(PA3)
          *pOutPutDataReg &= ~( 1 << 3);
          //scan the columns
          //check C1(PA12) low or high
            if(!(*pInPutDataReg & ( 1 << 12))){
 //key is pressed
 delay();
 printf("13\n");
  }
  //check C2(PA13) low or high
  if(!(*pInPutDataReg & ( 1 << 13))){
 //key is pressed
 delay();
 printf("14\n");
  }
  //check C3(PA14) low or high
  if(!(*pInPutDataReg & ( 1 << 14))){
 //key is pressed
 delay();
 printf("15\n");
  }
  //check C4(PA15) low or high
  if(!(*pInPutDataReg & ( 1 << 15))){
 //key is pressed
 delay();
 printf("16\n");
  }
      }//while end
}