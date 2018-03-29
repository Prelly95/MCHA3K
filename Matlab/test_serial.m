% Setup and open the serial connection
s = serial('COM4', 'BaudRate', 115200);   % TODO: Modify port and rate as needed
fopen(s);
try % you can't succeed if you don't
    % Set a variable
    fprintf(s, 'set x 123.456\n');

    % Retrieve a variable
    x = query(s, 'get x', '%s\n', 'x is %g\n')
    % If you need to troubleshoot the query command, it is equivalent to the following two lines:
    % fprintf(s, 'get x');        % Note: serial fprintf already includes newline
    % x = fscanf(s, 'x is %g\n')
catch hot_potato
    fclose(s); delete(s); clear s   % Close serial connection and clean up
    rethrow(hot_potato); % Someone else's problem now
end
fclose(s); delete(s); clear s       % Close serial connection and clean up

% Note: If the serial object is not cleaned up before opening another connection, 
%       Matlab will lose its handle to the previous serial object and lock the 
%       port, requiring a session restart to release the resource.