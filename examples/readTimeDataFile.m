function [ M S ] = readTimeDataFile( f )
%READTIMEDATAFILE Read time data value (3d) file into M(x,y,t)
% with the scales in S [1 -> T start, T end, 2 -> X Scale, 3 -> Y Scale]
    fid = fopen(f);
    
    X = zeros(3,1);
    S = zeros(3,2);
    for i = 1:3
        tline = fgetl(fid);
        N = sscanf(tline, '%i %f %f');
        X(i) = N(1);
        S(i,1) = N(2);
        S(i,2) = N(3);
    end
    
    M = zeros(X(2),X(3),X(1));
    for i = 1:X(1)
        fgetl(fid);
        for j = 1:X(2)
            tline = fgetl(fid);
            row = sscanf(tline, '%f');
            M(j,:,i) = row';
        end
    end
    
    fclose(fid);
end

