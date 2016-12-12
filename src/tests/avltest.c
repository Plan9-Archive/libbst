#include <u.h>
#include <libc.h>
#include <bst.h>

typedef struct Intnode {
	Bst;
	int v;
} Intnode;

enum {
	MAX =  1000000,
	NODES = 100000
};

Intnode pool[NODES];

int
intcmp(Bst *a, Bst *b)
{
	Intnode *an, *bn;

	an = (Intnode*)a;
	bn = (Intnode*)b;

	if(an->v < bn->v)
		return -1;
	if(an->v > bn->v)
		return 1;
	return 0;
}

int
longest(Bst *n)
{
	int l, r, m;
	if(n == nil)
		return 0;

	l = longest(n->c[0]);
	r = longest(n->c[1]);
	m = l > r ? l : r;
	return m+1;
}

void
checkbalance(Bst *n)
{
	int l, r;

	if(n == nil)
		return;

	l = longest(n->c[0]);
	r = longest(n->c[1]);
	assert(r-l == n->balance);
	checkbalance(n->c[0]);
	checkbalance(n->c[1]);
	return;
}

void
main(void)
{
	Bsttree *t;
	Intnode *a, d;
	int i, r;

	srand(time(nil));
	t = bstcreateavl(intcmp);

	for(a = pool; a < pool+nelem(pool); a++) {
		a->v = nrand(MAX);
		bstinsert(t, a);
	}
	checkbalance(bstroot(t));
	print("Balance is good after insertions.\n");
	r = 0;
	for(i = 0; i < NODES/2; i++) {
		d.v = nrand(MAX);
		a = (Intnode*)bstdelete(t, &d);
		if(a != nil)
			r++;
	}
	checkbalance(bstroot(t));
	print("Balance is good after %d deletions.\n", r);
	exits(nil);
}
