# OmahaHiLo
Omaha Hi Lo pocker game inputs proccessing for determine winners


Omaha Hi Lo pocker hands proccessing program accept a VALID input (fstream) include a sequence of two different opponents pocker hands (values) and a table (values), i.e. two different hands and a table as an input sequence element,
and determine a corresponding sequence of High winner(s) as well as Low winner(s) (if exist) for each two hands and a table (sequence element), according to Omaha Hi Lo pocker rules .

Support a general case of similiar pocker hands proccessing (two hands and a table inputs sequence as an input to determine a similar ouptput - except Low winner within common poker games),
while implemented for the Omaha Li Lo specific case (OMHLHandsprocessor extends abstract class HandsProcessor) 
   

please notice "high hands" cards combinations ranks goes same as with Texas Hold'em (best to worst): 
    royal flush
    straight flush
    four-of-a-kind
    full house
    flush
    straight
    three-of-a-kind
    two pair
    one pair
    high-card.

while there is NO difrentiations by suits (no suit cosidered better then the other) and there is a preferance for HIGHER CARDS VALUES 

for "low hands" winning and other relevant rules, 
please review the parts (within the next link) : Omaha Hi-Lo Basic Rules ,Omaha Hi-Lo Rules - The Showdown ,Omaha Hi-Lo Poker Hand Rules
https://www.pokerlistings.com/poker-rules-omaha-high-low
