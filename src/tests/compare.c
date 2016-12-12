#include <u.h>
#include <libc.h>
#include <bst.h>

enum {
	NODES =    500000,
	MAX =    10000000
};

typedef struct Inode Inode;

struct Inode {
	Bst;
	int v;
};

void setup(int);

int llrbinserts(Bsttree*);
int avlinserts(Bsttree*);
int nodecmp(Bst*, Bst*);
int deletes(Bsttree*);
int lookups(Bsttree*);

void
main(int argc, char **argv)
{
	Bsttree *llrb, *avl;
	int c, seq;
	long start[4], stop[4];

	seq = 0;
	ARGBEGIN {
	case 's':
		seq = 1;
		break;
	} ARGEND

	setup(seq);

	avl = bstcreate(AVL, nodecmp);
	llrb = bstcreate(LLRB, nodecmp);

	times(start);
	c = llrbinserts(llrb);
	times(stop);
	print("LLRB:\nTime for %d insertions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = avlinserts(avl);
	times(stop);
	print("AVL:\nTime for %d insertions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = lookups(llrb);
	times(stop);
	print("LLRB:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = lookups(avl);
	times(stop);
	print("AVL:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = deletes(llrb);
	times(stop);
	print("LLRB:\nTime for %d deletions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = deletes(avl);
	times(stop);
	print("AVL:\nTime for %d deletions, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = lookups(llrb);
	times(stop);
	print("LLRB:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);

	times(start);
	c = lookups(avl);
	times(stop);
	print("AVL:\nTime for %d lookups, %d successful: %ld\n", NODES, c, stop[0] - start[0]);
}

int rands[NODES];
Inode llrbpool[NODES], avlpool[NODES];

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
		llrbpool[i].v = avlpool[i].v;
	}
}

int
llrbinserts(Bsttree *t)
{
	Inode *n, *ni;
	int r;

	r = 0;
	for(ni = llrbpool; ni < llrbpool+nelem(llrbpool); ni++) {
		n = (Inode*)bstinsert(t, ni);
		if(n == nil)
			r++;
	}
	return r;
}

int
avlinserts(Bsttree *t)
{
	Inode *n, *ni;
	int r;

	r = 0;
	for(ni = avlpool; ni < avlpool+nelem(avlpool); ni++) {
		n = (Inode*)bstinsert(t, ni);
		if(n == nil)
			r++;
	}
	return r;
}

int
deletes(Bsttree *t)
{
	Inode n, *m;
	int *i, r;

	r = 0;
	for(i = rands; i < rands+nelem(rands); i++) {
		n.v = *i;
		m = (Inode*)bstdelete(t, &n);
		if(m != nil)
			r++;
	}
	return r;
}

int
lookups(Bsttree *t)
{
	Inode n, *m;
	int *i, r;

	r = 0;
	for(i = rands; i < rands+nelem(rands); i++) {
		n.v = *i;
		m = (Inode*)bstlookup(t, &n);
		if(m != nil)
			r++;
	}
	return r;
}

int
nodecmp(Bst *a, Bst *b)
{
	Inode *na, *nb;

	na = (Inode*)a;
	nb = (Inode*)b;

	if(na->v < nb->v)
		return -1;
	if(na->v > nb->v)
		return 1;
	return 0;
}
