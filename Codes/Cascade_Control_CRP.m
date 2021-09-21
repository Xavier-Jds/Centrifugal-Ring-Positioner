% To be used with Cascade_Control_CRP.slx

clc;close all; clear all;
p = tf('p');

% parameters
m = 0.0239;                         % ring mass
ang = deg2rad(20);                  % rod tilt angle
c = 0.30;                           % ring viscous friction coefficient
g = 9.81;                           % acceleration of gravity
Cf = 1.48;                          % ring static friction

Ts = 0.001;                         % sampling time
Tm = 0.0165;                        % ring position sensor dead time


% operation point
s0 = 0.08 ;    
w0 = ((g*sin(ang))/((cos(ang)^2)*s0))^0.5;
x0 = [s0 0 w0];

sp = 0.08;                                       % initial ring position
wsp = ((g*sin(ang))/((cos(ang)^2)*sp))^0.5;     % corresponding initial angular velocity

% inner loop TF
kp = 0.15;                          % proportionnal gain of the controller
Num_m = [769.4];                    % motor TF numerator
Den_m = [1.3,1];                    % motor TF denominator
G_m = tf(Num_m,Den_m);              % motor TF
CL_m = feedback(G_m*kp,1);          % inner closed-loop

% outerloop TF
Num_s = [2*s0*w0*cos(ang)^2];           % ring TF numerator
Den_s = [1 c/m -(w0*cos(ang))^2];       % ring TF denominator
G_s = tf(Num_s,Den_s);                  % ring TF
G = G_s*CL_m;                          
[Num,Den] = tfdata(G,'v');

% outerloop controller
racines = roots(Den);
psys = abs(racines(2));
N = 5;
Td = 1/psys;
Tf = Td/N;

% uncomment for PID controller
Ti = 0.16;
D = (Td*p+1)*(1+p*Ti)/(p*Ti)/(1+p*Tf);      
kp_out = 216;

% uncomment for PD controller
%{
D = (Td*p+1)/(1+p*Tf)                      
kp_out = 328;
%}

[Num_D,Den_D] = tfdata(D,'v');              % outerloop controller TF
D_d = c2d(D*kp_out,Ts,'tustin');            % discrete-time controller TF
[Num_D_d,Den_D_d] = tfdata(D_d,'v');


% openloops
OL = G*D*kp_out;                           % continuous-time openloop
OL_Tm = OL*exp(-Tm*p);                     % continuous-time openloop with sensor deadtime
OL_Tm_d = OL*exp(-Tm*p)*exp(-Ts*p);        % discrete-time openloop

% rootlocus
figure;
rlocus(OL);

% bode diagram
figure
margin(OL_Tm_d);


