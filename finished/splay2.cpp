/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem: splay spaly 二叉平衡树维护区间信息区间性质二叉搜索伸展树的旋转信息维护单点插入单点删除合并重复结点前驱查找后继查找排名查找第k大第k小排名查询普通平衡树bzoj 3224
 * BUAA开学季!
 * 深入贯彻落实新时代中国特色社会主义思想，为决胜全面建成小康社会减少bug！
 * Thoroughly apply the Thinking of Socialism with Chinese Characteristics in the New Era, fight for accomplishing a broad wealthy society to reduce bugs!
 * */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include <climits>
#include <cstring>
#include <cassert>
#define NAME "bzoj3224"
#define read(x) scanf("%d", &x)
#ifdef DEBUG
#	define DP(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#	define DP(fmt, ...) 0
#endif

using namespace std;

//二叉树本体
struct tree {
	int v, cnt, size;	//cnt指当前结点的重复数量，size指当前子树的cnt的和
	tree *father, *son[2];
} T[100005];	//内存池

int N, M, cnt;
tree *root, *nil;	//root并不固定，nil是固定的空节点，其cnt、size都是0

tree *new_tree() {
	return T+cnt++;
}

//旋转之后，对区间信息进行维持
void maintain(tree *x) {
	x->size = x->son[0]->size + x->son[1]->size + x->cnt;	//注意这里不是加一，而是加上当前结点的cnt值
}

//旋转操作，将x变成y的父亲
void rotate(tree *x, tree *y) {
	int c;
	x->father = y->father;
	if(y->father != nil) {
		tree *z = y->father;
		z->son[z->son[1] == y] = x;
	}
	c = y->son[1] == x;
	y->son[c] = x->son[c^1];
	x->son[c^1]->father = y;
	y->father = x;
	x->son[c^1] = y;
	if(root == y) {		//注意维护root的指向
		root = x;
	}
	//注意先维持现在的儿子（y）的信息，再维护父亲（x）的信息（自底向上维护信息）
	maintain(y);
	maintain(x);
}

//splay操作，将x旋转至fat下方
void splay(tree *x, tree *fat = nil) {
	while(x != nil && x != root && x->father != fat) {
		tree *y = x->father;
		if(y->father == fat) {
			rotate(x, y);
		} else {
			tree *z = y->father;
			int c, d;
			c = y->son[1] == x;
			d = z->son[1] == y;
			if(c == d) {
				rotate(y, z);
				rotate(x, y);
			} else {
				rotate(x, y);
				rotate(x, z);
			}
		}
	}
}

//构造数，从l到r顺序构造
tree *build(int l, int r, tree *fat) {
	tree *t;
	if(l <= r) {
		t = new_tree();
	} else {
		return nil;
	}
	int m = (l+r)/2;
	t->v = m;
	t->son[0] = build(l, m-1, t);
	t->son[1] = build(m+1, r, t);
	t->size = t->son[0]->size+t->son[1]->size+1;
	t->father = fat;
	return t;
}

//插入操作
tree *insert(int x) {
	tree *now = root, *fat = nil, *ret = nil;
	while(now != nil) {
		fat = now;
		if(now->v >= x) {
			ret = now;
		}
		now = now->son[now->v < x];
	}
	//fat找到的就是待插入位置的父亲，now已经是无异议的nil了
	if(ret->v == x) {
		ret->cnt++;
	} else {
		ret = new_tree();
		ret->father = fat;
		ret->son[0] = ret->son[1] = nil;	//初始化，将ret的指针指向正确的方向
		ret->v = x;
		ret->size = 1;
		ret->cnt = 1;
		if(fat == nil) {
			root = ret;
		} else {
			fat->son[x > fat->v] = ret;
		}
	}
	splay(ret);		//注意将ret转至顶
	return ret;
}

