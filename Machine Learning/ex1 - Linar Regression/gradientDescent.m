function [theta, J_history] = gradientDescent(X, y, theta, alpha, num_iters)
%GRADIENTDESCENT Performs gradient descent to learn theta
%   theta = GRADIENTDESCENT(X, y, theta, alpha, num_iters) updates theta by 
%   taking num_iters gradient steps with learning rate alpha

% Initialize some useful values
m = length(y); % number of training examples
J_history = zeros(num_iters, 1);

for iter = 1:num_iters

    % ====================== YOUR CODE HERE ======================
    % Instructions: Perform a single gradient step on the parameter vector
    %               theta. 
    %
    % Hint: While debugging, it can be useful to print out the values
    %       of the cost function (computeCost) and gradient here.
    %
    
    prediction = X * theta;
    % to remove sum we need to multiby by X matrix
    %change1 = (1/m) * (prediction - y)' * X; 
    % prediction - y is 97x1 , X is 97x2 so (pred -y)' is 1x97
    change2 =  (1/m) * (X' * (prediction-y));
    % so both will give 1x2 and we need 2x1,will cost us another transpose operation
    %theta = theta - alpha * change1';
    theta = theta - alpha * change2;
    
    %fprintf('this is theta1 %f , this is theta2 %f \n', theta(1), theta(2));
    % ============================================================

    % Save the cost J in every iteration    
    J_history(iter) = computeCost(X, y, theta);

end

end
