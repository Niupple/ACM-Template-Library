/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem: dfs�� dfs order �߶��� segment tree seg tree �����ѯ�����޸������޸�������ѯ
 * BUAA��ѧ��!
 * ����᳹��ʵ��ʱ���й���ɫ�������˼�룬Ϊ��ʤȫ�潨��С��������bug��
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

//edge������ع�����ǰ���ǵ�ʵ��
struct edge{
	int to, next;
} ege[200005];

//tree���߶����Ľڵ�
struct tree{
	int v, mark;	//vΪ�ڵ�洢��ֵ��markΪlazy���
	tree *ls, *rs;
} tre[3000006];		//������ע�⿪N��20��������

/*������
 * nde		first edge of node i
 * cne		count of edges
 * cnt		count of tree
 * av[]		array of values
 * pos[][]	starting[0] and ending[1] position of node i on dfs sequences
 * cnp		count of positions (subtrees which was visited till now)
 */
int N, M, nde[100005], cne, cnt, av[100005], pos[100005][2], cnp;
tree *root;

//ǰ���Ǽӱ߷���������ߣ���f(rom)��t(o)
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

//dfs����������ǰ�ڵ�Ϊx���丸�ڵ�Ϊf(ather)
void dfs(int x, int f) {
	int now;	//������չ�ڵ�
	pos[x][0] = ++cnp;	//�ڿ�ʼһ�����ı���֮ǰ��¼�ýڵ�
	for(int i = nde[x]; i != -1; i = ege[i].next) {
		//ע��˴��жϣ��Ƿ�ֹ�ظ�dfs�Ĺؼ�
		if(ege[i].to != f) {
			now = ege[i].to;
			dfs(now, x);
		}
	}
	pos[x][1] = cnp;	//��һ�����������֮���¼���ڵ㣬��������ұղ��������Ҷ˵㲻ռ�ݿռ�
}

tree *new_tree() {
	return &tre[cnt++];	//ע��&
}

//�����н����߶�������
tree *build(int l, int r, tree *t) {
	//�˵�ע���ʼ��
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

//�˴�mark��ʾ�ýڵ��v�Ѿ����޸ģ��������������ᣬ��Ҫpush_down�ⶳ
void push_down(int l, int r, tree *t) {
	if(t->mark) {
		int m = (l+r)/2;
		if(t->rs) {
			t->rs->mark = !t->rs->mark;
			t->rs->v = r-m - t->rs->v;	//����ע�⽫����v����
		}
		if(t->ls) {
			t->ls->mark = !t->ls->mark;
			t->ls->v = m+1-l - t->ls->v;
		}
		t->mark = 0;
	}
}

//�����޸ķ���
void modify(int l, int r, tree *t, int ml, int mr) {
	if(ml <= l && mr >= r) {		//��ȫ����
		t->mark = !t->mark;
		t->v = r-l+1 - t->v;
		return;
	} else if(ml > r || mr < l) {	//��ȫ������
		return;
	}
	int m = (l+r)/2;
	push_down(l, r, t);
	//���������ж���ȫ��������������ۣ��ʲ���Ҫ����t->ls,t->rs�����ڵ����
	modify(l, m, t->ls, ml, mr);
	modify(m+1, r, t->rs, ml, mr);
	//�����������м�ע��˴�������Ϣ����������v�Ѿ����޸ģ�����ֱ��ʹ��
	t->v = t->ls->v + t->rs->v;
}

//������Ϣ��ѯ����
int query(int l, int r, tree *t, int ql, int qr) {
	//��ȫ����
	if(ql <= l && qr >= r) {
		return t->v;
	}
	//��ȫ������
	if(ql > r || qr < l) {
		return 0;
	}
	int m = (l+r)/2;
	//ע���ڷ����ӽڵ�֮ǰ��Ҫpush_down
	push_down(l, r, t);
	//���⡢�߽��������ǰ�Ĵ�������ֱ�ӵ���
	return query(l, m, t->ls, ql, qr)+query(m+1, r, t->rs, ql, qr);
}

void process() {
	char c[5];	//�����ɣ�ʹ��scanf("%c")�����հ׷���%s�����ַ�������Կհ׷�
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
 
