#include "StdAfx.h"
#include "XMLfacade.h"


XMLfacade::XMLfacade(void)
	: tag(NULL)
{
	string idtmp;
	tag = new string[8];
	string tmp[8]={"id","history","age","sex","handed","studies","comments","session"};
	for (int i=0; i<8; i++) tag[i]=tmp[i];

	fileName = "data/userSession.xml";
	tree = readXMLfile();
	fileName = "data/userSession_old.xml";
	writeXMLfile();
	fileName = "data/userSession.xml";
}


XMLfacade::~XMLfacade(void)
{
	if (tree!=NULL) mxmlDelete(tree);
	delete [] tag;
}


mxml_node_t * XMLfacade::readXMLfile(void)
{
	FILE *fp;
	mxml_node_t * t=NULL;

    fp = fopen(fileName.c_str(), "r");
	if (fp == NULL) {
		mkdir("data/");
		t = createVoidXML();
	}
	else {
	    t = mxmlLoadFile(NULL, fp,
			                MXML_OPAQUE_CALLBACK);
		fclose(fp);
	}

	tree=t;
	//removeSpaceNodes();
	return t;
}



void XMLfacade::writeXMLfile(void)
{
	FILE *fp;

    fp = fopen(fileName.c_str(), "w+");
    mxmlSaveFile(tree, fp, MXML_NO_CALLBACK); //MXML_NO_CALLBACK)   whitespace_cb; 
   // mxmlSaveFile(tree, stdout, whitespace_cb); //MXML_NO_CALLBACK); 
    fclose(fp);
}




mxml_node_t * XMLfacade::findUserByNumId(void)
{
	mxml_node_t *node;
    
	//doc.getUserNumId().c_str()
    node = findUserByTag("00001","id");

	return node;
}


mxml_node_t * XMLfacade::findUserByHistoryNumber(void)
{
	mxml_node_t *node;
    
	//doc.getUserHistoryNumber().c_str()
    node = findUserByTag("","history");

	return node;
}

mxml_node_t * XMLfacade::findUserByTag(string userName, string tag)
{
	mxml_node_t *node, *idNode;
	string text;

    for (node = mxmlFindElement(tree, tree,
                                "user",
                                NULL, NULL,
                                MXML_DESCEND);
         node != NULL;
         node = mxmlFindElement(node, tree,
                                "user",
                                NULL, NULL,
                                MXML_DESCEND))
    {
		idNode = mxmlFindElement(node, node,
								tag.c_str(),
                                NULL, NULL,
                                MXML_DESCEND);
		text.assign(mxmlGetOpaque(idNode));
		if (text.compare(userName)==0) return node;
    }

	return NULL;
}

bool XMLfacade::existsUser(string userId)
{
	mxml_node_t *node = findUserByTag(userId, "id");
	
	return node!=NULL;
}


string * XMLfacade::getUserDataByNumId(string id)
{
	mxml_node_t *user=NULL;

	user = findUserByTag(id,"id"); 	
	return fillInUserData(user);
}

string * XMLfacade::getUserDataByHistory(string id)
{
	mxml_node_t *user=NULL;

	user = findUserByTag(id,"history"); 	
	return fillInUserData(user);
}


string * XMLfacade::fillInUserData(mxml_node_t * user)
{
	if (user !=NULL) {
		return getUserData(user);
	}
	else {
		string * userData = new string[8];
		for (int i=0; i<8; i++) {userData[i]="";}
		return userData;
	}
}


string * XMLfacade::getUserData(mxml_node_t * user)
{
	mxml_node_t *node;
	string * userData = new string[8];
	
	for (int i=0; i<8; i++) {
		node = mxmlFindElement(user, user,
								tag[i].c_str(),
			                    NULL, NULL,
				                MXML_DESCEND);
	
		if (mxmlGetFirstChild(node) != NULL)  {
			userData[i].assign(mxmlGetOpaque(node));
		}
		else {
			userData[i]="";
		}
	}

	return userData;
}

void XMLfacade::addUser(string * userData)
{
	mxml_node_t *user, *root;

	user = createUser(userData);

	//root = mxmlGetNextSibling(mxmlGetFirstChild(tree));
	root = mxmlGetFirstChild(tree); 

	mxmlAdd(root,MXML_ADD_AFTER,MXML_ADD_TO_PARENT,user);
}



mxml_node_t * XMLfacade::createUser(string * userData)
{
	mxml_node_t *node, *user;
	
    user = mxmlNewElement(MXML_NO_PARENT, "user");
	for (int i=0; i<8; i++) {
		node = mxmlNewElement(user, tag[i].c_str());
		mxmlNewOpaque(node, userData[i].c_str());
	}

	return user;
}


void XMLfacade::updateUser(string * userData)
{
	mxml_node_t *node , *user;
	
	user = findUserByTag(userData[0],"id"); 
	for (int i=0; i<8; i++) {
		node = mxmlFindElement(user, user,
								tag[i].c_str(),
			                    NULL, NULL,
				                MXML_DESCEND);

		if (mxmlGetFirstChild(node) == NULL) {
			mxmlNewOpaque(node, userData[i].c_str());
		}
		else {
			mxmlSetOpaque(node,userData[i].c_str());
		}

	}
}

string XMLfacade::getLastUserId(void)
{
	mxml_node_t *node, *idNode;
	string id, idmax="00000";
	int number, max=0;

    for (node = mxmlFindElement(tree, tree,
                                "user",
                                NULL, NULL,
                                MXML_DESCEND);
         node != NULL;
         node = mxmlFindElement(node, tree,
                                "user",
                                NULL, NULL,
                                MXML_DESCEND))
    {
		idNode = mxmlFindElement(node, node,
								"id",
                                NULL, NULL,
                                MXML_DESCEND);
		id.assign(mxmlGetOpaque(idNode));
		istringstream(id) >> number;
		if (number>max) {
			max=number;
			idmax=id;
		}
    }

	return idmax;
}


