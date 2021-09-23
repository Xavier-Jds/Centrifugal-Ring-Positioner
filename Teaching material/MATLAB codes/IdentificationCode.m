u=out.simout.Data(:,1).';
y=out.simout.Data(:,2).';
time=out.simout.Time.';
offset_u=; %Operating point
offset_y=; %Operating point
Ts=;
SystemOrder=[0 1]; %Number of zeros and of poles (0 and 1), respectively.
sysIdent=IdentifySystem(u-offset_u,y-offset_y,SystemOrder,Ts)
plot(out.simout.Time,y-offset_y,'.');
hold on;
lsim(sysIdent,u-offset_u,time);