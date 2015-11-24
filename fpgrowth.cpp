/*
	Names: Jeff Goymerac and Christine Wang
	CS 5001 - Final Project
													*/

#include <string>
#include <vector>
#include <algorithm>
#include "parser.h"

using namespace std;

//parsed from file
//int minCover, maxSize, minAcc, numBest, numAtts, numInsts;
//string[5000][500] data;
//string[50] attr;


struct itemSet
{
	string attName;
	string value;
	int cover,attrCol;
	vector<int> rows;
};
struct by_cover { 
    bool operator()(itemSet const &a, itemSet const &b) { 
        return a.cover > b.cover;
    }
};

struct treeNode{
	string attName;
	string value;
	int number;
	vector<treeNode*> next;
	treeNode* parent;
};

//global varialbes
vector< vector<itemSet> > mData;
vector<itemSet> dataSet, tempSets, finalSets;
vector<int> numSize;
string modData[5000][500];
treeNode myTree ;
int tempIndex, covIndex;
void discard();
void insertValue(int min, int max, string s, int i,int a);

void initialize();

int search(vector<itemSet>, int, int, string);

bool sorter(itemSet, itemSet);

void sort();

void initA1();
//void sortInsts();
void modify();

void makeSets();

void postOrder(treeNode* n, int pad);


int main()
{
	bool found=false;
	parser();
	//dataSet.resize(2);
	
	initA1();
	sort(finalSets.begin(), finalSets.end(), by_cover());

	//Finalsets can now also be used as the header table needed later by only displaying 
	//sets greater then the minCover.
	
	modify();
	
	myTree.attName="root";

	treeNode *tempTree;
	tempTree=&myTree;
	
	for(int i=0;i<mData.size();i++)
	{
		tempTree=&myTree;
		cout<<"\n"<<i<<" ";
		for(int j=0;j<mData[i].size();j++)
		{
			found=false;

			cout<<mData[i][j].attName<<"="<<mData[i][j].value<<",";
			for (int k=0; k<tempTree->next.size();k++ )
			{
				//cout<<tempTree->next[k]->attName<<"="<<tempTree->next[k]->value<<;

				if(tempTree->next[k]->attName==mData[i][j].attName && tempTree->next[k]->value==mData[i][j].value)
				{
					
					tempTree->next[k]->number+=1;
					tempTree=tempTree->next[k];
					found=true;
					//break;
				}
			}
			if(!found)
			{
				bool parentFound=false;//check parents vector for value
				treeNode *parentTrap;
				parentTrap= tempTree->parent;
				/*if(parentTrap!=NULL)
				{					
					for(int k=0; k<parentTrap->next.size();k++)
					{
						cout<<"TESTIGAEAOIFEJ\n";	
						if(parentTrap->next[k]->attName==mData[i][j].attName && parentTrap->next[k]->value==mData[i][j].value)
						{
							cout<<"TESTING--found node in parent vector--\n";
							parentTrap->next[k]->number+=1;
							//tempTree=tempTree->next[k];
							parentFound=true;
							//break;
						}
					}	
				}*/


				if(!parentFound)
				{
					treeNode *newTree=new treeNode;
					newTree->attName=mData[i][j].attName;
					newTree->number=1;	
					newTree->value=mData[i][j].value;
					newTree->parent=tempTree;
					tempTree->next.push_back(newTree);
					tempTree=newTree;
					//postOrder(newTree);	
					//cout<<"TESTING--New Node created\n";
				}
			}
			//myTree.next.push_back(tempTree);
		}
		cout<<"\n";
		//myTree.next.push_back(tempTree);


		//myTree=*tempTree;

	}
		postOrder(&myTree, 0);

	//sortInsts();
}



