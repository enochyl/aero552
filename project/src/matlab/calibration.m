function calibration()
%
% PURPOSE: Calibrate magnetometers and CSS sensors and output  the corresponding
%          nonlinear curve-fit coefficients
%
% INPUTS:
%           tsat_data.txt : Data from tsat
%
% OUTPUTS:
%      magCalibration.txt : magnetometer calibration coefficients and
%                           associated constants
%      cssCalibration.txt : CSS calibration coefficients and
%                           associated constants
%
%% Data Loading %%
dataFile = load('tsat_data.txt');

% User Input, based on data collection
startingIndex = 1;
endingIndex = length(dataFile);

%% Data Extraction %%
t         = dataFile(:,1);
t_command = dataFile(:,2);
gyro_rate = dataFile(:,3);
css1      = dataFile(:,4);
css2      = dataFile(:,5);
css3      = dataFile(:,6);
css4      = dataFile(:,7);
mag1      = dataFile(:,8);
mag2      = dataFile(:,9);
mag3      = dataFile(:,10);
timespan = t(startingIndex:endingIndex);

%% Data Processing %%
% Curve-fit
% Mag1
     mag1_cfit_f = fit(t(startingIndex:endingIndex),mag1(startingIndex:endingIndex),'fourier8');
mag1_coeffvals_f = coeffvalues(mag1_cfit_f);
     mag1_cfit_s = fit(t(startingIndex:endingIndex),mag1(startingIndex:endingIndex),'sin8');
mag1_coeffvals_s = coeffvalues(mag1_cfit_s); 
  [~, mag1Fit_f] = calcFitPeriod(timespan,mag1_coeffvals_f,1);
  [~, mag1Fit_s] = calcFitPeriod(timespan,mag1_coeffvals_s,2);
% CSS1
     css1_cfit_f = fit(t(startingIndex:endingIndex),css1(startingIndex:endingIndex),'fourier8');
css1_coeffvals_f = coeffvalues(css1_cfit_f);
     css1_cfit_s = fit(t(startingIndex:endingIndex),css1(startingIndex:endingIndex),'sin8');
css1_coeffvals_s = coeffvalues(css1_cfit_s);
  [~, css1Fit_f] = calcFitPeriod(timespan,css1_coeffvals_f,1);
  [~, css1Fit_s] = calcFitPeriod(timespan,css1_coeffvals_s,2);

% Comparing Norms
if norm(mag1Fit_f-mag1(startingIndex:endingIndex)) > norm(mag1Fit_s-mag1(startingIndex:endingIndex))
    tag_mag = 2; % Use sin
else
    tag_mag = 1; % Use fourier
end
if norm(css1Fit_f-css1(startingIndex:endingIndex)) > norm(css1Fit_s-css1(startingIndex:endingIndex))
    tag_css = 2; % Use sin
else
    tag_css = 1; % Use fourier
end

% Finishing the Curve-fit for other magnetometers
if tag_mag == 1 % Fourier
         mag1_cfit = mag1_cfit_f;
    mag1_coeffvals = mag1_coeffvals_f;
         mag2_cfit = fit(t(startingIndex:endingIndex),mag2(startingIndex:endingIndex),'fourier8');
    mag2_coeffvals = coeffvalues(mag2_cfit);
         mag3_cfit = fit(t(startingIndex:endingIndex),mag3(startingIndex:endingIndex),'fourier8');
    mag3_coeffvals = coeffvalues(mag3_cfit);
    
    [mag1_period, mag1Fit] = calcFitPeriod(timespan,mag1_coeffvals,tag_mag);
    [mag2_period, mag2Fit] = calcFitPeriod(timespan,mag2_coeffvals,tag_mag);
    [mag3_period, mag3Fit] = calcFitPeriod(timespan,mag3_coeffvals,tag_mag);
else % Sin
         mag1_cfit = mag1_cfit_s;
    mag1_coeffvals = mag1_coeffvals_s;
         mag2_cfit = fit(t(startingIndex:endingIndex),mag2(startingIndex:endingIndex),'sin8');
    mag2_coeffvals = coeffvalues(mag2_cfit);
         mag3_cfit = fit(t(startingIndex:endingIndex),mag3(startingIndex:endingIndex),'sin8');
    mag3_coeffvals = coeffvalues(mag3_cfit);
    
    [mag1_period, mag1Fit] = calcFitPeriod(timespan,mag1_coeffvals,tag_mag);
    [mag2_period, mag2Fit] = calcFitPeriod(timespan,mag2_coeffvals,tag_mag);
    [mag3_period, mag3Fit] = calcFitPeriod(timespan,mag3_coeffvals,tag_mag);
