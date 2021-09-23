%*********************************************************************
% Initialization
%*********************************************************************
openinout;
Tcycle=0.01;
lengthExp=20;
N0=lengthExp/Tcycle;
Data=zeros(N0,2);
DataCommands=zeros(N0,1);
cond=1;
i=1;
reference=6.5;
DataCommands(:)=reference;
%*********************************************************************
% Save in real time
%*********************************************************************
while cond==1
    tic
    [in1,in2,in3,in4,in5,in6,in7,in8]=anain;

    % PUT YOUR CONTROL LAW HERE
    out1=Kp*(reference-in1);
    % e.g. a proportional control law
    anaout(out1,0);
    %anaout is the function that applies the desired voltage out1.
    % STORE THE MEASUREMENTS, THE COMMAND OR ANY OTHER USEFUL SIGNALS
    Data(i,1)=in1;
    Data(i,2)=in2;
    DataCommands(i)=out1;

    i=i+1;
    t=toc;
    if t>Tcycle
        disp('Sampling time to small');
    else
    while toc<=Tcycle
    end
    end

    if i==N0+1
    cond=0;
    end
end
%*********************************************************************
% Plot
%*********************************************************************
closeinout;
i=i-1;
time=0:Tcycle:(i-1)*Tcycle;

figure
plot(time, Data(:,1), time, DataCommands(:),time,Data(:,2));
