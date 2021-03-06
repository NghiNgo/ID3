#include <bits/stdc++.h>

using namespace std;

class DATASET
{
public:
	vector<string> attributes;
	vector<vector<string>>example;
	void readDATASET(string fileName);
	void OutputData();
	vector<string> getOutput();//Lay toan bo gia tri output
	int getSizeAtt();
	int getSizeLineExp();

	vector<string> GetAttributesValues(int Index);
	vector< vector<string>> GetAttributeValuesScores(string The_Attribute);//Dem tan so gia tri cua thuoc tinh
	DATASET doNewDATASET(DATASET cur, string The_Attibute, string values);
};

string delDot(string& cur);
vector<string> getUniOutput(vector<string>out);//Lay gia tri output duy nhat
string getFreOutput(vector<string> Scores);//Lay tan so cua output

double ComputeEntropy(vector<string> Scores); // Tinh entropy
double ComputeAE(DATASET data, string The_Attribute);//Tinh AE

class TREE
{
public:
	string Node;
	string Branch;
	vector<TREE*> child;
	TREE();
	TREE* buildTree(TREE* tree, DATASET dataS);
	void writeFile(ofstream& ofs, TREE* tree, int size_tree);
	void ID3(string fOutName, DATASET dataS, TREE*& root);
	void Prediction(string fInTestName, string fOutTestName, TREE* tree);
};
struct Data_Test
{
	string attribute;
	string result;
};
class TEST
{
public:
	vector<Data_Test> test;
	string doFind(string cur);
};

void DATASET::readDATASET(string fileName)
{
	ifstream ifs;
	ifs.open(fileName);
	if (!ifs.is_open()) {
		cout << "Can't open File: " + fileName << endl;
		exit(EXIT_FAILURE);
	}
	//attributes
	string att;
	getline(ifs, att);
	att = delDot(att);
	istringstream ist(att);
	string tmp;
	while (ist >> tmp) {
		attributes.push_back(tmp);
	}
	//examples
	vector<string>exp;
	while (!ifs.eof())
	{
		getline(ifs, tmp);
		tmp = delDot(tmp);
		istringstream ist(tmp);
		string att;
		while (ist >> att) {
			if (att.length())
				exp.push_back(att);

		}
		if (tmp.length()) {
			example.push_back(exp);
			exp.erase(exp.begin(), exp.end());
		}
	}
	ifs.close();
}

void DATASET::OutputData()
{
	for (int i = 0; i < attributes.size(); i++)
		cout << attributes[i] << " ";
	cout << endl;
	for (int i = 0; i < example.size(); i++) {
		for (int j = 0; j < example[i].size(); j++)
			cout << example[i][j] << " ";
		cout << endl;
	}
}
vector<string> DATASET::getOutput()
{
	vector<string>tmp;
	for (int i = 0; i < example.size(); i++)
		tmp.push_back(example[i][attributes.size() - 1]);
	return tmp;
}
int DATASET::getSizeAtt()
{
	return attributes.size();
}
int DATASET::getSizeLineExp()
{
	return example.size();
}
vector<string>  DATASET::GetAttributesValues(int Index)
{
	vector<string> Attribute_Values;
	for (int i = 0; i < example.size(); i++)
	{
		Attribute_Values.push_back(example[i][Index]);
	}
	sort(Attribute_Values.begin(), Attribute_Values.end());
	Attribute_Values.erase(unique(Attribute_Values.begin(), Attribute_Values.end()), Attribute_Values.end());
	return Attribute_Values;
}

