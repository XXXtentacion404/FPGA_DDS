module key_con (
	input clk, 
	input rst_n,
	input key1_in,  // 开始扫频按键
	input key2_in,		//切
	input key3_in,
	input	 [31:0]	SPI_fre_start,
	input  [31:0]	SPI_fre_end,
	input  [31:0]	SPI_step,
	input  [31:0]	SPI_cycle,
	output [2:0] 	sel_wave,
	output [31:0]  fre_k
);

	reg  [31:0]    fre;
	reg  [31:0]   	fre_start;
	reg  [31:0]		fre_end;
	reg  [31:0]		fre_step;
	reg  [31:0]		cycle;
	reg  [1:0]		cnt;
	reg  [1:0] 		status;
	reg  [2:0]     sel;
	reg  [31:0]    cycle_count;
	reg  [31:0]		cnt_1;

	initial begin
		fre <=  32'd34300;
		cnt <= 2'd0;
		status <= 2'd0;
		cycle_count <= 32'd0;
		cnt_1<=32'd0;
	end

	wire             key1_out;
	wire             key2_out;

	
	// delay 
key_delay u_key1_delay (
		.clk(clk),
		.kin(key1_in),
		.kout(key1_out)
	);
key_delay u_key2_delay (
		.clk(clk),
		.kin(key2_in),
		.kout(key2_out)
	);

key_delay u_key3_delay (
		.clk(clk),
		.kin(key3_in),
		.kout(key3_out)
	);
always @(negedge key1_out) begin //完成初始化
	//fre_start<=SPI_fre_start;
	//fre_end<=SPI_fre_end;
//	fre_step<=SPI_step;
//	cycle<=SPI_cycle;
		if(status < 3) begin
			status <= status + 1;
		end else begin 
			status <= 2'd0;
		end
end
always@ (posedge clk)begin
	//if(status==0)
//	begin
	//	fre_start<=32'd34300;
//	end
	if(status==1)begin
		fre_start<=SPI_fre_start;
		fre_end<=SPI_fre_end;
		fre_step<=SPI_step;
		cycle<=SPI_cycle;
	end
	if(status==2)begin
		fre_start	<=	32'd34300;
		fre_end		<=	32'd3430000;
		fre_step		<=	32'd8575;
		cycle			<=	32'd93_750_000;
end


end
always@(posedge clk or negedge rst_n)//将时钟上升沿作为敏感信号或者复位信号的下降沿作为敏感信号，只要其中发生变化，就执行always里面的内容
		if(rst_n==1'b0)			//复位状态是为0的时候。
			cycle_count<=32'd0;
			else if(status==1)
			begin		//状态1，由STM32决定
					if(cycle_count==cycle)			//由于记数是从0开始的
					cycle_count<=32'd0;			//如果加了12_500_000次，就置零
					else if(cycle_count<cycle)
					cycle_count<=cycle_count+1'b1;
			end
		else if(status==2)
			begin		//状态2，开始慢慢扫频
			 if(cycle_count==cycle)
			 begin		//由于记数是从0开始的
				cycle_count<=32'd0;			//如果加了100_000_000次，就置零
				cnt_1=cnt_1+1'b1;				//计数cnt
				end
				else if(cycle_count<cycle)
				cycle_count<=cycle_count+1'b1;
		end
always @ (negedge clk or negedge rst_n)//自动步进
begin 
	if(!rst_n)
		fre<= 0;
		else if(status==1)begin
				 if (fre>fre_end) //如果超过截止频率就复位
						fre<=fre_end;
					else if(cycle_count==cycle)//每0.1秒步进一次
					fre<=fre+fre_step;
			end
		else if(status==2)
				begin
					if (cnt_1==1)
						begin
						fre<=32'd240100;
						//	cnt_1<= cnt + 1;
						end
					else if(cnt_1<=21)
						begin
						if (fre>fre_end) //如果超过截止频率就复位
							fre<=fre_end;
						else if(cycle_count==cycle)//每0.1秒步进一次
							fre<=fre+fre_step;
						end
					else if(cnt_1>20)		//此时频率为12K，开始快速步进
						begin
							if (fre>fre_end) //如果超过截止频率就停止
								fre<=fre_end;
							else if(cycle_count==cycle)//每0.1秒步进一次
								 fre<=fre+32'd452760;
						end
				end
end

	
	// 输出波形选择
	always @(negedge key2_out) begin 
		if(cnt < 3) begin
			cnt <= cnt + 1;
		end else begin 
			cnt <= 2'd0;
		end
	end
	always @(posedge clk) begin 
		case (cnt) 
			2'b00 : sel <= 3'b110;
			2'b01 : sel <= 3'b101;
			2'b10 : sel <= 3'b011;
			default : sel <= 3'b110;
		endcase 
	end 

	assign fre_k = fre;
	assign sel_wave = sel;

endmodule
