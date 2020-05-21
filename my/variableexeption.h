#ifndef VARIABLEEXEPTION_H
#define VARIABLEEXEPTION_H

#include "alien/scanner.h"
#include<vector>
#include <map>
#include <iostream>
using namespace std;

class VariableExeption
{
public:
    VariableExeption();
    void setExpression(vector<map<LexClass,string>> _expression);

    bool find_name_variable(vector<map<LexClass, string> > find);

    vector<map<LexClass,string>> getExpression();
    map<LexClass,string> getResult();
private:
       void Work(int position);


       map<LexClass,string> Associativity(map<LexClass,string> leftOperend,map<LexClass,string> rightOperand,map<LexClass,string> operation);

       void findParentheses();
       vector<vector<vector<string>>> findPriority(int index);

       void zapolnenieOperationsPriorityForward(vector<string> operands, int priority, int forward);
       bool right();


private:
    vector<map<LexClass,string>> expression;
    map<LexClass,string> result;


    //
     map<int,vector<vector<string>>> mapOperationsAndPriority;

    vector<vector<map<LexClass,string>>*> virtualExpression;
    vector<map<LexClass,string>> virtualExpression2;
   // int position;

};

#endif // VARIABLEEXEPTION_H
