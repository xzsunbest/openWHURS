function [trainTargets, trainOutputs, testTargets, testOutputs] = modelAnalysis(name, Mdl, trainX, trainY, testX, testY, shouldPlot, verbose)
%MODELANALYSIS Summary of this function goes here
%   Detailed explanation goes here
fprintf('****** Start Analysing %s ******\n', name);

disp('** Start Training Set Analysis **');
[trainTargets, trainOutputs] = classErrorAnalysis(trainX, trainY, Mdl, name, verbose);
if shouldPlot
    figure, plotconfusion(trainTargets, trainOutputs), title(sprintf('Training Set Confusion for %s', name));
end
disp('** End Training Set Analysis **');

disp('** Start Testing Set Analysis **');
[testTargets, testOutputs] = classErrorAnalysis(testX, testY, Mdl, name, verbose);
if shouldPlot
    figure, plotconfusion(testTargets, testOutputs), title(sprintf('Testing Set Confusion for %s', name));
end
disp('** End Testing Set Analysis **');

fprintf('****** End Analysing %s ******\n \n', name);
end

