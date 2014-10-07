#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>
#include <deque>
#include <set>
#include <map>
#include <bitset>
#define		NONE	'*'
using namespace std;

#define DEBUG

typedef struct _node{
	char	name;
	short	weight;
	struct _node*	lc;
	struct _node*	rc;
}hnode;

bool comp(hnode* n1, hnode* n2)
{
	if(n1->weight <= n2->weight){
		return true;
	}else{
		return false;
	}
}

hnode* make_huffman_tree(const char* sample)
{
	if(!sample){
		return NULL;
	}
	set<char> record;
	deque<hnode*> forest;
	for(int i=0; i<strlen(sample); i++){
		if(record.count(sample[i])==0){
			record.insert(sample[i]);
			hnode* tmp = new hnode;
			tmp->name = sample[i];
			tmp->weight =1;
			tmp->lc = NULL;
			tmp->rc = NULL;
			forest.push_back(tmp);
		}else{
			for(int j=0; j<forest.size(); j++){
				if(forest[j]->name == sample[i]){
					forest[j]->weight ++;
					break;
				}
			}
		}
	}
#ifdef DEBUG
	deque<hnode*>::iterator it = forest.begin();
	while(it != forest.end()){
		cout<<(*it)->name<<": "<<(*it)->weight<<endl;
		it++;
	}
#endif
	while(forest.size()>1){
		sort(forest.begin(),forest.end(),comp);
		hnode* tmp = new hnode;
		tmp->name = NONE;
		tmp->weight = forest[0]->weight + forest[1]->weight;
		tmp->lc = forest[0];
		tmp->rc = forest[1];
		forest.push_back(tmp);
		forest.pop_front();
		forest.pop_front();
	}

	return forest[0];
}

static map<char,string> HuffCode;

void make_huffman_code(hnode* btree) /* DFS algorithm */
{
	static deque<char> track;
	if(!btree){
		return ;
	}else{
		if(btree->name != NONE){
			string tmp="";
			for(int i=0; i<track.size(); i++){
				tmp += track[i];
			}
			HuffCode[btree->name] = tmp;
			//cout<<btree->name<<": "<<HuffCode[btree->name]<<endl;
		}
		if(btree->lc){
			track.push_back('0');
			make_huffman_code(btree->lc);
			track.pop_back();
		}
		if(btree->rc){
			track.push_back('1');
			make_huffman_code(btree->rc);
			track.pop_back();
		}
	}
}

void debug()
{
#ifdef DEBUG
	if(HuffCode.empty()){
		return ;
	}
	for(map<char,string>::iterator it = HuffCode.begin(); it!=HuffCode.end(); it++){
		cout<<"["<<it->first<<"] "<<it->second<<endl;
	}
#endif
}

string code(const char* sample)
{
	if(HuffCode.empty()){
		return "";
	}
	string tmp = "";
	for(int i=0; i<strlen(sample); i++){
		tmp += HuffCode[sample[i]];
	}
	return tmp;
}

int main()
{
	const char* test ="abbbdddddddeeeeeeeeeeeeeeee";
	hnode* HuffmanTree = make_huffman_tree(test);
	make_huffman_code(HuffmanTree);
	debug();
	string result = code(test);
	cout<<"huffman code : "<<result<<endl;

	return 0;
}
