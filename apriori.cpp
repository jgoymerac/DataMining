/*
	Names: Jeff Goymerac and Christine Wang
	CS 5001 - Final Project
													*/


#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "parser.h"

using namespace std;

struct itemSet
{
	vector<string> attName;
	vector<string> atts;		// attribute-value pairs in set
	int cover;					// coverage
	vector<int> rows;			// rows with this itemSet
	float accuracy;			// accuracy (not used until making rules)
	int antecedents;			// number of antecedents

	// default constructor
	itemSet()
	{
		cover = 1;
	}
};


//global variables
vector<itemSet> finalSets, tempSets, tempSets2,
				finalRules, finalRulesTemp, first;
vector<int> numSize;
// function definitions

void initA1();

void initA2();

void fillA();

void makeRules();

void remove(string);

void discard();

void print();

bool sorter(itemSet, itemSet);

void insertAtt(int, int, string, int, int);

int findFirst(string);

int main()
{
	// Parse File
	parser();

	// Get all itemsets of size 1
	initA1();

	/// Get all itemsets of size 2
	initA2();

	// Get rest of itemsets
	fillA();

	// Make association rules
	makeRules();

	// Print results
	print();

	return 0;
}

void initA1()
{
	tempSets.clear();

	//	Add all item sets of size 1 to global array tempSets
	for(int a=0; a<numAtts; a++)
	{
		tempSets2.clear();
		for(int i=0; i<numInsts; i++)	
		{
			string s = data[i][a];

			insertAtt(0, tempSets2.size()-1, s , i, a);	
			//cout<<"2\n\n0";
		}

		tempSets.insert(tempSets.end(),tempSets2.begin(), tempSets2.end());		
	}

	// remove itemSets with coverage less than minimum coverage requirement
	discard();

	// Copy tempSets to finalSets
	finalSets.clear();
	finalSets = tempSets;

	// add number of item sets of size 1
	numSize.push_back(tempSets.size());

	// First is a vector of all the itemsets of size 1 used later
	first=finalSets;
}

// Get all item sets with size 2
void initA2()
{
	tempSets.clear();
	
	// Go through all current itemsets (size 1)
	for(int x=0; x< finalSets.size(); x++)
	{
		vector<string> s1 = finalSets[x].atts;

		string s11 = finalSets[x].attName[0];

		
		// combine attribute-value pairs
		for(int y = x+1; y<finalSets.size(); y++)
		{

			string s22 = finalSets[y].attName[0];

			string s2 = finalSets[y].atts[0];
			
			// If not comparing two items of the same attribute
			if(s11!=s22)
			{
				itemSet its;

				// add all elements of atts from finalSets[x] and finalSets[y] to new itemSet
				its.atts.insert(its.atts.end(), finalSets[x].atts.begin(), finalSets[x].atts.end());
				its.atts.insert(its.atts.end(), finalSets[y].atts.begin(), finalSets[y].atts.end());

				// v to hold intersection of rows for index x and y iterator for set_union
				vector<int> v(max(finalSets[x].rows.size(),finalSets[y].rows.size()));
				vector<int>::iterator it;

				it = set_intersection(finalSets[x].rows.begin(), finalSets[x].rows.end(), finalSets[y].rows.begin(), finalSets[y].rows.end(), v.begin());
				v.resize(it-v.begin());


				// Insert rows for new item set into tempSets and update coverage
				its.rows = v;
				its.cover = v.size();

				its.attName.insert(its.attName.end(), finalSets[x].attName.begin(), finalSets[x].attName.end());
				its.attName.insert(its.attName.end(), finalSets[y].attName.begin(), finalSets[y].attName.end());
				
				// Insert new item set into tempSets
				tempSets.push_back(its);
			}
		}
	}

	// Remove item sets that don't meet minimum coverage requirements
	discard();

	// Add remaining item sets in tempSets to finalSets
	finalSets.reserve(finalSets.size() + tempSets.size());
	finalSets.insert(finalSets.end(), tempSets.begin(), tempSets.end());
	
	// Add number of sets with size 2 to numSize
	numSize.push_back(tempSets.size());
}


