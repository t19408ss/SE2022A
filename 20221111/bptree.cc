#include "bptree.h"
#include <bits/stdc++.h>
#include <vector>
#include <sys/time.h>

#define DATA_LENGTH 10000000

struct timeval
cur_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

void
print_tree_core(NODE *n)
{
	printf("["); 
	for (int i = 0; i < n->nkey; i++) {
		if (!n->isLeaf) print_tree_core(n->chi[i]); 
		printf("%d", n->key[i]); 
		if (i != n->nkey-1 && n->isLeaf) putchar(' ');
	}
	if (!n->isLeaf) print_tree_core(n->chi[n->nkey]);
	printf("]");
}

void
print_tree(NODE *node)
{
	print_tree_core(node);
	printf("\n"); fflush(stdout);
}

NODE *
find_leaf(NODE *node, int key)
{
	int kid;

	if (node->isLeaf) return node;
	for (kid = 0; kid < node->nkey; kid++) {
		if (key < node->key[kid]) break;
	}

	return find_leaf(node->chi[kid], key);
}

TEMP *
copy_leaf_to_temp(TEMP *temp, NODE *leaf)
{
	int i;
	temp->isLeaf = leaf->isLeaf;
	temp->nkey = leaf->nkey;
	for (i = 0; i < leaf->nkey; i++) {
		temp->key[i] = leaf->key[i];
		temp->chi[i] = leaf->chi[i];
	}
	temp->chi[leaf->nkey] = leaf->chi[leaf->nkey];
	return temp;
}

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
{
	int i;
	if (key < leaf->key[0]) {
		for (i = leaf->nkey; i > 0; i--) {
			leaf->chi[i] = leaf->chi[i-1] ;
			leaf->key[i] = leaf->key[i-1] ;
		} 
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;
	}
	else {
		for (i = 0; i < leaf->nkey; i++) {
			if (key < leaf->key[i]) break;
		}
		for (int j = leaf->nkey; j > i; j--) {		
			leaf->chi[j] = leaf->chi[j-1] ;
			leaf->key[j] = leaf->key[j-1] ;
		} 
		leaf->key[i] = key;
		leaf->chi[i] = (NODE *)data;
	}
	leaf->nkey++;

	return leaf;
}

TEMP *
insert_in_temp(TEMP *temp, int key, DATA *data)
{
	int i;
	if (key < temp->key[0]) {
		for (i = temp->nkey; i > 0; i--) {
			temp->chi[i] = temp->chi[i-1] ;
			temp->key[i] = temp->key[i-1] ;
		} 
		temp->key[0] = key;
		temp->chi[0] = (NODE *)data;
	}
	else {
		for (i = 0; i < temp->nkey; i++) {
			if (key < temp->key[i]) break;
		}
		for (int j = temp->nkey; j > i; j--) {		
			temp->chi[j] = temp->chi[j-1] ;
			temp->key[j] = temp->key[j-1] ;
		} 
		temp->key[i] = key;
		temp->chi[i] = (NODE *)data;
	}
	temp->nkey++;

	return temp;
}

NODE *
alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

NODE *
insert_key_and_ptr(NODE *parent, int key, NODE *ptr)
{
	int i;
	if (key < parent->key[0]) {
		for (i = parent->nkey; i > 0; i--) {
			parent->chi[i+1] = parent->chi[i] ;
			parent->key[i] = parent->key[i-1] ;
		} 
		parent->key[0] = key;
		parent->chi[1] = ptr;
	}
	else {
		for (i = 0; i < parent->nkey; i++) {
			if (key < parent->key[i]) break;
		}
		for (int j = parent->nkey; j > i; j--) {		
			parent->chi[j+1] = parent->chi[j] ;
			parent->key[j] = parent->key[j-1] ;
		} 
		parent->key[i] = key;
		parent->chi[i+1] = ptr;
	}
	parent->nkey++;

	return parent;
}

TEMP *
insert_key_and_ptr_temp(TEMP *temp, int key, NODE *ptr)
{
	int i;
	if (key < temp->key[0]) {
		for (i = temp->nkey; i > 0; i--) {
			temp->chi[i+1] = temp->chi[i] ;
			temp->key[i] = temp->key[i-1] ;
		} 
		temp->key[0] = key;
		temp->chi[1] = ptr;
	}
	else {
		for (i = 0; i < temp->nkey; i++) {
			if (key < temp->key[i]) break;
		}
		for (int j = temp->nkey; j > i; j--) {		
			temp->chi[j+1] = temp->chi[j] ;
			temp->key[j] = temp->key[j-1] ;
		} 
		temp->key[i] = key;
		temp->chi[i+1] = ptr;
	}
	temp->nkey++;

	return temp;
}

NODE *
clear_leaf(NODE *leaf)
{
	int i;
	for (i = 0; i < N; i++) {
		if (i < N-1) leaf->key[i] = 0;
		leaf->chi[i] = NULL;
	}
	leaf->nkey = 0;
	return leaf;
}

TEMP *
alloc_temp()
{
	TEMP *temp;
	if (!(temp = (TEMP *)calloc(1, sizeof(TEMP)))) ERR;
	temp->isLeaf = true;
	temp->nkey = 0;

	return temp;
}

