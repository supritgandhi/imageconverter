IMAGE CONVERTER
        This code basically converts .odg [open document graphic] to .bmp [bitmap]. 
        .odg stores data in vector graphic form in the form of XML.
        So, this code converts XML into human readable format i.e. this process is called XML Parsing.
        After getting the data from the xml it is plotted into bmp format using bmp.c .
.
In this two files have been uploaded,
one is parse1.c which roughly contains the code of whole project which is yet to finished completely and other is,
bmp.c which is a part of parse1.c which  creates a bmp image window with black background which executes.

Instructions :
For compiling bmp.c simply, type : gcc bmp.c and to run ./a.out
For compiling parse1.c, type : gcc parse1.c -I/usr/include/libxml2/libxml -L/usr/lib -lxml2 -lz -lpthread -lm
