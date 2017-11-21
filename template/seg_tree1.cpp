/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem: dfs序 dfs order 线段树 segment tree seg tree 区间查询区间修改子树修改子树查询
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
#define NAME "rqnoj60"
#undef DEBUG
#define read(x) scanf("%d", &x)

using namespace std;

//edge及其相关过程是前向星的实现
struct edge{
	int to, next;
} ege[200005];

//tree是线段树的节点
struct tree{
	int v, mark;	//v为节点存储的值，mark为lazy标记
	tree *ls, *rs;
} tre[3000006];		//！！！注意开N的20倍！！！

/*变量名
 * nde		first edge of node i
 * cne		count of edges
 * cnt		count of tree
 * av[]		array of values
 * pos[][]	starting[0] and ending[1] position of node i on dfs sequences
 * cnp		count of positions (subtrees which was visited till now)
 */
int N, M, nde[100005], cne, cnt, av[100005], pos[100005][2], cnp;
tree *root;

//前向星加边方法，有向边，从f(rom)到t(o)
void add_edge(int f, int t) {
	ege[cne].to = t;
	ege[cne].next = nde[f];
	nde[f] = cne++;
}

void readin() {
	int a, b;
	scanf("%d", &N);
	for(int i = 1; i <= N; ++i) {
		nde[i] = -1;
	}
	for(int i = 1; i < N; ++i) {
		scanf("%d%d", &a, &b);
		add_edge(a, b);
		add_edge(b, a);
	}
}

//dfs遍历树，当前节点为x，其父节点为f(ather)
void dfs(int x, int f) {
	int now;	//尝试扩展节点
	pos[x][0] = ++cnp;	//在开始一个树的遍历之前记录该节点
	for(int i = nde[x]; i != -1; i = ege[i].next) {
		//注意此处判断，是防止重复dfs的关键
		if(ege[i].to != f) {
			now = ege[i].to;
			dfs(now, x);
		}
	}
	pos[x][1] = cnp;	//在一棵树遍历完成之后记录出节点，采用左闭右闭操作，故右端点不占据空间
}

tree *new_tree() {
	return &tre[cnt++];	//注意&
}

//从序列建立线段树方法
tree *build(int l, int r, tree *t) {
	//端点注意初始化
	if(l == r) {
		t->v = 0;
		t->mark = 0;
		return t;
	}
	int m = (l+r)/2;
	t->ls = build(l, m, new_tree());
	t->rs = build(m+1, r, new_tree());
	t->v = t->mark = 0;
	return t;
}

void preprocess() {
	dfs(1, 1);
	root = build(1, cnp, new_tree());
}

//此处mark表示该节点的v已经被修改，但是子树被冻结，需要push_down解冻
void push_down(int l, int r, tree *t) {
	if(t->mark) {
		int m = (l+r)/2;
		if(t->rs) {
			t->rs->mark = !t->rs->mark;
			t->rs->v = r-m - t->rs->v;	//所以注意将其子v更新
		}
		if(t->ls) {
			t->ls->mark = !t->ls->mark;
			t->ls->v = m+1-l - t->ls->v;
		}
		t->mark = 0;
	}
}

//区间修改方法
void modify(int l, int r, tree *t, int ml, int mr) {
	if(ml <= l && mr >= r) {		//完全包含
		t->mark = !t->mark;
		t->v = r-l+1 - t->v;
		return;
	} else if(ml > r || mr < l) {	//完全不包含
		return;
	}
	int m = (l+r)/2;
	push_down(l, r, t);
	//由于上文有对完全不包含情况的讨论，故不需要考虑t->ls,t->rs不存在的情况
	modify(l, m, t->ls, ml, mr);
	modify(m+1, r, t->rs, ml, mr);
	//！！！！！切记注意此处更新信息！由于其子v已经被修改，可以直接使用
	t->v = t->ls->v + t->rs->v;
}

//区间信息查询方法
int query(int l, int r, tree *t, int ql, int qr) {
	//完全包含
	if(ql <= l && qr >= r) {
		return t->v;
	}
	//完全不包含
	if(ql > r || qr < l) {
		return 0;
	}
	int m = (l+r)/2;
	//注意在访问子节点之前都要push_down
	push_down(l, r, t);
	//特殊、边界情况已在前文处理，可以直接调用
	return query(l, m, t->ls, ql, qr)+query(m+1, r, t->rs, ql, qr);
}

void process() {
	char c[5];	//处理技巧，使用scanf("%c")会读入空白符，%s读入字符串会忽略空白符
	int a;
	scanf("%d", &M);
	for(int i = 1; i <= N; ++i) {
		scanf("%s%d", c, &a);
		if(c[0] == 'C') {
			modify(1, N, root, pos[a][0], pos[a][1]);
		} else {
			printf("%d\n", query(1, N, root, pos[a][0], pos[a][1]));
		}
	}
}

int main(int argc, char **argv) {
	freopen(NAME".in", "r", stdin);
#ifndef DEBUG
	freopen(NAME".out", "w", stdout);
#endif

	readin();
	preprocess();
	process();

	return 0;
}
 
