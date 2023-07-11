//*****************************************************************
//*	描    述:	待测信号二分频模块
//*	开始时间:	2015-03-14
//*	完成时间:	2015-03-14
//*	修改时间:	2015-03-15
//*	版    本:	V1.0
//*	作    者:	凌智电子
//*	说    明:	将待测信号二分频为占空比50%的信号, 解决输入信号占
//*			空比非50%时不能测量问题. 
//*	备    注:	
//*****************************************************************

module two_frequency (
	input  clk,				// 驱动时钟
	input  rst_n,			// 复位信号
	input  ina,				// 待测信号 a
	input  inb,				// 待测信号 b 

	output ina_out,		// 信号a 二分频输出
	output inb_out			// 信号b 二分频输出
);

// 待测信号输入缓存
reg  [1:0]  ina_r  = 2'b00;
reg  [1:0]  inb_r  = 2'b00;

reg  ina_two = 1'b0;
reg  inb_two = 1'b0;

reg  ina_out_reg = 1'b0;
reg  inb_out_reg = 1'b0;

assign ina_out = ina_out_reg;
assign inb_out = inb_out_reg;


//*****************************************************************
//* 对输入信号进行缓存
//*****************************************************************
always @(posedge clk) 
begin
    ina_r <= {ina_r[0], ina};
    inb_r <= {inb_r[0], inb};
end

//*****************************************************************
//* ina 信号二分频
//*****************************************************************
always @(posedge clk) 
begin
  if (ina_r == 2'b01) 		// 检测到待测信号a的上升沿
  begin      
    ina_two <= ~ina_two;   // 信号输出状态取反
  end
end

//*****************************************************************
//* inb 信号二分频
//*****************************************************************
always @(posedge clk) 
begin
  if (inb_r == 2'b01) 		// 检测到待测信号b的上升沿
  begin      
    inb_two <= ~inb_two;   // 信号输出状态取反
  end
end


//*****************************************************************
//* ina、 inb 输出缓存
//*****************************************************************
always @(posedge clk) 
begin
  ina_out_reg <= ina_two;   
  inb_out_reg <= inb_two;    
end

endmodule 

//*****************************************************************
//*                    END OF FILE 
//*****************************************************************