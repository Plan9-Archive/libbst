typedef struct Avltree Avltree;
typedef Bst *(*Bstop)(Bsttree*, Bst*);

struct Bsttree
{
	int (*cmp)(Bst*, Bst*);
	Bst *root;
	Bstop lookup;
	Bstop delete;
	Bstop insert;
};

Bst *avllookup(Bsttree*, Bst*);
Bst *avldelete(Bsttree*, Bst*);
Bst *avlinsert(Bsttree*, Bst*);

Bst *llrblookup(Bsttree*, Bst*);
Bst *llrbdelete(Bsttree*, Bst*);
Bst *llrbinsert(Bsttree*, Bst*);
