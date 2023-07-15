/**
*********************************************************************
*********
* @project_name :templateDevelopment
* @file : Competition.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/7/9
*********************************************************************
*********
*/
//

#include "Competition.h"
#include "spi_communication.h"
#include "delay.h"
//#include "spi.h"
#define Rowspacing 30
#define Colspacing 30
#define INIITx 230
#define Width 300
#define mul 6
#define FRECON 34.3
void Competiton::Channel_one() {

    char * Lcdrec;
    char temp[24]="0";
    uint8_t ChannelID=0;
    uint32_t enterData=0;
    lcd_show_string(5,Colspacing*mul,Width,24,24,"Please enter Initfre",GREEN);

    while (1) {
        Lcdrec = CompetitionLCD.MyLCDMonitorScreen(20);
        if (Lcdrec != NULL) {
            enterData = atoi(Lcdrec);
            if (enterData >=10 && enterData <=1000000) {
                SPI_Communication_Send_Cmd_Data(ChannelID,enterData*FRECON);
                this->InitFre=enterData;
                lcd_fill(5, Colspacing*mul, 5+Width, (Colspacing*mul)+60, WHITE);
                lcd_show_string(5, Colspacing*mul, 200, 24, 24, "Send Success", RED);
                HAL_Delay(500);
                lcd_fill(5, Colspacing*mul, 5+Width, (Colspacing*mul)+60, WHITE);
                lcd_fill(Colspacing*8,Rowspacing,Colspacing*8+Width,Rowspacing*2,WHITE);
                sprintf(temp,"%dhz",enterData);
                lcd_show_string(Colspacing*8,Rowspacing,Width,24,24,temp,RED);
                //lcd_show_string(5,30,200,24,24,"1:Initial frequency",GREEN);
                break;
            } else {
                CompetitionLCD.MyLCDShowerror();
            }
        }
    }

}

void Competiton::Channel_two() {
    char * Lcdrec;
    char temp[24]="0";
    uint32_t enterData=0;
    uint8_t ChannelID=1;
    lcd_show_string(5,Colspacing*mul,Width,24,24,"Please enter Fianlfre",GREEN);
    while (1) {
        Lcdrec = CompetitionLCD.MyLCDMonitorScreen(20);
        if (Lcdrec != NULL) {
            enterData = atoi(Lcdrec);
            if (enterData >=10 && enterData <=100000) {
                SPI_Communication_Send_Cmd_Data(ChannelID,enterData*FRECON);
                this->FinalFre=enterData;
                lcd_fill(5, Colspacing*mul, 5+Width, (Colspacing*mul)+60, WHITE);
                lcd_show_string(5, Colspacing*mul, 200, 24, 24, "Send Success", RED);
                HAL_Delay(500);
                lcd_fill(5, Colspacing*mul, 5+Width, (Colspacing*mul)+60, WHITE);
                lcd_fill(Colspacing*8,Rowspacing*2,Colspacing*8+Width,Rowspacing*3,WHITE);
                sprintf(temp,"%dhz",enterData);
                lcd_show_string(Colspacing*8,Rowspacing*2,Width,24,24,temp,RED);
//                Lcdusart->Myprintf("FinalFre:%d\n",FinalFre);
                //lcd_show_string(5,30+Rowspacing,200,24,24,"2:Final frequency",GREEN);
                break;
            } else {
                CompetitionLCD.MyLCDShowerror();
            }
        }
    }

}

