function [Kx] = ppl(A, B, P)
% ppl - pole placement routine for full-state feedback system
%   This routine is an implementation of Bass Gura method of determining
%   the feedback gains for a full-state feedback system

%% First, A and B must be put into controller canonical form
% Find similarity transform which takes A and B to controller canonical
% form: T = T1*T2*T3

% T1 = controllability matrix
T1 = zeros(size(A));
for i = 1:length(A)
    T1(:,i) = A^(i-1)*B;
end
if (rank(T1) < length(A))
    warning('System not Controllable');
    return;
end

% T2 = a matrix related to the system's characteristic polynomial
p = poly(A);
T2 = zeros(size(A));
for i = 1:length(A)
    T2(i,:) = [zeros(1,i-1) p(1,1:(length(A)-i+1))];
end

% T3 = flip matrix
T3 = flip(A^0);

% Putting it all together
T = T1*T2*T3;
Az = inv(T)*A*T;
Bz = inv(T)*B;

%% Find the feedback gains to match current poles to desired poles
Pd = poly(P);
Pa = poly(A);
dP = Pd-Pa;
Kz = flip(dP(2:end));

%% Convert back to original form and return gains
Kx = Kz*inv(T);
end