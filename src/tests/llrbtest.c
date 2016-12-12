#include <u.h>
#include <libc.h>
#include <bst.h>

typedef struct Intnode {
	Bst;
	int v;
} Intnode;

enum {
	RED,
	BLACK,
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


void
blackbalance(Bst *h, int b, int *old)
{
	if(h == nil) {
		if(*old == 0)
			*old = b;
		else {
			if (b != *old) fprint(2, "black balance: %d %d\n", *old, b);
			assert(b == *old);
		}
		return;
	}
	if(h->color == BLACK){
		blackbalance(h->c[0], b+1, old);
		blackbalance(h->c[1], b+1, old);
	}else{
		blackbalance(h->c[0], b, old);
		blackbalance(h->c[0], b, old);
	}
}

int
checkbalance(Bsttree *t)
{
	int b;

	b = 0;
	blackbalance(bstroot(t), 0, &b);
	return b;
}

void
main(void)
{
	Bsttree *t;
	Intnode *a, d;
	int i, r;

	srand(time(nil));
	t = bstcreatellrb(intcmp);

	for(a = pool; a < pool+nelem(pool); a++) {
		a->v = nrand(MAX);
		bstinsert(t, a);
	}
	checkbalance(t);
	print("Balance is good after insertions.\n");
	r = 0;
	for(i = 0; i < NODES/2; i++) {
		d.v = nrand(MAX);
		a = (Intnode*)bstdelete(t, &d);
		if(a != nil)
			r++;
	}
	checkbalance(t);
	print("Balance is good after %d deletions.\n", r);
	exits(nil);
}
