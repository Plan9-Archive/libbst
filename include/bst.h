#pragma lib "libbst.a"
#pragma src "/sys/src/libbst"

typedef struct Bst Bst;
typedef struct Bsttree Bsttree;

#pragma incomplete Bsttree

struct Bst {
	Bst *c[2];
	union {
		int color;
		int balance;
	};
};

Bsttree *bstcreateavl(int(*cmp)(Bst*, Bst*));
Bsttree *bstcreatellrb(int(*cmp)(Bst*, Bst*));
Bst *bstroot(Bsttree*);
Bst *bstlookup(Bsttree*, Bst*);
Bst *bstdelete(Bsttree*, Bst*);
Bst *bstinsert(Bsttree*, Bst*);
