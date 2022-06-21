-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2019 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu

 signal sel_MX1 : std_logic;
 signal sel_MX2 : std_logic;
 signal sel_MX3 : std_logic_vector(1 downto 0) := (others => '0');

 signal MX2_out : std_logic_vector(12 downto 0) := "1000000000000";
 signal pc_counter : std_logic_vector(12 downto 0) := (others => '0');
 signal ptr_counter : std_logic_vector(12 downto 0) := "1000000000000";
 signal cnt_counter : std_logic_vector(7 downto 0) := (others => '0');

 signal pc_dec : std_logic := '0';
 signal pc_inc : std_logic := '0';
 signal ptr_dec : std_logic := '0';
 signal ptr_inc : std_logic := '0';
 signal cnt_dec : std_logic := '0';
 signal cnt_inc : std_logic := '0';

 type instr_type is (
 ins_shr, ins_shl, ins_inc, ins_dec,
 ins_bwh, ins_ewh, ins_wr, ins_rd,
 ins_st_tmp, ins_ld_tmp, ins_halt, ins_undef
 );

 signal instr: instr_type;

 -- Stavy konecneho automatu
 type states_type is(
 shr, shl, inc_1, inc_2, dec_1, dec_2, bwh_1, bwh_2, bwh_3, bwh_4,
 ewh_1, ewh_2, ewh_3, ewh_4, ewh_5, wr_1, wr_2, rd_1, rd_2, st_tmp_1, st_tmp_2, ld_tmp_1, ld_tmp_2,
 halt, undef, fetch, decod
 );

 signal actual_state : states_type;
 signal next_state : states_type;

begin

