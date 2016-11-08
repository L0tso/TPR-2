#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
using namespace std;
struct EquipmentCharacters
{
	float weight;
	float price;

} typedef Eq;

istream & operator >> (istream & const is, Eq  & const set)
{
	is >> set.weight >> set.price;
	return is;
}

ostream & operator << (ostream & const os, Eq  & const set)
{
	os << set.weight << "  " << set.price;
	return os;

}

int getIndexByTemperature(int temperature) {
	if (temperature <= -10 )
		return 1;
	else if (temperature <= 0 && temperature > -10)
		return 2;
	else if (temperature <= 10 && temperature >= 1)
		return 3;
	else if (temperature <= 20 && temperature >= 11)
		return 4;
	else if (temperature <= 30 && temperature >= 21)
		return 5;
	else if (temperature > 30)
		return 6;
}

struct Set {
	int equipmentsIndexes[5];
	float weight;
	int setPrice;
};

istream & operator >> (istream & const is, Set  & const set)
{
	for (size_t i = 0; i < 5; i++)
		is >> set.equipmentsIndexes[i];
	is >> set.weight;
	set.setPrice = set.weight * 10;
	return is;
}

ostream & operator << (ostream & const os, Set  & const set)
{
	for (size_t i = 0; i < 5; i++)
		os << set.equipmentsIndexes[i] << "  ";
	os << set.weight << endl;
	return os;

}

float calculating(map<int, Set> sets, map<int, Eq> &equipments, int firstIndex, int secondIndex) {

	float cost = 0.0;
	for (size_t i = 0; i < 5; i++)
	{
		if (sets[firstIndex].equipmentsIndexes[i] != sets[secondIndex].equipmentsIndexes[i] && sets[secondIndex].equipmentsIndexes[i] != -1)
		{
			cost += (equipments[sets[secondIndex].equipmentsIndexes[i]].price + 2);
		}
	}

	return cost;
}

void probabilityByMonth(float matrix[6][12], map<int, Set>& sets ) {
	float stack;
	float min = LONG_MAX;
	int indexMin = 0;
	for (size_t i = 0; i < 6; i++)
	{ 
		stack = 0;

		for (size_t j = 0; j < 12; j++)
		{
			stack += (matrix[i][j] + sets[i+1].setPrice)/12.0;
		}
		cout << stack << endl;
		if (stack < min)
		{
			min = stack;
			indexMin = i+1;
		}
	}
	cout << "Best Strategy : H" << indexMin << endl;
	
}

void probabilityByWinter3x(float matrix[6][12], map<int, Set>& sets) {
	float  stack;
	float min = LONG_MAX;
	int indexMin = 0;
	for (size_t i = 0; i < 6; i++)
	{
		stack = 0;
		for (size_t j = 0; j < 12; j++)
		{
			if (j == 0 || j == 1 || j==11)
			stack += (matrix[i][j] + sets[i + 1].setPrice)/6.0;
			else
			stack += (matrix[i][j] + sets[i + 1].setPrice) / 18.0;
			
		}
		cout << stack << "  "<<endl;
		if (stack < min)
		{
			min = stack;
			indexMin = i + 1;
		}
	}
	cout << "Best Strategy : H" << indexMin << endl;

}

void probabilityByDays(float matrix[6][12], map<int, Set>& sets) {
	float  stack;
	float min = LONG_MAX;
	int indexMin = 0;
	for (size_t i = 0; i < 6; i++)
	{
		stack = 0;
		for (size_t j = 0; j < 12; j++)
		{
			if (j == 1)
				stack += (matrix[i][j] + sets[i + 1].setPrice)  *29.0 / 365.f;
			else if (j == 3|| j == 5|| j == 8|| j == 10)
				stack += (matrix[i][j] + sets[i + 1].setPrice) * 30 / 365.f;
			else
				stack += (matrix[i][j] + sets[i + 1].setPrice) * 31 / 365.f;
		}
		cout << stack << "  " << endl;
		if (stack < min)
		{
			min = stack;
			indexMin = i + 1;
		}
	}
	cout << "Best Strategy : H" << indexMin << endl;

}

void probabilityBySeasons(float matrix[6][12], map<int, Set>& sets) {
	string seasonStr[4] = { "winter","spring","summer","autumn" };
	float stack;
	short season = 0;
	float min = LONG_MAX;
	int indexMin = 0;
	for (int k = 2; k < 12; k += 3)
	{
		float min = LONG_MAX;
		int indexMin = 0;
	
		for (size_t i = 0; i < 6; i++)
		{
			stack = 0;
			
			for (size_t j = 0; j < 12; j++)
			{  
				if (j == k - 1 || j == k - 2 || j == ( (k - 3)<0 ? 11 : k - 3))
					stack += (matrix[i][j] + sets[i + 1].setPrice) / 3.0;
				//cout << " K" << k-1 << " " << k - 3<<endl;
			}
			cout << stack << endl;
			if (stack < min)
			{
				min = stack;
				indexMin = i + 1;
			}
		}
		cout << "Best Strategy of season # "<< seasonStr[season++] <<": H" << indexMin << endl;
	}
}



int main() {
	setlocale(LC_ALL, "");
	map<int, Eq> equipments;
	map<int, Set> sets;
	int temperature[12];
	float matrix[6][12];

	ifstream temperatureInput("temperature.txt");
	ifstream equipmentInput("Equipment.txt");
	ifstream setsInput("sets.txt");
	int i = 0;
	while (!temperatureInput.eof())
		temperatureInput >> temperature[i++];

	i = 1;
	while (!equipmentInput.eof())
		equipmentInput >> equipments[i++];
	
	i = 1;
	while (!setsInput.eof())
		setsInput >> sets[i++];

	for (size_t i = 0; i < 6; i++)
		for (size_t j = 0; j < 12; j++)
		{
			if (getIndexByTemperature(temperature[j]) == i + 1)
				matrix[i][j] = 0;
			else
				matrix[i][j] = calculating(sets, equipments,i+1,getIndexByTemperature(temperature[j]));
		}


	probabilityByMonth(matrix, sets);
	probabilityBySeasons(matrix, sets);
	probabilityByWinter3x(matrix, sets);
	probabilityByDays(matrix, sets);

	for (size_t i = 0; i < 6; i++)
	{
		for (size_t j = 0; j < 12; j++)
		{
			cout <<setw(5)<< matrix[i][j] ;
		}
		cout << endl;
	}


	equipments[2].price = equipments[2].price / 3.0;
	equipments[4].price = equipments[4].price / 3.0;
	equipments[9].price = equipments[9].price / 3.0;
	equipments[15].price = equipments[15].price / 3.0;
	equipments[16].price = equipments[16].price / 3.0;
	
	for (size_t i = 0; i < 6; i++)
	{
		sets[i].setPrice = 0;
		for (size_t j = 0; j < 5; j++)
		{
			sets[i].setPrice += sets[i].equipmentsIndexes[j];
		}

	}

	for (size_t i = 0; i < 6; i++)
		for (size_t j = 0; j < 12; j++)
		{
			if (getIndexByTemperature(temperature[j]) == i + 1)
				matrix[i][j] = 0;
			else
				matrix[i][j] = calculating(sets, equipments, i + 1, getIndexByTemperature(temperature[j]));
		}




	
	probabilityByMonth(matrix, sets);

	/*for (auto i : sets)
		cout << i.first << "  " << i.second << endl;*/

	system("pause");
}