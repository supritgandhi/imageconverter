#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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
	return 0;

//	parseDoc(argv[1], "<office:automatic-styles>", "<style:style style:name="gr1" style:family="graphic" style:parent-style-name="objectwithoutfill">");
}
