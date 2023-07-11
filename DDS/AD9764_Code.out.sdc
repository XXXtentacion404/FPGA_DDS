## Generated SDC file "AD9764_Code.out.sdc"

## Copyright (C) 1991-2013 Altera Corporation
## Your use of Altera Corporation's design tools, logic functions 
## and other software and tools, and its AMPP partner logic 
## functions, and any output files from any of the foregoing 
## (including device programming or simulation files), and any 
## associated documentation or information are expressly subject 
## to the terms and conditions of the Altera Program License 
## Subscription Agreement, Altera MegaCore Function License 
## Agreement, or other applicable license agreement, including, 
## without limitation, that your use is for the sole purpose of 
## programming logic devices manufactured by Altera and sold by 
## Altera or its authorized distributors.  Please refer to the 
## applicable agreement for further details.


## VENDOR  "Altera"
## PROGRAM "Quartus II"
## VERSION "Version 13.0.1 Build 232 06/12/2013 Service Pack 1 SJ Full Version"

## DATE    "Sat Jul 24 10:53:20 2021"

##
## DEVICE  "EP4CE10E22C8"
##


#**************************************************************
# Time Information
#**************************************************************

set_time_format -unit ns -decimal_places 3



#**************************************************************
# Create Clock
#**************************************************************

create_clock -name {altera_reserved_tck} -period 100.000 -waveform { 0.000 50.000 } [get_ports {altera_reserved_tck}]
create_clock -name {clk} -period 20.000 -waveform { 0.000 10.000 } [get_ports {clk}]
create_clock -name {pll_c0} -period 20.000 -waveform { 0.000 10.000 } [get_nets {U_PLL|altpll_component|auto_generated|wire_pll1_clk[0]}]
create_clock -name {pll_c1} -period 20.000 -waveform { 0.000 10.000 } [get_nets {U_PLL|altpll_component|auto_generated|wire_pll1_clk[1]}]


#**************************************************************
# Create Generated Clock
#**************************************************************



#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************

set_clock_uncertainty -rise_from [get_clocks {pll_c1}] -rise_to [get_clocks {pll_c1}]  0.150  
set_clock_uncertainty -rise_from [get_clocks {pll_c1}] -fall_to [get_clocks {pll_c1}]  0.150  
set_clock_uncertainty -fall_from [get_clocks {pll_c1}] -rise_to [get_clocks {pll_c1}]  0.150  
set_clock_uncertainty -fall_from [get_clocks {pll_c1}] -fall_to [get_clocks {pll_c1}]  0.150  
set_clock_uncertainty -rise_from [get_clocks {pll_c0}] -rise_to [get_clocks {pll_c0}]  0.150  
set_clock_uncertainty -rise_from [get_clocks {pll_c0}] -fall_to [get_clocks {pll_c0}]  0.150  
set_clock_uncertainty -fall_from [get_clocks {pll_c0}] -rise_to [get_clocks {pll_c0}]  0.150  
set_clock_uncertainty -fall_from [get_clocks {pll_c0}] -fall_to [get_clocks {pll_c0}]  0.150  
set_clock_uncertainty -rise_from [get_clocks {clk}] -rise_to [get_clocks {clk}]  0.150  
set_clock_uncertainty -rise_from [get_clocks {clk}] -fall_to [get_clocks {clk}]  0.150  
set_clock_uncertainty -fall_from [get_clocks {clk}] -rise_to [get_clocks {clk}]  0.150  
set_clock_uncertainty -fall_from [get_clocks {clk}] -fall_to [get_clocks {clk}]  0.150  


#**************************************************************
# Set Input Delay
#**************************************************************



#**************************************************************
# Set Output Delay
#**************************************************************



#**************************************************************
# Set Clock Groups
#**************************************************************

set_clock_groups -asynchronous -group [get_clocks {altera_reserved_tck}] 


#**************************************************************
# Set False Path
#**************************************************************



#**************************************************************
# Set Multicycle Path
#**************************************************************



#**************************************************************
# Set Maximum Delay
#**************************************************************



#**************************************************************
# Set Minimum Delay
#**************************************************************



#**************************************************************
# Set Input Transition
#**************************************************************

