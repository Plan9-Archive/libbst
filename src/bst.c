#include <u.h>
#include <libc.h>
#include <bst.h>
#include "bstimpl.h"

Bsttree*
bstcreate(int type, int(*cmp)(Bst*, Bst*))
{
	Bsttree *t;

	t = malloc(sizeof(*t));
	if(t == nil)
		return nil;

	if(type == AVL) {
		t->lookup = avllookup;
		t->delete = avldelete;
		t->insert = avlinsert;
	} else if(type == LLRB) {
		t->lookup = llrblookup;
		t->delete = llrbdelete;
		t->insert = llrbinsert;
	} else {
		werrstr("Unknown bst type");
		free(t);
		return nil;
	}
	t->cmp = cmp;
	t->root = nil;
	return t;
}

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
