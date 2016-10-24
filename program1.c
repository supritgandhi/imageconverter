#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <errno.h>
#include <fcntl.h>

#define LINE 10
#define RECTANGLE 20
#define ELLIPSE 30

unsigned char bmp[200000];
int shape;


void msg(char *msg)
{
  printf("%s", msg);
  return;
}
void file() {
	FILE *fptr1, *fptr2;

	fptr1 = fopen("bmp_data.txt", "w");
	fclose(fptr1);
}	

void getReference_geometry(xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *type;
//	cur = cur->xmlChildrenNode;
	while(cur != NULL) {
		if((!xmlStrcmp(cur->name, (const xmlChar *)"enhanced-geometry"))) {
			type = xmlGetProp(cur, "type");
			printf("TYPE : %s\n", type);

			if((!strcmp(type, "rectangle"))) { 
				shape = RECTANGLE;
			}
			else if((!strcmp(type, "ellipse"))) {
				shape = ELLIPSE;
			}
			else
				shape = LINE;
		}
	cur = cur->next;
	}
}

void parseNode_geometry(xmlDocPtr doc, xmlNodePtr cur, char *subchild) {
//	printf("Check\n");
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"custom-shape"))) {
                        getReference_geometry(doc, cur);
                        // key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

                        //printf("%s\n", key);
                        //xmlFree(key);
                }
                cur = cur->next;
        }
        return;
}
			

void getReference (xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *x1, *x2, *y1, *y2, *width, *height, *x, *y;
	double X1, X2, Y1, Y2, WIDTH, HEIGHT, X, Y;
	xmlNodePtr temp;
//	temp = cur;
	cur = cur->xmlChildrenNode;
	temp = cur;
        while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"line"))) {
                        x1 = xmlGetProp(cur, "x1");
                        printf("x1: %s\n", x1);
			x2 = xmlGetProp(cur, "x2");
                        printf("x2: %s\n", x2);
			y1 = xmlGetProp(cur, "y1");
                        printf("y1: %s\n", y1);
			y2 = xmlGetProp(cur, "y2");
                        printf("y2: %s\n", y2);

			X1 = atof(x1);
			X2 = atof(x2);
			Y1 = atof(y1);
  			Y2 = atof(y2);


			FILE *fptr;
			fptr = fopen("bmp_data.txt", "w");
			fprintf(fptr, "%lf ", X1);
			fprintf(fptr, "%lf \n", X2);
			fprintf(fptr, "%lf ", Y1);
			fprintf(fptr, "%lf \n", Y2);

			fclose(fptr);

                        xmlFree(x1);
			xmlFree(x2);
			xmlFree(y1);
			xmlFree(y2);

                }

		else if((!xmlStrcmp(cur->name, (const xmlChar *)"custom-shape"))) {

			xmlChar *type;		
	
			width = xmlGetProp(cur, "width");
			printf("WIDTH: %s\n", width);
			height = xmlGetProp(cur, "height");
                        printf("HEIGHT: %s\n", height);
			x = xmlGetProp(cur, "x");
                        printf("X: %s\n", x);	
			y = xmlGetProp(cur, "y");
                        printf("Y: %s\n", y);

			WIDTH = atof(width);
                        HEIGHT = atof(height);
                        Y = atof(y);
                        X = atof(x);
			
                        FILE *fptr;
                        fptr = fopen("bmp_data.txt", "w");
                        fprintf(fptr, "%lf ", X);
                        fprintf(fptr, "%lf \n", Y);
                        fprintf(fptr, "%lf ", HEIGHT);
                        fprintf(fptr, "%lf \n", WIDTH);

                        fclose(fptr);

                        xmlFree(x);
                        xmlFree(y);
                        xmlFree(height);
                        xmlFree(width);

//			parseNode_geometry(doc, cur, "custom-shape");
			temp = temp->xmlChildrenNode;
			while(temp != NULL) {
				if ((!xmlStrcmp(temp->name, (const xmlChar *)"enhanced-geometry"))){
					getReference_geometry(doc, temp);
				/*	type = xmlGetProp(cur, "type");
                        		printf("TYPE : %s\n", type);

                        		if((!strcmp(type, "rectangle"))) { 
                                		return RECTANGLE;
                        		}		
                        		else if((!strcmp(type, "ellipse"))) {
                                		return ELLIPSE;
                        		}
                        		else
                                		return LINE;*/
					
				}
			temp = temp->next;
			}
		}
			
        cur = cur->next;
        }
        return;
}