void initA1()
{
//	Add all item sets of size 1 to global array tempSets
	for(int i=0; i<numInsts; i++)
	{
		for(int a=0; a<numAtts; a++)	
		{
			string s = data[i][a];

			insertValue(0, tempSets.size()-1, s , i,a);

			/*cout << s << "-\t";
			for(int x=0; x<tempSets.size(); x++)
				for(int y=0; y<tempSets[x].value.size(); y++)
					cout << tempSets[x].value[y] << "|";
				cout << "\n\n";*/
		}
	}

	// remove itemSets with coverage less than minimum coverage requirement
	discard();

	// Copy tempSets to finalSets
	//finalSets.clear();
	finalSets = tempSets;

	// add number of item sets of size 1
	numSize.push_back(tempSets.size());
}



void modify()
{
	itemSet temp;
	mData.resize(numInsts);
	for(int i=0;i<finalSets.size();i++)
	{
		temp.attName=finalSets[i].attName;
		temp.value=finalSets[i].value;
		for(int j=0;j<finalSets[i].rows.size();j++)
		{
			mData[finalSets[i].rows[j]].push_back(temp);
		}
	}
	//sort mod data based on FinalSets ordering
	

}





int search(vector<itemSet> v, int low, int high, string key)
{
	// Exit if not found
	if(high < low)
	{
		//cout<<"\ninitialize2\n\n";

		// global variable for index to insert
		tempIndex = low;
		return -1;
	}

	// Midpoint
	int mid = low + ((high - low) / 2);
		//cout<<"\ninitialize3\n\n";

	string s = v[mid].attName + v[mid].value;

	// If key is greater than string at midpoint, low = mid + 1
    if (key > s)
        return search(v, high, mid + 1, key);

    // If key is less than string at midpiont, high = mid-1
    else if (key < s)
        return search(v, low, mid- 1, key);

    // If found, return index
    return mid;
}

// helper function to sort dataSet by coverage
bool sorter(itemSet it1, itemSet it2)
{
	return(it1.cover<it2.cover);
}



void insertValue(int min, int max, string s, int i, int a)
{
	for(int x=0; x<s.size(); x++)
		s[x]=tolower(s[x]);
	int mid = min + (max-min)/2;
	//cout << min << " " << max << " " << mid << "\n";
	
	if(max<min || tempSets.size()<=2)
	{
		itemSet its;
		its.value=s;
		its.cover = 1;
		its.rows.push_back(i);
		its.attName = attributes[a];
		its.attrCol=a;
		if(max==2)
		{
			if(tempSets[0].value<s)
				mid ==1;
			else
				mid ==0;
		}
		
		tempSets.insert(tempSets.begin()+mid, its);
	}
	else
	{
		string temp = tempSets[mid].value;
		for(int x=0; x<temp.size(); x++)
			temp[x]=tolower(temp[x]);
		if(temp==s)
		{
			tempSets[mid].cover++;
			tempSets[mid].rows.push_back(i);
		}
		else if(temp < s)
			insertValue(mid + 1, max, s, i,a);
		else if(temp > s)
			insertValue(min, mid-1, s, i,a);
	}
	
}

void discard()
{
	//tempSets=finalSets;
	int i=0, last=tempSets.size();

	// Go through tempSets and remove itemsets that don't meet minimum coverage
	while(i<last)
	{
		if(tempSets[i].cover < minCover)
		{
			//cout<<"Test"<<tempSets[i].cover<<"\n";
			// Remove element from tempSets
			tempSets.erase(tempSets.begin()+i);
			last--;
		}
		else
			i++;
	}
	//finalSets=tempSets;
}

void postOrder(treeNode* n, int pad)
{
	string s(pad,' ');
	if (n==NULL)
		cout<<endl;

	else 
	{
		cout<<s<<n->attName<<"="<<n->value<<" "<<n->number<<"\n";


		for(int a=0;a<n->next.size();a++)
		{
			postOrder(n->next[a],pad+5);
			//cout<<s<<n->attName<<"="<<n->value<<" "<<n->number<<"\n";
		}
		
	}

}
void makeSets(treeNode * n)
{

}