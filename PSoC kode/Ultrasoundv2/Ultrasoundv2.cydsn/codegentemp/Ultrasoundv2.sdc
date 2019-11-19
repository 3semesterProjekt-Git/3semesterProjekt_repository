# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\lucasiversen\Desktop\Ultrasoundv2\Ultrasoundv2.cydsn\Ultrasoundv2.cyprj
# Date: Tue, 19 Nov 2019 13:51:15 GMT
#set_units -time ns
create_clock -name {CLK_500kHz(routed)} -period 2000 -waveform {0 1000} [list [get_pins {ClockBlock/dclk_1}]]
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {CLK_24MHz} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {ADC_SAR_1_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 15 31} [list [get_pins {ClockBlock/aclk_glb_0}]]
create_generated_clock -name {CLK_500kHz} -source [get_pins {ClockBlock/clk_sync}] -edges {1 49 97} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {soundUART_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 313 627} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {clock_motor} -source [get_pins {ClockBlock/clk_sync}] -edges {1 481 961} [list [get_pins {ClockBlock/dclk_glb_3}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\lucasiversen\Desktop\Ultrasoundv2\Ultrasoundv2.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\lucasiversen\Desktop\Ultrasoundv2\Ultrasoundv2.cydsn\Ultrasoundv2.cyprj
# Date: Tue, 19 Nov 2019 13:51:06 GMT
