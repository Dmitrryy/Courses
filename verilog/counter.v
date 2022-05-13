module new_rev(
    input clk_i,
    input rst_i,   //Кнопка сброса
    input ce_i,	//Включить 
    input ctr_i,	//Управление сложением и вычитанием
    output [3:0] d_out
    );
	
	 reg[3:0]counter=0;
	 assign d_out=counter;
	always@(posedge clk_i or posedge rst_i)
		if(rst_i==1)									//Сброс
			begin
				counter<=0;
			end
		else
			if(ce_i==0)								//Включить 
				begin
					counter<=counter;
				end
			else 									//Управление сложением и вычитанием										
				if(ctr_i==1)
					case(counter)
						0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15:begin
																			if(counter==15)
																				counter<=0;
																			else counter<=counter+1;
																			end													
					endcase
				else
					case(counter)
						0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15: begin
																			if(counter==0)
																				counter<=15;
																			else
																				counter<=counter-1;
																			end
					endcase	    
endmodule