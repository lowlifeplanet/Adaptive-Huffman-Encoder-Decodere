Use the Matlab code to differential encode and decode.
Use the C programs to Huffman encode and decode.

--------------------------

Flowchart:
	Encoding:
		Original Image >>> (Matlab mainEncode.m) >>> Differential encoded file >>> (C AH_Encoder executable) >>> Encoded file
	Decoding:
		Encoded file >>> (C AH_Decoder executable) >>> Differential encoded file >>> (Matlab mainDecode.m) >>> Original Image's PREVIEW

--------------------------

Differential encode routine:
	Run Matlab
	Change current working directory to where 'mainEncode.m' resides
	In Matlab console, run the following:
		`$ mainEncode('inputFile', 'outputFile')`
	Where inputFile is the input image, and outputFile is the file to save to.
	Note that both names must be surrounded by '' in order to be recognized.
	
Differential decode routine:
	Run Matlab
	Change current working directory to where 'mainEncode.m' resides
	In Matlab console, run the following:
		`$ mainDecode('encodedFile')`
	Where encodedFile is the file that contains the differential encoded information.
	Note that the file name must be surrounded by '' in order to be recognized.	

-------------------------

Adaptive Huffman encode routine:
	Build the executables with the provided Makefile
	Execute AH_Encoder
	When prompted, enter the file name of the differential encoded file as input,
	and the desired name for the output file.

Adaptive Huffman decode routine:
	Build the executables with the provided Makefile
	Execute AH_Decoder
	When prompted, enter the file name of the Huffman encoded file as input,
	and the desired name for the output file.

--------------------------

Additional notes:

	Our implementation of the encoders both save their information as individual characters (the characters 1 and 0 as opposed to bits 1 and 0). As such, the encoded file will likely end up larger than the non-encoded version. Since each ASCII character takes 8 bits to represent, the actual sizes that the encoded file would take in real implementations would be approximately 1/8 of our implementation (give or take some header space to represent file length).
