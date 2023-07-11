//*****************************************************************
//*	描    述:	高速计数模块
//*	开始时间:	2015-03-14
//*	完成时间:	2015-03-14
//*	修改时间:	2015-03-15
//*	版    本:	V1.0
//*	作    者:	凌智电子
//*	说    明:	对频率、相位差、测量周期、超前、滞后计数
//*	备    注:	
//*****************************************************************

module cnt_module (
	input clk,						// 驱动时钟
	input rst_n,					// 复位信号
	input cnt_en,					// 频率测量计数使能信号
	input or_en,					// 相位测量计数使能信号
	input ina_in,					// 待测信号 a 输入
	input inb_in,					// 待测信号 b 输入
	input read_en,					// 计数值更新控制信号
	input cnt_clr,
	input up_data,
	input q_up,						// 超时控制信号
	
	output reg [31:0] a_cnt,	// 频率计数
	output reg [31:0] b_cnt,	// 相位计数
	output reg [31:0] c_cnt,	// 测量周期计数
	output reg [31:0] e_cnt,	// 超前计数
	output reg [31:0] f_cnt		// 滞后计数
);

wire [31:0] fre_cnt;
wire [31:0] pha_cnt;
wire [31:0] cyc_cnt;
wire [31:0] adv_cnt;
wire [31:0] lag_cnt;

reg  [1:0]  ina_in_r  = 1'b0;
reg  [1:0]  inb_in_r = 1'b0;

reg  fre_cnt_en = 1'b0;
reg  pha_cnt_en = 1'b0;
reg  cyc_cnt_en = 1'b0;
reg  adv_cnt_en = 1'b0;
reg  lag_cnt_en = 1'b0;


always @(posedge clk) 
begin
  ina_in_r  <= {ina_in_r[0], ina_in};
  inb_in_r  <= {inb_in_r[0], inb_in};
end

always @(posedge clk) 
begin
  fre_cnt_en <= cnt_en;
  pha_cnt_en <= or_en;
end 
 
always @(posedge clk) 
begin
  if ( (ina_in_r == 2'b01) & cnt_en)
    cyc_cnt_en <= 1'b1;
  else 
    cyc_cnt_en <= 1'b0;
end 
 
always @(posedge clk) 
begin
  if ( (ina_in_r == 2'b01) & inb_in_r[0] & cnt_en)
    adv_cnt_en <= 1'b1;
  else 
    adv_cnt_en <= 1'b0;
end 
 
always @(posedge clk) 
begin
  if ( (inb_in_r == 2'b01) & ina_in_r[0] & cnt_en)
    lag_cnt_en <= 1'b1;
  else 
    lag_cnt_en <= 1'b0;
end 

//*****************************************************************
//* 频率测量计数
//*****************************************************************
cntt cntt_u1 (
  .clock     ( clk          ),          // 驱动时钟
  .cnt_en    ( fre_cnt_en   ),          // 频率计数使能信号为1
  .sclr      ( cnt_clr      ),          // 复位或测量结束后
  .q         ( fre_cnt      )           // 频率计数值输出
);

//*****************************************************************
//* 相位差测量计数
//*****************************************************************
cntt cntt_u2 (
  .clock    ( clk           ),          // 驱动时钟
  .cnt_en   ( pha_cnt_en    ),          // 相位计数使能为1 （两输入信号异或）
  .sclr     ( cnt_clr       ),          // 复位或测量结束后
  .q        ( pha_cnt       )           // 相位差计数值输出
);

//*****************************************************************
//* 周期个数计数
//*****************************************************************
cntt cntt_u3 (
  .clock    ( clk           ),        // 驱动时钟
  .cnt_en   ( cyc_cnt_en    ),        // in_a上升沿到来且计数使能信号为1
  .sclr     ( cnt_clr       ),        // 复位或测量结束后
  .q        ( cyc_cnt       )         // 测量周期个数计数值输出
);

//*****************************************************************
//* ina相对于inb 相位超前次数计数
//*****************************************************************
cntt cntt_u4 (
  .clock    ( clk           ),        // 驱动时钟
  .cnt_en   ( adv_cnt_en    ),        // in_a上升沿到来且in_b为1
  .sclr     ( cnt_clr       ),        // 复位或测量结束后
  .q        ( adv_cnt       )         // 超前计数值输出
);

//*****************************************************************
//* ina相对于inb 相位滞后次数计数
//*****************************************************************
cntt cntt_u5 (
  .clock    ( clk           ),        // 驱动时钟
  .cnt_en   ( lag_cnt_en    ),        // in_b上升沿到来且in_a信号为高电平
  .sclr     ( cnt_clr       ),        // 复位或者测量结束后
  .q        ( lag_cnt       )         // 滞后计数值输出
);

reg  up_data_flag  = 1'b0;
always @(posedge clk) begin
  up_data_flag <= ((~read_en && q_up) | up_data);
end 
//*****************************************************************
//* 计数值输出控制，其更新条件为：read_en为0且 q_up为1（超时）或者
//*  up_data为1(测量结束)。
//*****************************************************************
always @(posedge clk) begin
  if ( up_data_flag ) begin
    a_cnt <= fre_cnt;      // 频率测量计数值
    b_cnt <= pha_cnt;      // 相位差测量计数值
    c_cnt <= cyc_cnt;      // 测量周期个数计数值
    e_cnt <= adv_cnt;      // 超前周期个数计数值
    f_cnt <= lag_cnt;      // 滞后周期个数计数值
  end
end


endmodule 

//*****************************************************************
//*                    END OF FILE 
//*****************************************************************