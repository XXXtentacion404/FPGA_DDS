/**
*********************************************************************
*********
* @project_name :templateDevelopment
* @file : Mymain.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/4/27 
*********************************************************************
*********
*/
//
#include "Myapplication.h"
#include "IncludeALL.h"
#include "MySchedule.h"
#include "Competition.h"
#include "spi_communication.h"

#define SIZE 1024

#define Rowspacing 30
#define Colspacing 30
ESP esp;
Competiton competiton(1);
//定义变量或者数组只能声明局部变量 不能声明全局否则会找不到地址
void Mymain()
{
    char * Lcdrec="0";
    uint16_t select=0;
    uint16_t adcNum[SIZE]={0};
    MyKey myKey;

    MyADC myAdc(adcNum,SIZE,&htim3,&hadc1);
    MyUsart myUsart(&huart1);
    MySchedule mySchedule;
    MyLCD myLcd;
//    myAdc.myADCCollectData();
//    myUsart.SendInt(myAdc.GetADCStorageBuff(),myAdc.GetADCStorageBuffSize());
    myLcd.MyLCDTouchInit();
    competiton.CompetitionInit();
    SPI_Communication_Init();
    for (;;) {
      Lcdrec=myLcd.MyLCDMonitorScreen(20);
        if (Lcdrec!=NULL){
            select=atoi(Lcdrec);
            switch (select) {
                case 1:
                    myUsart.Myprintf("1 error\n");
                    competiton.Channel_one();//起始频率
                    break;
                case 2:
                    myUsart.Myprintf("2 error\n");
                    competiton.Channel_two();//终止频率
                    break;
                case 3:
                    myUsart.Myprintf("3 error\n");
                    competiton.Channel_three();//步长
                    break;
                case 4:
                    myUsart.Myprintf("4 error\n");
                    competiton.Channel_four();
                    break;
                case 5:
                    competiton.NextPage();
                    competiton.CompetitionInit();
                    competiton.LoadData();
                    break;
                default:
                    myUsart.Myprintf("5 error\n");
                    myLcd.MyLCDShowerror();
                    Lcdrec=NULL;
                    break;
            }
        }







    }




}
/***************************************************************
  *  @brief     让中断文件可以调用c++函数
  *  @param     参数
  *  @note      备注 ：此函数放在it文件当中
  *  @Sample usage:     函数的使用方法 ：直接调用
   * @author     wangzibo
 **************************************************************/

void ItUsart(DMA_HandleTypeDef * hdma_usartx_rx)
{
   esp.myUsart->ReceiveDataCallBack(hdma_usartx_rx);

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance==TIM6){
//        competiton.ShowAmplitudeFrequency();
    }
}