NODE *
insert_in_parent(NODE *n, int key, NODE *nd)
{
	if (n == Root) {
		NODE *r;
		r  = alloc_leaf(NULL);
		r->isLeaf = false;
		r->chi[0] = n;
		r->key[0] = key;
		r->chi[1] = nd;
		r->nkey++;
		n->parent = r;
		nd->parent = r;
		Root = r;
		return r;
	}
	nd->parent = n->parent;
	if (n->parent->nkey < N - 1) {
		n->parent = insert_key_and_ptr(n->parent, key, nd);
	} else {
		// split parent
		TEMP *temp;
		NODE *parentd;
		int i, c;

		temp = alloc_temp();
		parentd = alloc_leaf(NULL);
		parentd->isLeaf = false;

		temp = copy_leaf_to_temp(temp, n->parent);
		temp = insert_key_and_ptr_temp(temp, key, nd);

		n->parent = clear_leaf(n->parent);

		// copy from temp to n->parent and parentd
		c = std::ceil(((float)N+1)/2);
		for (i = 0; i < c; i++) {
			if (i < c-1) {
				n->parent->key[i] = temp->key[i];
				n->parent->nkey++;
			}
			n->parent->chi[i] = temp->chi[i];
			//n->parent->chi[i]->parent = n->parent;
			if (c + i < temp->nkey) {
				parentd->key[i] = temp->key[c + i];
				parentd->nkey++;
			}
			if (c + i < temp->nkey+1) {
				parentd->chi[i] = temp->chi[c + i];
			}
		}

		insert_in_parent(n->parent, temp->key[c-1], parentd);

		for (i = 0; i < parentd->nkey+1; i++) {
			parentd->chi[i]->parent = parentd;
		}
	}
	return n->parent;
}

void 
insert(int key, DATA *data)
{
	NODE *leaf;

	if (Root == NULL) {
		leaf = alloc_leaf(NULL);
		Root = leaf;
	}
	else {
		leaf = find_leaf(Root, key);
	}
	if (leaf->nkey < (N-1)) {
		leaf = insert_in_leaf(leaf, key, data);
	}
	else {
		// split
		TEMP *temp;
		NODE *leafd;
		int i, c;

		temp = alloc_temp();
		leafd = alloc_leaf(NULL);

		temp = copy_leaf_to_temp(temp, leaf);
		temp = insert_in_temp(temp, key, data);

		leafd->chi[N-1] = leaf->chi[N-1];
		leaf->chi[N-1] = leafd;

		leaf = clear_leaf(leaf);

		// copy from temp to leaf and leafd
		c = std::ceil((float)N/2);
		for (i = 0; i < c; i++) {
			leaf->key[i] = temp->key[i];
			leaf->nkey++;
			leaf->chi[i] = temp->chi[i];

			leafd->key[i] = temp->key[c + i];
			leafd->nkey++;
			leafd->chi[i] = temp->chi[c + i];
		}

		insert_in_parent(leaf, leafd->key[0], leafd);
	}
}

void
init_root(void)
{
	Root = NULL;
}

int 
interactive()
{
	int key;

	std::cout << "Key: ";
	std::cin >> key;

	return key;
}

int
main(int argc, char *argv[])
{
	struct timeval begin, end;
	int i;
	std::vector<int> a;
	std::vector<int> d;
	std::vector<int> r;
	NODE *l;

	init_root();

	printf("---Insert----\n");
	for (i = 1; i <= DATA_LENGTH; i++) {
		insert(i, NULL);
		if(i % 1000000 == 0) printf("Inserted: %d\n", i);
		a.push_back(i);
		d.push_back(DATA_LENGTH - i + 1);
		r.push_back(i);
	}
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(r.begin(), r.end(), engine);
	printf("----Search:ASC-----\n");
	printf("<<TIME MEASUREMENT START>>\n");
	begin = cur_time();
	for (i = 0; i < DATA_LENGTH; i++) {
		l = find_leaf(Root, a[i]);
		if (i % 1000000 == 0) {
			printf("Found: a[%d] = %d\n", i, a[i]);
			print_tree(l->parent);
		}
	}
	end = cur_time();
	printf("<<TIME MEASUREMENT STOP>>\n");
	printf("time = %lds\n", end.tv_sec - begin.tv_sec);
	printf("----Search:DESC----\n");
	printf("<<TIME MEASUREMENT START>>\n");
	begin = cur_time();
	for (i = 0; i < DATA_LENGTH; i++) {
		l = find_leaf(Root, d[i]);
		if (i % 1000000 == 0) {
			printf("Found: d[%d] = %d\n", i, d[i]);
			print_tree(l->parent);
		}
	}
	end = cur_time();
	printf("<<TIME MEASUREMENT STOP>>\n");
	printf("time = %lds\n", end.tv_sec - begin.tv_sec);
	printf("----Search:RAND----\n");
	printf("<<TIME MEASUREMENT START>>\n");
	begin = cur_time();
	for (i = 0; i < DATA_LENGTH; i++) {
		l = find_leaf(Root, r[i]);
		if (i % 1000000 == 0) {
			printf("Found: r[%d] = %d\n", i, r[i]);
			print_tree(l->parent);
		}
	}
	end = cur_time();
	printf("<<TIME MEASUREMENT STOP>>\n");
	printf("time = %lds\n", end.tv_sec - begin.tv_sec);

	return 0;
}
