#ifndef J_AC_BM_H
#define J_AC_BM_H
#define PATTERN_LEN	128

typedef struct _pattern_tree_node {
	int label;
	int depth;
	
	unsigned char ch;
	int GSshift;		// Good Suffix shift for this char

	unsigned char one_child;
	struct _pattern_tree_node *childs[256];
	int nchild;
	struct _pattern_tree_node *parent;
} pattern_tree_node;

/* */
typedef struct _pattern_data {
	int len;
	int flags;
	unsigned char data[PATTERN_LEN];
} pattern_data;

typedef struct _pattern_tree {
	pattern_tree_node *root; // root of tree
	int max_depth;
	int min_pattern_size;
	int BCshift[256];	// Bad Char shift for 256 chars
	pattern_data *pattern_list; 	// pointed to structure arrays
	int pattern_count;
} pattern_tree;

typedef struct _matched_info {
	int pattern_i;
	unsigned long offset; 
} matched_info_t;

/* pas_acbm.c */
pattern_tree *pasAcbmInit(pattern_data *patterns, int npattern);
int pasAcbmSearch(pattern_tree *ptree, unsigned char *text, int text_len, 
		unsigned int matched_indexs[], int nmax_index);
void pasAcbmClean(pattern_tree *ptree);
int pasAcbmSearchEx(pattern_tree *ptree, unsigned char *text, int text_len,
		matched_info_t matched_items[], int nmax_index);

#endif

