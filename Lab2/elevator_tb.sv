`timescale 1ns/1ns

module elevator_tb();
  
  logic i_clock;
  logic i_reset;
  logic [1:0] i_buttons;
  logic [1:0] o_current_floor;

  elevator DUT(
    .i_clock(i_clock),
    .i_reset(i_reset),
    .i_buttons(i_buttons),
    .o_current_floor(o_current_floor)
  );
  
  always #10 i_clock = ~i_clock;    
  
  task automatic move_floor(input logic [1:0] floor);
  begin          
    
    // We use this to decide how long to wait to check if the elevator has moved. 
    integer floor_change; 

    // Wait 1 cycle before changing floors. 
    @(posedge i_clock);
    $display("Pressing button on floor %d.", floor);
    i_buttons = floor;
        
    floor_change = (floor - o_current_floor);
    floor_change = (floor_change<0) ? (-floor_change):floor_change;

	// This waits 10 cycles per floor. If this is too slow for you, you can reduce it.
    repeat(10*floor_change) @(posedge i_clock);

    if (o_current_floor !== floor)
          $display("ERROR: Elevator should be floor %d, but was on floor %d instead.", floor, o_current_floor);
        else
      $display("Elevator correctly moved to floor %d.", o_current_floor);
    end

  endtask
  
  initial begin
    i_clock = '0;
    i_reset = '1;
    i_buttons = '0;
    
    @(posedge i_clock);
    i_reset = '0;   
    @(posedge i_clock);
    
    move_floor(2'd0);  
    move_floor(2'd1);

    // Add more test cases here. 
    
    $stop();
    
  end  

endmodule      
