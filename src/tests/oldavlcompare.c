#include <u.h>
#include <libc.h>
#include <bst.h>
#include <avl.h>

enum {
	NODES =    500000,
	MAX =    10000000
};

typedef struct Bstnode Bstnode;
typedef struct Avlnode Avlnode;

struct Bstnode {
	Bst;
	int v;
};

struct Avlnode {
	Avl;
	int v;
};

void setup(int);

int bstinserts(Bsttree*);
int bstnodecmp(Bst*, Bst*);
int bstdeletes(Bsttree*);
int bstlookups(Bsttree*);

int avlinserts(Avltree*);
int avlnodecmp(Avl*, Avl*);
int avldeletes(Avltree*);
int avllookups(Avltree*);

void
main(int argc, char **argv)
{
	Bsttree *new;
	Avltree *old;
	int c, seq;
	long start[4], stop[4];

	seq = 0;
	ARGBEGIN {
	case 's':
		seq = 1;
		break;
	} ARGEND

	setup(seq);

	new = bstcreate(AVL, bstnodecmp);
	old = mkavltree(avlnodecmp);

	times(start);
	c = bstinserts(new);
	times(stop);
	print("New AVL:\nTime for %d insertions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = avlinserts(old);
	times(stop);
	print("Old AVL:\nTime for %d insertions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = bstlookups(new);
	times(stop);
	print("New AVL:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = avllookups(old);
	times(stop);
	print("Old AVL:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = bstdeletes(new);
	times(stop);
	print("New AVL:\nTime for %d deletions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = avldeletes(old);
	times(stop);
	print("Old AVL:\nTime for %d deletions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = bstlookups(new);
	times(stop);
	print("New AVL:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = avllookups(old);
	times(stop);
	print("Old AVL:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);
}

int rands[NODES];
Bstnode bstpool[NODES];
Avlnode avlpool[NODES];

void
setup(int s)
{
	int i;

	srand(time(nil));
	for(i = 0; i < NODES; i++) {
		if(s) {
			rands[i] = i;
			avlpool[i].v = i;
		} else {
			rands[i] = nrand(MAX);
			avlpool[i].v = nrand(MAX);
		}
		bstpool[i].v = avlpool[i].v;
	}
}

int
bstinserts(Bsttree *t)
{
	Bstnode *n, *ni;
	int r;

	r = 0;
	for(ni = bstpool; ni < bstpool+nelem(bstpool); ni++) {
		n = (Bstnode*)bstinsert(t, ni);
		if(n == nil)
			r++;
	}
	return r;
}

int
bstdeletes(Bsttree *t)
{
	Bstnode n, *m;
	int *i, r;

	r = 0;
	for(i = rands; i < rands+nelem(rands); i++) {
		n.v = *i;
		m = (Bstnode*)bstdelete(t, &n);
		if(m != nil)
			r++;
	}
	return r;
}

int
bstlookups(Bsttree *t)
{
	Bstnode n, *m;
	int *i, r;

	r = 0;
	for(i = rands; i < rands+nelem(rands); i++) {
		n.v = *i;
		m = (Bstnode*)bstlookup(t, &n);
		if(m != nil)
			r++;
	}
	return r;
}

int
avlinserts(Avltree *t)
{
	Avlnode *n, *ni;
	int r;

	r = 0;
	for(ni = avlpool; ni < avlpool+nelem(avlpool); ni++) {
		n = nil;
		insertavl(t, ni, (Avl**)&n);
		if(n == nil)
			r++;
	}
	return r;
}

int
avldeletes(Avltree *t)
{
	Avlnode n, *m;
	int *i, r;

	r = 0;
	for(i = rands; i < rands+nelem(rands); i++) {
		n.v = *i;
		m = nil;
		deleteavl(t, &n, (Avl**)&m);
		if(m != nil)
			r++;
	}
	return r;
}

int
avllookups(Avltree *t)
{
	Avlnode n, *m;
	int *i, r;

	r = 0;
	for(i = rands; i < rands+nelem(rands); i++) {
		n.v = *i;
		m = (Avlnode*)lookupavl(t, &n);
		if(m != nil)
			r++;
	}
	return r;
}

int
bstnodecmp(Bst *a, Bst *b)
{
	Bstnode *na, *nb;

	na = (Bstnode*)a;
	nb = (Bstnode*)b;

	if(na->v < nb->v)
		return -1;
	if(na->v > nb->v)
		return 1;
	return 0;
}

int
avlnodecmp(Avl *a, Avl *b)
{
	Avlnode *na, *nb;

	na = (Avlnode*)a;
	nb = (Avlnode*)b;

	if(na->v < nb->v)
		return -1;
	if(na->v > nb->v)
		return 1;
	return 0;
}
