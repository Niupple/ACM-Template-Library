/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem:splay spaly set��չ��������������������ƽ�����������������ת
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
#define NAME "bzoj1588"
#define read(x) scanf("%d", &x)

using namespace std;

//splay�ı���
struct tree {
	int v;
	tree *father, *son[2];	//son��������ʽ�Ĵ洢������Ч��֤��Գ��ԣ�����son[0]������ӣ�son[1]���Ҷ���
} tre[50004];	//splay�������һ������

int N, cnt;
tree *nil, *root;	//nilָ��ǳ���Ҫ����ҲӦ�ñ�����һ���յĽ��

//x�Ƕ��ӣ�y�Ǹ��ף�ͨ����ת��������λ��
void rotate(tree *x, tree *y) {
	int c;
	c = y->son[1] == x;		//���cΪ0����x��y������ӣ���֮��Ȼ��
	if(y->father != nil) {	//�����ж�y�ĸ����Ƿ���ڣ��Ӷ��޸�z�Ķ���ָ��
		tree *z = y->father;
		z->son[z->son[1] == y] = x;
	}
	//һ��ע����Щ��ָ�뽻��λ�õ�˳��
	x->father = y->father;		//����y�Ƿ���ʵ��father����һ����ֵ���ǳ�����
	y->father = x;
	y->son[c] = x->son[c^1];
	x->son[c^1]->father = y;
	x->son[c^1] = y;
	if(y == root) {				//�����һ���ж�ʮ����Ҫ��root�Ƿ�������������ڣ�һ��Ҫʱ�̱��������
		root = x;
	}
}

//��xת������
void splay(tree *x) {
	while(x != nil && x->father != nil) {	//ͨ���ж�father�Ƿ�Ϊnil�ж�x�Ƿ�Ϊnil�ǲ����еģ���Ϊ���ϸ�rotate�У��������ุ�׵ĸ��ģ����ǲ�û���ж������ǲ���nil����nil->father��һ���������ֵ
		if(x->father->father == nil) {
			rotate(x, x->father);
		} else {
			tree *y = x->father;
			tree *z = y->father;
			if((z->son[1] == y) == (y->son[1] == x)) {	//˫����splay���Ӷȵı�֤����ѧ��
				rotate(y, z);
				rotate(x, y);
			} else {
				rotate(x, y);
				rotate(x, z);
			}
		}
	}
}

//��ȷ����
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
	splay(ret);		//��Ҫ
	return ret;
}

//С�ڵ���x�����ֵ
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
	splay(ret);		//��Ҫ
	return ret;
}

//С��x�����ֵ
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
	splay(ret);		//��Ҫ
	return ret;
}

//�½���һ�������������ĵ�ַ
tree *new_tree() {
	tre[cnt].father = tre[cnt].son[0] = tre[cnt].son[1] = nil;
	return tre+(cnt++);
}

//����
tree *insert(int x) {
	tree *now = root, *fat = nil;
	//�����ﶼ�ǲ���
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
	splay(ret);		//��Ҫ
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
 
 
