#include <u.h>
#include <libc.h>
#include <bst.h>
#include "bstimpl.h"

/* See Knuth Volume 3, 6.2.3 */

static Bst *avllookup(Bsttree*, Bst*);
static Bst *avldelete(Bsttree*, Bst*);
static Bst *avlinsert(Bsttree*, Bst*);

Bsttree*
bstcreateavl(int (*cmp)(Bst*, Bst*))
{
	Bsttree *t;

	t = malloc(sizeof(*t));
	if(t == nil)
		return nil;

	t->cmp = cmp;
	t->root = nil;
	t->lookup = avllookup;
	t->delete = avldelete;
	t->insert = avlinsert;
	return t;
}

static Bst*
avllookup(Bsttree *t, Bst *k)
{
	Bst *h;
	int c;

	h = t->root;
	while(h != nil){
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

static int insert(int (*)(Bst*, Bst*), Bst**, Bst*, Bst**);

static Bst*
avlinsert(Bsttree *t, Bst *k)
{
	Bst *old;

	old = nil;
	insert(t->cmp, &t->root, k, &old);
	return old;
}

static int insertfix(int, Bst**);

static int
insert(int (*cmp)(Bst*, Bst*), Bst **p, Bst *k, Bst **oldp)
{
	Bst *q;
	int fix, c;

	q = *p;
	if(q == nil) {
		k->c[0] = nil;
		k->c[1] = nil;
		k->balance = 0;
		*p = k;
		return 1;
	}

	c = cmp(k, q);
	c = c > 0 ? 1 : c < 0 ? -1: 0;
	if(c == 0) {
		*oldp = q;
		*k = *q;
		*p = k;
		return 0;
	}
	fix = insert(cmp, q->c + (c+1)/2, k, oldp);
	if(fix)
		return insertfix(c, p);
	return 0;
}

static Bst *singlerot(int, Bst*);
static Bst *doublerot(int, Bst*);

static int
insertfix(int a, Bst **t)
{
	Bst *s;

	s = *t;
	if(s->balance == 0) {
		s->balance = a;
		return 1;
	}
	if(s->balance == -a) {
		s->balance = 0;
		return 0;
	}
	if(s->c[(a+1)/2]->balance == a)
		 s = singlerot(a, s);
	else
		s = doublerot(a, s);
	s->balance = 0;
	*t = s;
	return 0;
}

static int delete(int (*cmp)(Bst*, Bst*), Bst**, Bst*, Bst**);
static int deletemin(Bst**, Bst**);
static int deletefix(int, Bst**);

static Bst*
avldelete(Bsttree *t, Bst *k)
{
	Bst *old;

	if(t->root == nil)
		return nil;
	old = nil;
	delete(t->cmp, &t->root, k, &old);
	return old;
}

static int
delete(int (*cmp)(Bst*, Bst*), Bst **p, Bst *k, Bst **oldp)
{
	Bst *q, *e;
	int c, fix;

	q = *p;
	if(q == nil)
		return 0;

	c = cmp(k, q);
	c = c > 0 ? 1 : c < 0 ? -1: 0;
	if(c == 0) {
		*oldp = q;
		if(q->c[1] == nil) {
			*p = q->c[0];
			return 1;
		}
		fix = deletemin(q->c+1, &e);
		*e = *q;
		*p = e;
		if(fix)
			return deletefix(-1, p);
		return 0;
	}
	fix = delete(cmp, q->c + (c+1)/2, k, oldp);
	if(fix)
		return deletefix(-c, p);
	return 0;
}

static int
deletemin(Bst **p, Bst **oldp)
{
	Bst *q;
	int fix;

	q = *p;
	if(q->c[0] == nil) {
		*oldp = q;
		*p = q->c[1];
		return 1;
	}
	fix = deletemin(q->c, oldp);
	if(fix)
		return deletefix(1, p);
	return 0;
}

static Bst *rotate(int, Bst*);

static int
deletefix(int a, Bst **t)
{
	Bst *s;

	s = *t;
	if(s->balance == 0) {
		s->balance = a;
		return 0;
	}
	if(s->balance == -a) {
		s->balance = 0;
		return 1;
	}
	if(s->c[(a+1)/2]->balance == 0) {
		s = rotate(a, s);
		s->balance = -a;
		*t = s;
		return 0;
	}
	if(s->c[(a+1)/2]->balance == a)
		s = singlerot(a, s);
	else if(s->c[(a+1)/2]->balance == -a) 
		s = doublerot(a, s);
	s->balance = 0;
	*t = s;
	return 1;
}

static Bst*
singlerot(int a, Bst *s)
{
	s->balance = 0;
	s = rotate(a, s);
	return s;
}

static Bst*
doublerot(int a, Bst *s)
{
	Bst *r, *p;
	int c;

	c = (a+1)/2;
	r = s->c[c];
	s->c[c] = rotate(-a, s->c[c]);
	p = rotate(a, s);

	if(p->balance == a) {
		s->balance = -a;
		r->balance = 0;
	} else if(p->balance == -a) {
		s->balance = 0;
		r->balance = a;
	} else
		s->balance = r->balance = 0;
	return p;
}
	
static Bst*
rotate(int a, Bst *s)
{
	Bst *r;
	int c;

	c = (a+1)/2;
	r = s->c[c];
	s->c[c] = r->c[c^1];
	r->c[c^1] = s;
	return r;
}
