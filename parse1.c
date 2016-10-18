#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml2/libxml/xmlversion.h>
#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/parser.h>

void error(char *msg) {
	printf("\n%s\n", msg);
}

void parseDoc(char *docname, char *child, char *subchild) {	
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if(doc == NULL) {
		fprintf(stderr, "Parsing Unsuccessfull \n");
		return ;
	}

	cur = xmlDocGetRootElement(doc);

	if(cur == NULL) {
		fprintf(stderr, "Document is empty \n");
		xmlFreeDoc(doc);
		return;
	}

	cur = cur->xmlChildrenNode;
	while(cur != NULL) {
		if((!xmlStrcmp(cur->name, (const xmlChar *)child))) {
			parseNode(doc, cur, subchild);
		}
	cur = cur->next;
	}
	return;
}

void parseNode(xmlDocPtr doc, xmlNodePtr cur, char *subchild) {
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while(cur != NULL) {
/*In this part user can get the imporatant text or information encoded in XML */
/*This needs to get modified */
		if((!xmlStrcmp(cur->name, (const xmlChar *)subchild))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("%s\n", key);
			xmlFree(key);
		}
	cur = cur->next;
	}
	return;
}

/*int main(int argc, char*argv[]) {
	char *docname;

	if(argc < 4 || argc != 4) {
		printf("Usage : <docname> <child> <subchild> \n");
		return 0;
	}

	docname = argv[1];
}*/
/* We need to parse 
  <style:style style:name = "gr1" style family="graphic" style parent:parent-style-name="objectwithoutfill">
  and 
  <draw:page draw:name="page1"draw:style-name="dp1"draw:master-page-name="Default"> */
	parseDoc(argv[1], argv[2], argv[3]);
	return 0;

//	parseDoc(argv[1], "<office:automatic-styles>", "<style:style style:name="gr1" style:family="graphic" style:parent-style-name="objectwithoutfill">");

/*------------------------------------------------BMP---------------------------------------------------------------------------------------------------------*/
/* General Code for BMP starts here */

unsigned char bmp[200000];


void make()
{
	int i,color1=256,color2=54+4*color1;
	static unsigned char temp=0;

//FILE HEADER
	bmp[0] = 'B';
	bmp[1] = 'M';

//file size
	bmp[2] = 0xc6;
	bmp[3] = 0x05;
	bmp[4] = 0;
	bmp[5] = 0;

//reserved field     //Yet to be done
	for(i = 6; i < 10; i++) {
		bmp[i] = 0;
	}

//offset of pixel data inside the image

//If we did not use this or manipulate some values in this then just black screen appears on output image  
	bmp[10]=0x36;
	bmp[11]=0x04;
	for(i = 12; i < 14; i++) {
		bmp[i] = 0;
        }


//header size  //This is value is conventionally constant
	bmp[14] = 40;
	for(i = 15; i < 18; i++) {
		 bmp[i] = 0;
	}
//Width
	bmp[18] = 255;
	for(i = 19; i < 22; i++) {
		bmp[i] = 0;
	}
//Height of the image
	bmp[22] = 255;
   	for(i = 23; i < 26; i++) {
		bmp[i] = 0;
	}

//Colour planes
	bmp[26] = 1;
	bmp[27] = 0;

//number of bits per pixel   
//Here we have used 8 bits as only we are creating images in black and white form but if we need to use RGB i.e. Red Blue Green form then 24 bits is necessary
	bmp[28] = 8; // 1 byte
	bmp[29] = 0;

//compression method (no compression here) convention
	for(i = 30; i < 34; i++) {
		 bmp[i] = 0;
	}
//size of pixel data  //Yet to be done
   	bmp[34] = 0x90; 
    	bmp[35] = 0x01;
    	bmp[36] = 0;
    	bmp[37] = 0;

//Resolution yet to be done

 /*   // horizontal resolution of the image 
    	bmp[38] = 0;
    	bmp[39] = 0;
    	bmp[40] = 0;
    	bmp[41] = 0;

    // vertical resolution of the image 
    	bmp[42] = 0;
   	bmp[43] = 0;
    	bmp[44] = 0;
    	bmp[45] = 0;
*/
    // color palette information here 256			//Still did not use it significantly
    	bmp[46]=0;
    	bmp[47]=1;
   	for(i = 48; i < 50; i++) {
		bmp[i] = 0;
	}

    // number of important colors
    // if 0 then all colors are important
	for(i = 50; i < 54; i++) {
		bmp[i] = 0;
	}
    //Color Palette
    	for (i = 54;i<color2;i+=4) {
        	bmp[i]=temp;
        	bmp[i+1]=temp;
        	bmp[i+2]=temp;
        	bmp[i+3]=0;
        	temp++;
    	}
//Pixel data to be plotted on image window
    	for(i = color2; i < color2+16000; i++) {
		bmp[i] = 255;//255 because I have white square as image so far and 255 means brightest white and 0 means darkest black
	}

void write()
{
	FILE *file;
    	int i;

    	file = fopen("test.bmp", "wb+");
    	for( i = 0; i < 200000; i++) {
        	fputc(bmp[i], file);
    	}
    	fclose(file);
}
int main(int argc, char*argv[]) {
        char *docname;

        if(argc < 4 || argc != 4) {
                printf("Usage : <docname> <child> <subchild> \n");
                return 0;
        }

        docname = argv[1];
/* We need to parse 
  <style:style style:name = "gr1" style family="graphic" style parent:parent-style-name="objectwithoutfill">
  and 
  <draw:page draw:name="page1"draw:style-name="dp1"draw:master-page-name="Default"> */
        parseDoc(argv[1], argv[2], argv[3]);
      

//      parseDoc(argv[1], "<office:automatic-styles>", "<style:style style:name="gr1" style:family="graphic" style:parent-style-name="objectwithoutfill">");


//Main part for bmp;
	make();
  	write();
	return 0;
}

