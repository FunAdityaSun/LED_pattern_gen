module led_scroll(
    input clk, // clock signal
    input [1:0] mode,
    output reg [7:0] leds // 8 LEDs
);

    reg dir = 0;
    reg [7:0] pos = 8'b00000001;
    reg [7:0] lfsr = 8'b10101010;
    
    always @(posedge clk) begin
            case (mode)
                2'b00: begin
                    // 1 LED Bounce mode
                    if (dir == 0) begin
                    	if (pos == 8'b10000000)
                    		dir <= 1;
                    	else 
                    		pos <= pos << 1;
                    	leds <= pos;
                    end else begin
                    	if (pos == 8'b00000001)
                    		dir <= 0;
                    	else
                    		pos <= pos >> 1;
                    	leds <= pos;
                    end
                end

                2'b01: begin
                    // 3 LEDs wide
                    if (dir == 0) begin
                    	if (pos == 8'b10000000)
                    		dir <= 1;
                    	else 
                    		pos <= pos << 1;
                    	leds <= pos | (pos >> 1) | (pos << 1);
                    end else begin
                    	if (pos == 8'b00000001)
                    		dir <= 0;
                    	else
                    		pos <= pos >> 1;
                    	leds <= pos | (pos >> 1) | (pos << 1);
                    end
                end

                2'b10: begin
                    // Somewhat random pattern using LFSR
                    lfsr <= {lfsr[6:0], lfsr[7] ^ lfsr[5] ^ lfsr[4] ^ lfsr[3]};
                    leds <= lfsr;
                end

                default: leds <= 8'b00000000;
            endcase
    end

endmodule

