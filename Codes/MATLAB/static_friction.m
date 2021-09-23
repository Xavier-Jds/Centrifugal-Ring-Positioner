function [y]=static_friction(Cf,w0,s0,s,w,ds)

if s <= s0 || ds == 0;
    y = Cf;
%elseif s~=s0 & ds == 0
    y = Cf;
else
   y = 0;
end

