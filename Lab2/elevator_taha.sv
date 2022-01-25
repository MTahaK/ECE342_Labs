`timescale 1ns/1ns

module elevator 
(
  input i_clock,
  input i_reset,
  input [1:0] i_buttons,
  output [1:0] o_current_floor
);
  
  logic dp_up, dp_down, done_moving;
  
  controlpath thecontrolpath(
    .i_clock(i_clock),
    .i_reset(i_reset),
    .i_buttons(i_buttons),
    .o_current_floor(o_current_floor),
    .i_done(done_moving),
    .o_dp_up(dp_up),
    .o_dp_down(dp_down)
  );
  
  datapath thedatapath(
    .i_clock(i_clock),
    .i_reset(i_reset),
    .i_dp_up(dp_up),
    .i_dp_down(dp_down),
    .o_done(done_moving)
  );  

endmodule

module controlpath (
  input i_clock,
  input i_reset,
  input [1:0] i_buttons,
  output logic [1:0] o_current_floor,
  input i_done, 
  output logic o_dp_up,
  output logic o_dp_down
);
  
  // Declare two objects, 'state' and 'nextstate' of type enum.
  // Add more states as needed for your design. 
  enum int unsigned{
    S_G,
    S_1,
    S_2,
    S_3
  } state, nextstate;
  
  // Clocked always block for making state registers
  always_ff @ (posedge i_clock or posedge i_reset) begin
    if (i_reset) state <= S_G;
    else state <= nextstate;
  end
  
  // Buttons:
  // 0/2'b00=>Floor 1, 1/2b'01=>Floor 2, 2/2b'10=>Floor 3,
  // 3/2'b11=>Floor 4 

  always_comb begin
    
    case(state)

	S_G:
	begin
		if(i_buttons == 2'd0)
		begin
			nextstate = S_G;
			o_dp_up = 1'b0;
			o_dp_down = 1'b0;
		end
		else
		begin
			o_dp_up = 1'b1;
			o_dp_down = 1'b0;
			nextstate = S_1;
			o_current_floor = 2'd0;
		end
		if(i_done)
			o_current_floor = 2'b00;
	end

	S_1:
	begin
		if(i_buttons == 2'b00)
		begin
			nextstate = S_G;
			o_dp_up = 1'b0;
			o_dp_down = 1'b1;
		end
		else if(i_buttons == 2'b01)
		begin
			nextstate = S_1;
			o_dp_up = 1'b0;
			o_dp_down = 1'b0;
		end
		else if( (i_buttons == 2'b10) || (i_buttons == 2'b11))
		begin
			nextstate = S_2;
			o_dp_up = 1'b1;
			o_dp_down = 1'b0;
		end
		if(i_done)
			o_current_floor = 2'b01;
	end	

	S_2:
	begin
		if(i_buttons == 2'b00 || i_buttons == 2'b01)
		begin
			nextstate = S_1;
			o_dp_up = 1'b0;
			o_dp_down = 1'b1;
		end
		else if(i_buttons == 2'b10)
		begin
			nextstate = S_2;
			o_dp_up = 1'b0;
			o_dp_down = 1'b0;
		end
		else if(i_buttons == 2'b11)
		begin
			nextstate = S_3;
			o_dp_up = 1'b1;
			o_dp_down = 1'b0;
		end
		if(i_done)
			o_current_floor = 2'b10;
	end	

	S_3:
	begin
		if(i_buttons == 2'b00 || i_buttons == 2'b01 || i_buttons == 2'b10)
		begin
			nextstate = S_2;
			o_dp_up = 1'b0;
			o_dp_down = 1'b1;
		end
		else if(i_buttons == 2'b11)
		begin
			nextstate = S_3;
			o_dp_up = 1'b0;
			o_dp_down = 1'b0;
		end
		if(i_done)
			o_current_floor = 2'b10;
	end
        // Add code for your FSM here. 
    endcase
  end
endmodule

// The datapath takes a move up or move down input and waits 5 cycles 
// before asserting o_done, to indicate that it has moved 1 floor. 
// NOTE 1: You do not need to edit the data path code. 
// NOTE 2: The datapath doesn't do any error checking; so if you ask it to move down
// from G or move up from 3, it will still do that. All checks should happen
// inside the control path. 

module datapath (
  input i_clock,
  input i_reset,
  input i_dp_up,
  input i_dp_down,
  output o_done 
);
  
  logic [2:0] count;
  
  always_ff @ (posedge i_clock or posedge i_reset) begin
    if ((i_reset) || (i_dp_up) || (i_dp_down))
      count <= 3'd0;
    else
      count <= count + 3'd1;
  end
  
  assign o_done = (count == 3'd5);
          
endmodule