-- COUNTER pro pocitani WHILU
while_counter: process (RESET, CLK)
begin
	if(RESET = '1') then
		cnt_counter <= (others => '0');
	elsif (CLK'event) and (CLK = '1') then
		if (cnt_inc = '1') then
			cnt_counter <= cnt_counter + 1;
		elsif (cnt_dec = '1') then
			cnt_counter <= cnt_counter - 1;
		end if;
	end if;
end process;

-- COUNTER PC
program_counter: process (RESET, CLK)
begin
	if(RESET = '1') then
		pc_counter <= (others => '0');
	elsif (CLK'event) and (CLK = '1') then
		if (pc_inc = '1') then
			pc_counter <= pc_counter + 1;
		elsif (pc_dec = '1') then
			pc_counter <= pc_counter - 1;
		end if;
	end if;
end process;

-- COUNTER PTR
pointer_counter: process (RESET, CLK)
begin
	if(RESET = '1') then
		ptr_counter <= "1000000000000";
	elsif(CLK'event) and (CLK = '1') then
		if(ptr_inc = '1') then
			if(ptr_counter = X"1FFF") then
				ptr_counter <= "1000000000000";
			else
				ptr_counter <= ptr_counter + 1;
			end if;
		elsif(ptr_dec = '1') then
			if(ptr_counter = X"1000") then
				ptr_counter <= "1111111111111";
			else
				ptr_counter <= ptr_counter - 1;
			end if;
		end if;
	end if;
end process;

-- Dekoder instrukci
decoder: process (DATA_RDATA)
begin
	case (DATA_RDATA) is
		when X"3E" => instr <= ins_shr;
		when X"3C" => instr <= ins_shl;
		when X"2B" => instr <= ins_inc;
		when X"2D" => instr <= ins_dec;
		when X"5B" => instr <= ins_bwh;
		when X"5D" => instr <= ins_ewh;
		when X"2E" => instr <= ins_wr;
		when X"2C" => instr <= ins_rd;
		when X"24" => instr <= ins_st_tmp;
		when X"21" => instr <= ins_ld_tmp;
		when X"00" => instr <= ins_halt;
		when others => instr <= ins_undef;
	end case;
end process;

-- MULTIPLEXOR MX1
MX1: process (CLK, sel_MX1, pc_counter, MX2_out)
begin
	if(sel_MX1 = '0') then
		DATA_ADDR <= MX2_out;
	else
		DATA_ADDR <= pc_counter;
	end if;
end process;

-- MULTIPLEXOR MX2
MX2: process (CLK, sel_MX2, ptr_counter)
begin
	if(sel_MX2 = '0') then
		MX2_out <= "1000000000000";
	else
		MX2_out <= ptr_counter;
	end if;
end process;

-- MULTIPLEXOR MX3
MX3: process (CLK, sel_MX3, DATA_RDATA, IN_DATA)
begin
	if(sel_MX3 = "00") then
		DATA_WDATA <= DATA_RDATA;
	elsif(sel_MX3 = "01") then
		DATA_WDATA <= DATA_RDATA + 1;
	elsif(sel_MX3 = "10") then
		DATA_WDATA <= DATA_RDATA - 1;
	else
		DATA_WDATA <= IN_DATA;
	end if;
end process;

-- ------------------------------------KONECNY_AUTOMAT---------------------------------
-- ------------------------------------------------------------------------------------

-- Spusteni automatu
FSM_start: process (RESET, CLK)
begin
	if(RESET = '1') then
		actual_state <= fetch;
	elsif(CLK'event) and (CLK = '1') then
		if(EN = '1') then
			actual_state <= next_state;
		end if;
	end if;
end process;

-- Reprezentace AUTOMATU
FSM: process (IN_VLD, OUT_BUSY, DATA_RDATA, actual_state, instr, pc_counter, ptr_counter, cnt_counter, sel_MX1, sel_MX2, sel_MX3)
begin
	
	DATA_EN <= '0';
	DATA_RDWR <= '0';

	IN_REQ <= '0';
	OUT_WE <= '0';

	pc_inc <= '0';
	pc_dec <= '0';
	ptr_inc <= '0';
	ptr_dec <= '0';
	cnt_inc <= '0';
	cnt_dec <= '0';

	case actual_state is
		  
		-- Nacteni instrukce
		when fetch =>
			next_state <= decod;
			DATA_EN <= '1';
			sel_MX1 <= '1';

		-- Prevod instrukci na stavy automatu
		when decod =>
			case instr is
				when ins_shr => next_state <= shr;
				when ins_shl => next_state <= shl;
				when ins_inc => next_state <= inc_1;
				when ins_dec => next_state <= dec_1;
				when ins_bwh => next_state <= bwh_1;
				when ins_ewh => next_state <= ewh_1;
				when ins_wr =>  next_state <= wr_1;
				when ins_rd =>  next_state <= rd_1;
				when ins_st_tmp => next_state <= st_tmp_1;
				when ins_ld_tmp => next_state <= ld_tmp_1;
				when ins_halt => next_state <= halt;
				when ins_undef => next_state <= undef;
			end case;
	
		-- ----------------SHR--------------
		when shr =>
			next_state <= fetch;
			ptr_inc <= '1';
			pc_inc <= '1';
		-- ---------------------------------
		-- ----------------SHl--------------
		when shl => 
			next_state <= fetch;
			ptr_dec <= '1';
			pc_inc <= '1';
		-- ---------------------------------
		-- ----------------INC--------------
		when inc_1 =>
			next_state <= inc_2;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			sel_MX2 <= '1';
			sel_MX1 <= '0';

		when inc_2 =>
			next_state <= fetch;
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			sel_MX2 <= '1';
			sel_MX1 <= '0';
			sel_MX3 <= "01";
			pc_inc <= '1';
		-- ---------------------------------
		-- ----------------DEC--------------	
		when dec_1 =>
			next_state <= dec_2;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			sel_MX2 <= '1';
			sel_MX1 <= '0';

		when dec_2 =>
			next_state <= fetch;
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			sel_MX2 <= '1';
			sel_MX1 <= '0';
			sel_MX3 <= "10";
			pc_inc <= '1';
		-- -----------------------------------------
		-- ----------------WHILE-BEGIN--------------
		when bwh_1 =>
			next_state <= bwh_2;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			sel_MX2 <= '1';
			sel_MX1 <= '0';
			pc_inc <= '1';

		when bwh_2 =>
			next_state <= fetch;
			if(DATA_RDATA = "00000000") then
				cnt_inc <= '1';
				next_state <= bwh_3;
			end if;

		when bwh_3 =>
			next_state <= fetch;
			if(cnt_counter /= X"00") then
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel_MX1 <= '1';
				next_state <= bwh_4;
			end if;

		when bwh_4 =>
			next_state <= bwh_3;
			if(DATA_RDATA = X"5B") then
				cnt_inc <= '1';
			elsif(DATA_RDATA = X"5D") then
				cnt_dec <= '1';
			end if;
			pc_inc <= '1';
		-- -----------------------------------------
		-- ----------------WHILE-END----------------
		when ewh_1 =>
			next_state <= ewh_2;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			sel_MX2 <= '1';
			sel_MX1 <= '0';

		when ewh_2 =>
			if(DATA_RDATA = "00000000") then
				pc_inc <= '1';
				next_state <= fetch;
			else
				cnt_inc <= '1';
				pc_dec <= '1';
				next_state <= ewh_3;
			end if;

		when ewh_3 =>
			next_state <= fetch;
			if(cnt_counter /= X"00") then
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel_MX1 <= '1';
				next_state <= ewh_4;
			end if;

		when ewh_4 =>
			next_state <= ewh_5;
			if(DATA_RDATA = X"5D") then
				cnt_inc <= '1';
			elsif(DATA_RDATA = X"5B") then
				cnt_dec <= '1';
			end if;

		when ewh_5 =>
			next_state <= ewh_3;
			if(cnt_counter = X"00") then
				pc_inc <= '1';
			else
				pc_dec <= '1';
			end if;
		-- -----------------------------------
		-- ----------------WRITE--------------
		when wr_1 =>
			next_state <= wr_2;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			sel_MX2 <= '1';
			sel_MX1 <= '0';

		when wr_2 =>
			next_state <= wr_2;
			if(OUT_BUSY = '0') then
				next_state <= fetch;
				OUT_WE <= '1';
				OUT_DATA <= DATA_RDATA;
				pc_inc <= '1';
			end if;
		-- ----------------------------------
		-- ----------------READ--------------
		when rd_1 =>
			next_state <= rd_2;
			IN_REQ <= '1';

		when rd_2 =>
			next_state <= rd_2;
			IN_REQ <= '1';
			if(IN_VLD /= '0') then
				next_state <= fetch;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				sel_MX2 <= '1';
				sel_MX1 <= '0';
				sel_MX3 <= "11";
				pc_inc <= '1';
			end if;
			
		-- ------------------------------------
		-- ----------------LD-TMP--------------
		when ld_tmp_1 =>
			next_state <= ld_tmp_2;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			sel_MX2 <= '0';
			sel_MX1 <= '0';

		when ld_tmp_2 =>
			next_state <= fetch;
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			sel_MX2 <= '1';
			sel_MX1 <= '0';
			sel_MX3 <= "00";
			pc_inc <= '1';
		
		-- ------------------------------------
		-- ----------------ST-TMP--------------
		when st_tmp_1 =>
			next_state <= st_tmp_2;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			sel_MX2 <= '1';
			sel_MX1 <= '0';

		when st_tmp_2 =>
			next_state <= fetch;
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			sel_MX2 <= '0';
			sel_MX1 <= '0';
			sel_MX3 <= "00";
			pc_inc <= '1';
		-- ------------------------------------
		-- ----------------HALT----------------
		when halt =>
			next_state <= halt;
		-- ------------------------------------
		-- ----------------UNDEF---------------
		when undef => 
			next_state <= fetch;
			pc_inc <= '1';
		-- ------------------------------------
		-- ----------------OTHERS--------------
		when others => next_state <= undef;
	end case;
end process;

end behavioral;
 
