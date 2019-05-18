% Takes Huffman decoded file (from C program) and decodes it back to RGB
% values, then displays the decoded image.
function mainDecode(encodedFile)
    % Check input format
    if(~ischar(encodedFile))
        error('must input a string for input file name. Check README for format.');
    end
    
    % Read encoded file
    encMx = importdata(encodedFile);
    
    % grab matrix dimensions
    [mm, n] = size(encMx);
    
    % Since input file entails all 3 channels in one elongated matrix,
    % must split matrix into 3 separate matrices by splitting rows.
    m = mm / 3;
    
    % Split concatenated matrix RGB channels
    R_enc = encMx(1:m, :);
    G_enc = encMx((m+1):(m*2), :);
    B_enc = encMx(((2*m) +1):mm, :);
    
    % Decode differential encoded channels
    Rdec = diffDecode(R_enc);
    Gdec = diffDecode(G_enc);
    Bdec = diffDecode(B_enc);
    
    % Show decoded image
    figure(1);
    imshow(cat(3, uint8(Rdec), uint8(Gdec), uint8(Bdec)));
    title('Decoded Image');
end