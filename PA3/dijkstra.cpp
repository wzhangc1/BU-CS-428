#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

const int WIDTH = 11;
const int WIDTH2 = 16;

int main(int argc, char *argv[])
{
	char source = argv[3][0];
	std::ifstream fin;
	std::ofstream fout;
	std::map<char,int> nodes;

	//*************************************************************************
	//READ NODES
	//*************************************************************************
	char tempChar;

	//Open Input file
	fin.open(argv[1]);

	//Input node (flag)
	fin >> tempChar;

	//While input file is open
	while (fin)
	{
		//Determine if the node is not stored
		if (nodes.count(tempChar) == 0)
		{
			//Assign node
			nodes[tempChar] = nodes.size();
		}

		//Input node
		fin >> tempChar;

		//Determine if the node is not stored
		if (nodes.count(tempChar) == 0)
		{
			//Store node
			nodes[tempChar] = nodes.size();
		}
		
		//Input node (flag)
		fin.ignore(256, '\n');
		fin >> tempChar;
	}

	//*************************************************************************
	//INITIALIZE WEIGHTS
	//*************************************************************************
	int weights[nodes.size()][nodes.size()];

	//For each weights row
	for (int i = 0; i < sizeof(weights)/sizeof(weights[0]); i++)
	{
		//For each weights column
		for (int j = 0; j < sizeof(weights[0])/sizeof(weights[0][0]); j++)
		{
			//Determine if row is equal to column
			if (i == j)
			{
				//Assign weight
				weights[i][j] = 0;
			}
			else
			{
				//Assign Weight
				weights[i][j] = INT32_MAX;
			}
		}
	}

	//*************************************************************************
	//READ WEIGHTS
	//*************************************************************************
	char nodeFrom;

	//Clear input file
	fin.clear();
	fin.seekg(0, std::ios::beg);

	//Input node (flag)
	fin >> nodeFrom;

	//While input file is open
	while (fin)
	{
		char nodeTo;
		int weight;

		//Input node and weight
		fin >> nodeTo;
		fin >> weight;
		
		//Assign weight
		weights[nodes[nodeFrom]][nodes[nodeTo]] = weight;
		weights[nodes[nodeTo]][nodes[nodeFrom]] = weight;

		//Input node (flag)
		fin.ignore(256, '\n');
		fin >> nodeFrom;
	}

	//CLose input file
	fin.close();

	//*************************************************************************
	//PRINT HEADERS
	//*************************************************************************
	//Open output file
	fout.open(argv[2]);

	//Output header
	fout << std::setw(WIDTH2) << "Nprime";

	//For each node
	for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
	{
		std::stringstream ssHeader;

		//Create string for header
		ssHeader << "D(" << itr->first << "),p(" << itr->first << ')';

		//Output header
		fout << std::setw(WIDTH) << ssHeader.str();
	}

	fout << '\n' << std::setw(WIDTH2) << "------";

	for (int i = 0; i < nodes.size(); i++)
	{
		fout << std::setw(WIDTH) << "--------";
	}

	fout << '\n';

	//*************************************************************************
	//PRINT DATA
	//*************************************************************************
	char path[nodes.size()];
	std::set<char> nPrime;
	std::set<char> remainingNodes;

	//For each node
	for (int i = 0; i < nodes.size(); i++)
	{
		//Assign path
		path[i] = source;
	}

	//Update nPrime
	nPrime.insert(source);

	//For each node
	for(auto itr = nodes.begin(); itr != nodes.end(); itr++)
	{
		//Assign remaining nodes
		remainingNodes.insert(itr->first);
	}

	//Update remaining nodes
	remainingNodes.erase(source);

	//While there are remaining nodes
	while(remainingNodes.size() > 0)
	{
		char minNode;
		int min = INT32_MAX;
		std::stringstream ssNPrime;

		//For each Nprime
		for (auto itr = nPrime.begin(); itr != nPrime.end(); itr++)
		{
			//Create string for Nprime
			ssNPrime << ' ' << (*itr);
		}
		
		//Output Nprime
		fout << std::setw(WIDTH2) << ssNPrime.str();

		//For each node
		for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
		{
			//Determine if remainingNodes contains node
			if (remainingNodes.count(itr->first) == 0)
			{
				//Output delimiter
				fout << std::setw(WIDTH) << '-';
			}
			else
			{
				std::stringstream ssWeight;

				//Determine if weight is infinite
				if (weights[nodes[source]][itr->second] == INT32_MAX)
				{
					//Create string for weight
					ssWeight << "infty";
				}
				else
				{
					//Create string for weight
					ssWeight << weights[nodes[source]][itr->second] << ',' << path[itr->second];
				}

				//Output weight
				fout << std::setw(WIDTH) << ssWeight.str();
			}
		}
		
		fout << '\n';

		//---------------------------------------------------------------------
		//Calculations
		//---------------------------------------------------------------------
		//For each remaining node
		for (auto itr = remainingNodes.begin(); itr != remainingNodes.end(); itr++)
		{
			//Determine the minimum weight
			if (weights[nodes[source]][nodes[*itr]] < min)
			{
				//Assign minimum weight
				min = weights[nodes[source]][nodes[*itr]];

				//Assign minimum node
				minNode = *itr;
			}
		}

		//Determine is a node was found
		if (min == INT32_MAX)
		{
			//Clear remaining nodes
			remainingNodes.clear();
		}
		else
		{
			//Update Nprime
			nPrime.insert(minNode);

			//Update remaining nodes
			remainingNodes.erase(minNode);

			//For each remaining node
			for (auto itr = remainingNodes.begin(); itr != remainingNodes.end(); itr++)
			{
				//Determine if weight is infinite
				if (weights[nodes[minNode]][nodes[*itr]] != INT32_MAX)
				{
					//Determine if new weight is less than current weight
					if (weights[nodes[source]][nodes[minNode]] + weights[nodes[minNode]][nodes[*itr]] < weights[nodes[source]][nodes[*itr]])
					{
						//Update
						weights[nodes[source]][nodes[*itr]] = weights[nodes[source]][nodes[minNode]] + weights[nodes[minNode]][nodes[*itr]];

						//Update path
						path[nodes[*itr]] = minNode;
					}
				}
			}
		}
	}

	//*************************************************************************
	//PRINT LAST ROW
	//*************************************************************************
	std::stringstream ssNPrime2;

	//For each node
	for (auto itr = nPrime.begin(); itr != nPrime.end(); itr++)
	{
		//Create string for Nprime
		ssNPrime2 << ' ' << (*itr);
	}
	
	//Output Nprime
	fout << std::setw(WIDTH2) << ssNPrime2.str();

	//For each node
	for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
	{
		//Determine if remainingNodes contains node
		if (remainingNodes.count(itr->first) == 0)
		{
			//Output delimiter
			fout << std::setw(WIDTH) << '-';
		}
		else
		{
			std::stringstream ssWeight;

			//Determine if weight is infinite
			if (weights[nodes[source]][itr->second] == INT32_MAX)
			{
				//Create string for weight
				ssWeight << "infty";
			}
			else
			{
				//Create string for weight
				ssWeight << weights[nodes[source]][itr->second] << ',' << path[itr->second];
			}

			//Output weight
			fout << std::setw(WIDTH) << ssWeight.str();
		}
	}		
				
	fout << '\n';

	//Close output file
	fout.close();

	return 0;
}