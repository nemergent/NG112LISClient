/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-20T20:22:23+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-02-26T16:24:36+01:00
*/
#pragma once

#define MY_ENCODING "UTF-8"

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "held.h"

extern int verbose;

typedef struct {
	char* ipo;
	char* porto;
	char* ipd;
	char* portd;
	char* layer3;
	char* layer4;
}flow_filter;

char* xml_LocReq (char* IPid, char* URIid, char* flow, char* types, int exact);
held_response* xml_parse(char* txtXML);

int check_error(xmlDocPtr doc, xmlXPathContextPtr, held_response* held_r);
int check_presence(xmlDocPtr doc, xmlXPathContextPtr, held_response* held_r);
xmlXPathObjectPtr eval_xpath (xmlXPathContextPtr xpathCtx, char* xpathExpr);
xmlXPathObjectPtr eval_xpath_ctx (xmlXPathContextPtr xpathCtx, char* xpathExpr, xmlNodePtr node);
int register_namespaces (xmlXPathContextPtr xpathCtx);
int register_namespaces2 (xmlXPathContextPtr xpathCtx, char* prefix, char* uri);
char* prepare_types(char* types);
flow_filter* prepare_flow(char* flow);
char* recursive_text(xmlDocPtr doc, xmlNodePtr node);
char *trimwhitespace(char *str);
void xml_debug(const char* msg);
