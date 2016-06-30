/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-20T20:21:40+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-05-30T23:22:56+02:00
*/

#include "xml.h"

char* xml_LocReq (char* IPid, char* URIid, char* flow, char* types, int exact)
{
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    xmlBufferPtr buf;
    xmlTextWriterPtr writer;
    int rc;

    /* Create a new XML buffer, to which the XML document will be
     * written */
    buf = xmlBufferCreate();
    if (buf == NULL) {
        printf("testXmlwriterMemory: Error creating the xml buffer\n");
        goto error;
    }

    /* Create a new XmlWriter for memory, with no compression.
     * Remark: there is no compression for this kind of xmlTextWriter */
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL) {
        printf("testXmlwriterMemory: Error creating the xml writer\n");
        goto error;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartDocument\n");
        goto error;
    }


    /* Start an element named "locationRequest". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElementNS(
        writer,
        NULL,
        BAD_CAST "locationRequest",
        BAD_CAST "urn:ietf:params:xml:ns:geopriv:held");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error writing locationRequest element\n");
        goto error;
    }

    if (IPid != NULL || URIid != NULL)
    {
        rc = xmlTextWriterStartElementNS(
            writer,
            NULL,
            BAD_CAST "device",
            BAD_CAST "urn:ietf:params:xml:ns:geopriv:held:id");
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing DEVICE element\n");
                goto error;
        }

        if (IPid != NULL)
        {
            rc = xmlTextWriterStartElement(
                writer,
                BAD_CAST "ip");
            if (rc < 0) {
                printf
                    ("testXmlwriterMemory: Error writing IP element\n");
                    goto error;
            }

            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "v",
                BAD_CAST "4");
            if (rc < 0) {
                printf
                    ("testXmlwriterMemory: Error writing IP V attribute\n");
                    goto error;
            }

            rc = xmlTextWriterWriteFormatString(writer, "%s",
                IPid);
            if (rc < 0) {
                printf
                    ("testXmlwriterMemory: Error writing IP content.\n");
                    goto error;
            }

            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                printf("testXmlwriterMemory: Error closing ip element.\n");
                return NULL;
            }
        } else if (URIid != NULL)
        {
            rc = xmlTextWriterStartElement(
                writer,
                BAD_CAST "uri");
            if (rc < 0) {
                printf
                    ("testXmlwriterMemory: Error writing URI element\n");
                    goto error;
            }

            rc = xmlTextWriterWriteFormatString(writer, "%s",
                URIid);
            if (rc < 0) {
                printf
                    ("testXmlwriterMemory: Error writing URI content.\n");
                    goto error;
            }

            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                printf("testXmlwriterMemory: Error closing URI element.\n");
                goto error;
            }
        }


        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            printf("testXmlwriterMemory: Error closing device element.\n");
            return NULL;
        }
    } else if (flow != NULL)
    {
        flow_filter* f = prepare_flow(flow);
        if (f == NULL)
            goto error;

        rc = xmlTextWriterStartElementNS(
            writer,
            NULL,
            BAD_CAST "flow",
            BAD_CAST "urn:ietf:params:xml:ns:geopriv:held:flow");
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing FLOW element\n");
                goto error;
        }

        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "layer4",
            BAD_CAST f->layer4);
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing FLOW LAYER4 attribute\n");
                goto error;
        }

        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "layer3",
            BAD_CAST f->layer3);
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing FLOW LAYER3 attribute\n");
                goto error;
        }

        rc = xmlTextWriterStartElement(
            writer,
            BAD_CAST "src");
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing SRC element\n");
                goto error;
        }

        rc = xmlTextWriterWriteFormatElement(
            writer,
            BAD_CAST "address",
            "%s",
            f->ipo);
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing ADDRESS element\n");
                goto error;
        }

        rc = xmlTextWriterWriteFormatElement(
            writer,
            BAD_CAST "port",
            "%s",
            f->porto);
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing PORT element\n");
                goto error;
        }

        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            printf("testXmlwriterMemory: Error closing SRC element.\n");
            return NULL;
        }

        rc = xmlTextWriterStartElement(
            writer,
            BAD_CAST "dst");
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing DST element\n");
                goto error;
        }

        rc = xmlTextWriterWriteFormatElement(
            writer,
            BAD_CAST "address",
            "%s",
            f->ipd);
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing ADDRESS element\n");
                goto error;
        }

        rc = xmlTextWriterWriteFormatElement(
            writer,
            BAD_CAST "port",
            "%s",
            f->portd);
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing PORT element\n");
                goto error;
        }

        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            printf("testXmlwriterMemory: Error closing DST element.\n");
            return NULL;
        }

        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            printf("testXmlwriterMemory: Error closing FLOW element.\n");
            return NULL;
        }

        free(f);

    }

    if (types != NULL)
    {
        rc = xmlTextWriterStartElement(
            writer,
            BAD_CAST "locationType");
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing locationType element.\n");
                goto error;
        }

        if (exact)
        {
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "exact",
                BAD_CAST "true");
            if (rc < 0) {
                printf
                    ("testXmlwriterMemory: Error writing locationType exact attribute.\n");
                    goto error;
            }
        }


        rc = xmlTextWriterWriteFormatString(writer, "%s",
            prepare_types(types));
        if (rc < 0) {
            printf
                ("testXmlwriterMemory: Error writing types in locationType element.\n");
                goto error;
        }

        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            printf("testXmlwriterMemory: Error closing locationType element.\n");
            return NULL;
        }
    }

    /* Here we could close the element locationRequest using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndDocument\n");
        goto error;
    }

    xmlFreeTextWriter(writer);

    char* txtDoc = malloc(sizeof(char)*(strlen((const char*)buf->content)+1));
    strcpy(txtDoc, (const char*)buf->content);
    xmlBufferFree(buf);

    xmlCleanupParser();

    return txtDoc;

error:
    if (writer != NULL) xmlFreeTextWriter(writer);
    if (buf != NULL) xmlBufferFree(buf);
    xmlCleanupParser();
    return NULL;

}

held_response* xml_parse(char* txtXML)
{
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;

    held_response* held_r = create_held_response();

    /* Init libxml */
    xmlInitParser();
    LIBXML_TEST_VERSION

    if (verbose) xml_debug("Loading document into memory");
    doc = xmlReadMemory(txtXML, strlen(txtXML), NULL, MY_ENCODING, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
        goto error;
    }

    if (verbose) xml_debug("Document loaded, creating XPath Ctx");
    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        goto error;
    }

    if (verbose) xml_debug("Registering namespaces");
    /* Register namespaces from list */
    if(register_namespaces(xpathCtx) < 0) {
        fprintf(stderr,"Error: failed to register namespaces list");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        goto error;
    }

    if (verbose) xml_debug("Checking for <error> element");
    if(check_error(doc, xpathCtx, held_r) < 0)
    {
        puts("Error detecting <error> element");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        goto error;
    }

    if (verbose) xml_debug("Checking for <presence> element");
    if (held_r->error_code == NULL)
    {
        if (check_presence(doc, xpathCtx, held_r) < 0)
        {
            puts("Error detecting <presence> element");
            xmlXPathFreeContext(xpathCtx);
            xmlFreeDoc(doc);
            goto error;
        }
    }

    if (verbose) xml_debug("Work done, freeing resources");
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return held_r;


