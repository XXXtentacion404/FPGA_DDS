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
/*______________________���������������________________:*/
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
    u32 PHA_CNT;                        // ��λ���������ֵ
    u32 FRE_CNT;                        // Ƶ�ʲ�������ֵ
    u32 CYC_CNT;                        // �������ڸ�������ֵ
    u32 ADV_CNT;                        // ��ǰ���ڼ���ֵ
    u32 LAG_CNT;                        // �ͺ����ڼ���ֵ
};
#ifdef __cplusplus
}
#endif
#endif //CPROJECT_COMPETITION_H
