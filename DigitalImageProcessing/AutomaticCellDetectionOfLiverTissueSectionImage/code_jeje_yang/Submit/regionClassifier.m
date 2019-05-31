close all; clc; clear all;

files = dir('extractedFeatures2/*.mat');
keySet = {'IC', 'C', 'CV', 'BD', 'PV'};
valueSet = [1, 2, 3, 4, 5];
labelMap = containers.Map(keySet, valueSet);
labelReverseMap = containers.Map(valueSet, keySet);


%% Get Test and train data 

trainFiles = [1,3,4,5,7,9]; %[1, 2, 3, 4, 5, 6, 7, 8];
testFiles = [2,6,8]; %[9, 10, 11];

trainX = [];
trainY = [];

testX = [];
testY = [];

for k = 1 : length(files)
    load(strcat('extractedFeatures2/', files(k).name))
    
    Xk = cell2mat(struct2cell(rmfield(cellProps, {'Centroid', 'PixelList', 'BoundingBox', 'label', 'Orientation'})))';
    Yk = zeros(length(cellProps), 1);
    for l = 1 : length(cellProps)
        Yk(l) = labelMap(cellProps(l).label);
    end  
    ind = find(Yk ~= 1);
    
    if ismember(k, trainFiles)
        trainX = [trainX; Xk];%(ind)];
        trainY = [trainY; Yk];%(ind)];
    elseif ismember(k, testFiles)
        testX = [testX; Xk];%(ind)];
        testY = [testY; Yk];%(ind)];
    end
    
end

[balancedTrainX, balancedTrainY] = balanceTrainSet(trainX, trainY);

%% Train Multiclass One-to-One Binary SVM classifier on BASIC data

tic
SVMMdl = fitcecoc(trainX, trainY);
toc;

%% Train Multiclass One-to-One Binary SVM classifier on BALANCED data

tic
SVMBalMdl = fitcecoc(balancedTrainX, balancedTrainY);
toc;

%% Train K nearest neighbor classifier on BASIC data

tic 
KNNMdl = fitcknn(trainX, trainY, 'NumNeighbors', 2);
toc;

%% Train K nearest neighbor classifier on BALANCED data

tic 
KNNBalMdl = fitcknn(balancedTrainX, balancedTrainY, 'NumNeighbors', 2);
toc;


%% Train Multinomial logistic regression on BASIC data

tic;
B_basic = mnrfit(trainX, trainY);
toc;

%% Train Multinomial logistic regression on BALANCED data

tic;
B_balanced = mnrfit(balancedTrainX, balancedTrainY);
toc;

%% Test

% numError = 0;
% counts = [0, 0, 0, 0, 0];
% testCounts = [0, 0, 0, 0, 0];
% 
% targets = zeros(length(valueSet), length(testY));
% 
% outputsSVM = zeros(length(valueSet), length(testY));
% outputsKNN = zeros(length(valueSet), length(testY));
% outputsMulNom = zeros(length(valueSet), length(testY));
% 
% for k = 1 : size(testX, 1)
% %     if testY(k) == 1
% %         continue
% %     end
% %     targetk = zeros(size(valueSet))';
% %     outputk = zeros(size(valueSet))';
%     
%     pred = predict(SVMMdl, testX(k, :));
%     actual = testY(k);
%     
%     targets(actual, k) = 1;
%     outputs(pred, k) = 1;
%         
%     counts(pred) = counts(pred) + 1;
%     testCounts(actual) = testCounts(actual) + 1;
%     if pred ~= actual
%         numError = numError + 1;
%         %pred, testY(k)
%         fprintf('Predicted: %s, Actual: %s \n', labelReverseMap(pred), labelReverseMap(actual));
%     end
% end
% 
% fprintf('Number of errors: %d \n', numError);
% fprintf('Error rate: %f \n', numError / size(testX, 1));
% counts
% testCounts
% 
% figure, plotconfusion(targets, outputs);

%% Test

%targets = zeros(length(valueSet), length(testY));

%[trainTargets, trainOutputs, testTargets, testOutputs] = modelAnalysis('SVMMdl', SVMMdl, trainX, trainY, testX, testY);

% [trainTargetsSVM, trainOutputsSVM] = classErrorAnalysis(trainX, trainY, SVMMdl, 'SVMMdl');
% [testTargetsSVM, testOutputsSVM] = classErrorAnalysis(testX, testY, SVMMdl, 'SVMMdl');



%[targets, outputsKNN] = zeros(length(valueSet), length(testY));
%[targets, outputsMulNom] = zeros(length(valueSet), length(testY));

%% SVM basic data test

[trainTargets, trainOutputs, testTargets, testOutputs] = ...
    modelAnalysis('SVMMdl', SVMMdl, trainX, trainY, testX, testY, false, false);

%% SVM balanced data test

[trainTargets, trainOutputs, testTargets, testOutputs] = ...
    modelAnalysis('SVMBalMdl', SVMBalMdl, balancedTrainX, balancedTrainY, testX, testY, false, false);

%% KNN basic data test

[trainTargets, trainOutputs, testTargets, testOutputs] = ...
    modelAnalysis('KNNMdl', KNNMdl, trainX, trainY, testX, testY, false, false);

%% SVM balanced data test

[trainTargets, trainOutputs, testTargets, testOutputs] = ...
    modelAnalysis('KNNBalMdl', KNNBalMdl, balancedTrainX, balancedTrainY, testX, testY, false, false);

%% KNN basic data test

[trainTargets, trainOutputs, testTargets, testOutputs] = ...
    modelAnalysis('MulNom', B_basic, trainX, trainY, testX, testY, false, false);

%% SVM balanced data test

[trainTargets, trainOutputs, testTargets, testOutputs] = ...
    modelAnalysis('BalMulNom', B_balanced, balancedTrainX, balancedTrainY, testX, testY, false, false);