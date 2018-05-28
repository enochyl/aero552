clear
close all
clc

calibration();
plotRate();
gyroCalibration();

%% Data Loading %%
testFile = load('tsat_data_sample.txt');

% User Input, based on data collection
n = 5;
startingIndex = 1 + 2*n;
endingIndex = length(testFile) - 2*n;

%% Data Extraction %%
t         = testFile(:,1);
t_command = testFile(:,2);
gyro_rate = testFile(:,3);
css1      = testFile(:,4);
css2      = testFile(:,5);
css3      = testFile(:,6);
css4      = testFile(:,7);
mag1      = testFile(:,8);
mag2      = testFile(:,9);
mag3      = testFile(:,10);
timespan = t(startingIndex:endingIndex);

%% Data Processing %%
mag1_processed5 = movingAvg(5, t, mag1)';
mag2_processed5 = movingAvg(5, t, mag2)';
mag3_processed5 = movingAvg(5, t, mag3)';
css1_processed5 = movingAvg(5, t, css1)';
css2_processed5 = movingAvg(5, t, css2)';
css3_processed5 = movingAvg(5, t, css3)';
css4_processed5 = movingAvg(5, t, css4)';

mag1_processed10 = movingAvg(10, t, mag1)';
mag2_processed10 = movingAvg(10, t, mag2)';
mag3_processed10 = movingAvg(10, t, mag3)';
css1_processed10 = movingAvg(10, t, css1)';
css2_processed10 = movingAvg(10, t, css2)';
css3_processed10 = movingAvg(10, t, css3)';
css4_processed10 = movingAvg(10, t, css4)';



%% Data Plotting %%

plotrangeStart = 1 + 2*n;
plotrangeEnd   = find(timespan==255.001000000000);

figure(1)
subplot(4,1,1);
plot(t,css1);
ylabel('CSS 1'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,2);
plot(t,css2);
ylabel('CSS 2'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,3);
plot(t,css3);
ylabel('CSS 3'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,4);
plot(t,css4);
ylabel('CSS 4'); grid on;
xlabel('Time (s)');
xlim([t(plotrangeStart), t(plotrangeEnd)]);

figure(2)
subplot(3,1,1)
plot(t,mag1)
ylabel('Mag 1'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(3,1,2)
plot(t,mag2)
ylabel('Mag 2'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(3,1,3)
plot(t,mag3)
ylabel('Mag 3'); grid on;
xlabel('Time (s)');
xlim([t(plotrangeStart), t(plotrangeEnd)]);

figure(3)
subplot(4,1,1);
plot(t,css1_processed5);
ylabel('CSS 1, n = 5'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,2);
plot(t,css2_processed5);
ylabel('CSS 2, n = 5'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,3);
plot(t,css3_processed5);
ylabel('CSS 3, n = 5'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,4);
plot(t,css4_processed5);
ylabel('CSS 4, n = 5'); grid on;
xlabel('Time (s)');
xlim([t(plotrangeStart), t(plotrangeEnd)]);

figure(4)
subplot(3,1,1)
plot(t,mag1_processed5)
ylabel('Mag 1, n = 5'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(3,1,2)
plot(t,mag2_processed5)
ylabel('Mag 2, n = 5'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(3,1,3)
plot(t,mag3_processed5)
ylabel('Mag 3, n = 5'); grid on;
xlabel('Time (s)');
xlim([t(plotrangeStart), t(plotrangeEnd)]);

figure(5)
subplot(4,1,1);
plot(t,css1_processed10);
ylabel('CSS 1, n = 10'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,2);
plot(t,css2_processed10);
ylabel('CSS 2, n = 10'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,3);
plot(t,css3_processed10);
ylabel('CSS 3, n = 10'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(4,1,4);
plot(t,css4_processed10);
ylabel('CSS 4, n = 10'); grid on;
xlabel('Time (s)');
xlim([t(plotrangeStart), t(plotrangeEnd)]);

figure(6)
subplot(3,1,1)
plot(t,mag1_processed10)
ylabel('Mag 1, n = 10'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(3,1,2)
plot(t,mag2_processed10)
ylabel('Mag 2, n = 10'); grid on;
xlim([t(plotrangeStart), t(plotrangeEnd)]);
subplot(3,1,3)
plot(t,mag3_processed10)
ylabel('Mag 3, n = 10'); grid on;
xlabel('Time (s)');
xlim([t(plotrangeStart), t(plotrangeEnd)]);