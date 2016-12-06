function write_matrix(M, outputFile)
% write_matrix(M, outputFile)
% 
% M: matrix with singles
% outputFile: filename to save

fid = fopen(outputFile, 'w');


[n, d] = size(M);
fwrite(fid, n, 'integer*4');
fwrite(fid, d, 'integer*4');
Features = 1:d;
for j = 1 : n
    fwrite(fid, M(j,Features)', 'single');
end;



fclose(fid);