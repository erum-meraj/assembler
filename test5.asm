for: 
	ldc res	
   ldnl 0   
   adc width  ; load width 
   ldc res  
   stnl 0   
   ldc height  ; load height
   ldnl 0   
   adc -1   
   ldc height  
   stnl 0   
   ldc height  
   ldnl 0   
   brz exit 
   br for 
exit: 
	HALT     ; Terminate Program 

width: SET 5
height: data 7
res: data 0
