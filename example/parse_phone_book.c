#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

static int parse_phone(xmlDocPtr doc, xmlNodePtr cur)
{
    xmlChar *key;
    
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name,(const xmlChar *)"name"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            printf("name:%s\t", key);
            xmlFree(key);
        }
        
        if ((!xmlStrcmp(cur->name,(const xmlChar *)"tel"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            printf("tel:%s\t", key);
            xmlFree(key);
        }
        
        
        if ((!xmlStrcmp(cur->name,(const xmlChar *)"address"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            printf("address:%s\t", key);
            xmlFree(key);
        }
        
        cur = cur->next;
    }
    return 0;
}

static int parse_phone_book(const char *file_name)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar *id;
    
    //获取树形结构
    doc = xmlParseFile(file_name);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse xml file(%s)\n", file_name);
        return -1;
    }
    
    //获取根节点
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        fprintf(stderr, "Failed to parse root node(%s)\n", file_name);
        return -1;
    }
    
    //处理根结点
    if ((xmlStrcmp(cur->name, (const xmlChar *)"phone_books"))) {
        fprintf(stderr, "The root is not phone_book\n");
        return -1;
    }
    //处理根结点的子结点
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"phone"))) {
            id = xmlGetProp(cur, "id");
            printf("id:%s\t",id);
            parse_phone(doc,cur);
        }
        cur = cur->next;
    }
    
    
    xmlFreeDoc(doc);
    return 0;
}

int main(int argc,char *argv[])
{
    char *xml_file = NULL;
    if (argc == 2 ) {
        xml_file = argv[1];
    } else {
        printf("Usage:%s xml_file\n", argv[0]);
        return 0;
    }
    
    if (parse_phone_book(xml_file) != 0) {
        fprintf(stderr, "Filded to parse xml.\n");
        return -1;
    }
    
    return 0;
}