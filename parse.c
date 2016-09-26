#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
 
void die(char *msg)
{
  printf("%s", msg);
  return;
}
 
void
parseNode (xmlDocPtr doc, xmlNodePtr cur, char *subchild)
{
 
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)subchild)))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
 
            printf("%s\n", key);
            xmlFree(key);
        }
        cur = cur->next;
    }
    return;
}
 
static void
parseDoc(char *docname, char *child, char *subchild)
{
 
    xmlDocPtr doc;
    xmlNodePtr cur;
 
    doc = xmlParseFile(docname);
 
    if (doc == NULL )
        die("Document parsing failed. \n");
 
    cur = xmlDocGetRootElement(doc); //Gets the root element of the XML Doc
 
    if (cur == NULL)
    {
        xmlFreeDoc(doc);
        die("Document is Empty!!!\n");
    }
 
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)child)))
        {
            parseNode (doc, cur, subchild);
        }
        cur = cur->next;
    }
 
    xmlFreeDoc(doc);
    return;
}
 
int
main(int argc, char **argv)
{
    char *docname;
 
    if (argc != 4)
    {
        printf("Usage: %s <docname> <child> <subchild>\n", argv[0]);
        return(0);
    }
 
    docname = argv[1];
    parseDoc (docname,argv[2],argv[3]);
 
    return (1);
}
