library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;


entity aff_matrice is
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
end aff_matrice;

architecture Behavioral of aff_matrice is

constant    const_div_clk_SSD :     integer  := (const_CLK_MHz*1000); --Constante pour la frequence de comunication, trouvee avec methode empirique
constant    nbclocktotal :     std_logic_vector (15  downto 0):= conv_std_logic_vector(const_div_clk_SSD, 16); --Constante pour la frequence de comunication, trouvee avec methode empirique

signal  nbclock :        std_logic_vector (15 downto 0) := (others => '0');    --Counter du nombre de clock
signal  nbenvois :   STD_LOGIC_VECTOR (3 downto 0):= (others => '0');--Counter du nombre d'envois
signal  segm :          STD_LOGIC_VECTOR (6 downto 0);   --Donnee a envoyer
signal  STR  :          STD_LOGIC := '0';    --Strobe

signal reset : std_logic_vector(6 downto 0):= (6 => '1', others => '0');

 
begin

local_CLK_proc: process(CLK) --Clock d'envois
begin

   if(CLK'event and CLK = '1') then
      nbclock <= nbclock + 1;
      
      if (nbclock = nbclocktotal) then
           nbclock <= (others => '0');
           STR <= not STR;
           
           if(STR = '1') then
           
               if(nbenvois = "1000") then
                    nbenvois <= (others => '0');
               else 
                    nbenvois <= nbenvois + '1';
               end if;
           end if;
      end if;
   end if;
end process;

inst_shift : process(nbenvois) --Selon le numero de l'envois, on gere une colonne differente
    begin
        case nbenvois is
            when "000" => segm <= C8;
            when "001" => segm <= C7;
            when "010" => segm <= C6;
            when "011" => segm <= C5;
            when "100" => segm <= C4;
            when "101" => segm <= C3;
            when "110" => segm <= C2;
            when "111" => segm <= C1;
            when "1000"=> segm <=reset;
            when others=> segm <="0000000";
            
        end case;
  
end process;        

   PmodC(0) <= segm(4); --Assignee au port A2 de l'arduino
   PmodC(1) <= segm(5); --Assignee au port A3 de l'arduino
   PmodC(2) <= segm(6); --Assignee au port A4 de l'arduino
   PmodC(3) <= STR;     --Assignee au port A5 de l'arduino
   PmodC(4) <= segm(0); --Assignee au port 10 de l'arduino
   PmodC(5) <= segm(1); --Assignee au port 11 de l'arduino
   PmodC(6) <= segm(2); --Assignee au port A0 de l'arduino
   PmodC(7) <= segm(3); --Assignee au port A1 de l'arduino

end Behavioral;