end

% Finishing the Curve-fit for other sun sensors
if tag_css == 1 % Fourier
         css1_cfit = css1_cfit_f;
    css1_coeffvals = css1_coeffvals_f;
         css2_cfit = fit(t(startingIndex:endingIndex),css2(startingIndex:endingIndex),'fourier8');
    css2_coeffvals = coeffvalues(css2_cfit);
         css3_cfit = fit(t(startingIndex:endingIndex),css3(startingIndex:endingIndex),'fourier8');
    css3_coeffvals = coeffvalues(css3_cfit);
         css4_cfit = fit(t(startingIndex:endingIndex),css4(startingIndex:endingIndex),'fourier8');
    css4_coeffvals = coeffvalues(css4_cfit);
    
    [css1_period, css1Fit] = calcFitPeriod(timespan,css1_coeffvals,tag_css);
    [css2_period, css2Fit] = calcFitPeriod(timespan,css2_coeffvals,tag_css);
    [css3_period, css3Fit] = calcFitPeriod(timespan,css3_coeffvals,tag_css);
    [css4_period, css4Fit] = calcFitPeriod(timespan,css4_coeffvals,tag_css);
else % Sin
         css1_cfit = css1_cfit_s;
    css1_coeffvals = css1_coeffvals_s;
         css2_cfit = fit(t(startingIndex:endingIndex),css2(startingIndex:endingIndex),'sin8');
    css2_coeffvals = coeffvalues(css2_cfit);
         css3_cfit = fit(t(startingIndex:endingIndex),css3(startingIndex:endingIndex),'sin8');
    css3_coeffvals = coeffvalues(css3_cfit);
         css4_cfit = fit(t(startingIndex:endingIndex),css4(startingIndex:endingIndex),'sin8');
    css4_coeffvals = coeffvalues(css4_cfit);
    
    [css1_period, css1Fit] = calcFitPeriod(timespan,css1_coeffvals,tag_css);
    [css2_period, css2Fit] = calcFitPeriod(timespan,css2_coeffvals,tag_css);
    [css3_period, css3Fit] = calcFitPeriod(timespan,css3_coeffvals,tag_css);
    [css4_period, css4Fit] = calcFitPeriod(timespan,css4_coeffvals,tag_css);
end


%% Data Plotting %%

figure(8)
subplot(3,1,1)
plot(mag1_cfit, t(startingIndex:endingIndex),mag1(startingIndex:endingIndex))
ylabel('Mag 1'); grid on; xlim([t(startingIndex),t(endingIndex)]);
xlim([t(startingIndex),t(endingIndex)]);
subplot(3,1,2)
plot(mag2_cfit, t(startingIndex:endingIndex),mag2(startingIndex:endingIndex))
ylabel('Mag 2'); grid on; xlim([t(startingIndex),t(endingIndex)]);
subplot(3,1,3)
plot(mag3_cfit, t(startingIndex:endingIndex),mag3(startingIndex:endingIndex))
ylabel('Mag 3'); grid on; xlim([t(startingIndex),t(endingIndex)]);
xlabel('Time (s)');

figure(9)
subplot(4,1,1)
plot(css1_cfit, t(startingIndex:endingIndex),css1(startingIndex:endingIndex))
ylabel('CSS 1'); grid on; xlim([t(startingIndex),t(endingIndex)]);
subplot(4,1,2)
plot(css2_cfit, t(startingIndex:endingIndex),css2(startingIndex:endingIndex))
ylabel('CSS 2'); grid on; xlim([t(startingIndex),t(endingIndex)]);
subplot(4,1,3)
plot(css3_cfit, t(startingIndex:endingIndex),css3(startingIndex:endingIndex))
ylabel('CSS 3'); grid on; xlim([t(startingIndex),t(endingIndex)]);
subplot(4,1,4)
plot(css4_cfit, t(startingIndex:endingIndex),css4(startingIndex:endingIndex))
ylabel('CSS 4'); grid on; xlim([t(startingIndex),t(endingIndex)]);
xlabel('Time (s)');

