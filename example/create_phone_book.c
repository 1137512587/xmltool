#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
/*
XML数据结构
根元素 -> 元素 (属性) -> 元素 -> 文本
*/

#define PHONE_BOOK_FILE "phone_book.xml"
#define ID_STR_LEN 16
#define NAME_STR_LEN 32
#define TEL_STR_LEN 16
#define ADDR_STR_LEN 128

typedef struct phone_t {
    int id;
    char name[NAME_STR_LEN];
    char tel[TEL_STR_LEN];
    char address[ADDR_STR_LEN];
}phone;

//设置节点项属性值
static void set_phone_item(phone *phone_item)
{
    phone_item->id = 10;
    snprintf(phone_item->name, NAME_STR_LEN, "%s", "Anker");
    snprintf(phone_item->tel, TEL_STR_LEN, "%s", "123456789");
    snprintf(phone_item->address, ADDR_STR_LEN, "%s", "Shenzhen");
    
}

static xmlNodePtr create_phone_node(const phone *phone_item)
{
    char id[ID_STR_LEN] = {0};
    xmlNodePtr phone_node = NULL;
    
    phone_node = xmlNewNode(NULL, BAD_CAST"phone");
    if (phone_node == NULL) {
        return NULL;
    }
    //设置xml属性
    snprintf(id, ID_STR_LEN, "%d", phone_item->id);
    xmlNewProp(phone_node, BAD_CAST"id",  (xmlChar*)id);
    
    xmlNewChild(phone_node, NULL, BAD_CAST"name",  (xmlChar*)phone_item->name);
    xmlNewChild(phone_node, NULL, BAD_CAST"tel",  (xmlChar*)phone_item->tel);
    xmlNewChild(phone_node, NULL, BAD_CAST"address",  (xmlChar*)phone_item->address);
    
    return phone_node;
}

static int add_phone_node_to_root(xmlNodePtr root_node)
{
    xmlNodePtr phone_node = NULL;
    phone *phone_item = NULL;
    
    phone_item = (phone *)malloc(sizeof(phone));
    if (phone_item == NULL) {
        return -1;
    }
    
    set_phone_item(phone_item);
    phone_node = create_phone_node(phone_item);
    if (phone_node == NULL) {
        fprintf(stderr, "Failed to create the prop node.");
        free(phone_item);
        return -1;
    }
    
    xmlAddChild(root_node, phone_node);
    free(phone_item);
    
    return 0;
}

static int create_phone_books(const char *phone_book_file)
{
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    
    //创建xml文件
    doc = xmlNewDoc(BAD_CAST"1.0");
    if (doc == NULL) {
        fprintf(stderr, "Failed to create the xml file.");
        return -1;
    }
    
    //创建根节点
    root_node = xmlNewNode(NULL, BAD_CAST"phone_books");
    if (root_node == NULL) {
        fprintf(stderr, "Failed to create the root node.");
        xmlFree(doc);
        return -1;
    }
    
    xmlDocSetRootElement(doc, root_node);
    
    if (add_phone_node_to_root(root_node) != 0) {
        fprintf(stderr, "Failed to add the root node.");
        xmlFree(doc);
        return -1;
    }
    
    xmlSaveFormatFileEnc(phone_book_file, doc, "UTF-8", 1);
    xmlFree(doc);
    
    return 0;
    
}

static int add_phone_node(const char *phone_book_file)
{
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNodePtr phone_node = NULL;
    phone *phone_item = NULL;
    
    if (phone_book_file == NULL) {
        fprintf(stderr, "The %s file dose not exist.", phone_book_file);
        return -1;
    }
    
    doc = xmlParseFile(phone_book_file);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse the %s file.", phone_book_file);
        return -1;
    }
    
    root_node = xmlDocGetRootElement(doc);
    if (root_node ==NULL) {
        fprintf(stderr, "Failed to obtain root element.");
        xmlFree(doc);
        return -1;
    }
    
    if (add_phone_node_to_root(root_node) != 0) {
        xmlFree(doc);
        return -1;
    }
    
    xmlSaveFormatFileEnc(phone_book_file, doc, "UTF-8", 1);
    xmlFree(doc);
    
    return 0;
    
}

int main(int argc, char *argv[])
{
    int ret;
    char *phone_book_file;
    if (argc = 2) {
        phone_book_file = argv[1];
    }
    
    //判断文件是否存在
    if (access(phone_book_file, F_OK) == 0) {
        //文件存在则添加节点
        ret = add_phone_node(phone_book_file);
    } else {
        //文件不存在则创建文件再添加节点
        ret = create_phone_books(PHONE_BOOK_FILE);
    }
    
    if (ret != 0) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}