void Competiton::Channel_three() {
    char * Lcdrec;
    char temp[24]="0";
    uint32_t enterData=0;
    uint8_t ChannelID=2;
    lcd_show_string(5,Colspacing*mul,Width,24,24,"Please enter Step",GREEN);
    while (1) {
        Lcdrec = CompetitionLCD.MyLCDMonitorScreen(20);
        if (Lcdrec != NULL) {
            enterData = atoi(Lcdrec);
            if (enterData >=10 && enterData <=1000 ) {
                SPI_Communication_Send_Cmd_Data(ChannelID,enterData*FRECON);
                this->step=enterData;
                lcd_fill(5, Colspacing*mul, 5+Width, (Colspacing*mul)+60, WHITE);
                lcd_show_string(5, Colspacing*mul, 200, 24, 24, "Send Success", RED);
                HAL_Delay(500);
                lcd_fill(5, Colspacing*mul, 5+Width, (Colspacing*mul)+60, WHITE);
                lcd_fill(Colspacing*8,Rowspacing*3,Colspacing*8+Width,Rowspacing*4,WHITE);
                sprintf(temp,"%dhz",enterData);
                lcd_show_string(Colspacing*8,Rowspacing*3,Width,24,24,temp,RED);
                //lcd_show_string(5,30+(Rowspacing*2),200,24,24,"3:Scanning step",GREEN);
                break;
            } else {
                CompetitionLCD.MyLCDShowerror();
            }
        }
    }
}

void Competiton::Channel_four() {
    char * Lcdrec;
    char temp[24]="0";
    float32_t Calculate=0;
    uint32_t enterData=0;
    uint8_t ChannelID=3;
    float midd=(this->FinalFre-this->InitFre)/this->step;
    float other=10/midd;
    uint32_t end=other*1000;
//    Calculate=((10/(/this->step))*1000);
    lcd_show_string(5,Colspacing*mul,Width*2,24,24,"SwitchingTime",GREEN);
    lcd_fill(Colspacing*8,Rowspacing*4,Colspacing*8+Width,Rowspacing*5,WHITE);
    sprintf(temp,"%dms",end);
    lcd_show_string(Colspacing*8,Rowspacing*4,Width,24,24,temp,RED);
    lcd_show_string(5,Colspacing*(mul+1),Width*2,24,24,"it's right?Enter,9 Error?Enter 8",GREEN);

    while (1) {
        Lcdrec = CompetitionLCD.MyLCDMonitorScreen(20);
        if (Lcdrec != NULL) {
            enterData = atoi(Lcdrec);
            if (enterData==9) {
                SPI_Communication_Send_Cmd_Data(ChannelID,end*125000);
//                this->cycle=enterData;
                lcd_fill(5, Colspacing*mul, 5+Width, (Colspacing*mul)+30, WHITE);
                lcd_show_string(5, Colspacing*mul, 200, 24, 24, "Send Success", RED);
                HAL_Delay(500);
                lcd_fill(5,Colspacing*(mul),Colspacing*8+Width,Colspacing*(mul+2),WHITE);
//                lcd_fill(Colspacing*8,Rowspacing*4,Colspacing*8+Width,Rowspacing*5,WHITE);
//                sprintf(temp,"%ds",enterData);
//                lcd_show_string(Colspacing*8,Rowspacing*4,Width,24,24,temp,RED);
                // lcd_show_string(5,30+(Rowspacing*3),200,24,24,"4:Scanning Cycle",GREEN);
                break;
            }
            else if (enterData==8){
                lcd_fill(5,Colspacing*(mul),Colspacing*8+Width,Colspacing*(mul+2),WHITE);
                break;
            }
            else {
                CompetitionLCD.MyLCDShowerror();
            }
        }
    }
}

Competiton::Competiton(uint8_t x) {

    this->FinalFre=this->InitFre=this->cycle=this->step=0;
//    InitFre=1000;
//    FinalFre=100000;
//    step=1000;
}

