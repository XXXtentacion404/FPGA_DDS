//*****************************************************************
//*	描    述:	超时检测模块
//*	开始时间:	2015-03-14
//*	完成时间:	2015-03-14
//*	修改时间:	2015-03-15
//*	版    本:	V1.0
//*	作    者:	凌智电子
//*	说    明:	测量时间超过规定时间后, 更新数据.
//*	备    注:	
//*****************************************************************

module time_cs (
	input  clk,			// 驱动时钟
	input  rst_n,		// 复位信号
	input  ina,			// 待测信号 a 输入
	input  inb,			// 待测信号 b 输入

	output ina_out,	// 待测信号 a 输出
	output inb_out,	// 待测信号 b 输出
	output test_en,	// 相位测量有效信号
	output cnt_clr,	// 计数清零
	output up_data,	// 计数清零
	output z_out		// 超时指示信号
);

localparam [31:0]         FRE_CLK     = 150000000;   // 150MHz
localparam [31:0]         TIME_OUT    = 4200;        // 4.2s
localparam [31:0]         TRICK_TIME  = 500;         // 500ms

localparam    time_out_max    = ( FRE_CLK / 1000 ) * TIME_OUT;      // 超时时间 2s / 150M
localparam    trick_time_min  = ( FRE_CLK / 1000 ) * TRICK_TIME;    // 砸门时间 500ms / 150M

localparam    STATE_IDLE    = 2'b00;
localparam    STATE_START   = 2'b01;
localparam    STATE_WAIT    = 2'b10;
localparam    STATE_STOP    = 2'b11;

parameter    TIME_CNT_WIDTH  = 32;
parameter    TRICK_CNT_WIDTH = 32;

reg [1:0] state = STATE_IDLE;

// 超时检测计数器和测量闸门计数器
wire     [TIME_CNT_WIDTH  - 1:0]        time_out_cnt;
wire     [TRICK_CNT_WIDTH - 1:0]        trick_time_cnt;
// reg
reg  [4:0]	ina_r = 5'd0;
reg  [4:0]  inb_r = 5'd0;

reg  updata_reg  = 1'b0;
reg  time_out_reg = 1'b0;
reg  aclr_reg = 1'b0;
reg  trick_count_en = 1'b0;
reg  time_out_flag = 1'b0;

reg  test_ok = 1'b0;

reg  external_signal_rising = 1'b0;

assign test_en = trick_count_en;

assign cnt_clr = aclr_reg;
assign up_data = updata_reg;

assign z_out   = time_out_reg;


assign ina_out = ina_r[3];
assign inb_out = inb_r[3];



//*****************************************************************
//* ina_out
//* inb_out
//*****************************************************************
always @(posedge clk) 
begin
  ina_r  <= {ina_r[3:0], ina};
  inb_r  <= {inb_r[3:0], inb};
end


always @(posedge clk) 
begin
  case (state)
    STATE_IDLE  : begin
      if (external_signal_rising == 1) // 待测信号上升沿到来
		begin  
        state <= STATE_START;
      end 
    end
	 
    STATE_START : begin 
      state <= STATE_WAIT;
    end
	 
    STATE_WAIT  : begin 
      if ( test_ok ) 		// 待测信号上升沿到来且砸门时间结束
		begin            
        state <= STATE_STOP;
      end 
      if ( time_out_flag ) 
		begin
        state <= STATE_STOP;
      end 
    end 
	 
    STATE_STOP  : begin 
      state <= STATE_IDLE;
    end 
  endcase 
end 




always @(posedge clk) 
begin 
  if ( ina_r[1:0] == 2'b01 ) 
  begin 
    external_signal_rising <= 1'b1;
  end 
  else 
  begin 
    external_signal_rising <= 1'b0;
  end 
end 

always @(posedge clk) 
begin
  test_ok <=  (ina_r[1:0] == 2'b01) & (trick_time_cnt > trick_time_min);
end 

always @(posedge clk) 
begin
  if ( time_out_cnt >= time_out_max ) 
  begin 
    time_out_flag <= 1'b1;
  end 
  else 
  begin 
    time_out_flag <= 1'b0;
  end 
end 


always @(posedge clk) begin
  if ( state == STATE_STOP ) 
  begin 
    updata_reg <= 1'b1;
  end 
  else 
  begin 
    updata_reg <= 1'b0;
  end 
end 

always @(posedge clk) 
begin
  if ( time_out_flag ) 
  begin 
    time_out_reg <= 1'b1;
  end 
  else 
  begin 
    time_out_reg <= 1'b0;
  end 
end 

always @(posedge clk) 
begin
  if ( state == STATE_START ) 
  begin 
    aclr_reg <= 1'b1;
  end 
  else 
  begin 
    aclr_reg <= 1'b0;
  end 
end 

always @(posedge clk) 
begin
  if ( state == STATE_WAIT ) 
  begin 
    trick_count_en <= 1'b1;
  end 
  else 
  begin 
    trick_count_en <= 1'b0;
  end 
end 

counter_32bit time_out_count (
  .aclr               ( updata_reg      ),
  .clk_en             ( 1'b1            ),
  .clock              ( clk             ),
  .cnt_en             ( 1'b1            ),
  .q                  ( time_out_cnt    )
);

counter_32bit trick_count (
  .aclr               ( updata_reg      ),
  .clk_en             ( 1'b1            ),
  .clock              ( clk             ),
  .cnt_en             ( trick_count_en  ),
  .q                  ( trick_time_cnt  )
);

endmodule


//*****************************************************************
//*                    END OF FILE 
//*****************************************************************