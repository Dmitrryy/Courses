`timescale 1 ns / 100 ps

/*
*   Introduction to FPGA and Verilog
*
*   Viktor Prutyanov, 2019
*
*   Testbench for counter
*/

module testbench(); /* No inputs, no outputs */

	// Inputs
	reg clk_i;
	reg rst_i;
	reg ce_i;
	reg ctr_i;

	// Outputs
	wire [3:0] d_out;

	// Instantiate the Unit Under Test (UUT)
	new_rev uut (
		.clk_i(clk_i), 
		.rst_i(rst_i), 
		.ce_i(ce_i), 
		.ctr_i(ctr_i), 
		.d_out(d_out)
	);

	initial begin
		// Initialize Inputs
		clk_i = 0;
		rst_i = 0;
		ce_i = 0;
		ctr_i = 0; 

		// Wait 100 ns for global reset to finish
		#25;
		rst_i = 0;         //Сигнал сброса низкий и считается нормально.
		ce_i = 1;
		ctr_i = 1;
		
		# 200; // Сигнал сброса высокий
		rst_i = 1;
		ce_i = 1;
		ctr_i = 1;
		
		# 25; // Продолжаем добавлять как обычно
		rst_i = 0;
		ce_i = 1;
		ctr_i = 1;
		
		#100;
		rst_i = 0;			//Разрешающий порт закрывается, счет останавливается, а текущее значение сохраняется.
		ce_i = 0;
		ctr_i = 1;
		
		#50;
		rst_i = 0;			//Порт включения открыт, подсчет продолжается
		ce_i = 1;
		ctr_i = 1;
		
		#100;
		rst_i = 0;			//Обратный отсчет
		ce_i = 1;
		ctr_i = 0;
		
		
        
		// Add stimulus here
        $dumpvars;      /* Open for dump of signals */
        $display("Test started...");   /* Write to console */
        #100 $finish;    /* Stop simulation after 10ns */

	end
	
	always#5 clk_i=~clk_i;

      
endmodule

