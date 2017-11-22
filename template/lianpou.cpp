/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem:树链剖分链修改节点修改链查询路径查询重链剖分区间和线段树单点修改区间查询区间最大值区间最值树dfs子树大小节点深度LCA
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
#define NAME "bzoj1036"
#define read(x) scanf("%d", &x)

using namespace std;

//线段树节点
struct tree {
	int s, m;
	tree *ls, *rs;
} tree_tank[600005];	//注意开20倍序列长

//前向星的链表实现
struct edge {
	int to;
	edge *next;
} edge_tank[60004];	//注意树开2倍长

//变量名已经很详细了
int array_or_values[30004], tree_nodes_values[30004], size_of_subtree[30004], son_with_maximum_size[30004], head_of_chain[30004], order_in_new[30004], depth_of[30004], father_of[30004];
int N, M, edges_count, tree_count, in_order_count;
edge *first_edge_of[30004];
tree *root;

//有向边从f到t
void add_edge(int f, int t) {
	edge_tank[edges_count].next = first_edge_of[f];
	edge_tank[edges_count].to = t;
	first_edge_of[f] = &edge_tank[edges_count++];
}

void readin() {
	int a, b;
	scanf("%d", &N);
	for(int i = 1; i < N; ++i) {
		scanf("%d%d", &a, &b);
		add_edge(a, b);
		add_edge(b, a);
	}
	for(int i = 1; i <= N; ++i) {
		scanf("%d", tree_nodes_values+i);
	}
}

//第一次预处理信息
void dfs1(int x, int f) {
	int now, max_size = -1;
	size_of_subtree[x] = 1;			//子树大小初始值
	son_with_maximum_size[x] = -1;	//最大儿子初始值
	depth_of[x] = depth_of[f]+1;	//深度递推公式
	father_of[x] = f;
	//遍历指针版前向星方法
	for(edge *i = first_edge_of[x]; i != NULL; i = i->next) {
		now = i->to;
		//注意此处判断
		if(now != f) {
			dfs1(now, x);
			size_of_subtree[x] += size_of_subtree[now];
			if(size_of_subtree[now] > max_size) {
				son_with_maximum_size[x] = now;
				max_size = size_of_subtree[now];	//注意更新此处max_size值，否则重链剖分失败
			}
		}
	}
}

//第二次真正开刀
void dfs2(int x, int f, int link) {
	int now;
	head_of_chain[x] = link;			//处理链剖信息
	order_in_new[x] = ++in_order_count;	//处理新序信息
	array_or_values[order_in_new[x]] = tree_nodes_values[x];	//将节点值转移到序列值
	//优先递归重子，将其循环独立
	for(edge *i = first_edge_of[x]; i != NULL; i = i->next) {
		now = i->to;
		if(now != f && now == son_with_maximum_size[x]) {
			dfs2(now, x, link);
			break;
		}
	}
	//再递归其他儿子，注意判断，不要重复遍历
	for(edge *i = first_edge_of[x]; i != NULL; i = i->next) {
		now = i->to;
		if(now != f && now != son_with_maximum_size[x]) {
			dfs2(now, x, now);
		}
	}
}

tree *new_tree() {
	return &tree_tank[tree_count++];
}

tree *build(int l, int r, tree *t) {
	if(l == r) {
		t->s = t->m = array_or_values[l];
		return t;
	}
	int m = (l+r)/2;
	t->ls = build(l, m, new_tree());
	t->rs = build(m+1, r, new_tree());
	t->m = max(t->ls->m, t->rs->m);
	t->s = t->ls->s + t->rs->s;
	return t;
}

void modify(int l, int r, tree *t, int pos, int v) {
	if(l == r) {
		t->m = t->s = v;
		return;
	}
	int m = (l+r)/2;
	if(pos <= m) {
		modify(l, m, t->ls, pos, v);
	} else {
		modify(m+1, r, t->rs, pos, v);
	}
	t->m = max(t->ls->m, t->rs->m);
	t->s = t->ls->s + t->rs->s;
}

//由于此题单点修改，不需要维护树上性质
void modify(int pos, int v) {
	modify(1, N, root, order_in_new[pos], v);
}

int mquery(int l, int r, tree *t, int ql, int qr) {
	if(ql <= l && qr >= r) {
		return t->m;
	}
	if(qr < l || ql > r) {
		return INT_MIN;
	}
	int m = (l+r)/2;
	return max(mquery(l, m, t->ls, ql, qr), mquery(m+1, r, t->rs, ql, qr));
}

int mquery(int u, int v) {
	int ret = INT_MIN;	//！！千万注意此处的缺省值必须是最小值，否则分分钟WA穿还不知道为啥
	//先让链头深的节点向上走
	while(head_of_chain[u] != head_of_chain[v]) {
		if(depth_of[head_of_chain[u]] < depth_of[head_of_chain[v]]) {
			swap(u, v);
		}
		//走的同时统计此段信息
		ret = max(ret, mquery(1, N, root, order_in_new[head_of_chain[u]], order_in_new[u]));
		u = father_of[head_of_chain[u]];
	}
	if(depth_of[u] < depth_of[v]) {
		swap(u, v);
	}
	//不要妄加特判
	ret = max(ret, mquery(1, N, root, order_in_new[v], order_in_new[u]));
	return ret;
}

int squery(int l, int r, tree *t, int ql, int qr) {
	if(ql <= l && qr >= r) {
		return t->s;
	}
	if(qr < l || ql > r) {
		return 0;
	}
	int m = (l+r)/2;
	return squery(l, m, t->ls, ql, qr)+squery(m+1, r, t->rs, ql, qr);
}

int squery(int u, int v) {
	int ret = 0;
	while(head_of_chain[u] != head_of_chain[v]) {
		if(depth_of[head_of_chain[u]] < depth_of[head_of_chain[v]]) {
			swap(u, v);
		}
		//注意分清两个询问分别调用的函数，不要调错了
		ret += squery(1, N, root, order_in_new[head_of_chain[u]], order_in_new[u]);
		u = father_of[head_of_chain[u]];
	}
	if(depth_of[u] < depth_of[v]) {
		swap(u, v);
	}
	ret += squery(1, N, root, order_in_new[v], order_in_new[u]);
	return ret;
}

//预处理一共三件事：两次dfs，建立线段树
void preprocess() {
	dfs1(1, 1);
	dfs2(1, 1, 1);
	root = build(1, N, new_tree());
}

void process() {
	scanf("%d", &M);
	char c[10];
	int a, b;
	for(int i = 1; i <= M; ++i) {
		scanf("%s%d%d", c, &a, &b);
		if(c[1] == 'M') {
			printf("%d\n", mquery(a, b));
		} else if(c[1] == 'S') {
			printf("%d\n", squery(a, b));
		} else {
			modify(a, b);
		}
	}
}

int main(int argc, char **argv) {
	freopen(NAME".in", "r", stdin);

	readin();
	preprocess();
	process();

	return 0;
}
 
 
