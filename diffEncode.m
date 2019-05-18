% Differential encoding routine.
% Encodes RGB color levels by taking difference of actual levels and
% predicted values of image.
function mxOut = diffEncode(mxIn)
    % cast input into signed int
    % type is int16 due to possible range of -255 to 255
    tempMxIn = int16(mxIn);

    % detect matrix dimensions
    [m, n] = size(mxIn);
    
    % prepare output dimensions
    mxOut = int16(zeros(m,n));
    
    % loop through all array entries
    % i = rows, j = columns
    for i = 1:m
        for j = 1:n
            if j==1
                % if in first column, no value exist to left and top-left of
                % current pixel.
                a = -1;
                c = -1;
            else
                % if not in first column, pixel exists to left of current pixel.
                % Take its value.
                a = tempMxIn(i, j-1);
            end

            if i==1
                % if in first row, no value exist to top and top-left of
                % current pixel.
                b = -1;
                c = -1;
            else
                % if not in first row, pixel exists above current pixel.
                % Take its value.
                b = tempMxIn(i-1, j);
            end

            if((a ~= -1) && (b ~= -1))
                % if not in first row or first column, pixel to top-left 
                % of current pixel exists, take its value
                c = tempMxIn(i-1, j-1); 
            end
            
            if ((a == -1) && (b == -1))
                % if current pixel is top-left most, take its value
                % directly as baseline.
                mxOut(1,1) = tempMxIn(1,1);
            else
                % otherwise, predict value of current pixel based on
                % surrounding pixel data.
                mxOut(i,j) = tempMxIn(i,j) - predict(a, b, c);
            end
        end
    end
end