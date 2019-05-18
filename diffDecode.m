% Differential decoding routine.
% Decodes differentially encoded image data back to original RGB levels.
function mxDec = diffDecode(mxEnc)
    % detect matrix dimensions
    [m,n] = size(mxEnc);
    
    % prepare output dimensions
    mxDec = zeros(m,n);
    
    % loop through all array entries
    % i = rows, j = columns
    for i=1:m
       for j=1:n
           % Note that unlike the encoder, predictions are based on already
           % decoded values, thus we read from mxDec instead of mxEnc.
            if j==1
                % if in first column, no value exist to left and top-left of
                % current pixel.
                a = -1;
                c = -1;
            else
                % if not in first column, pixel exists to left of current pixel.
                % Take its value.
                a = mxDec(i, j-1);
            end

            if i==1
                % if in first row, no value exist to top and top-left of
                % current pixel.
                b = -1;
                c = -1;
            else
                % if not in first row, pixel exists above current pixel.
                % Take its value.
                b = mxDec(i-1, j);
            end

            if((a ~= -1) && (b ~= -1))
                % if not in first row or first column, pixel to top-left 
                % of current pixel exists, take its value
                c = mxDec(i-1, j-1); 
            end

            if ((a == -1) && (b == -1))
                % if current pixel is top-left most, take its value
                % directly as baseline.
                mxDec(1,1) = mxEnc(1,1);
            else
                % otherwise, predict value of current pixel based on
                % surrounding pixel data.
                mxDec(i,j) = mxEnc(i,j) + predict(a, b, c);
            end
       end
    end
    
    % cast output back into uint8 for image color levels.
    mxDec = uint8(mxDec);
end