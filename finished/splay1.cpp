/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem:splay spaly set伸展树二叉树二叉搜索树自平衡树插入查找树的旋转
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
#define NAME "bzoj1588"
#define read(x) scanf("%d", &x)

using namespace std;

//splay的本体
struct tree {
	int v;
	tree *father, *son[2];	//son以这样形式的存储可以有效保证其对称性，这里son[0]是左儿子，son[1]是右儿子
} tre[50004];	//splay结点数开一倍即可

int N, cnt;
tree *nil, *root;	//nil指针非常重要，其也应该被赋予一个空的结点

//x是儿子，y是父亲，通过旋转交换二者位置
void rotate(tree *x, tree *y) {
	int c;
	c = y->son[1] == x;		//如果c为0，则x是y的左儿子（反之亦然）
	if(y->father != nil) {	//这里判断y的父亲是否存在，从而修改z的儿子指针
		tree *z = y->father;
		z->son[z->son[1] == y] = x;
	}
	//一定注意这些个指针交换位置的顺序
	x->father = y->father;		//无论y是否有实际father，这一个赋值都是成立的
	y->father = x;
	y->son[c] = x->son[c^1];
	x->son[c^1]->father = y;
	x->son[c^1] = y;
	if(y == root) {				//最后这一个判断十分重要，root是访问整个树的入口，一定要时刻保持其更新
		root = x;
	}
}

//将x转至根部
void splay(tree *x) {
	while(x != nil && x->father != nil) {	//通过判断father是否为nil判定x是否为nil是不可行的，因为在上个rotate中，设计了许多父亲的更改，但是并没有判定儿子是不是nil，故nil->father是一个无意义的值
		if(x->father->father == nil) {
			rotate(x, x->father);
		} else {
			tree *y = x->father;
			tree *z = y->father;
			if((z->son[1] == y) == (y->son[1] == x)) {	//双旋是splay复杂度的保证（玄学）
				rotate(y, z);
				rotate(x, y);
			} else {
				rotate(x, y);
				rotate(x, z);
			}
		}
	}
}

//精确查找
tree *find(int x) {
	tree *now = root, *ret = nil;
	while(now != nil) {
		if(now->v == x) {
			ret = now;
			break;
		} else {
			now = now->son[x > now->v];
		}
	}
	splay(ret);		//重要
	return ret;
}

//小于等于x的最大值
tree *lower_bound(int x) {
	tree *now = root, *ret = nil;
	while(now != nil) {
		if(now->v >= x) {
			ret = now;
			now = now->son[0];
		} else {
			now = now->son[1];
		}
	}
	splay(ret);		//重要
	return ret;
}

//小于x的最大值
tree *even_lower_bound(int x) {
	tree *now = root, *ret = nil;
	while(now != nil) {
		if(now->v < x) {
			ret = now;
			now = now->son[1];
		} else {
			now = now->son[0];
		}
	}
	splay(ret);		//重要
	return ret;
}

//新建立一棵树，返回它的地址
tree *new_tree() {
	tre[cnt].father = tre[cnt].son[0] = tre[cnt].son[1] = nil;
	return tre+(cnt++);
}

//插入
tree *insert(int x) {
	tree *now = root, *fat = nil;
	//到这里都是查找
	while(now != nil) {
		fat = now;
		if(now->v == x) {
			return now;
		}
		now = now->son[x > now->v];
	}
	tree *ret = new_tree();
	ret->father = fat;
	ret->v = x;
	if(fat == nil) {
		root = ret;
	} else {
		fat->son[x > fat->v] = ret;
	}
	splay(ret);		//重要
	return ret;
}

void readin() {
	scanf("%d", &N);
}


void process() {
	int x, ans = 0;
	tree *l, *r;
	nil = new_tree();
	root = nil->father = nil->son[0] = nil->son[1] = nil;
	for(int i = 1; i <= N; ++i) {
		scanf("%d", &x);
		l = even_lower_bound(x);
		r = lower_bound(x);
		if(l == nil && r == nil) {
			ans += x;
		} else if(l == nil) {
			ans += r->v - x;
		} else if(r == nil) {
			ans += x - l->v;
		} else {
			ans += min(x - l->v, r->v - x);
		}
		insert(x);
	}
	printf("%d\n", ans);
}

int main(int argc, char **argv) {
#ifdef DEBUG
	freopen(NAME".in", "r", stdin);
#endif

	readin();
	process();

	return 0;
}
 
 
