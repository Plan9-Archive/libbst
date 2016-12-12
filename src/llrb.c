#include <u.h>
#include <libc.h>
#include <bst.h>
#include "bstimpl.h"

/* See Sedgewick 'Left-leaning Red-Black Trees' */

static Bst *llrblookup(Bsttree*, Bst*);
static Bst *llrbdelete(Bsttree*, Bst*);
static Bst *llrbinsert(Bsttree*, Bst*);

Bsttree*
bstcreatellrb(int (*cmp)(Bst*, Bst*))
{
	Bsttree *t;

	t = malloc(sizeof(*t));
	if(t == nil)
		return nil;

	t->cmp = cmp;
	t->root = nil;
	t->lookup = llrblookup;
	t->delete = llrbdelete;
	t->insert = llrbinsert;
	return t;
}

enum {
	RED,
	BLACK
};

#define isred(h) ((h) == nil ? 0 : (h)->color == RED)

static Bst*
llrblookup(Bsttree *t, Bst *k)
{
	Bst *h;
	int c;

	h = t->root;
	while(h != nil){
		assert(!isred(h->c[1]));
		assert(!isred(h->c[0]) || !isred(h->c[0]->c[0]));
		c = (t->cmp)(k, h);
		if(c < 0){
			h = h->c[0];
			continue;
		}
		if(c > 0){
			h = h->c[1];
			continue;
		}
		return h;
	}
	return nil;
}

static Bst *insert(int(*)(Bst*, Bst*), Bst*, Bst*, Bst**);

static Bst*
llrbinsert(Bsttree *t, Bst *n)
{
	Bst *op;

	if(n == nil)
		return nil;
	op = nil;
	t->root = insert(t->cmp, t->root, n, &op);
	t->root->color = RED;
	return op;
}

static Bst *flip(Bst*);
static Bst *rotate(int, Bst*);

static Bst*
insert(int (*cmp)(Bst*, Bst*), Bst *h, Bst *n, Bst **op)
{
	int c;

	if(h == nil) {
		n->c[0] = n->c[1] = nil;
		n->color = RED;
		return n;
	}

	c = cmp(n, h);
	if(c == 0) {
		*op = h;
		*n = *h;
		h = n;
	} else if(c < 0) 
		h->c[0] = insert(cmp, h->c[0], n, op);
	else
		h->c[1] = insert(cmp, h->c[1], n, op);

	if(isred(h->c[0]) && isred(h->c[1]))
		flip(h);
	else if(isred(h->c[0]) && isred(h->c[0]->c[0]))
		h = flip(rotate(0, h));
	else if(isred(h->c[1]))
		h = rotate(1, h);
	return h;
}

static Bst *delete(int(*)(Bst*, Bst*), Bst*, Bst*, Bst**);

static Bst*
llrbdelete(Bsttree *t, Bst *n)
{
	Bst *op;

	op = nil;
	t->root = delete(t->cmp, t->root, n, &op);
	if(t->root != nil)
		t->root->color = RED;
	return op;
}
static Bst *moveredleft(Bst*);
static Bst *moveredright(Bst*);
static Bst *deletemin(Bst*, Bst**);

static Bst*
delete(int (*cmp)(Bst*, Bst*), Bst *h, Bst *n, Bst **delp)
{
	Bst *e;

	if(h == nil)
		return nil;

	if(cmp(n, h) < 0) {
		if(h->c[0] == nil)
			return h;
		if(!isred(h->c[0]) && !isred(h->c[0]->c[0]))
			h = moveredleft(h);
		h->c[0] = delete(cmp, h->c[0], n, delp);
		if(isred(h->c[0]) && isred(h->c[1]))
			flip(h);
		else if(isred(h->c[1]))
			h = rotate(1, h);
		return h;
	}

	if(isred(h->c[0]))
		h = rotate(0, h);
	if(h->c[1] == nil) {
		if(cmp(n, h) == 0) {
			assert(isred(h));
			*delp = h;
			return nil;
		}
		return h;
	}

	if(!isred(h->c[1]) && !isred(h->c[1]->c[0]))
		h = moveredright(h);
	if(cmp(n, h) == 0) {
		*delp = h;
		h->c[1] = deletemin(h->c[1], &e);
		*e = *h;
		h = e;
	} else
		h->c[1] = delete(cmp, h->c[1], n, delp);

	if(isred(h->c[0]) && isred(h->c[1]))
		flip(h);
	else if(isred(h->c[1]))
		h = rotate(1, h);
	return h;
}

static Bst*
deletemin(Bst *h, Bst **op)
{
	if(h->c[0] == nil) {
		assert(isred(h));
		*op = h;
		return nil;
	}

	if(!isred(h->c[0]) && !isred(h->c[0]->c[0]))
		h = moveredleft(h);
	h->c[0] = deletemin(h->c[0], op);

	if(isred(h->c[0]) && isred(h->c[1]))
		flip(h);
	else if(isred(h->c[1]))
		h = rotate(1, h);
	return h;
}

static Bst*
rotate(int a, Bst *h)
{
	Bst *x;

	assert(isred(h->c[a]));
	x = h->c[a];
	h->c[a] = x->c[a^1];
	x->c[a^1] = h;
	x->color = h->color;
	h->color = RED;
	return x;
}

static Bst*
flip(Bst *h)
{
	assert(h->c[0]->color == h->c[1]->color);
	h->color ^= 1;
	h->c[0]->color ^= 1;
	h->c[1]->color ^= 1;
	return h;
}

static Bst*
moveredleft(Bst *h)
{
	flip(h);
	if(isred(h->c[1]->c[0])){
		h->c[1] = rotate(0, h->c[1]);
		h = rotate(1, h);
		flip(h);
	}
	return h;
}

static Bst*
moveredright(Bst *h)
{
	flip(h);
	if(isred(h->c[0]->c[0])){
		h = rotate(0, h);
		flip(h);
	}
	return h;
}