void parseNode1 (xmlDocPtr doc, xmlNodePtr cur, char *subchild) {

	xmlChar *key;
	cur = cur->xmlChildrenNode;
   	while (cur != NULL) {
        	if ((!xmlStrcmp(cur->name, (const xmlChar *)"page"))) {
           		getReference(doc, cur);
           		// key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            		//printf("%s\n", key);
            		//xmlFree(key);
        	}
        	cur = cur->next;
    	}
    	return;
}

 
void parseNode (xmlDocPtr doc, xmlNodePtr cur, char *subchild) {
 
	xmlChar *key;
    	cur = cur->xmlChildrenNode;
    	while (cur != NULL) {
        	if ((!xmlStrcmp(cur->name, (const xmlChar *)"drawing"))) {
	    		parseNode1(doc, cur, "page");
           		// key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
 
            		//printf("%s\n", key);
            		//xmlFree(key);
        	}
        	cur = cur->next;
    	}
   	return;
}
 
static void parseDoc (char *docname, char *child, char *subchild) {
	xmlDocPtr doc;
    	xmlNodePtr cur;
 
    	doc = xmlParseFile(docname);
 
    	if (doc == NULL ) {
        	msg("Document parsing failed. \n");
	}
 
    	cur = xmlDocGetRootElement(doc); //Gets the root element of the XML Doc
    
    	if (cur == NULL) {
        	xmlFreeDoc(doc);
        	msg("Document is Empty!!!\n");
    	}
	if (xmlStrcmp(cur->name, (const xmlChar *) "document-content")) {
		fprintf(stderr,"document of the wrong type, root node is incorrect");
		xmlFreeDoc(doc);
		return;
	} 
    	cur = cur->xmlChildrenNode;
    	while (cur != NULL) {
        	if ((!xmlStrcmp(cur->name, (const xmlChar *)"body"))) {
            		parseNode (doc, cur, subchild);
        	}	
        	cur = cur->next;
    	}	
	//    getReference (doc, cur);
 
    	xmlFreeDoc(doc);

    	return;
}
/*
void
getReference (xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *uri;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"draw:line"))) {
			uri = xmlGetProp(cur, "uri");
			printf("uri: %s\n", uri);
			xmlFree(uri);
		}
	cur = cur->next;
	}
	return;
}
*/
/*void
parseDoc(char *docname) {
	xmlDocPtr doc;
	xmlNodePtr cur;
	doc = xmlParseFile(docname);
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	printf("Test 1\n");
	if (xmlStrcmp(cur->name, (const xmlChar *) "document-content")) {
//	if (strcmp(cur->name, "office:document-content")) {
		fprintf(stderr,"document of the wrong type, root node != story ");
//		perror("Error :");
		xmlFreeDoc(doc);
		return;
	}	
	printf("Test 2\n");
	getReference (doc, cur);
	xmlFreeDoc(doc);
	return;
}*/
/*int
main(int argc, char *argv[]) {
	char *docname;
	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}
	docname = argv[1];
	parseDoc (docname);
	return (1);
}*/

