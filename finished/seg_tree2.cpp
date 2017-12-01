/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem:可持久化线段树可持久化数据结构可持久化数组前缀和套线段树撤销操作尾部添加challenge 2
 * BCPC复活季!
 * 深入贯彻落实习近平新时代中国特色社会主义思想，为决胜全面建成小康社会减少bug！
 * Thoroughly apply the Xi Jinping's Thought of Socialism with Chinese Characteristics in the New Era, fight for accomplishing a broad wealthy society to reduce bugs!
 * */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include <climits>
#include <cstring>
#define NAME "challenge2"

using namespace std;

//此题目中需要的追加操作需要记录子树已有的元素数
struct tree {
	int v, size;
	tree *ls, *rs;
} T[2000006];

tree *root[100005], nil;	//nil是一个空节点，当然也可以建立一棵空树，但是这样更优
int M, cnt, cnn;			//cnn用于记录当前已经使用了多少棵树了，这是因为Q不增加树

void readin() {
	scanf("%d", &M);
}

void preprocess() {
	nil.ls = nil.rs = &nil;
	nil.v = nil.size = 0;
	root[0] = &nil;
}

tree *new_tree() {
	return T+cnt++;
}

//ot--old tree; nt--new tree
tree *append(int l, int r, tree *ot, tree *nt, int v) {
	//当找到边界时，修改权值以及附加值
	if(l == r) {
		nt->v = v;
		nt->size = 1;
		return nt;
	}
	int m = (l+r)/2;
	//注意这里条件与query不同，这里是找到第一个没有填数的位置（size为0）
	if(ot->ls->size == m-l+1) {
		nt->rs = append(m+1, r, ot->rs, new_tree(), v);
		nt->ls = ot->ls;	//注意将未更新的方向连回原来的树
	} else {
		nt->ls = append(l, m, ot->ls, new_tree(), v);
		nt->rs = ot->rs;
	}
	nt->v = 0;
	nt->size = nt->ls->size + nt->rs->size;	//一定更新数据
	return nt;
}

//查找与普通的单点查找无异
int query(int l, int r, tree *t, int pos) {
	if(l == r) {
		return t->v;
	}
	int m = (l+r)/2;
	if(pos <= m) {
		return query(l, m, t->ls, pos);
	} else {
		return query(m+1, r, t->rs, pos);
	}
}

//撤销操作其实相当简单，只需要访问曾经的根即可，甚至无需拷贝那一个链
void undo(int x, int b) {
	root[x] = root[x-1-b];
}

void process() {
	int a;
	char c[3];
	cnn = 1;
	for(int i = 1; i <= M; ++i) {
		scanf("%s%d", c, &a);
		if(c[0] == 'A') {
			root[cnn] = append(1, M, root[cnn-1], new_tree(), a);
			cnn++;
		} else if(c[0] == 'Q') {
			printf("%d\n", query(1, M, root[cnn-1], a));
		} else if(c[0] == 'U') {
			undo(cnn++, a);
		}
	}
}

int main(void) {
#ifdef DEBUG
	freopen(NAME".in", "r", stdin);
#endif

	readin();
	preprocess();
	process();

	return 0;
}
 
 
