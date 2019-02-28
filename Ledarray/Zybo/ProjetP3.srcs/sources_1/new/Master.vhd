library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VComponents.all;

entity Master is
    Port ( 
          i_btn       : in    std_logic_vector (3 downto 0); -- Boutons de la carte Zybo
          i_sw        : in    std_logic_vector (3 downto 0); -- Interrupteurs de la carte Zybo
          sysclk      : in    std_logic;                     -- horloge systeme
          o_matrice       : out   std_logic_vector (7 downto 0) -- vers connecteur pmod
          );
          
end Master;

architecture Behavioral of Master is

constant freq_sys_MHz    : integer := 125;  -- 125 MHz 
   
signal C1       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 1 de la matrice
signal C2       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 2 de la matrice
signal C3       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 3 de la matrice
signal C4       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 4 de la matrice      
signal C5       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 5 de la matrice
signal C6       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 6 de la matrice     
signal C7       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 7 de la matrice
signal C8       :   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 8 de la matrice  

signal clk_5MHz :   std_logic;    -- horloge divise utilise pour le circuit  
signal d_s_1Hz  :   std_logic;    -- Signal temoin 1 Hz


component synchro_module_v2 is
   generic (const_CLK_syst_MHz: integer := freq_sys_MHz);
      Port ( 
           clkm        : in  STD_LOGIC;  -- Entrée  horloge maitre
           o_CLK_5MHz  : out STD_LOGIC;  -- horloge divise utilise pour le circuit             
           o_S_1Hz     : out  STD_LOGIC  -- Signal temoin 1 Hz
            );
end component;      

component aff_matrice is
generic (const_CLK_MHz: integer := 100);                     -- horloge en MHz, typique 100 MHz 
    Port (   clk      : in   STD_LOGIC;                      -- horloge systeme, typique 100 MHz (preciser par le constante)
             C1       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 1 de la matrice
             C2       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 2 de la matrice
             C3       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 3 de la matrice
             C4       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 4 de la matrice
             C5       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 5 de la matrice
             C6       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 6 de la matrice      
             C7       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 7 de la matrice
             C8       : in   STD_LOGIC_VECTOR (6 downto 0);  -- Colonne 8 de la matrice                                  
             PmodC    : out  STD_LOGIC_VECTOR (7 downto 0)   -- sorties directement adaptees au connecteur Pmod
           ); 
end component;           

   

begin

-------------------TEMPORAIRE-DEMO----------------------
C1 <= (0          => i_sw(3),   others => '0');
C2 <= (1 downto 0 => i_sw(2),   others => '0');
C3 <= (2 downto 0 => i_sw(1),   others => '0');
C4 <= (3 downto 0 => i_sw(0),   others => '0');
C5 <= (4 downto 0 => i_btn(3),  others => '0');
C6 <= (5 downto 0 => i_btn(2),  others => '0');
C7 <= (6 downto 0 => i_btn(1),  others => '0');
C8 <= (5 downto 0 => i_btn(0),  others => '0');
--------------------------------------------------------


inst_synch : synchro_module_v2 generic map (const_CLK_syst_MHz => freq_sys_MHz)
                             port map(  clkm => sysclk,     o_CLK_5MHz => clk_5MHz,     o_S_1Hz => d_S_1Hz);  
                             
                             
inst_aff_matrice : aff_matrice generic map (const_CLK_MHz => 5)
                               port map (  clk => clk_5MHz, C1 => C1, C2 => C2, C3 => C3, C4 => C4, C5 => C5, C6 => C6, C7 => C7, C8 => C8, PmodC => o_matrice);

end Behavioral;