error:
    /* Shutdown libxml */
    xmlCleanupParser();
    return NULL;
}

int check_presence(xmlDocPtr doc, xmlXPathContextPtr xpathCtx, held_response* held_r)
{
    //Get the presence node
    xmlXPathObjectPtr pres_xpath = eval_xpath (xpathCtx, "/d:locationResponse/p:presence");
    if (pres_xpath == NULL)
        goto error;

    if (pres_xpath->type != XPATH_NODESET || xmlXPathNodeSetIsEmpty(pres_xpath->nodesetval))
    {
        if (verbose) xml_debug("<presence> element not found");
        xmlXPathFreeObject(pres_xpath);
        goto error;
    }

    //Get the entity attribute of the presence node
    char* entity = (char*) xmlGetProp(pres_xpath->nodesetval->nodeTab[0], BAD_CAST "entity");
    if (entity == NULL)
    {
        if (verbose) xml_debug("entity attribute not present in <presence> element.");
        goto error;
    }

    held_r->entity = entity;
    xmlXPathFreeObject(pres_xpath);

    //Get all the LI nodes
    if (verbose) xml_debug("Getting all the <location-info> elements.");
    xmlXPathObjectPtr li_xpath = eval_xpath (xpathCtx, "//gp:geopriv/gp:location-info");
    if (li_xpath == NULL)
        goto error;
    if (li_xpath->type != XPATH_NODESET || xmlXPathNodeSetIsEmpty(li_xpath->nodesetval))
    {
        if (verbose) xml_debug("<location-info> elements not found.");
        xmlXPathFreeObject(li_xpath);
        goto error;
    }

    int i = 0;
    while(li_xpath->nodesetval->nodeTab[i] != NULL)
    {
        if (verbose) xml_debug("Searching the element:");
        if (verbose) xml_debug((char*) li_xpath->nodesetval->nodeTab[i]->parent->parent->name);
        //Search for Civic Information nodes
        xmlXPathObjectPtr ca_xpath = eval_xpath_ctx (xpathCtx,"./ca:civicAddress",
            li_xpath->nodesetval->nodeTab[i]);
        if (ca_xpath != NULL)
        {
            if (ca_xpath->type != XPATH_NODESET || xmlXPathNodeSetIsEmpty(ca_xpath->nodesetval))
            {
                xmlXPathFreeObject(ca_xpath);
            }else
            {
                if (verbose) xml_debug("->CA Info Found!");

                char* ca_info = recursive_text(
                    doc,
                    ca_xpath->nodesetval->nodeTab[0]->xmlChildrenNode
                );
                held_r->civicAddress = ca_info;
                xmlXPathFreeObject(ca_xpath);
            }

        }

        //Search for Geodetic Information nodes
        xmlXPathObjectPtr point_xpath = eval_xpath_ctx (xpathCtx,"./gml:Point",
            li_xpath->nodesetval->nodeTab[i]);
        if (point_xpath != NULL)
        {
            if (point_xpath->type != XPATH_NODESET || xmlXPathNodeSetIsEmpty(point_xpath->nodesetval))
            {
                xmlXPathFreeObject(point_xpath);
            }
            else
            {
                if (verbose) xml_debug("->Geodetic Info Found!");

                char* point_info = recursive_text(
                    doc,
                    point_xpath->nodesetval->nodeTab[0]->xmlChildrenNode
                );
                held_r->geodetic = point_info;
                xmlXPathFreeObject(point_xpath);
            }
        }

        i++;
    }

    //Get the locationUriSet node
    xmlXPathObjectPtr uri_xpath = eval_xpath (xpathCtx, "/d:locationResponse/d:locationUriSet");
    if (uri_xpath != NULL)
    {
        if (uri_xpath->type != XPATH_NODESET || xmlXPathNodeSetIsEmpty(uri_xpath->nodesetval))
        {
            if (verbose) xml_debug("<locationUriSet> element not found");
            xmlXPathFreeObject(uri_xpath);
        }else
        {
            char* uri_info = recursive_text(
                doc,
                uri_xpath->nodesetval->nodeTab[0]->xmlChildrenNode
            );

            held_r->refURL = uri_info;

            xmlXPathFreeObject(uri_xpath);
        }

    }


return 0;

error:
    return -1;
}

