# TASK
a file compressor based on Huffman Coding which uses a custom save file format to compress text files with upto 40% reduction in size.

## Descriptionm

The compressor module creates a hufman tree from the given file, encrypts the file using the tree and stores the tree along with the encrypted text in a '.tcs' file which is a custom file format we created for this project.
The decompressor module uses the tree in the '.tcs' file to decompress the information and reproduce the original file.

##How To Use

To use the program, place the file you need to compress in the "huffman coding" folder and run the "main.cpp" file. Follow the instructions in the menu of the execution and enjoy a smaller file :).
