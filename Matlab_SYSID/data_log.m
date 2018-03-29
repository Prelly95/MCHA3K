data10 = dlmread('sin10.csv',',',1,0);

t = data10(:, 1);

figure;
hold on;

plot(t, data10(:, 2));