// Get all item sets with size greater than 2
void fillA()
{
	// Counter for itemsets with same attribute
	int jump;
	vector<string> s1, s2;
	bool go;
	
	// all itemset sizes until max size
	for(int s=2; s<maxSize; s++)
	{
		tempSets.clear();
		int temp1 = numSize[s-2];						// number of itemsets of size s-2
		int temp2 = numSize[s-1];						// number of itemsets of size s-1
		int temp3 = finalSets.size() - (temp1+temp2);	// used for loops


		for(int x=temp3; x<temp3+temp1; x++)
		{
			s1 = finalSets[x].attName;			


			// combine attribute-value pairs
			for(int y=x+temp1; y<finalSets.size(); y++)
			{
				s2 = finalSets[y].attName;
				
				go = true;
				
				// Prevent combining itemsets with the same attributes
				for(int a=0; a<s1.size(); a++)
				{
					for(int b=0; b<s2.size(); b++)
						if(s2[b].find(s1[a])!=string::npos)
						{
							go = false;
							b=s2.size();
							a=s1.size();
						}
				}
				
				// If not the same attributes, combine
				if(go)
				{
					itemSet its;


					// add all elements of atts from finalSets[x] and finalSets[y] to new itemSet
					its.atts.insert(its.atts.begin(), finalSets[x].atts.begin(), finalSets[x].atts.end());
					its.atts.insert(its.atts.end(), finalSets[y].atts.begin(), finalSets[y].atts.end());
				
					// v to hold intersection of rows for index x and y iterator for set_union
					vector<int> v(max(finalSets[x].rows.size(),finalSets[y].rows.size()));
					vector<int>::iterator it;

					it = set_intersection(finalSets[x].rows.begin(), finalSets[x].rows.end(), finalSets[y].rows.begin(), finalSets[y].rows.end(), v.begin());
					v.resize(it-v.begin());

					// Insert rows for new item set into tempSets and update coverage
					its.rows = v;
					its.cover = v.size();
					its.attName.insert(its.attName.end(), finalSets[x].attName.begin(), finalSets[x].attName.end());
					its.attName.insert(its.attName.end(), finalSets[y].attName.begin(), finalSets[y].attName.end());

					// Insert new item set into tempSets
					tempSets.push_back(its);
				}
			}
		}

		// Remove item sets that don't meet minimum coverage requirements
		discard();

		// Add remaining item sets in tempSets to finalSets
		finalSets.reserve(finalSets.size() + tempSets.size());
		finalSets.insert(finalSets.end(), tempSets.begin(), tempSets.end());
	
		// Add number of sets with size s+1 to numSize
		numSize.push_back(tempSets.size());
		
	}
}

void makeRules()
{
	finalRulesTemp = finalSets;
	int start=0, loc;
	float cov;
	float acc;
	int ante;
	itemSet tempIT;

	// Do for all itemset sizes greater than 1
	for(int s=1; s<maxSize; s++)
	{
		// Used to only check one itemset size at a time
		start += numSize[s-1];
		for(int x=start; x<start+numSize[s]; x++)
		{
			//Check accuracy with each attribute as the antecedent
			for(int y=0; y<finalRulesTemp[x].atts.size(); y++)
			{
				// Attribute being checked
				string attrib = finalRulesTemp[x].atts[y];
				cov=0;

				// Find index of attribute in First to get coverage
				loc=findFirst(attrib);
				cov=finalSets[loc].cover;

				// Calculate accuracy
				acc = (float)finalRulesTemp[x].cover/cov;

				if(isinf(acc))
					break;
				
				int shuffleCount=0;
				// Add to rules if accuracy meets minimum requirement
				while(shuffleCount<s)
				{
					tempIT=finalRulesTemp[x];

					tempIT.accuracy = acc;

					tempIT.antecedents=1;

					// Valid for first attribute as antecedent
					ante = 1;

					// Add rule
					finalRules.push_back(tempIT);

					// Vectors to hold union of rows to check if
					//  antecedents>1 meet minimum accuracy requirements
					int index=1;
					vector<int> v, v2;
					v.reserve(finalSets.size());
					v.insert(v.begin(), finalSets[loc].rows.begin(), finalSets[loc].rows.end());

					// Check if antecedents >1 meet minimum accuracy requirements
					for(int x=1; x<s; x++)
					{
						// Antecedent # updated
						ante++;

						//Union rows
						v2 = v;
						v.clear();
						loc = findFirst(tempIT.atts[index]);
						set_union(v2.begin(), v2.end(), finalSets[loc].rows.begin(), finalSets[loc].rows.end(), v.begin());

						// Check accuracy for antecedent #
						acc = (float)finalRulesTemp[x].cover/(float)v.size();
						
						// If valid, add to antecedent and accuracy vectors for itemset
						if(acc>=minAcc && !isinf(acc))
						{
							if(isinf((float)tempIT.cover))
								acc=-1;
							tempIT.accuracy=acc;
							tempIT.antecedents=ante;
							finalRules.push_back(tempIT);
						}
						else
							x=s;	//If antecedent # doesn't work, stop looking
					};

					//Shuffle
					tempIT.atts.erase(find(tempIT.atts.begin(), tempIT.atts.end(), attrib));

					tempIT.atts.insert(tempIT.atts.begin(), attrib);

					shuffleCount++;
				}
			}
		}
	}

	for(int x=0; x<finalRules.size(); x++)
		if(isinf((float)finalRules[x].accuracy))
		{
			finalRules.erase(finalRules.begin()+x);
			x--;
		}


	// Sort final rules list by descending accuracy 
	sort(finalRules.begin(), finalRules.end(), sorter);
	
}