void Competiton::CompetitionInit() {
    lcd_clear(WHITE);
    this->CompetitionLCD.MyLCDTouchInit();
    this->Lcdusart=new MyUsart(&huart1);
    this->lcdadc=new MyADC(&hadc1);
//    lcd_fill(5,30+(Rowspacing*8),Colspacing*4,30+(Rowspacing*10),GRAYBLUE);
//    lcd_show_string(25,45+(Rowspacing*8),200,24,24,"Change",BLACK);
    lcd_show_string(((lcddev.width-200)/2),0,200,24,24,"Please select",RED);
    lcd_show_string(5,30,200,24,24,"1:Initial frequency",RED);
    lcd_show_string(5,30+Rowspacing,200,24,24,"2:Final frequency",RED);
    lcd_show_string(5,30+(Rowspacing*2),200,24,24,"3:Scanning step",RED);
    lcd_show_string(5,30+(Rowspacing*3),300,24,24,"4:SwitchingTime",RED);
    lcd_show_string(5,30+(Rowspacing*4),300,24,24,"5:Switch pages",RED);
//    lcd_show_string(5,30+(Rowspacing*4),200,24,24,"1:Init_frequency",RED);
//    lcd_show_string(5,(30+Rowspacing*5),200,24,24,"2:Final frequency",RED);
//    lcd_show_string(5,30+(Rowspacing*6),200,24,24,"3:Scanning step",RED);
//    lcd_show_string(5,30+(Rowspacing*7),200,24,24,"4:Scanning Cycle",RED);
    CompetitionLCD.MyLCDLoadKeyBoard();
}

