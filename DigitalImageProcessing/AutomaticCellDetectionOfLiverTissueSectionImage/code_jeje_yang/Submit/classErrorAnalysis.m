function [targets, outputs, numError, errorRate] = classErrorAnalysis(X, Y, Mdl, type, verbose)
numError = 0;
keySet = {'IC', 'C', 'CV', 'BD', 'PV'};
valueSet = [1, 2, 3, 4, 5];
labelMap = containers.Map(keySet, valueSet);
labelReverseMap = containers.Map(valueSet, keySet);

targets = zeros(length(valueSet), length(Y));
outputs = zeros(length(valueSet), length(Y));

for k = 1 : size(X, 1)
    if length(type) >= 3 && strcmp(type(end-2:end), 'Mdl')
        pred = predict(Mdl, X(k, :));
    elseif length(type) >= 6 && strcmp(type(end-5:end), 'MulNom')
        pred = multiNomPredict(Mdl, X(k, :));
    end
    actual = Y(k);
    
    targets(actual, k) = 1;
    outputs(pred, k) = 1;
    
    if pred ~= actual
        numError = numError + 1;
        if verbose
            fprintf('Model: %s - Predicted: %s, Actual: %s \n', type, labelReverseMap(pred), labelReverseMap(actual));
        end
    end
end

errorRate = numError / size(X, 1);
fprintf('Number of errors: %d out of %d\n', numError, size(X, 1));
fprintf('Error rate: %f \n', errorRate);


end