void create_bmp() {
	int i, color1 = 256, color2 = 54 + (4 * color1);
	int ref = color2;
	static unsigned char temp = 0;

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
	bmp[10] = 0x36;
	bmp[11] = 0x04;
	for(i = 12; i < 14; i++) {
		bmp[i] = 0;
        }


//header size  //This is value is conventionally constant
	bmp[14] = 40;
	for(i = 15; i < 18; i++) {
		 bmp[i] = 0;
	}
//Width
	bmp[18] = 180;
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
    	bmp[46] = 0;
    	bmp[47] = 1;
   	for(i = 48; i < 50; i++) {
		bmp[i] = 0;
	}

    // number of important colors
    // if 0 then all colors are important
	for(i = 50; i < 54; i++) {
		bmp[i] = 0;
	}
    //Color Palette
    	for (i = 54;i < color2; i+=4) {
        	bmp[i] = temp;
        	bmp[i + 1] = temp;
        	bmp[i + 2] = temp;
        	bmp[i + 3] = 0;
        	temp++;
    	}
//Pixel data to be plotted on image window
    	for(i = ref; i < (ref + 65536); i++) {
		bmp[i] = 255;//255 because I have white square as image so far and 255 means brightest white and 0 means darkest black
	}

	FILE *fptr;
	double arr_bmp[10];
	float x1, x2, y1, y2;
	int y, x, Y1, Y2, X1, X2, j, count = 0, height, width, height1, X, Y;
	int HEIGHT, WIDTH, WIDTH1, HEIGHT1;
	fptr = fopen("bmp_data.txt", "r");
	while((fscanf(fptr, "%lf ", &arr_bmp[count])) != EOF) {
		count++;
	}
	if(shape == LINE) {
		x1 = (int)arr_bmp[0]; 
		x2 = (int)arr_bmp[1];
		y1 = (int)(27 - arr_bmp[2]);
		y2 = (int)(27 - arr_bmp[3]);

		if(y1 == y2) {
			y = ((9 * (y2 - 1)) * 180);
			X1 = y + (9 * (x1 - 1));
			printf("%d\n", X1);
			X2 = X1 + (9 * (x2 - 1));
			printf("!!!\n");
			for(j = ref + X1; j < (ref + X2); j++) {
				bmp[j] = 0;
			}	
		}	
	
	
		else if(x1 == x2) {
			Y1 = (180 * (9 * y1));
			Y2 = (180 * (9 * y2));
			x = Y1 + ( 9 * x1);
			for(j = ref + x; j < ref + Y2; j+=256) {
				bmp[j] = 0;
			}	
		
		}
	} 

	if(shape == RECTANGLE) {
		x = (int)arr_bmp[0];			
		y = (int)(27 - arr_bmp[1]);
		height = (int)arr_bmp[2];
		width = (int)arr_bmp[3];
		height1 = y - height;
		
		Y = ((9 * (y - 1)) * 180);
		HEIGHT1 = ((9 * (height1 - 1)) * 180);

		X = Y + (9 * (x - 1));	
		X1 = HEIGHT1 + (9 * (x - 1));

		WIDTH = Y + (9 * (width - 1));
		WIDTH1 = HEIGHT1 + (9 * (width - 1));		
//		HEIGHT = Y + (9 * (height - 1));
//		HEIGHT1 = y - height;
		bmp[ref + X] = 0;
		bmp[ref + WIDTH] = 0;
		for(j = ref + X; j < (ref + WIDTH); j++) {
			bmp[j] = 0;
		}
		bmp[ref + X1] = 0;
		for( j = ref + X1; j < (ref + WIDTH1); j++) {
			bmp[j] = 0;
		}
	/*	for(j = ref + HEIGHT1; j > (ref + (HEIGHT * 180)); j-=256) {
			bmp[j] = 0;
		}*/
		
	}
}

void write_bmp() {
	FILE *file;
    	int i;

    	file = fopen("test.bmp", "wb+");
    	for( i = 0; i < 200000; i++) {
        	fputc(bmp[i], file);
    	}
    	fclose(file);
}
 
int main(int argc, char *argv[]) {
	char *docname;
/* 	if (argc != 4) {
        	printf("Usage: %s <docname> <child> <subchild>\n", argv[0]);
        	return(0);
    	}*/
 
//    	docname = argv[1];
//    	parseDoc (docname,argv[2],argv[3]);
	parseDoc("content2.xml", "body", "drawing");
	
	create_bmp();
	write_bmp();


	
    	return (1);
}
