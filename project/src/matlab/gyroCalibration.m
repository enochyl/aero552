function gyroCalibration()
%
% PURPOSE: Calibrate gyro readings and plot the linear relationship between
%          the gyro-volts to degrees/second
%
% INPUTS:
%     tsat_data3 : Data for GV = 3
%     tsat_data4 : Data for GV = 4
%    tsat_data35 : Data for GV = 3.5
%
% OUTPUTS:
%   gyroCalibration.png : Plot
%

%% Data Loading %%
testFile3 = load('tsat_data3.txt');
testFile4 = load('tsat_data4.txt');
testFile35 = load('tsat_data35.txt');

t_3         = testFile3(:,1);
t_command_3 = testFile3(:,2);
gyro_rate_3 = testFile3(:,3);
mag1_3      = testFile3(:,8);

t_4         = testFile4(:,1);
t_command_4 = testFile4(:,2);
gyro_rate_4 = testFile4(:,3);
mag1_4      = testFile4(:,8);

t_35         = testFile35(:,1);
t_command_35 = testFile35(:,2);
gyro_rate_35 = testFile35(:,3);
mag1_35      = testFile35(:,8);

startingIndex = 1;
endingIndex = length(testFile3); % Same length for all three

timespan3  = t_3(startingIndex:endingIndex);
timespan4  = t_4(startingIndex:endingIndex);
timespan35 = t_35(startingIndex:endingIndex);


% Curve-fit
     mag1_cfit_3 = fit(t_3(startingIndex:endingIndex),mag1_3(startingIndex:endingIndex),'fourier8');
mag1_coeffvals_3 = coeffvalues(mag1_cfit_3); 
    [period3, ~] = calcFitPeriod(timespan3,mag1_coeffvals_3,1);

     mag1_cfit_4 = fit(t_4(startingIndex:endingIndex),mag1_4(startingIndex:endingIndex),'fourier8');
mag1_coeffvals_4 = coeffvalues(mag1_cfit_4); 
    [period4, ~] = calcFitPeriod(timespan4,mag1_coeffvals_4,1);
  
     mag1_cfit_35 = fit(t_35(startingIndex:endingIndex),mag1_35(startingIndex:endingIndex),'fourier8');
mag1_coeffvals_35 = coeffvalues(mag1_cfit_35); 
    [period35, ~] = calcFitPeriod(timespan35,mag1_coeffvals_35,1);


angVel3  = 360/period3;
angVel4  = 360/period4;
angVel35 = 360/period35;

 avgGV3 = mean(gyro_rate_3);
 avgGV4 = mean(gyro_rate_4);
avgGV35 = mean(gyro_rate_35);

avgGV = [avgGV3, avgGV35, avgGV4];
angVel = [angVel3, angVel35, angVel4];

figure(1)
plot(avgGV, angVel, '-*')
ylabel('deg/s'); grid on; xlabel('Gyro Volt');
print('-f1', 'gyroCalibration', '-dpng');

polyfit(avgGV,angVel,1)

end


