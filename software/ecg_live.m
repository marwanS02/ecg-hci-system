close all;
clear all;

% --- Serial object ---
s = serial('COM7','BaudRate',9600);   % Change COM port as needed

% --- Open serial port ---
fopen(s);

% --- Initialize variables ---
i = 1;
j = 1;
flag = 0;
beats = 0;
t = tic;
data = [];

% --- Main loop ---
while (1)
    % Read one sample from Arduino
    data(i) = str2double(fscanf(s));
    
    % Plot the ECG
    plot(data);
    drawnow;
    
    % Peak detection (simple threshold)
    if (data(i) >= 400 && flag == 0)
        beats = beats + 1;
        flag = 1;
        
    elseif (data(i) < 400 && flag == 1)
        flag = 0;
    end
    
    % Every 402 samples (~10s window if 2410 samples/min)
    if (j == 402)
        bpm = beats * 6;   % scale to 1 minute
        disp("Average BPM:");
        disp(bpm);
        
        % Send BPM back to Arduino
        fprintf(s, '%d', bpm);
        pause(0.5);
        
        % Reset counters
        beats = 0;
        j = 0;
    end
    
    pause(0.01);   % ~100 Hz sampling
    i = i + 1;
    j = j + 1;
end

% --- Close port on exit ---
fclose(s);
