#include "scale.h"

int readScaleStatus(void){
    
    int status_scale = 0;
    uint8 tmp_result = 0;
    
    for(int i = 0; i < 10; i++)
    {
        if (ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT))   //Venter på et resultat.
        {
            
            for (uint32_t i = 0; i < 10; i++){
                             
            tmp_result += ADC_SAR_1_GetResult16();
            
            CyDelay(10);
            
            }
                       
            measurement++;
            
            result = tmp_result/10;
            
            status_scale = 0;
            
            if(result > 160)
            {
                count_not_low++;
                if (count_not_low == 5)
                {
                    
                    status_scale = 0;
                    count_not_low = 0;
                }
            }
            else if(result <= 160 && result >= 70)
            {
                count_ok++;
                if (count_ok == 5)
                {
                  
                    status_scale = 1;
                    count_ok = 0;
                }
            }
            else if(result < 70)
            {
                count_not_high++;
                if (count_not_high == 5)
                {
                    
                    status_scale = 0;
                    count_not_high = 0;
                }
            }
            
           
            
            if (measurement == 10)
            {
                measurement = 0;
                count_not_high = 0;
                count_not_low = 0;
                count_ok = 0;
            }
        
        }
    }
    
    return status_scale;
}