/*
void XMLfacade::updateUser(string * userData)
{
	mxml_node_t *user, *node;
	int i;

	user = findUserByTag(userData[0],"id"); 
	//node = mxmlGetFirstChild(user);
	node = mxmlWalkNext(user,user,MXML_DESCEND_FIRST);
	node = mxmlWalkNext(node,user,MXML_NO_DESCEND);
	i=0;
	while (node != NULL) {  //mxmlWalkNext
		mxmlSetOpaque(node,userData[i].c_str());
		//node = mxmlGetNextSibling(node);
		node = mxmlWalkNext(node,user,MXML_NO_DESCEND);
		node = mxmlWalkNext(node,user,MXML_NO_DESCEND);
		i++;
	}
}

*/
void XMLfacade::removeSpaceNodes()
{
	mxml_node_t *  node, * rem;
	string tmp;
	bool deleted = false;

	node = mxmlWalkNext(tree,tree,MXML_DESCEND_FIRST);
	while (node != NULL) {
		deleted = false;
		if (mxmlGetType(node) == MXML_OPAQUE) {
			tmp.assign(mxmlGetOpaque(node));
			if (tmp.compare("\n")==0 || tmp.compare("\t")==0) {
				rem = node;
				node = mxmlWalkNext(node,tree,MXML_DESCEND);
				mxmlRemove(rem);
				mxmlDelete(rem);
				rem=NULL;
				deleted = true;
			}
		}
		if (!deleted) node = mxmlWalkNext(node,tree,MXML_DESCEND);
	}
}


/*
 * 'whitespace_cb()' - Let the mxmlSaveFile() function know when to insert
 *                     newlines and tabs...
 */

const char *				/* O - Whitespace string or NULL */
whitespace_cb(mxml_node_t *node,	/* I - Element node */
              int         where)	/* I - Open or close tag? */
{

  mxml_node_t	*parent;		/* Parent node */
  int		level;			/* Indentation level */
  const char	*name;			/* Name of element */
  static const char *tabs = "\t\t\t\t\t\t\t\t";
					/* Tabs for indentation */


 /*
  * We can conditionally break to a new line before or after any element.
  * These are just common HTML elements...
  */

  name = node->value.element.name;

if (!strncmp(name, "?xml", 4))
  {
    if (where == MXML_WS_AFTER_OPEN)
      return ("\n");
    else
      return (NULL);
  }
  else if (where == MXML_WS_BEFORE_OPEN || (where == MXML_WS_BEFORE_CLOSE && !node->child))
  {
    for (level = -1, parent = node->parent;
         parent;
	 level ++, parent = parent->parent);

    if (level > 8)
      level = 8;
    else if (level < 0)
      level = 0;
	
	if (level ==0) return NULL;
	if (level ==1) return "\t";
	if (level ==2) return "\t\t";
	if (level ==3) return "\t\t\t";
//    else return (tabs + 8 - level + (2*(level-1)));
  }
  else if (where == MXML_WS_AFTER_CLOSE )
    return ("\n");
  else if (where == MXML_WS_AFTER_OPEN && !node->child)
    return ("\n");

 /*
  * Return NULL for no added whitespace...
  */
  return (NULL);
}



mxml_node_t * XMLfacade::createDocument(void)
{
	    mxml_node_t *xml;    /* <?xml ... ?> */
    mxml_node_t *data;   /* <data> */
    mxml_node_t *node;   /* <node> */
    mxml_node_t *group;  /* <group> */

    xml = mxmlNewXML("1.0");

    data = mxmlNewElement(xml, "root");

        group = mxmlNewElement(data, "user");
        node = mxmlNewElement(group, "id");
        mxmlNewText(node, 0, "00001");
        node = mxmlNewElement(group, "history");
        mxmlNewText(node, 0, "00004");
        node = mxmlNewElement(group, "age");
        mxmlNewText(node, 0, "12");
        node = mxmlNewElement(group, "sex");
        mxmlNewText(node, 0, "male");
        node = mxmlNewElement(group, "handed");
        mxmlNewText(node, 0, "left");
        node = mxmlNewElement(group, "studies");
        mxmlNewText(node, 0, "Estudios obligatorios");
        node = mxmlNewElement(group, "comments");
        mxmlNewText(node, 0, "aaa aaa");
        node = mxmlNewElement(group, "session");
        mxmlNewText(node, 0, "00018");

        group = mxmlNewElement(data, "user");
        node = mxmlNewElement(group, "id");
        mxmlNewText(node, 0, "00002");
        node = mxmlNewElement(group, "history");
        mxmlNewText(node, 0, "00002");
        node = mxmlNewElement(group, "age");
        mxmlNewText(node, 0, "5");
        node = mxmlNewElement(group, "sex");
        mxmlNewText(node, 0, "male");
        node = mxmlNewElement(group, "handed");
        mxmlNewText(node, 0, "right");
        node = mxmlNewElement(group, "studies");
        mxmlNewText(node, 0, "sin Estudios");
        node = mxmlNewElement(group, "comments");
        mxmlNewText(node, 0, "");
        node = mxmlNewElement(group, "session");
        mxmlNewText(node, 0, "00018");

 	return xml;
}




mxml_node_t * XMLfacade::createVoidXML(void)
{
	mxml_node_t * xml, * node;
		
	xml = mxmlNewXML("1.0");
	node = mxmlNewElement(xml, "root");
	//mxmlNewOpaque(node, "");

	return xml;
}
