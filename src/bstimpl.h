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
