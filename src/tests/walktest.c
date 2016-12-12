#include <u.h>
#include <libc.h>
#include <bst.h>
#include <thread.h>
#include <bstwalk.h>

typedef struct Intnode {
	Bst;
	int v;
} Intnode;

enum {
	MAX =  1000,
	NODES = 100
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
threadmain(int argc, char **argv)
{
	Bsttree *t;
	Intnode *a, *sp, *ep, s, e;
	Channel *c;
	int type;

	type = AVL;
	ARGBEGIN {
	case 'a':
		type = AVL;
		break;
	case 'l':
		type = LLRB;
		break;
	} ARGEND

	sp = nil;
	ep = nil;
	if(argc == 2) {
		s.v = strtol(argv[0], nil, 10);
		sp = &s;
		e.v = strtol(argv[1], nil, 10);
		ep = &e;
	}
	srand(time(nil));
	t = bstcreate(type, intcmp);

	for(a = pool; a < pool+nelem(pool); a++) {
		a->v = nrand(MAX);
		bstinsert(t, a);
	}

	c = bstwalk(t, sp, ep, IN);
	while((a = recvp(c)) != nil)
		print("%d ", a->v);
	chanfree(c);
}
