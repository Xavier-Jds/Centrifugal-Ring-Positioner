function y = physical_constraint(s0,s,dds)

if dds < 0 & s <= s0  
   y = 0;
else
   y = dds;
end


