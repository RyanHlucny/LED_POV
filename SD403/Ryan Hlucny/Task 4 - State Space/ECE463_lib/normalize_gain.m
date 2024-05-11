function [Kr] = normalize_gain(A,B,C)
% normalize_gain returns a gain Kr to normalize the DC gain to 1
Kr = inv(-C*inv(A)*B);
end