//清除是将x真正从tree上删除
void clear(tree *x) {
	tree *now = x, *s, *y = x->father;
	if(x->son[0] == nil || x->son[1] == nil) {	//如果x的儿子不满
		s = x->son[x->son[0] == nil];
		y->son[y->son[1] == x] = s;
		s->father = y;
		if(x == root) {
			root = s;
		}
		splay(y);			//信息破坏点是y而不是s，如果s是nil，splay(s)则无法更新信息
	} else {	//如果x的两个儿子都满
		now = x->son[1];
		while(now->son[0] != nil) {
			now = now->son[0];
		}
		now->son[0] = x->son[0];
		x->son[0]->father = now;
		x->son[1]->father = y;
		if(y != nil) {		//这个操作类似rotate中的判断，特判y是否是nil
			y->son[y->son[1] == x] = x->son[1];
		}
		if(x == root) {		//只要涉及树形修改，就要保持root更新
			root = x->son[1];
		}
		splay(now);			//splay不仅仅是复杂度的保证，在这里对size信息的更新是必要的
	}
}

//总的删除操作，传入的是欲删除的键值
void remove(int x) {
	tree *now = root;
	while(now != nil) {
		if(now->v == x) {
			if(now->cnt > 0) {			//这个是特判，鲁棒性的表现
				if(--now->cnt == 0) {	//当cnt==0时，必须将now从树上移除，否则查找的二分性将丧失！！！
					clear(now);
					return;
				}
			}
			break;	//这里不可return，因为now如果发生了cnt的变化，还是应该通过splay进行信息维护
		} else {
			now = now->son[x > now->v];
		}
	}
	splay(now);
}

//查询x在序列中的排名
int rank_of(int x) {
	tree *now = root;
	int ret = 0;
	while(now != nil) {
		if(now->v == x) {	//找到了
			ret += now->son[0]->size+1;
			splay(now);		//注意旋转
			break;
		} else if(now->v < x) {
			ret += now->cnt+now->son[0]->size;
		}
		now = now->son[now->v < x];
	}
	//找不到就不转了呗
	return ret;
}

//查询排名为x的结点
tree *ranked(int x) {
	tree *now = root, *ret = nil;
	while(now != nil && x > 0) {
		if(now->son[0]->size < x) {
			ret = now;
			x -= now->son[0]->size + now->cnt;
			now = now->son[1];		//注意这里不能合在一起写，x在if语句中发生了改变！！！
		} else {
			now = now->son[0];
		}
	}
	splay(ret);
	return ret;
}

//求x的前驱
tree *pred(int x) {
	tree *now = root, *ret = nil;
	while(now != nil) {
		if(now->v < x && now->cnt > 0) {
			ret = now;
		}
		now = now->son[now->v < x];
	}
	splay(ret);
	return ret;
}

//求x的后继
tree *succ(int x) {
	tree *now = root, *ret = nil;
	while(now != nil) {
		if(now->v > x && now->cnt > 0) {
			ret = now;
		}
		now = now->son[now->v <= x];
	}
	splay(ret);
	return ret;
}

//这里是用于调试的splay输出的函数
#ifdef DEBUG
void print(tree *t) {
	if(t == nil) {
		return;
	}
	print(t->son[0]);
	printf("	v:%d size:%d cnt:%d\n", t->v, t->size, t->cnt);
	print(t->son[1]);
}
#endif

//注意将niu分配好，并且将指针都指向有用的空间
void preprocess() {
	nil = new_tree();
	nil->son[0] = nil->son[1] = nil->father = nil;
	nil->size = nil->cnt = 0;
	root = nil;
}

void process() {
	int a, c;
	scanf("%d", &N);
	for(int i = 1; i <= N; ++i) {
		scanf("%d%d", &c, &a);
		if(c == 1) {
			insert(a);
		} else if(c == 2) {
			remove(a);
		} else if(c == 3) {
			printf("%d\n", rank_of(a));
		} else if(c == 4) {
			printf("%d\n", ranked(a)->v);
		} else if(c == 5) {
			printf("%d\n", pred(a)->v);
		} else if(c == 6) {
			printf("%d\n", succ(a)->v);
		}
	}
}

int main(int argc, char **argv) {
	freopen(NAME".in", "r", stdin);
#ifdef DEBUG
	freopen(NAME".out", "w", stdout);
#endif

	preprocess();
	process();

	return 0;
}
 
 
