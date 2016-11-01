/*****************************************************************************
 * Copyright (C) SUPRIT ATUL GANDHI supritgandhi@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/


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
int shape1, shape2, shape3;
char function_file[50];

void create_bmp();
void write_bmp(char *file_name);
void default_bmp();
void bmp1();

void msg(char *msg) {
  printf("%s", msg);
  return;
}
	
void getReference_geometry(xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *type;
	while(cur != NULL) {
		if((!xmlStrcmp(cur->name, (const xmlChar *)"enhanced-geometry"))) {
			type = xmlGetProp(cur, "type");
			printf("TYPE : %s\n", type);

			if((!strcmp(type, "rectangle"))) { 
				shape1 = RECTANGLE;
			}
			if((!strcmp(type, "ellipse"))) {
				shape1 = ELLIPSE;
			}
/*			else
				shape = LINE;*/		
		}
	cur = cur->next;
	}
}

void parseNode_geometry(xmlDocPtr doc, xmlNodePtr cur, char *subchild) {
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"custom-shape"))) {
                        getReference_geometry(doc, cur);
                }
                cur = cur->next;
        }
        return;
}
			

void getReference (xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *x1, *x2, *y1, *y2, *width, *height, *x, *y;
	double X1, X2, Y1, Y2, WIDTH, HEIGHT, X, Y;
	xmlNodePtr temp;

	cur = cur->xmlChildrenNode;
	temp = cur;
        while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"line"))) {

			shape1 = LINE;
								//Printf for co-ordinates have been commented.
                        x1 = xmlGetProp(cur, "x1");
                //      printf("x1: %s\n", x1);
			x2 = xmlGetProp(cur, "x2");
                //      printf("x2: %s\n", x2);
			y1 = xmlGetProp(cur, "y1");
                //      printf("y1: %s\n", y1);
			y2 = xmlGetProp(cur, "y2");
                //      printf("y2: %s\n", y2);

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
			bmp1();				//BMP function call
		

                        xmlFree(x1);
			xmlFree(x2);
			xmlFree(y1);
			xmlFree(y2);
			
		

                }

		 else if((!xmlStrcmp(cur->name, (const xmlChar *)"custom-shape"))) {

			xmlChar *type;		
	
			width = xmlGetProp(cur, "width");
		//	printf("WIDTH: %s\n", width);
			height = xmlGetProp(cur, "height");
                //      printf("HEIGHT: %s\n", height);
			x = xmlGetProp(cur, "x");
                //      printf("X: %s\n", x);	
			y = xmlGetProp(cur, "y");
                //      printf("Y: %s\n", y);

			WIDTH = atof(width);
                        HEIGHT = atof(height);
                        Y = atof(y);
                        X = atof(x);
			


                        xmlFree(x);
                        xmlFree(y);
                        xmlFree(height);
                        xmlFree(width);


			shape1 = RECTANGLE;
			FILE *fptr;
                        fptr = fopen("bmp_data.txt", "w");
                
           
                  	fprintf(fptr, "%lf ", X);
                        fprintf(fptr, "%lf \n", Y);
                        fprintf(fptr, "%lf ", HEIGHT);
                        fprintf(fptr, "%lf \n", WIDTH);
			fclose(fptr);
          		bmp1();			//BMP function call.
                        			

                        			
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
	
 
    	xmlFreeDoc(doc);

    	return;
}

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

//reserved field     
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
//size of pixel data  
   	bmp[34] = 0x90; 
    	bmp[35] = 0x01;
    	bmp[36] = 0;
    	bmp[37] = 0;

