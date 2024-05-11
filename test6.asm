call totalN
HALT

totalN:     stl 0       
            adj -1      
    loop:   ldc num
            ldnl 0      
            ldc total
            ldnl 0      
            add         
            ldc total
            stnl 0      
            ldc num
            ldnl 0      
            adc 1      
            brz done    
            ldc num
            stnl 0      
            br loop     
    done:   
            adj 1       
            ldl 0       
            return      

num: data -3      
total: data 0         
