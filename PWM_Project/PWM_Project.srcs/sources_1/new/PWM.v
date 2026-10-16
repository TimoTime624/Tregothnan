`timescale 1ns / 1ps

module PWM(
    input CLK,
    input [9:0] SW,
    output reg [0:0] pulse
    
    );
    
    localparam integer DIV = 5;
    localparam integer WID = 1024;
    
    reg [9:0] counter = 0;
    reg [2:0] clk_div = 0;
    
    always @(posedge CLK) begin
        if(clk_div == (DIV - 1))
            clk_div <= 0;
        else
            clk_div <= clk_div + 1;
    end

    wire pwm_clk = (clk_div == 0);
    
    always @(posedge CLK) begin
        if(pwm_clk) begin
            if(counter >= WID - 1)
                counter <= 0;
            else
                counter <= counter + 1;
        end
    end


    always @(posedge CLK) begin
        if(counter < SW)
            pulse <= 1;
        else
            pulse <= 0;
    end
    
endmodule
