#include "scale.h"

int readScaleStatus(void){
    
    ADC_SAR_1_StartConvert();
    
    clearScaleVariables();
    
    for(int i = 0; i < 20; i++)
    {
        if (ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT))   //Venter på et resultat.
        {
            result = ADC_SAR_1_GetResult16();  
            
            CyDelay(10);
            
            measurement++;
            
            if(result < 3500)
            {
                count_ok++;
                if (count_ok == 8)
                {
                    ADC_SAR_1_StopConvert();
                    status_scale = 1;
                    return status_scale;
                }
            }
            else if(result > 3500)
            {
                count_not_ok++;
                if (count_not_ok == 8)
                {
                    ADC_SAR_1_StopConvert();
                    status_scale = 0;
                    return status_scale;
                }
            }
        }
    }
    
    return 0;
}

void clearScaleVariables(){
    status_scale = 0;
    result = 0;
    count_not_ok = 0;
    count_ok = 0;
    measurement = 0;
}


    