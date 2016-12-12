#include <u.h>
#include <libc.h>
#include <bst.h>
#include "bstimpl.h"

Bst*
bstroot(Bsttree *t)
{
	return t->root;
}

Bst*
bstlookup(Bsttree *t, Bst *p)
{
	return t->lookup(t, p);
}

Bst*
bstdelete(Bsttree *t, Bst *p)
{
	return t->delete(t, p);
}

Bst*
bstinsert(Bsttree *t, Bst *p)
{
	return t->insert(t, p);
}
