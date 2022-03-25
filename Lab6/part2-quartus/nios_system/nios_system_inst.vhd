	component nios_system is
		port (
			clk_clk : in std_logic := 'X'  -- clk
		);
	end component nios_system;

	u0 : component nios_system
		port map (
			clk_clk => CONNECTED_TO_clk_clk  -- clk.clk
		);

