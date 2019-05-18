all: encoder decoder

encoder:
	g++ -o AH_Encoder huffmanEncoder.cpp adaptiveHuffman_Node.cpp adaptiveHuffman_Tree.cpp adaptiveHuffman_Encoder.cpp

decoder:
	g++ -o AH_Decoder huffmanDecoder.cpp adaptiveHuffman_Node.cpp adaptiveHuffman_Tree.cpp adaptiveHuffman_Decoder.cpp

test:
	g++ -o test test3.cpp adaptiveHuffman_Node.cpp adaptiveHuffman_Tree.cpp

clean:
	rm -f *.o test test2 test3 AH_Encoder AH_Decoder
