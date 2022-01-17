module upcount15
(
    input clk,
    input sreset, // sync reset
    output [3:0] o_val,
    input i_enable,
    output o_last
);
    logic [3:0] count;
    always_ff @(posedge clk) begin
        if (sreset) count <= 0;
        else begin
            if (i_enable) begin
                if (o_last) count <= 0;
                else count <= count + 1;
            end
        end
    end

    assign o_val = count;
    assign o_last = (count == 4'b1111);

endmodule
