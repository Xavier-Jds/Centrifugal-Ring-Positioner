%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Control System Design Lab: Sample Code
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Setup
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

openinout; %Open the ports of the analog computer.
Ts=;%Set the sampling time.
lengthExp=; %Set the length of the experiment (in seconds).
N0=lengthExp/Ts; %Compute the number of points to save the datas.
Data=zeros(N0,1); %Vector saving the datas. If there are several datas to save, change "1" to the number of outputs.
DataCommands=ones(N0,1); %Vector storing the input sent to the plant.
cond=1; %Set the condition variable to 1.
i=1; %Set the counter to 1.
tic %Begins the first strike of the clock.
time=0:Ts:(N0-1)*Ts; %Vector saving the time steps.
input=; %Set the input value.

%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Loop
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

while cond==1
    [in1,in2,in3,in4,in5,in6,in7,in8]=anain; %Acquisition of the measurements.
    anaout(input,0); %Command to send the input to the analog computer.
    Data(i,1)=in1; %Save in1 (in volts).
    DataCommands(i,1)=input; %Save the input.
    t=toc; %Second strike of the clock.
    if t>i*Ts
        disp('Sampling time too small');%Test if the sampling time is too small.
    else
        while toc<=i*Ts %Does nothing until the second strike of the clock reaches the sampling time set.
        end
    end
    if i==N0 %Stop condition.
        cond=0;
    end
    i=i+1;
end

closeinout %Close the ports.

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Plots
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure %Open a new window for plot.
plot(time,Data(:,1),time,DataCommands(:)); %Plot the experiment (input and output).