int check_error(xmlDocPtr doc, xmlXPathContextPtr xpathCtx, held_response* held_r)
{
    xmlXPathObjectPtr error_xpath = eval_xpath (xpathCtx, "/d:error");
    if (error_xpath == NULL || error_xpath->type != XPATH_NODESET)
        goto error;

    if (error_xpath->nodesetval->nodeNr > 0)
    {
        char* code = (char*) xmlGetProp(error_xpath->nodesetval->nodeTab[0], BAD_CAST "code");
        if (code == NULL)
            goto error;

        held_r->error_code = code;

        xmlXPathObjectPtr error_xpath_msg = eval_xpath (xpathCtx, "/d:error/d:message");
        if (error_xpath_msg != NULL &&
            error_xpath_msg->type == XPATH_NODESET &&
            error_xpath_msg->nodesetval->nodeNr > 0)
        {
            char* msg = (char*) xmlNodeListGetString(doc,
                error_xpath_msg->nodesetval->nodeTab[0]->xmlChildrenNode,
                1);
            held_r->error_msg = msg;
            xmlXPathFreeObject(error_xpath);
        }else
        {
            xmlXPathFreeObject(error_xpath_msg);
            goto error;
        }
    }

    return 0;

error:
    xmlXPathFreeObject(error_xpath);
    return -1;
}