//Resolution yet to be done

    // horizontal resolution of the image 
    	bmp[38] = 0;
    	bmp[39] = 0;
    	bmp[40] = 0;
    	bmp[41] = 0;

    // vertical resolution of the image 
    	bmp[42] = 0;
   	bmp[43] = 0;
    	bmp[44] = 0;
    	bmp[45] = 0;

    // color palette information here 2
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
}
void bmp1(){
 	int i, color1 = 256, color2 = 54 + (4 * color1);
        int ref = color2;
        static unsigned char temp = 0;
	
	FILE *fptr;
	double arr_bmp[10];
	float x1, x2, y1, y2, slope, incrementf;
	int y, x, Y1, Y2, X1, X2, j, count = 0, height, width, height1, X, Y, increment;
	int HEIGHT, WIDTH, WIDTH1, HEIGHT1;
	fptr = fopen("bmp_data.txt", "r");
	while((fscanf(fptr, "%lf ", &arr_bmp[count])) != EOF) {
		count++;
	}

	if(shape1 == LINE) {
		x1 = (int)arr_bmp[0]; 
		x2 = (int)arr_bmp[1];
		y1 = (int)(27 - arr_bmp[2]);
		y2 = (int)(27 - arr_bmp[3]);


		if(y1 == x2) {
			y = (int)((9 * (y2 - 1)) * 180);
			X1 = y + (9 * (x1));
			X2 = X1 + (5 * (x2 - 1));
			for(j = ref + X1; j < (ref + X2); j++) {
				bmp[j] = 0;
			}
		}

		else if(y1 == y2) {
			
			if(x2 > x1) {
				width = x2 - x1;
				Y = ((10 * y1) * 180);
				X = Y + (9.5 * x1);
				WIDTH = X + (10 * width);
		
				bmp[ref + X] = 0;
				bmp[ref + WIDTH] = 0;
				for(j = ref + X; j < ref + WIDTH; j++) {
					bmp[j] = 0;
				}
			}
			else if(x1 > x2) {
				width = x1 - x2;
				Y = ((10 * y1) * 180);
				X = Y + (9.5 * x2);
				WIDTH = X + (10 * width);
		
				for(j = ref + X;j < ref + WIDTH; j++) {
					bmp[j] = 0;
				}
			}
		}
			
		if(x1 == x2) {
			if( y1 > y2) {
				Y1 = (int)(180 * (10 * (y1 )));
				Y2 = (int)(180 * (11 * (y2)));
		
				height = y1 - y2;
				height1 = y1 - height;

				HEIGHT1 = ((10 * (height1) * 180));
				X1 = HEIGHT1 + (9.5 * x1);
				X =  Y1 + (9 * x1);
			
//				bmp[ref + X1 + Y1] = 0;				Intial and final pixels are commented, it is used for testing.
//				bmp[ref + X1 + Y2] = 0;
			
				for(j = ref + X1; j <= ref + X; j+=180) {
					bmp[j] = 0;
				}
			}

			else if(y2 > y1) {
				Y2 = (int)(180 * (10 * (y2)));
				Y1 = (int)(180 * (11 * (y1)));

				height = y2 - y1;
				height1 = y2 - height;
			
				HEIGHT1 = ((10 * (height1) * 180));
				X1 = HEIGHT1 + (9.5 * x2);
				X = Y2 + (9 * x1);
		//		printf("%d %d\n ", X1, X);
		
				for(j = ref + X1; j <= ref + X; j+=180) bmp[j] = 0;
			}
		}	
		
		if((x1 != x2) && (y1 != y2)) {
			x1 = arr_bmp[0];
               		x2 = arr_bmp[1];
                	y1 = (27 - arr_bmp[2]);
                	y2 = (27 - arr_bmp[3]);

			slope = (( y2 - y1) / ( x2 - x1));
			printf("slope : %f\n", slope);
		//	increment =(int)((180 * 180) / (256 * slope));
			increment = (int)(256 / slope);
			printf("increment : %d\n", increment);
			incrementf =((180 * 180) / (256 * slope));
			printf("incrementf %f\n", incrementf);
			Y1 = (int)((9 * (y1 - 1)) * 180);
			Y2 = (int)((9 * (y2 - 1)) * 180);
			X1 = (int)(Y1 + (9 * (x1 - 1)));
			X2 = (int)(Y2 + (9 * (x2 - 1)));
			bmp[ref + X1] = 0;
			bmp[ref + X2] = 0;
			for(j = ref + X1; j < ref + X2; j+=increment) {
				bmp[j] = 0;
			}
			
			
		}
	}
		
	 

	if(shape1 == RECTANGLE) {
		x = (int)arr_bmp[0];			
		y = (int)(27 - arr_bmp[1]);
		height = (int)arr_bmp[2];
		width = (int)arr_bmp[3];
		height1 = y - height;
		
		Y = ((10 * (y)) * 180);
		HEIGHT1 = ((10 * (height1)) * 180);

		X = Y + (9.5 * (x));	
		X1 = HEIGHT1 + (9.5 * (x));

		WIDTH = X + (9.5 * (width));
		WIDTH1 = X1 + (9.5 * (width));		
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

		for( j = ref + X1; j < (ref + X); j = j + 180) {
			bmp[j] = 0;
		}

		for( j = ref + WIDTH1; j < (ref + WIDTH); j = j + 180) {
                        bmp[j] = 0;
                }
		
	}
}


void write_bmp(char *file_name) {
	FILE *file;
    	int i;

	char arr[50];
        strcpy(arr, file_name);
        const char *ch = ".";
        char *ret, *ret1, *ret2, ret3[50] = "unzip ", c;
        char ret4[50];
        ret = strtok(arr, ch);
         
        ret1 = strcat(ret, ".bmp");
        
   	file = fopen(ret1, "wb+");
    	for( i = 0; i < 200000; i++) {
        	fputc(bmp[i], file);
    	}
    	fclose(file);
}
 
int main(int argc, char *argv[]) {
	char *docname;

	if(argc < 2) {
		errno = EINVAL;
		perror("USAGE : <docname> \n");
		return errno;
	}	

	if(!strcmp(argv[1], "-h")) {	
		FILE *fp;
		char s;

		fp = fopen("help.txt", "r");

		if(fp == NULL) {
			perror("Open failed : ");
			return errno;
		}
 
		while((fscanf(fp, "%c", &s)) != -1) {
			printf("%c", s);
		}
		
		fclose(fp);

		return 0;
	}
	
	else {
		char arr[50];
        	strcpy(arr, argv[1]);
		
		strcpy(function_file, argv[1]);	
		
        	const char *ch = ".";
        	char *ret, *ret1, *ret2, ret3[50] = "unzip ", c;
        	char ret4[50];
        	ret = strtok(arr, ch);
        	ret1 = strcat(ret, ".zip");
	
        	FILE *fptr1, *fptr2;
        	fptr1 = fopen(argv[1], "r");
        	fptr2 = fopen(ret1, "w");
        	while((fscanf(fptr1, "%c", &c)) != EOF) {
                	fprintf(fptr2, "%c", c);
        	}
        	fclose(fptr1);
        	fclose(fptr2);
        	strcpy(ret4, ret1);

        	ret2 = strcat(ret3, ret4);

        	system(ret2);
		create_bmp();
		

		parseDoc("content.xml", "body", "drawing");
		write_bmp(argv[1]);
	
    		return (1);
	}
	return 0;
}

