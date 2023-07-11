// 延时消抖
module delay_debounce#(
	parameter DELAY_PERIOD    = 12        // 延时周期数
)
(
	input  clk,
   input  din,
   output dout
);
  
reg [DELAY_PERIOD-1:0] din_reg;
reg dout_reg;
  
reg H_reg;
reg L_reg;
  
always @(posedge clk) 
begin
	din_reg <= {din_reg[DELAY_PERIOD-2:0], din};
end 

always @(posedge clk) 
begin
	if (din_reg == {DELAY_PERIOD{1'b1}}) 
	begin 
		H_reg <= 1'b1;
   end 
   else 
	begin
      H_reg <= 1'b0;
    end 
 end 

always @(posedge clk) 
begin
   if (din_reg == {DELAY_PERIOD{1'b0}}) 
	begin 
      L_reg <= 1'b1;
    end 
    else 
	 begin
      L_reg <= 1'b0;
    end 
end 



always @(posedge clk) 
begin
    if ( H_reg ) 
	 begin
      dout_reg <= 1'b1;
    end 
    else if ( L_reg ) 
	 begin
      dout_reg <= 1'b0;
    end 
    else 
	 begin
      dout_reg <= dout_reg;
    end 
end 

assign dout = dout_reg;

endmodule 