vector< vector<string>>  DATASET::GetAttributeValuesScores(string The_Attribute)//Dem tan so gia tri cua thuoc tinh
{
	int i, k;
	//Lay chi so cua thuoc tinh
	int Index = 0;
	for (int i = 0; i < attributes.size() - 1; i++)
	{
		if (attributes[i].compare(The_Attribute) == 0) {
			Index = i;
			break;
		}
	}
	vector<vector<string>> Attribute_Values_Scores;
	vector<string> Attribute_Values = GetAttributesValues(Index);
	vector<string> Row;
	for (k = 0; k < Attribute_Values.size(); k++) {
		for (i = 0; i < getSizeLineExp(); i++) {
			if (example[i][Index].compare(Attribute_Values[k]) == 0)
				Row.push_back(example[i][getSizeAtt() - 1]);
		}
		Attribute_Values_Scores.push_back(Row);
		Row.erase(Row.begin(), Row.end());
	}
	return Attribute_Values_Scores;
}
DATASET DATASET::doNewDATASET(DATASET cur, string The_Attibute, string values)
{
	attributes.erase(attributes.begin(), attributes.end());
	example.erase(example.begin(), example.end());
	int index = 0;
	//Xoa thuoc tinh da chon
	for (int i = 0; i < cur.getSizeAtt(); i++) {
		if (cur.attributes[i].compare(The_Attibute) != 0)
			attributes.push_back(cur.attributes[i]);
		else
			index = i;
	}
	//Them cac gia tri cua thuoc tinh moi
	vector<string> row;
	for (int i = 0; i < cur.getSizeLineExp(); i++) {
		for (int j = 0; j < cur.getSizeAtt(); j++) {
			if (cur.attributes[j].compare(The_Attibute) != 0 & cur.example[i][index].compare(values) == 0)
				row.push_back(cur.example[i][j]);
		}
		if (row.size() != 0) {
			example.push_back(row);
			row.erase(row.begin(), row.end());
		}
	}
	return *this;
}
string delDot(string& cur) {
	for (int i = 0; i < cur.size(); ++i) {
		if (cur[i] == ',')
			cur[i] = ' ';
	}
	return cur;
}
vector<string> getUniOutput(vector<string> out)
{
	sort(out.begin(), out.end());
	out.erase(unique(out.begin(), out.end()), out.end());
	return out;
}
string getFreOutput(vector<string> Scores)
{
	vector<string> uniOutput = getUniOutput(Scores);
	vector<int> Count(Scores.size(), 0);
	for (int i = 0; i < Scores.size(); i++) {
		for (int k = 0; k < uniOutput.size(); k++) {
			if (Scores[i].compare(uniOutput[k]) == 0)
				Count[k] = Count[k] + 1;
		}
	}
	int index = 0;
	for (int k = 0; k < uniOutput.size(); k++) {
		if (Count[k] > index)
			index = Count[k];
	}
	return uniOutput[index];
}
double ComputeEntropy(vector<string> Scores) // Tinh entropy
{
	vector<string> uniOutput = getUniOutput(Scores);
	if (uniOutput.size() == 0)
		return 0.0;
	else
	{
		double entropy = 0.0;
		int i, j;
		vector<int> Count(uniOutput.size(), 0);

		for (i = 0; i < Scores.size(); i++)
		{
			for (j = 0; j < uniOutput.size(); j++)
			{
				if (Scores[i].compare(uniOutput[j]) == 0)
				{
					Count[j] = Count[j] + 1;
				}
			}
		}

		double tmp_Entr;
		double tmp_P;
		for (j = 0; j < uniOutput.size(); j++) {
			if (Count[j] == 0)
				tmp_Entr = 0.0;
			else {
				tmp_P = (double)Count[j] / (double)(Scores.size());
				tmp_Entr = -tmp_P * log(tmp_P) / log(2.);
			}
			entropy = entropy + tmp_Entr;
		}
		return entropy;
	}
}
double ComputeAE(DATASET data, string The_Attribute)
{
	int Index = 0;
	for (int i = 0; i < data.attributes.size() - 1; i++) {
		if (data.attributes[i].compare(The_Attribute) == 0) {
			Index = i;
			break;
		}
	}
	double Original_Entropy, Gained_Entropy = 0.0;
	vector<string> Scores = data.getOutput();
	vector< vector<string> > Values_Scores = data.GetAttributeValuesScores(The_Attribute);
	Original_Entropy = ComputeEntropy(Scores);
	vector<string> Attribute_Values = data.GetAttributesValues(Index);

	double After_Entropy = 0.;
	double tmp_Entr;
	vector<string> Temp_Scores;
	int i, j;
	for (i = 0; i < Attribute_Values.size(); i++)
	{
		Temp_Scores = Values_Scores[i];
		tmp_Entr = ComputeEntropy(Temp_Scores) * (double)Temp_Scores.size() / (double)Scores.size();
		After_Entropy = After_Entropy + tmp_Entr;
	}
	Gained_Entropy = Original_Entropy - After_Entropy;
	return Gained_Entropy;
}

TREE::TREE()
{
	Node = "";
	Branch = "";
}

