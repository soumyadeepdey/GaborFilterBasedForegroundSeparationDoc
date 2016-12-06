function M = load_matrix(filename)
%
% M = load_matrix(filename)
%
% INPUT:
%    filename: file (.matrix) to load
%
% OUTPUT:
%    M: matrix

fid = fopen(filename, 'r');


n = fread(fid, 1, 'integer*4');
d = fread(fid, 1, 'integer*4');

M = zeros(n, d, 'single');
for i = 1 : n;
    M(i,:) = fread(fid, d, 'single');
end;

    
fclose(fid);