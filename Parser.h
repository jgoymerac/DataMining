#ifndef PARSER_H
#define PARSER_H
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


using namespace std;

int minCover,				//minimum Coverage
	maxSize,			//Max size of item sets to consider
	numBest,				//Number of "Best" rules to report
	numAtts =0,
	numInsts=0;

double minAcc; 					//Minimum Accuracy
	
string line, relation, fileName;		//file Name to use
//string attributes[50][50];
string data[5000][500];
string attributes[50];	//arrays for the attributes and data
ifstream myFile;
void getAttributes();
void getData();
string toLower(string strToConvert);



void parser()
{
	cout<<"\tEnter File name: ";
	cin>>fileName;
	
	cout<<"\n\t"<<"Enter the Minimum coverage requirements: ";
	cin>>minCover;
	
	cout<<"\n\t"<<"Enter the size of item sets to consider: ";
	cin>>maxSize;

	cout<<"\n\t"<<"Enter the minimum accuracy requirements: ";
	cin>>minAcc;

	cout<<"\n\t"<<"Enter the number of \"best\" rules to report (-1 for all): ";
	cin>>numBest;

	
	myFile.open(fileName.c_str());
		while(!myFile.eof()&& myFile)
		{
			getline (myFile,line);
			if (line[0]=='%'|| line=="")
			{
				//do nothing
			}
			else if (line[0]=='@')
			{
				getAttributes();
				//numInsts++;
			}
			else
			{
				getData();
				numInsts++;
			}
		}
		myFile.close();
		
	return ;
}


void getAttributes()				//get @attribute's and possible values or get @relation
{


	if(toLower(line).find ("@relation")==0) 
	{
		//line=line.substr(line.find_first_of(" "));
		relation = line.substr(line.find_first_of(" "));
		//cout<<relation<<"\n";
		//split string after @relation
	}
	else if(toLower(line).find("@attribute")==0)
	{
		line=line.substr(line.find_first_of(" \t")+1);
		string atName=line.substr(0, line.find_first_of (" \t"));
		
		attributes[numAtts]= atName;
		string value = line.substr(line.find_first_not_of(" \t"));
		numAtts++;
		
	}
	return;
}


void getData()
{
	//split line on , and put into data[][] numAtts++ after each entry
	line=line.substr(0, line.find_first_of("\r\t")-1);
	for (int i=0; i<numAtts;i++)
	{
		string tempStr=line.substr(line.find_first_not_of(" \t"), line.find_first_of( ",\n\r"));
		
		data[numInsts][i] = tempStr;
		line=line.substr(line.find_first_of(",")+1);

	}
	return;
}

string toLower(string strToConvert)
{
	//change each element of the string to lower case
	for(int i = 0; i < strToConvert.length(); i++)
		strToConvert[i] = tolower(strToConvert[i]);
	return strToConvert;//return the converted string
}


void display()													//display function for both programs	
{
	cout<<"File used "<<fileName<<"\n";
	cout<<"Minimum Coverage requirements specified: "<<minCover<<"\n";
	cout<<"Size of the item sets considered: "<<maxSize<<"\n";
	cout<<"Minimum Accuracy specified: "<<minAcc<<"\n";
	cout<<"Number of \"Best\" rules to report"<<numBest<<"\n";

	//Either add the output here of do it inside of each program.



	return ;
}

#endif