void Competiton::LoadData() {
//    Lcdusart->Myprintf("FinalFre:%d\n",FinalFre);
    char temp[24]="0";
    if (this->step!=0||this->cycle!=0
        ||this->InitFre!=0||this->FinalFre!=0){
        lcd_fill(Colspacing*8,Rowspacing,Colspacing*8+Width,Rowspacing*2,WHITE);
        sprintf(temp,"%dhz",this->InitFre);
        lcd_show_string(Colspacing*8,Rowspacing,Width,24,24,temp,RED);
        //lcd_show_string(5,30,200,24,24,"1:Initial frequency",GREEN);

        lcd_fill(Colspacing*8,Rowspacing*2,Colspacing*8+Width,Rowspacing*3,WHITE);
        sprintf(temp,"%dhz",this->FinalFre);
        lcd_show_string(Colspacing*8,Rowspacing*2,Width,24,24,temp,RED);
        //lcd_show_string(5,30+Rowspacing,200,24,24,"2:Final frequency",GREEN);

        lcd_fill(Colspacing*8,Rowspacing*3,Colspacing*8+Width,Rowspacing*4,WHITE);
        sprintf(temp,"%dhz",this->step);
        lcd_show_string(Colspacing*8,Rowspacing*3,Width,24,24,temp,RED);
        //lcd_show_string(5,30+(Rowspacing*2),200,24,24,"3:Scanning step",GREEN);

        lcd_fill(Colspacing*8,Rowspacing*4,Colspacing*8+Width,Rowspacing*5,WHITE);
        sprintf(temp,"%ds",this->cycle);
        lcd_show_string(Colspacing*8,Rowspacing*4,Width,24,24,temp,RED);
        // lcd_show_string(5,30+(Rowspacing*3),200,24,24,"4:Scanning Cycle",GREEN);
    }

}
int flag=0;
int flag1=0;
void Competiton::NextPage() {
    this->CompetitionLCD.MyLCDTouchInit();
    lcd_clear(WHITE);
    uint32_t timeout=20;
//    Lcdusart->Myprintf("FinalFre:%d\n",FinalFre);
    lcd_fill(5,5,115,65,BLACK);
    lcd_show_string(20,20,200,24,24,"Back",WHITE);

    lcd_fill(300,5,400,65,BLACK);
    lcd_show_string(330,20,300,24,24,"Start",WHITE);


    lcd_draw_line(5,40,5,400,BLACK);
    lcd_draw_line(5,400,480,400,BLACK);//横

    lcd_draw_line(5,420,5,780,BLACK);
    lcd_draw_line(5,780,480,780,BLACK);//横

    lcd_show_string(240,80,300,24,24,"AmplitudeFrequency：",BLACK);
    lcd_show_string(240,450,300,24,24,"PhaseFrequency：",BLACK);

    for (;;){
        uint32_t tickStart = HAL_GetTick();
        //按键扫 退出
        while (1) {
            tp_dev.scan(0);
            if ((tp_dev.sta) & (1 << 0)) {
                if (tp_dev.x[0] > 5 && tp_dev.x[0] < 115 && tp_dev.y[0] > 5 && tp_dev.y[0] < 65) {
                    while (tp_dev.x[0] > 5 && tp_dev.x[0] < 115 && tp_dev.y[0] > 5 && tp_dev.y[0] < 65) {
                        tp_dev.scan(0);
                    }
                    HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
                    lcd_fill(5, 5, 115, 65, RED);
                    HAL_Delay(50);
                    lcd_fill(5, 5, 115, 65, BLACK);
                    HAL_TIM_Base_Stop_IT(&htim6);
                    HAL_Delay(30);
                    return;
                }
                if (tp_dev.x[0] > 300 && tp_dev.x[0] < 400 && tp_dev.y[0] > 5 && tp_dev.y[0] < 65) {
                    while (tp_dev.x[0] > 350 && tp_dev.x[0] < 400 && tp_dev.y[0] > 5 && tp_dev.y[0] < 65) {

                        tp_dev.scan(0);
                    }
                    lcd_fill(300,5,400,65,RED);
                    lcd_show_string(330,20,300,24,24,"Start",WHITE);
                    flag = 1;
                    flag1=1;
                }
            }

            if (timeout != 0){
                if ((HAL_GetTick()-tickStart)>timeout)
                    break;

            }

        }
        if (flag){
            this->PHA_TEST();
        }
//        if (flag1){
//           this->ShowAmplitudeFrequency();
//        }

    }
}
float lastpoint=0;
void Competiton::ShowAmplitudeFrequency() {
//    float lastpoint=0;

//   HAL_Delay(300);

//    for (int i = 5; i < 480; ++i) {
//        float adctemp=this->lcdadc->myADCGetADCValue();
//        lcd_draw_line(i+1,390,i,110,WHITE);
//        lcd_draw_line(i,390-(lastpoint*84.8),i+1,390-(adctemp*84.8),BLACK);
//        lastpoint=adctemp;
//    }
//
    if(FREQUENCY>5000){
        float adctemp=this->lcdadc->myADCGetADCValue();
        lcd_draw_point((FREQUENCY-5000)/20-1,389-(adctemp*84.8),BLACK);
        lcd_draw_point((FREQUENCY-5000)/20-1,390-(adctemp*84.8),BLACK);
        lcd_draw_point((FREQUENCY-5000)/20-1,391-(adctemp*84.8),BLACK);
        lcd_draw_point((FREQUENCY-5000)/20,390-(adctemp*84.8),BLACK);
        lcd_draw_point((FREQUENCY-5000)/20,391-(adctemp*84.8),BLACK);
        lcd_draw_point((FREQUENCY-5000)/20,389-(adctemp*84.8),BLACK);
        lcd_draw_point(((FREQUENCY-5000)/20)+1,390-(adctemp*84.8),BLACK);
        lcd_draw_point(((FREQUENCY-5000)/20)+1,391-(adctemp*84.8),BLACK);
        lcd_draw_point(((FREQUENCY-5000)/20)+1,389-(adctemp*84.8),BLACK);
        }
////        uint16_t x0=0;
////        uint16_t x1=x0+1;
////        uint16_t y0=389-(lastadc*84.8);
////        uint16_t y1=389-(lastadc*84.8);
////        lcd_draw_line(x0,y0,x1,y1,RED);
////        lastadc=adctemp;
//    }

}

