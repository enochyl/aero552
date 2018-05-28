function [period, Fit] = calcFitPeriod(timespan, coeffvals, tag)
%
% PURPOSE: Calculate the curve fit function based on a fourier8/sin8 fit and
%          outputs the period.
%
% INPUTS:
%    timespan : Timespan of the data
%   coeffvals : Coefficients of the fourier8 fit
%
% OUTPUTS:
%      period : Period of the function
%         Fit : The curve fit function
%
Fit = zeros(length(timespan),1);
    for i = 1:length(timespan)
        Fit_temp = 0;
        if tag == 1 % For the case of Fourier8
            n = 1;
            for j = 2:2:length(coeffvals)-1
                Fit_temp = Fit_temp + coeffvals(j)*cos(timespan(i)*n*coeffvals(end)) + coeffvals(j+1)*sin(timespan(i)*n*coeffvals(end));
                n = n + 1;
            end
            Fit(i) = Fit_temp + coeffvals(1);
        else % For the case of Sin8
            for j = 1:3:length(coeffvals)-1
                Fit_temp = Fit_temp + coeffvals(j)*sin(coeffvals(j+1)*timespan(i) + coeffvals(j+2));
            end
            Fit(i) = Fit_temp;
        end
    end

j = 1;
avgVal = mean(Fit); % Use the mean value of the periodic function as a reference point
    for i = 1:length(timespan)-1
       if (Fit(i) <= avgVal && Fit(i+1) >= avgVal || Fit(i) >= avgVal && Fit(i+1) <= avgVal) % Mark the index where the curvefit passes through the mean value
           repeatIndex(j) = i;
           j = j + 1;
       end
    end

periodEnd = timespan(repeatIndex(3:3:end));
periodStr = timespan(repeatIndex(1:3:end-3));
if (length(periodEnd) - length(periodStr)) == 1 % Ensure that the vector lengths are consistent
    periodEnd(end) = [];
end
    
period = mean(periodEnd-periodStr); % Subtracting the vectors to find the average period

end