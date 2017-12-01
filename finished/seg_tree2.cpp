/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem:�ɳ־û��߶����ɳ־û����ݽṹ�ɳ־û�����ǰ׺�����߶�����������β�����challenge 2
 * BCPC���!
 * ����᳹��ʵϰ��ƽ��ʱ���й���ɫ�������˼�룬Ϊ��ʤȫ�潨��С��������bug��
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

//����Ŀ����Ҫ��׷�Ӳ�����Ҫ��¼�������е�Ԫ����
struct tree {
	int v, size;
	tree *ls, *rs;
} T[2000006];

tree *root[100005], nil;	//nil��һ���սڵ㣬��ȻҲ���Խ���һ�ÿ�����������������
int M, cnt, cnn;			//cnn���ڼ�¼��ǰ�Ѿ�ʹ���˶��ٿ����ˣ�������ΪQ��������

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
	//���ҵ��߽�ʱ���޸�Ȩֵ�Լ�����ֵ
	if(l == r) {
		nt->v = v;
		nt->size = 1;
		return nt;
	}
	int m = (l+r)/2;
	//ע������������query��ͬ���������ҵ���һ��û��������λ�ã�sizeΪ0��
	if(ot->ls->size == m-l+1) {
		nt->rs = append(m+1, r, ot->rs, new_tree(), v);
		nt->ls = ot->ls;	//ע�⽫δ���µķ�������ԭ������
	} else {
		nt->ls = append(l, m, ot->ls, new_tree(), v);
		nt->rs = ot->rs;
	}
	nt->v = 0;
	nt->size = nt->ls->size + nt->rs->size;	//һ����������
	return nt;
}

//��������ͨ�ĵ����������
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

//����������ʵ�൱�򵥣�ֻ��Ҫ���������ĸ����ɣ��������追����һ����
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
 
 
