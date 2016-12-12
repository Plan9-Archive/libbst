enum Walks {
	PRE,
	POST,
	IN
};

extern int bststacksize;

Channel *bstwalk(Bsttree *tree, Bst *start, Bst *finish, enum Walks type);