xmlXPathObjectPtr eval_xpath (xmlXPathContextPtr xpathCtx, char* xpathExpr)
{
    /* Evaluate xpath expression */
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpathExpr, xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        return NULL;
    }
    return xpathObj;
}

xmlXPathObjectPtr eval_xpath_ctx (xmlXPathContextPtr xpathCtx, char* xpathExpr, xmlNodePtr node)
{
    /* Evaluate xpath expression */
    xmlXPathObjectPtr xpathObj = xmlXPathNodeEval(node, BAD_CAST xpathExpr, xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        return NULL;
    }
    return xpathObj;
}

int register_namespaces (xmlXPathContextPtr xpathCtx)
{
    int i = 0;
    i += register_namespaces2(xpathCtx, "d","urn:ietf:params:xml:ns:geopriv:held");
    i += register_namespaces2(xpathCtx, "p","urn:ietf:params:xml:ns:pidf");
    i += register_namespaces2(xpathCtx, "gp","urn:ietf:params:xml:ns:pidf:geopriv10");
    i += register_namespaces2(xpathCtx, "gml","http://www.opengis.net/gml");
    i += register_namespaces2(xpathCtx, "gs","http://www.opengis.net/pidflo/1.0");
    i += register_namespaces2(xpathCtx, "ca","urn:ietf:params:xml:ns:pidf:geopriv10:civicAddr");
    return i;
}

int register_namespaces2 (xmlXPathContextPtr xpathCtx, char* prefix, char* uri)
{
    /* do register namespace */
	if(xmlXPathRegisterNs(xpathCtx, BAD_CAST prefix, BAD_CAST uri) != 0) {
	    fprintf(stderr,"Error: unable to register NS with prefix=\"%s\" and href=\"%s\"\n", prefix, uri);
	    return -1;
	}
    return 0;
}

char* prepare_types(char* types)
{
    size_t len = strlen(types), i;
    for (i = 0; i < len; i++) {
        if (types[i] == ',')
            types[i] = '\n';
    }

    return types;
}

flow_filter* prepare_flow(char* flow)
{
    if (verbose) xml_debug("Preparing flow:");
    if (verbose) xml_debug(flow);

    flow_filter* f = (flow_filter*) calloc(1, sizeof(flow_filter));
    f->ipo = strtok (flow,":-/");
    f->porto = strtok (NULL,":-/");
    f->ipd = strtok (NULL,":-/");
    f->portd = strtok (NULL,":-/");
    f->layer3 = strtok (NULL,":-/");
    f->layer4 = strtok (NULL,":-/");

    if (!f->ipo || !f->porto || !f->ipd || !f->portd || !f->layer3 || !f->layer4)
    {
        printf("[!] Bad flow syntax!\n");
        return NULL;
    }

    return f;
}

char* recursive_text(xmlDocPtr doc, xmlNodePtr node)
{
    char* text = (char*) malloc(1);
    text[0] = '\0';
    xmlNodePtr cur = node;
    while (cur != NULL) {
        char* tmp = (char*) xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
        if (!tmp)
            goto next;
        text = (char*) realloc(text, strlen(text)+strlen(tmp)+2);
        strcat(text, trimwhitespace(tmp));
        strcat(text, "\n\0");
        free(tmp);
next:
        cur = cur->next;
    }

    return text;
}

char* trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

void xml_debug(const char* msg)
{
    printf("[XML] %s\n", msg);
}
