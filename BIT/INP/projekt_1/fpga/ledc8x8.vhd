-- Autor reseni: Michal Šedý (xsedym02) 

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Popis rozhrani obvodu
	RESET: in std_logic;
	SMCLK: in std_logic;
	ROW: out std_logic_vector (7 downto 0);
	LED: out std_logic_vector (0 to 7)	-- 0 to 7 ulehci orientaci v jednom radku
);
end ledc8x8;

architecture main of ledc8x8 is

	 -- Definice vnitrnich signalu.
	signal clk_e: std_logic := '0';						-- signal pro zmenu radku
	signal state: std_logic_vector (1 downto 0) := "00";			-- signal pro zmenu sestavy ledek
	signal clk_half_s: std_logic_vector (21 downto 0) := (others => '0');	-- pocita do SMCLK/2 
	signal clk_ch_row: std_logic_vector (7 downto 0) := (others => '0');	-- pocita SMCLK/256
	signal wr_row: std_logic_vector (7 downto 0) := "00000001";		-- signal udava aktivni radek

begin


	-- Pocita kmity SMCLK.
	-- Kdyz zaznamena 256 taktu SMCLK vyda signal ke zmene aktivniho radku.
	clk_row: process(RESET, SMCLK)
	begin
		if RESET = '1' then
			clk_ch_row <= (others => '0');
		elsif (SMCLK'event) and (SMCLK = '1') then
			if clk_ch_row = "11111111" then
				clk_ch_row <= (others => '0');
				clk_e <= '1';
			else
				clk_ch_row <= clk_ch_row + 1;
				clk_e <= '0';
			end if;
		end if;
	end process clk_row;


	-- Pocita kmity SMCLK.
	-- Kdyz uplyne 1/2s vyda signal ke zmenì sestavy ledek,
	-- ale pouze v pripade, ze se cislo sestavy mensi nez 10 (v podstate 3. sestava).
	-- V treti sestave uz nema na ledky efekt.
	clk_sec: process(RESET, SMCLK)
	begin
		if RESET = '1' then
			clk_half_s <= (others => '0');
		elsif (SMCLK'event) and (SMCLK = '1') then
			if clk_half_s = "1110000100000000000000" then
				if state /= "10" then
					state <= state +1;
				end if;
				clk_half_s <= (others => '0');
			else
				clk_half_s <= clk_half_s + 1;
			end if;
		end if;
	end process clk_sec;
	

	-- Pokud dostane clk_e='1' pak provede zmenu aktivniho radku na dalsi.
	-- Implementovano rotacnim registrem.
	row_rotate: process(RESET, SMCLK, clk_e)
	begin
		if RESET = '1' then
			wr_row <= "00000001";
		elsif  (SMCLK'event) and SMCLK = '1' and clk_e = '1' then
			wr_row <= wr_row(6 downto 0) & wr_row(7);
			
		end if;
	end process row_rotate;
	ROW <= wr_row;


	-- Podle aktivniho radku vybere astivaci ledek
	-- V case 1/2s az 1s bude display zhasly
	write: process(wr_row, state)
	begin
		if state = "00"  or state = "10" then
			case wr_row is
				when "00000001" => LED <= "01110111";
				when "00000010" => LED <= "00100111";
				when "00000100" => LED <= "01010111";
				when "00001000" => LED <= "01110000";
				when "00010000" => LED <= "01110111";
				when "00100000" => LED <= "11111001";
				when "01000000" => LED <= "11111110";
				when "10000000" => LED <= "11110001";
				when others => LED <= "11111111";
			end case;
		else
			LED <= "11111111";
		end if;
	end process write;

end main;

-- ISID: 75579
