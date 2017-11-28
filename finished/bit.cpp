/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem:ֵ�����ݽṹֵ����״����������Ե����޸������ѯBIT
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
#define NAME "cv1688"
#define read(x) scanf("%d", &x)
#define lowbit(x) ((x)&(-(x)))
#ifdef DEBUG
#	define DP(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#	define DP(fmt, ...) 0
#endif

using namespace std;

/*������
 * N		count of numbers
 * av[]		array of value
 * tree[]	BIT itself
 * M		upper bound of BIT, maximum of av[i]
 */
long long N, av[100005], tree[100005], M;

void readin() {
	cin >> N;
	for(long long i = 1; i <= N; ++i) {
		cin >> av[i];
		M = max(av[i], M);
	}
}

//��״����ĵ����޸ģ���posһ·�ӵ��Ͻ�M
void modify(long long pos) {
	while(pos <= M) {
		tree[pos]++;	//������������������ͼ��ϸõ������ע����״������޸ı����������е�ֵ
		pos += lowbit(pos);
	}
}

//��״����ķ��ʣ�ֻ֧��ǰ׺�ͷ��ʣ���Ҫ��ά�����������������posһ·����0
//��ע�����[l,r]ʱ��ʹ��query(r)-query(r-1)
long long query(long long r) {
	long long ret = 0;
	while(r > 0) {
		ret += tree[r];
		r -= lowbit(r);
	}
	return ret;
}

void process() {
	long long ans = 0;
	for(long long i = 1; i <= N; ++i) {
		modify(av[i]);
		ans += i-query(av[i]);
	}
	cout << ans << endl;
}

int main() {
	freopen(NAME".in", "r", stdin);
#ifndef DEBUG
	freopen(NAME".out", "w", stdout);
#endif

	readin();
	process();

	return 0;
}
 
 
