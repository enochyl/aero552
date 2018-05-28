function plotRate()
%
% PURPOSE: Plot history of gyro rate (1, 3, and 4) and  their assocated
%          thrust command history
%
% INPUTS:
%    rate_data1 : Data for GV = 1
%    rate_data3 : Data for GV = 3
%    rate_data4 : Data for GV = 4
%
% OUTPUTS:
%   rate_data_gv1_tsat2.png : Plot, GV = 1
%   rate_data_gv3_tsat2.png : Plot, GV = 3
%   rate_data_gv4_tsat2.png : Plot, GV = 4
%

%% Data Loading %%
testFile1 = load('rate_data1.txt');
testFile3 = load('rate_data3.txt');
testFile4 = load('rate_data4.txt');

%% Data Extraction %%
t1         = testFile1(:,1);
t_command1 = testFile1(:,2);
gyro_rate1 = testFile1(:,3);
t3         = testFile3(:,1);
t_command3 = testFile3(:,2);
gyro_rate3 = testFile3(:,3);
t4         = testFile4(:,1);
t_command4 = testFile4(:,2);
gyro_rate4 = testFile4(:,3);

figure(1)
subplot(2,1,1)
plot(t1,t_command1);
ylabel('Thrust Command'); grid on;
subplot(2,1,2);
plot(t1,gyro_rate1);
ylabel('Gyro Volt'); grid on
xlabel('Time (s)');

figure(2)
subplot(2,1,1)
plot(t3,t_command3);
ylabel('Thrust Command'); grid on;
subplot(2,1,2);
plot(t3,gyro_rate3);
ylabel('Gyro Volt'); grid on
xlabel('Time (s)');

figure(3)
subplot(2,1,1)
plot(t4,t_command4);
ylabel('Thrust Command'); grid on;
subplot(2,1,2);
plot(t4,gyro_rate4);
ylabel('Gyro Volt'); grid on
xlabel('Time (s)');

print('-f1', 'rate_data_gv1_tsat2', '-dpng');
print('-f2', 'rate_data_gv3_tsat2', '-dpng');
print('-f3', 'rate_data_gv4_tsat2', '-dpng');

end

