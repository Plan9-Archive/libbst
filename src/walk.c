#include <u.h>
#include <libc.h>
#include <bst.h>
#include <thread.h>
#include <bstwalk.h>
#include "bstimpl.h"

typedef struct Walk Walk;

struct Walk
{
	Bst *root;
	Bst *lower;
	Bst *upper;
	int (*cmp)(Bst*, Bst*);
	Channel *c;
	int type;
};

static void walkthread(void*);
static void inwalk(Bst*, Walk*);
static void prewalk(Bst*, Walk*);
static void postwalk(Bst*, Walk*);
static void visit(Bst*, Walk*);

int bststacksize;

Channel*
bstwalk(Bsttree *t, Bst *lower, Bst *upper, int type)
{
	Walk *w;

	if(bststacksize == 0)
		bststacksize = 8192;
	w = malloc(sizeof(*w));
	if(w == nil)
		return nil;
	w->c = chancreate(sizeof(Bst*), 0);
	w->root = t->root;
	w->lower = lower;
	w->upper = upper;
	w->cmp = t->cmp;
	w->type = type;
	threadcreate(walkthread, w, bststacksize);
	return w->c;
}

static void
walkthread(void *a)
{
	Walk *w;
	
	w = a;
	if(w->lower != nil && w->upper != nil){
		if((w->cmp)(w->lower, w->upper) > 0)
			goto End;
	}
	switch(w->type){
	case PRE:
		prewalk(w->root, w);
		break;
	case POST:
		postwalk(w->root, w);
		break;
	case IN:
		inwalk(w->root, w);
		break;
	}
End:
	sendp(w->c, nil);
	free(w);
	threadexits(0);
}

static void
inwalk(Bst *h, Walk *w)
{
	if(h == nil)
		return;

	inwalk(h->c[0], w);
	visit(h, w);
	inwalk(h->c[1], w);
}

static void
prewalk(Bst *h, Walk *w)
{
	if(h == nil)
		return;

	visit(h, w);
	prewalk(h->c[0], w);
	prewalk(h->c[1], w);
}

static void
postwalk(Bst *h, Walk *w)
{
	if(h == nil)
		return;

	postwalk(h->c[0], w);
	postwalk(h->c[1], w);
	visit(h, w);
}

static void
visit(Bst *h, Walk *w)
{
	int r, lcmp, ucmp;

	r = 0;
	lcmp = 1;
	ucmp = -1;
	if(w->lower != nil)
		lcmp = (w->cmp)(h, w->lower);
	if(w->upper != nil)
		ucmp = (w->cmp)(h, w->upper);
	if(lcmp >= 0 && ucmp <= 0)
		r = sendp(w->c, h);
	if(r == -1){
		free(w);
		threadexits(0);
	}
}
