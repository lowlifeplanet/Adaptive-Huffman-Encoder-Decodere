% Takes input image and encodes into differential filtered encoded file.
% Encoded file to be used by C program to Huffman encode
function mainEncode(inputFile, outputFile)
    % Check input format
    if(~ischar(inputFile))
        error('must input a string for input file name. Check README for format.');
    end
    
    if(~ischar(outputFile))
        error('must input a string for output file name. Check README for format.');
    end
    
    % Read input file
    rgbImage = imread(inputFile);
    
    % extract RGB component
    R = rgbImage(:,:,1);
    G = rgbImage(:,:,2); 
    B = rgbImage(:,:,3);

    % Get image dimensions
    [imgLen, imgWid] = size(R);

    % differential encode on each channel
    Renc = diffEncode(R);
    Genc = diffEncode(G);
    Benc = diffEncode(B);
    
    % open file stream
    fid = fopen(outputFile, 'wt');
    
    % write each channel to output file
    % Red channel
    for i=1:imgLen
        fprintf(fid, '%g', Renc(i,1));
        fprintf(fid, ',%g', Renc(i,2:imgWid));
        fprintf(fid,'\n');
    end
    % Green channel
    for i=1:imgLen
        fprintf(fid, '%g', Genc(i,1));
        fprintf(fid, ',%g', Genc(i,2:imgWid));
        fprintf(fid,'\n');
    end
    % Blue channel
    for i=1:imgLen
        fprintf(fid, '%g', Benc(i,1));
        fprintf(fid, ',%g', Benc(i,2:imgWid));
        fprintf(fid,'\n');
    end
    
    % close file stream
    fclose(fid);
end