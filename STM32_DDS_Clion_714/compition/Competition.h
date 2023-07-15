/**
*********************************************************************
*********
* @project_name :templateDevelopment
* @file : Competition.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/7/9 
*********************************************************************
*********
*/
//

#ifndef CPROJECT_COMPETITION_H
#define CPROJECT_COMPETITION_H
#include "map"
#include "vector"
#ifdef __cplusplus
#include "MyLCD.h"
#include "Myapplication.h"
extern "C"
{
#endif
/*______________________类的声明在这下面________________:*/
class Competiton{
public:
    Competiton(uint8_t x);
    void CompetitionInit();
    void Channel_one();
    void Channel_two();
    void Channel_three();
    void Channel_four();
    void LoadData();
    void scanGRAYBLUE(uint32_t timeout);
    void NextPage();
    void ShowAmplitudeFrequency();
    void ShowPhaseFrequency();
    void PHA_TEST();
//    void Get_PHA();
private:
    std::vector<float> vec;
    std::vector<float> cec;
    //std::map<float,float> comMap;
    uint32_t InitFre;
    uint32_t FinalFre;
    uint32_t step;
    uint32_t cycle;
    MyLCD CompetitionLCD;
    MyUsart *Lcdusart;
    MyADC *lcdadc;
    uint32_t FREQUENCY;
    uint32_t DUTY_RATIO;
    u32 PHA_CNT;                        // 相位差测量计数值
    u32 FRE_CNT;                        // 频率测量计数值
    u32 CYC_CNT;                        // 测量周期个数计数值
    u32 ADV_CNT;                        // 超前周期计数值
    u32 LAG_CNT;                        // 滞后周期计数值
};
#ifdef __cplusplus
}
#endif
#endif //CPROJECT_COMPETITION_H