TREE* TREE::buildTree(TREE* tree, DATASET dataS)
{
	if (tree == NULL)
		tree = new TREE();
	//Lay ket qua duy nhat
	vector<string> out;
	out = getUniOutput(dataS.getOutput());
	//case: gia tri output co 2 gia tri
	if (out.size() == 1) {
		tree->Node = out[0];
		return tree;
	}
	//case: dataset co so thuoc tinh la 1
	if (dataS.getSizeAtt() == 1) {
		string FreqOutput = getFreOutput(dataS.getOutput());
		tree->Node = FreqOutput;
		return tree;
	}
	//Tinh AE
	double max = 0.0, tmp = 0.0;
	string max_att;
	int index = 0;
	for (int i = 0; i < dataS.attributes.size() - 1; i++) {
		tmp = ComputeAE(dataS, dataS.attributes[i]);
		if (tmp - max > 1.e-8) {
			max = tmp;
			max_att = dataS.attributes[i];
			index = i;
		}
	}
	//Chon node dau tien(lon nhat)
	tree->Node = max_att;
	//Lay gia tri thuoc tinh cua max_att,tao datas moi
	vector<string> values = dataS.GetAttributesValues(index);
	DATASET newData;
	for (int i = 0; i < values.size(); i++) {
		newData = newData.doNewDATASET(dataS, max_att, values[i]);
		TREE* newT = new TREE();
		newT->Branch = values[i];
		if (newData.getSizeAtt() == 1)
			newT->Node = getFreOutput(newData.getOutput());
		else
			buildTree(newT, newData);
		tree->child.push_back(newT);
	}
	return tree;
}

void TREE::writeFile(ofstream& ofs, TREE* tree, int size_tree)
{
	for (int i = 0; i < size_tree; i++) {
		if (i == size_tree - 1) {
			cout << "\t";
			ofs << "\t";
		}
		else {
			cout << "\t";
			ofs << "\t";
		}
	}
	if (tree->Branch.compare("") != 0) {
		cout << " - " << tree->Branch << endl;;
		ofs << " - " << tree->Branch << endl;;

		for (int i = 0; i < size_tree + 1; i++) {
			if (i == size_tree) {
				cout << "\t+ ";
				ofs << "\t+ ";
			}
			else {
				cout << " \t";
				ofs << " \t";
			}
		}
	}
	ofs << tree->Node << endl;
	cout << tree->Node << endl;
	for (int i = 0; i < tree->child.size(); i++)
		writeFile(ofs, tree->child[i], size_tree + 1);
}

void TREE::ID3(string fOutName, DATASET dataS, TREE*& root)
{
	root = root->buildTree(root, dataS);
	ofstream ofs(fOutName);
	writeFile(ofs, root, -1);
	ofs.close();
}

void TREE::Prediction(string fInTestName, string fOutTestName, TREE* tree)
{
	Data_Test data_t;
	TEST t;
	ifstream ifs(fInTestName);
	ofstream ofs(fOutTestName);
	vector <string> atts;
	string att, tmp;
	getline(ifs, tmp);
	istringstream iss(tmp);
	while (getline(iss, att, ','))
		atts.push_back(att);
	TREE* cur = tree;
	while (!ifs.eof()) {
		tree = cur;
		string line;
		getline(ifs, line);
		istringstream iss(line);
		int i = 0;
		while (getline(iss, tmp, ',')) {
			data_t.attribute = atts[i++];
			data_t.result = tmp;
			t.test.push_back(data_t);
		}
		while (tree->child.size() != 0) {
			string r = t.doFind(tree->Node);
			for (int i = 0; i < tree->child.size(); i++) {
				if (tree->child[i]->Branch == r)
					tree = tree->child[i];
			}
		}
		ofs << tree->Node << endl;
	}
	ifs.close();
	ofs.close();
}

string TEST::doFind(string cur)
{
	for (int i = 0; i < test.size(); i++) {
		if (test[i].attribute == cur)
			return test[i].result;
	}
}

void main()
{
	DATASET datas;
	datas.readDATASET("D:\\Code\\ID3\\train2.txt");
	//datas.OutputData();
	//cout<<ComputeAE(datas, "outlook");
	TREE* root = NULL;
	root->ID3("D:\\Code\\ID3\\tree.txt", datas, root);
	//Test
	root->Prediction("D:\\Code\\ID3\\test2.txt", "D:\\Code\\ID3\\prediction.txt", root);

	system("pause");
}