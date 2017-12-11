/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem: KMP
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
#define NAME "kmp"

using namespace std;

char txt[1002], pat[102];
int nxt[102], let, lep;

void readin() {
	scanf("%s%s", pat, txt);
	let = strlen(txt);
	lep = strlen(pat);
}

void preprocess() {
	for(int i = 1; i < lep; ++i) {
		if(pat[i] == pat[nxt[i]]) {
			nxt[i+1] = nxt[i]+1;
		} else {
			int j;
			for(j = nxt[i]; j != 0 && pat[j] != pat[i]; j = nxt[j]);
			if(pat[j] == pat[i]) {
				nxt[i+1] = j+1;
			} else {
				nxt[i+1] = 0;
			}
		}
	}
}

void process() {
	for(int i = 0, j = 0; i < let; ++i) {
		while(j && pat[j] != txt[i]) {
			j = nxt[j];
		}
		if(txt[i] == pat[j]) {
			if(++j == lep) {
				printf("MATCH at %d\n", i-lep+1);
			}
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
 
 