void Competiton::ShowPhaseFrequency() {
    uint32_t recPhase=0;
    uint32_t lastpoint=0;
    for (int i = 5; i < 480; ++i) {
        recPhase=SPI_Communication_Rece_Data();
        if (recPhase<=360&&recPhase>=0) {
            lcd_draw_line(i + 1, 480, i, 760, WHITE);
            lcd_draw_line(i, 760 - (lastpoint * 0.6), i + 1, 760 - (recPhase * 0.6), BLACK);
            lastpoint = recPhase;
        }
        else{
            HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
             }

    }
}

void Competiton::PHA_TEST (void) {
//    uint32_t timeout = 10;

    unsigned char display_buf[16];        // 显示缓存
//    float FREQUENCY;                                    // 频率值
//    float DUTY_RATIO;                                // 相位差值
    uint32_t latdata = 0;
    uint32_t lastpoint = 0;
    uint32_t count = 0;
    // 变量初始化
    PHA_CNT = 0;
    FRE_CNT = 0;
    CYC_CNT = 0;
    ADV_CNT = 0;
    LAG_CNT = 0;
    FREQUENCY = 0;
    DUTY_RATIO = 0;
    // 获取计数值
//    while (FREQUENCY != 100000) {
    uint32_t tickStart = HAL_GetTick();
    SPI_Communication_Send_Cmd_Data(1, 1);                // 禁止计数值更新
    FRE_CNT = SPI_Communication_Rece_Cmd_Data(2);    // 获取频率计数值
    PHA_CNT = SPI_Communication_Rece_Cmd_Data(3);    // 获取相位差计数值
    CYC_CNT = SPI_Communication_Rece_Cmd_Data(4);    // 获取测量周期个数值
    ADV_CNT = SPI_Communication_Rece_Cmd_Data(5);    // 获取超前周期计数值
    LAG_CNT = SPI_Communication_Rece_Cmd_Data(6);    // 获取滞后周期计数值
    SPI_Communication_Send_Cmd_Data(1, 0);                // 允许计数值更新
    // 频率及相位差计算
    /******************************************************************
    f = 1/t = (c_cnt * 10^8) / (f_cnt * 2)
    t = (f_cnt / c_cnt) * 20 ns = (f_cnt * 20) / (c_cnt * 10^9) s
    ******************************************************************/
//	FREQUENCY  = (CYC_CNT * 200000000.00) / FRE_CNT;
//	DUTY_RATIO = PHA_CNT * 360.00 / FRE_CNT;
    this->FREQUENCY = (CYC_CNT * 300000000.00 / 1.0000182) / FRE_CNT;
    DUTY_RATIO = PHA_CNT * 360.00 / FRE_CNT;

//    for (int i = 5; i < 480; ++i) {
//        lcd_draw_line(i,500,i,780,WHITE);
//        lcd_draw_line(i,761-(lastpoint*0.6),i+1,761-(DUTY_RATIO*0.6),BLACK);
//        lastpoint=DUTY_RATIO;
//        this->Lcdusart->Myprintf("%d\r\n", DUTY_RATIO);// 相位差值
//    }
     //超前滞后判读
    if (ADV_CNT <= LAG_CNT)                        // 滞后
    {
        DUTY_RATIO = 360 - DUTY_RATIO;    // 调整为超前
    }
    // 显示频率及相位差

//      this->Lcdusart->Myprintf("FRE_CNT = %d\r\n", FRE_CNT);            // 频率值
     this->Lcdusart->Myprintf("CYC_CNT = %d\r\n", CYC_CNT);// 相位差值
     this->Lcdusart->Myprintf("FREQUENCY = %d\r\n", FREQUENCY);// 相位差值
    if (this->FREQUENCY>0&&this->FREQUENCY<=100){
//        uint16_t x0=240*lastfre/100;
//        uint16_t x1=240*FREQUENCY/100;
//        uint16_t y0=760-((360-lastpha)*0.6);
//        uint16_t y1=760-((360-DUTY_RATIO)*0.6);
//        lcd_draw_line(x0,y0,x1,y1,RED);
//        lastfre=FREQUENCY;
//        lastpha=DUTY_RATIO;
        lcd_draw_point(FREQUENCY-1,759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY-1,761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY-1,760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY,760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY,761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY,759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY+1,760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY+1,759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(FREQUENCY+1,761-(DUTY_RATIO*0.6),BLACK);

    }
    if (FREQUENCY>100&&FREQUENCY<=1000){
//        uint16_t x0=240*lastfre/1000;
//        uint16_t x1=240*FREQUENCY/1000;
//        uint16_t y0=760-((360-lastpha)*0.6);
//        uint16_t y1=760-((360-DUTY_RATIO)*0.6);
//        lcd_draw_line(x0,y0,x1,y1,RED);
//        lastfre=FREQUENCY;
//        lastpha=DUTY_RATIO;

        lcd_draw_point(99+(FREQUENCY/10),759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(99+(FREQUENCY/10),761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(99+(FREQUENCY/10),760-(DUTY_RATIO*0.6),BLACK);

        lcd_draw_point(100+(FREQUENCY/10),760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(100+(FREQUENCY/10),761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(100+(FREQUENCY/10),759-(DUTY_RATIO*0.6),BLACK);

        lcd_draw_point(101+(FREQUENCY/10),760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(101+(FREQUENCY/10),759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(101+(FREQUENCY/10),761-(DUTY_RATIO*0.6),BLACK);

    }
    if (FREQUENCY>1000&&FREQUENCY<=10000){
//        uint16_t x0=240*lastfre/10000;
//        uint16_t x1=240*FREQUENCY/10000;
//        uint16_t y0=760-((360-lastpha)*0.6);
//        uint16_t y1=760-((360-DUTY_RATIO)*0.6);
//        lcd_draw_line(x0,y0,x1,y1,RED);
//        lastfre=FREQUENCY;
//        lastpha=DUTY_RATIO;


        lcd_draw_point(199+(FREQUENCY/1000),759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(199+(FREQUENCY/1000),761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(199+(FREQUENCY/1000),760-(DUTY_RATIO*0.6),BLACK);

        lcd_draw_point(200+(FREQUENCY/1000),760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(200+(FREQUENCY/1000),761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(200+(FREQUENCY/1000),759-(DUTY_RATIO*0.6),BLACK);

        lcd_draw_point(201+(FREQUENCY/1000),760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(201+(FREQUENCY/1000),759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(201+(FREQUENCY/1000),761-(DUTY_RATIO*0.6),BLACK);
    }
    if ((FREQUENCY<100000)&&FREQUENCY>10000){
//        uint16_t x0=240*lastfre/100000;
//        uint16_t x1=240*FREQUENCY/100000;
//        uint16_t y0=760-((360-lastpha)*0.6);
//        uint16_t y1=760-((360-DUTY_RATIO)*0.6);
//        lcd_draw_line(x0,y0,x1,y1,RED);
//        lastfre=FREQUENCY;
//        lastpha=DUTY_RATIO;

        lcd_draw_point(299+(FREQUENCY/10000),759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(299+(FREQUENCY/10000),761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(299+(FREQUENCY/10000),760-(DUTY_RATIO*0.6),BLACK);

        lcd_draw_point(300+(FREQUENCY/10000),760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(300+(FREQUENCY/10000),761-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(300+(FREQUENCY/10000),759-(DUTY_RATIO*0.6),BLACK);

        lcd_draw_point(301+(FREQUENCY/10000),760-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(301+(FREQUENCY/10000),759-(DUTY_RATIO*0.6),BLACK);
        lcd_draw_point(301+(FREQUENCY/10000),761-(DUTY_RATIO*0.6),BLACK);
    }


/*        vec.push_back(FREQUENCY);
        cec.push_back(DUTY_RATIO);

       //this->comMap.insert((std::pair<float, float>(FREQUENCY, DUTY_RATIO)));
        lcd_fill(40, 450, 200, 470, WHITE);
        lcd_show_string(40, 450, 200, 24, 24, "Waiting!", RED);
        count++;
        if (count > 1000) {
            break;
        }
        if (timeout != 0) {
            if ((HAL_GetTick() - tickStart) > timeout)
                break;

        }
    }

    if (count < 1000) {
        lcd_fill(40, 450, 200, 470, WHITE);
        lcd_show_string(40, 450, 200, 24, 24, "GET!", RED);
    } else {
        flag1 = 0;
        lcd_fill(40, 450, 200, 470, WHITE);
        lcd_show_string(40, 450, 200, 24, 24, "error!", RED);
    }
    uint32_t prepoint = 0;
    uint32_t intdex = 0;//相位下标
    uint32_t Frequency_subscript=0;//幅度下标
    if (flag1) {
        for (int i = 5; i < 470; i=i+5) {
            if (vec[Frequency_subscript] <= 100) {
                if (i==5){
                    lcd_draw_line(i, 760, i, cec[intdex], BLACK);
                    intdex += 10;
                    prepoint=cec[intdex];
                    Frequency_subscript+=10;
                }
                lcd_draw_line(i, 760 - (prepoint * 0.6), i+1, (760-cec[intdex]), BLACK);
                prepoint=cec[intdex];
                intdex += 10;
                Frequency_subscript+=10;
            }
            if (vec[Frequency_subscript]>100&&vec[Frequency_subscript]<10000){
                lcd_draw_line(i, 760 - (prepoint * 0.6), i+1, (760-cec[intdex]), BLACK);
                intdex += 100;
                Frequency_subscript+100;
            }
            if (vec[Frequency_subscript]>1000&&vec[Frequency_subscript]<10000){
                lcd_draw_line(i, 760 - (prepoint * 0.6), i+1, (760-cec[intdex]), BLACK);
                intdex += 1000;
                Frequency_subscript+=1000;
            }
            if (vec[Frequency_subscript]>10000&&vec[Frequency_subscript]<100000){
                lcd_draw_line(i, 760 - (prepoint * 0.6), i+1, (760-cec[intdex]), BLACK);
                intdex += 10000;
                Frequency_subscript+=10000;
            }
        }


        auto it = this->comMap.begin();
        while ( it != this->comMap.end()) {
            if ((it->first > 0 && it->first < 15)) {
                lcd_draw_line(5, 760, 5, 760 - (it->second * 0.6), BLACK);
                prepoint = it->second;
            }
//            else if ((it->first  >=15&& it->first <= 110) ) {
//                lcd_draw_line(intdex, 760 - (prepoint * 0.6), intdex + 91, 760 - (it->second * 0.6), BLACK);
//                intdex += ;

             else if((it->first  >=15&& it->first <= 110)){
                lcd_draw_line(intdex, 760 - (prepoint * 0.6), intdex + 91, 760 - (it->second * 0.6), BLACK);
                it=it+2;
            }

        }



    this->comMap.clear();
     lcd_draw_line((latdata * 0.048), 760 - (lastpoint * 0.6), (xFREQUENCY * 0.048), 760 - (lastpoint * 0.6), BLACK);
     latdata=xFREQUENCY;lastpoint=lastpoint;
     for (int i = 5; i < 480; ++i) {
         if (i<160){//10--1000
             if (FREQUENCY-(i*214)<1000.0){
                 lcd_draw_line(i, 760 - (lastpoint * 0.6),i+1, 760 - (DUTY_RATIO * 0.6), BLACK);
                 lastpoint=DUTY_RATIO;
             }
         }
         if (i>=160&&i<320){
             if (FREQUENCY-(i*208)<100.0){
                 lcd_draw_line(i, 760 - (lastpoint * 0.6),i+1, 760 - (DUTY_RATIO * 0.6), BLACK);
                 lastpoint=DUTY_RATIO;
             }
         }*/
}