// Find index of att-val pair in vector First
int findFirst(string s)
{
	for(int x=0; x<first.size(); x++)
		if(first[x].atts[0]==s)
			return x;
}

// Remove itemSets that don't meet minimum coverage
void discard()
{
	int i=0, last=tempSets.size();

	// Go through tempSets and remove itemsets that don't meet minimum coverage
	while(i<last)
	{
		if(tempSets[i].cover < minCover)
		{
			// Remove element from tempSets
			tempSets.erase(tempSets.begin()+i);
			last--;
		}
		else
			i++;
	}
}

//Print results
void print()
{
	cout<< "\tName of File: " << fileName << endl
		<< "\tMinimum Coverage: " << minCover << endl
		<< "\tMaximum ItemSet Size: " << maxSize << endl
		<< "\tMinimum Accuracy Requirements: " << minAcc << endl;

	//If number of best rules is greater than number of rules
	// change size to number of final rules
	

	// Prints All Rules or # Best Rules
	if(numBest==-1)
			cout << "\n\n\tAll ";
	else
	{
		if(numBest>finalRules.size())
		{
			cout<< "\n\n\tThere are less than " << numBest
				<< " rules. Printing all rules:";
			numBest=finalRules.size();
		}
			cout << "\t\n\n" << numBest << " Best ";
	}
			
	cout << "Rules:\n\n";

	// Print numBest rules
	for(int x=0; x<numBest; x++)
	{
		// Print antecedents
		int num=finalRules[x].antecedents;
		cout<< "\tIf ";
		for(int z=0; z<num; z++)
		{		
			cout << finalRules[x].attName[z] << " = " 
				<< finalRules[x].atts[z];

			if(z!=num-1)
				cout << " and ";
		}
		cout<< ", then ";

		//Print Consequents
 		for(int w=num; w<=finalRules[x].atts.size()-num; w++)
		{
			cout << finalRules[x].attName[w] << " = " 
				<<finalRules[x].atts[w] << " ";
			if(w!=finalRules[x].atts.size()-num)
				cout << " and ";
		}

		//Print Coverage and Confidence
		cout<< "\n\t\tCoverage: " << finalRules[x].cover;
		cout << "\n\t\tConfidence: " << finalRules[x].accuracy << "\n\n";
	}
}

void insertAtt(int min, int max, string s, int i, int a)
{
	for(int x=0; x<s.size(); x++)
		s[x]=tolower(s[x]);

	int mid = min + (max-min)/2;

	if((max<min || tempSets2.size()<2))
	{
		itemSet its;
		its.atts.push_back(s);
		its.cover = 1;
		its.rows.push_back(i);
		its.attName.push_back(attributes[a]);

		if(max==-1 || min==mid)
		{

			for(int x=0; x<tempSets2.size(); x++)
			{
				if(s==tempSets2[x].atts[0])
				{
					tempSets2[x].cover++;
					tempSets2[x].rows.push_back(i);
					return;
				}
			}
		}
		else if(max==0)
		{
			if(s==tempSets2[0].atts[0])
			{
				tempSets2[0].cover++;
				tempSets2[0].rows.push_back(i);
				return;
			}
			else if(tempSets2[0].atts[0]<s)
			{
				mid ==1;
			}
			else
				mid ==0;
		}
		tempSets2.insert(tempSets2.begin()+mid, its);
	}
	else
	{
		string temp = tempSets2[mid].atts[0];
		for(int x=0; x<temp.size(); x++)
			temp[x]=tolower(temp[x]);
		if(temp==s)
		{
			tempSets2[mid].cover++;
			tempSets2[mid].rows.push_back(i);
		}
		else if(temp < s)
			insertAtt(mid + 1, max, s, i, a);
		else if(temp > s)
			insertAtt(min, mid-1, s, i, a);
	}
}

bool sorter(itemSet it1, itemSet it2)
{
	return it1.accuracy > it2.accuracy;
}