figure(10)
plot(mag1Fit,mag2Fit)
xlabel('Mag 1'); ylabel('Mag 2'); grid on;

%% Data Output %% 
print('-f8', 'mag_curvefitted', '-dpng');
print('-f9', 'css_curvefitted', '-dpng');
print('-f10', 'mag1_mag2', '-dpng');

if tag_mag == 1
    % Magnetometer calibration coefficients output, Fourier8
    magCalibration = zeros(3,length(mag1_coeffvals_s)+3);
    magCalibration(1,1) = 1;
    magCalibration(1,2) = mag1_coeffvals(end);
    magCalibration(2,2) = mag2_coeffvals(end);
    magCalibration(3,2) = mag3_coeffvals(end);
    magCalibration(1,3) = t(startingIndex);
    magCalibration(2,3) = mag1_period;
    magCalibration(1,4) = std(mag1(1:100));
    magCalibration(2,4) = std(mag2(1:100));
    magCalibration(3,4) = std(mag3(1:100));
    magCalibration(1,5:end) = mag1_coeffvals(1:end-1);
    magCalibration(2,5:end) = mag2_coeffvals(1:end-1);
    magCalibration(3,5:end) = mag3_coeffvals(1:end-1);
    fileID = fopen('magCalibration.txt', 'w');
    fprintf(fileID,'%f %f %f\n',magCalibration);
    fclose(fileID);
    else
    % Magnetometer calibration coefficients output, Sin8
    magCalibration = zeros(3,length(mag1_coeffvals_s)+3); 
    magCalibration(1,1) = 2;
    magCalibration(1,2) = t(startingIndex);
    magCalibration(2,2) = mag1_period;
    magCalibration(1,3) = std(mag1(1:100));
    magCalibration(2,3) = std(mag2(1:100));
    magCalibration(3,3) = std(mag3(1:100));
    magCalibration(1,4:end) = mag1_coeffvals(1:end);
    magCalibration(2,4:end) = mag2_coeffvals(1:end);
    magCalibration(3,4:end) = mag3_coeffvals(1:end);
    fileID = fopen('magCalibration.txt', 'w');
    fprintf(fileID,'%f %f %f\n',magCalibration);
    fclose(fileID);
end

if tag_css == 1
    % CSS calibration coefficients output, Fourier8
    cssCalibration = zeros(4,length(css1_coeffvals)+2);
    cssCalibration(1,1) = 1;
    cssCalibration(1,2) = css1_coeffvals(end);
    cssCalibration(2,2) = css2_coeffvals(end);
    cssCalibration(3,2) = css2_coeffvals(end);
    cssCalibration(4,2) = css2_coeffvals(end);
    cssCalibration(1,3) = t(startingIndex);
    cssCalibration(2,3) = css1_period;
    cssCalibration(1,4:end) = css1_coeffvals(1:end-1);
    cssCalibration(2,4:end) = css2_coeffvals(1:end-1);
    cssCalibration(3,4:end) = css3_coeffvals(1:end-1);
    cssCalibration(4,4:end) = css4_coeffvals(1:end-1);
    fileID = fopen('cssCalibration.txt', 'w');
    fprintf(fileID,'%f %f %f %f\n',cssCalibration);
    fclose(fileID);
    else
    % CSS calibration coefficients output, Sin8
    cssCalibration = zeros(4,length(css1_coeffvals)+2); 
    cssCalibration(1,1) = 2;
    cssCalibration(1,2) = t(startingIndex);
    cssCalibration(2,2) = css1_period;
    cssCalibration(1,3:end) = css1_coeffvals(1:end);
    cssCalibration(2,3:end) = css2_coeffvals(1:end);
    cssCalibration(3,3:end) = css3_coeffvals(1:end);
    cssCalibration(4,3:end) = css4_coeffvals(1:end);
    fileID = fopen('cssCalibration.txt', 'w');
    fprintf(fileID,'%f %f %f %f\n',cssCalibration);
    fclose(fileID);
end
fprintf('Magnetometer calibration:\n');
type magCalibration.txt;
fprintf('\n');
fprintf('CSS calibration:\n');
type cssCalibration.txt;
fprintf('\n');
end
