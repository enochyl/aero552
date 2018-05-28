function processed = movingAvg(n, t, inputData)
%
% PURPOSE: Process data using a moving-average
%
% INPUTS:
%           n : window size
%           t : time vector
%   inputData : Input data
%
% OUTPUTS:
%   processed : Processed data
%
    processed = zeros(1,length(t));
    for i = n:length(t)
       % Moving average, using the past n data points
       processed(i) = inputData(i);
       if (i < length(t)-n-1)
           for j=1:n-1
              processed(i) = processed(i)+inputData(i-j);
           end
       end
    end
    processed = processed./n;
end