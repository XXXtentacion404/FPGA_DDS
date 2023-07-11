module AD9764_Code(
	input clk,
	input KEY1,//开始扫频按键
	input KEY2,//切换波形
	input KEY3,//
	output dac_clk1,
	output dac_clk2,
	output [13:0] dac_data1,
	output [13:0] dac_data2,
	input in_a,				//待测信号 a 输入
	input in_b,				//待测信号 b 输入
	
	//spi通信模块
	input  spi_sdi,      // STM32发送数据,FPGA接收
   input  spi_cs_data,  // 数据片选端
   input  spi_cs_cmd,   // 命令片选端
   input  spi_scl,      // spi时钟线
   output spi_sdo       // STM32接收数据,FPGA发送
);

wire clk_125m;
wire rst;
wire [2:0] sel;
wire [31:0] fre_k;
wire [11:0] addr;
wire [13:0] wave_z;
wire [13:0] wave_s;
wire [13:0] wave_f;

wire [13:0] dac_data;
reg [13:0] data_buf1;
reg [13:0] data_buf2;
//spi寄存器模块
wire [31:0] fre_k_start;
wire [31:0] fre_k_end;
wire [31:0] step;
wire [31:0] cycle;
wire [31:0] sel_wave;
// 锯齿波数据
assign wave_s = {addr,2'b00};

// 方波数据
assign wave_f = addr[11] ? 14'b11_1111_1111_1111 : 14'b00_0000_0000_0000;

assign dac_data1 = data_buf1;
assign dac_data2 = data_buf2;
wire                       test_stop;
wire                       ina_buf_out;
wire                       inb_buf_out;
wire                       ina_two_out;
wire                       inb_two_out;
wire                       or_out;
wire                       ina_cs_out;
wire                       inb_cs_out;
wire                       cs_en;
wire                       cyc_en;

wire    [31:0]             fre_cnt;
wire    [31:0]             pha_cnt;
wire    [31:0]             cyc_cnt;
wire    [31:0]             adv_cnt;
wire    [31:0]             lag_cnt;
wire                       test_en;
wire                       up_data;
wire   							cnt_clr;
wire   							time_out;

wire    [31:0]             spi_test;
wire    [7:0]              sel_cmd;
wire    [31:0]             read_start;
wire                       read_en;
wire                       pha_shift_a;
wire                       pha_shift_b;

wire                			in_a_test;
wire                			in_b_test;

// CH ina
delay_debounce u_delay_debounce_sa
(
 .clk(clk1),
 .din(in_a),
 .dout(in_a_test)
);
defparam u_delay_debounce_sa.DELAY_PERIOD    = 16;  

// CH inb
delay_debounce u_delay_debounce_sb
(
 .clk(clk),
 .din(in_b),
 .dout(in_b_test)
);
defparam u_delay_debounce_sb.DELAY_PERIOD    = 16; 


// 锁相环
PLL U_PLL(
	.inclk0(clk),			// 外部时钟输入 50MHz
	.c0(clk_125m),
	.c1(dac_clk1),
	.c2(dac_clk2),
	.locked(rst)
);

//*****************************************************************
//* pll 例化, 为系统提供100MHz时钟信号.
//*****************************************************************
pllm pll_u1(
	.inclk0(clk),		// 外部时钟输入 50MHz
	.c0(clk1)					// 系统时钟 150MHz
);

//*****************************************************************
//* 待测信号缓存模块例化, 使待测信号与系统时钟同步.
//*****************************************************************
in_buf in_buf_u1(
	.clk(clk1),				// 驱动时钟
	.in_a(in_a_test),		// 待测信号 a
	.in_b(in_b_test),		// 待测信号 b
	.q_a(ina_buf_out),	// 待测信号 a 缓存输出
	.q_b(inb_buf_out)		// 待测信号 b 缓存输出
);
//*****************************************************************
//* 待测信号二分频模块例化, 解决待测信号占空比非50%不能测量问题.
//*****************************************************************
two_frequency two_frequency_u1(
	.clk(clk1),					// 驱动时钟
	.rst_n(rst_n),				// 复位信号
	.ina(ina_buf_out),		// 待测信号 a 
	.inb(inb_buf_out),		// 待测信号 b 
	.ina_out(ina_two_out),	// 待测信号 a 二分频输出
	.inb_out(inb_two_out)	// 待测信号 b 二分频输出
);
//*****************************************************************
//* 超时检测模块例化, 防止没信号输入时系统假死.
//*****************************************************************
time_cs time_cs_u1(
	.clk(clk1),					// 驱动时钟
	.rst_n(rst_n),				// 复位信号
	.ina(ina_two_out),		// 待测信号 a 二分频信号输入
	.inb(inb_two_out),		// 待测信号 b 二分频信号输入
	.ina_out(ina_cs_out),	// 待测信号 a 二分频信号输出
	.inb_out(inb_cs_out),	// 待测信号 b 二分频信号输出
	.test_en(cs_en),			// 测量闸门信号
	.cnt_clr(cnt_clr),
	.up_data(up_data),
	.z_out(time_out)			// 超时指示信号
);
//*****************************************************************
//* 计数使能信号产生模块例化, 主要实现输入信号的异或功能
//*****************************************************************
buf_or buf_or_u1(
	.clk(clk1),					// 驱动时钟
	.rst_n(rst_n),				// 复位信号
	.ina(ina_cs_out),			// 待测信号 a 二分频信号输入
	.inb(inb_cs_out),			// 待测信号 b 二分频信号输入
	.o_en(cs_en),				// 测量闸门信号输入
	.ina_out(cyc_en),			// 待测信号 a 二分频信号输出
	.inb_out(pha_shift_b),	// 待测信号 b 二分频信号输出
	.cnt_en(test_en),			// 频率计数使能信号
	.q(or_out)					// 相位计数使能信号(异或输出)
);
//*****************************************************************
//* 计数模块例化, 包括频率、相位、周期个数、超前、滞后计数.
//*****************************************************************
cnt_module cnt_module_u1(
	.clk(clk1),					// 驱动时钟
	.rst_n(rst_n),				// 复位信号
	.inb_in(pha_shift_b),	// 待测信号 a 二分频信号输入
	.ina_in(cyc_en),			// 待测信号 b 二分频信号输入
	.cnt_en(test_en),			// 频率计数使能信号输入
	.or_en(or_out),			// 相位计数使能信号输入
	.read_en(read_en),		// 计数值更新输出控制信号(由STM32控制)
	.cnt_clr( cnt_clr),		
	.up_data (up_data),
	.q_up(time_out),			// 超时指示信号输入
	.a_cnt(fre_cnt),			// 频率计数值输出
	.b_cnt(pha_cnt),			// 相位计数值输出
	.c_cnt(cyc_cnt),			// 测量周期个数计数值输出
	.e_cnt(adv_cnt),			// 超前周期个数计数值输出
	.f_cnt(lag_cnt)			// 滞后周期个数计数值输出
);

// 检测控制
key_con	u_key_con(
	.clk					(		clk_125m	),
	.rst_n				(		rst		),
	.key1_in				(		KEY1		),//开始扫频按键
	.key2_in				(		KEY2		),//切换波形
	.key3_in				(		KEY3		),
	.sel_wave			(		sel		),
	.SPI_fre_start		(	fre_k_start	),
	.SPI_fre_end		(	fre_k_end	),
	.SPI_step			(		step		),
	.SPI_cycle			(		cycle		),
	.fre_k				(		fre_k		)
	
);

spi u_spi(
	.clk				(clk),						// 系统时钟
   .rst_n			(rst),					// 复位
   .spi_sdi			(spi_sdi),			// STM32发送数据,FPGA接收
   .spi_cs_data	(spi_cs_data),	// 数据片选端
   .spi_cs_cmd		(spi_cs_cmd),	// 命令片选端
   .spi_scl			(spi_scl),			// spi时钟线
   .spi_sdo			(spi_sdo),			// STM32接收数据,FPGA发送
	
		 // reg out port
   .write_reg_0(spi_test),		// SPI 通信测试数据输出通道 	（通道0）
   .write_reg_1(), 				// FPGA -> STM32 通道2
   .write_reg_2(fre_cnt), 		// 频率计数值输出通道 			（通道2）
   .write_reg_3(pha_cnt), 		// 相位差计数值输出通道 		（通道3）
   .write_reg_4(cyc_cnt), 		// 测量周期个数计数值输出通道	（通道4）
   .write_reg_5(adv_cnt), 		// 超前计数值输出通道 			（通道5）
   .write_reg_6(lag_cnt), 		// 滞后计数值输出通道 			（通道6）


	 // reg in port
   .read_reg_0(	fre_k_start	),	 // STM32 -> FPGA 通道0
   .read_reg_1(	fre_k_end	),  // STM32 -> FPGA 通道1
   .read_reg_2(		step		),  // STM32 -> FPGA 通道2
   .read_reg_3(		cycle		),  // STM32 -> FPGA 通道3
	.read_reg_4(	spi_test   	),		// SPI 通信测试输入通道		（通道0）
   .read_reg_5(	read_start	),  	// 计数值输出更新控制信号   	（通道1）
);
// 累加器
add_32bit	u_add_32bit(
	.clk(clk_125m),
	.rst(rst),
	.fr_k(fre_k),
	.adder(addr)
);

// 正弦信号表
ROM_Sin	u_ROM_Sin(
	.clock(clk_125m),
	.address(addr),
	.q(wave_z)
);

// 输出波形选择
sel_wave u_sel_wave(
	.clk(clk_125m),
	.rst_n(rst),
	.sel(sel),
	.da_ina(wave_z),
	.da_inb(wave_s),
	.da_inc(wave_f),
	.da_out(dac_data)
);

// 硬件电路输出反相，即数字量0对应最大模拟量输出，数字亮4095对应最小模拟量输出。
// 通过16383-dac_data即可调整过来。
// 两通道数据保持一致输出。
always @(posedge clk_125m) 
begin 
	data_buf1 <= 14'h3FFF - dac_data;
	data_buf2 <= 14'h3FFF - dac_data;
end 
defparam
	u_spi.DATA_WIDTH      = 32,
	u_spi.CHANNEL_NUMBER  = 16;
	assign  read_en = read_start[0];
endmodule 


