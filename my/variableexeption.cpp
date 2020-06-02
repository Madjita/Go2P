#include "variableexeption.h"

#include "my/table.h"

#include <stack>

VariableExeption::VariableExeption()
{

    vector<vector<string>> forwardOperends;
    forwardOperends.push_back({}); // без навправления
    forwardOperends.push_back({}); // левый
    forwardOperends.push_back({}); // правый
    vector<string> operands = {"::","[","]","(",")",".","->"};
    zapolnenieOperationsPriorityForward(operands,16,2);
    operands = {"++","--"};
    zapolnenieOperationsPriorityForward(operands,15,1);
    operands = {"++","--","sizeof","sizeof()","^","!","-","+","&","*","new","new []","delete","delete []"};
    zapolnenieOperationsPriorityForward(operands,14,1);
    operands = {"(type)","casting"};
    zapolnenieOperationsPriorityForward(operands,14,0);
    operands = {"*","/","%"};
    zapolnenieOperationsPriorityForward(operands,13,2);
    operands = {"+","-"};
    zapolnenieOperationsPriorityForward(operands,12,2);
    operands = {">>","<<"};
    zapolnenieOperationsPriorityForward(operands,11,2);
    operands = {"<","<=",">",">="};
    zapolnenieOperationsPriorityForward(operands,10,2);
    operands = {"==","!="};
    zapolnenieOperationsPriorityForward(operands,9,2);
    operands = {"&"};
    zapolnenieOperationsPriorityForward(operands,8,2);
    operands = {"^"};
    zapolnenieOperationsPriorityForward(operands,7,2);
    operands = {"|"};
    zapolnenieOperationsPriorityForward(operands,6,2);
    operands = {"&&"};
    zapolnenieOperationsPriorityForward(operands,5,2);
    operands = {"||"};
    zapolnenieOperationsPriorityForward(operands,4,2);
    operands = {"?:"};
    zapolnenieOperationsPriorityForward(operands,3,1);
    operands = {"=","*=","/=","%=","+=","-=","<<=",">>=","&=","^=","|=","throw"};
    zapolnenieOperationsPriorityForward(operands,2,1);
    operands = {","};
    zapolnenieOperationsPriorityForward(operands,1,2);

}

void VariableExeption::zapolnenieOperationsPriorityForward(vector<string> operands, int priority, int forward)
{
    auto search =mapOperationsAndPriority.find(priority);

    vector<vector<string>> forwardOperends;
    if(search != mapOperationsAndPriority.end())
    {
        search->second[forward] = operands;
    }
    else {

        forwardOperends.push_back({}); // без навправления
        forwardOperends.push_back({}); // левый
        forwardOperends.push_back({}); // правый

        forwardOperends[forward] = operands;
        mapOperationsAndPriority.insert(make_pair(priority,forwardOperends));
    }

}

void VariableExeption::setExpression(vector<map<LexClass,string>> _expression)
{
    expression = _expression;

    virtualExpression2 = expression;
    int position = -1;

    if(virtualExpression2.size() > 1)
    {
        Work(position);
    }
}

vector<map<LexClass, string> > VariableExeption::getExpression()
{
    return  expression;
}


void VariableExeption::Work(int position)
{

_begin:
    position++;

    if(virtualExpression2.size() == position)
    {
        this->result = virtualExpression2[0];
        return;
    }

    auto lex = virtualExpression2[position].rbegin()->first;
    auto data = virtualExpression2[position].rbegin()->second;

    switch (lex)
    {
    case lcLCircle:
        Work(position);
        break;
    case lcRCircle:
        Work(position);
        break;
    case lcPlus:
    {
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();


        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) + atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }

        break;
    }
    case lcMinus:
    {
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();
        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) - atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }



        }
        break;

    }
    case lcStar:
    {
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();
        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) * atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }
        break;

    }
    case lcSlash:
    {
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();
        string  result = "undef";
        result = to_string(atof(leftOperendCopy) / atof(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }
        break;

    }
    case lcSmaller:{
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();


        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) < atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }

        break;
    }
    case lcLarger:{
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();


        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) > atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }
        break;
    }
    case lcSmallerEQ:{
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();


        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) <= atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }
        break;
    }
    case lcLargerEQ:{
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();


        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) >= atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }
         break;
    }
    case lcEQ:{
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();


        string  result = "undef";
        result = to_string(atoi(leftOperendCopy) == atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }
         break;
    }
    case lcMod:{
        auto leftOperendCopy = virtualExpression2[position-1].begin()->second.data();

        if(virtualExpression2[position+1].begin()->first == lcLCircle)
        {
            Work(position+1);
        }
        auto rightOperandCopy = virtualExpression2[position+1].begin()->second.data();


        string  result = "undef";

        //Вставка чтоб не было ошибки удалить данный класс по завершению трех-ажресного кода
        if(*rightOperandCopy == '-')
        {
            result = "0";
        }
        else
        {
            result = to_string(atoi(leftOperendCopy) % atoi(rightOperandCopy));
        }


        //result = to_string(atoi(leftOperendCopy) % atoi(rightOperandCopy));
        virtualExpression2.erase(virtualExpression2.begin()+position-1,virtualExpression2.begin()+position+2);

        map<LexClass,string> agr;
        agr.insert(make_pair(lcIntNum,result));
        position -= 1;
        virtualExpression2.insert(virtualExpression2.begin()+position,agr);


        if(position > 0)
        {
            if(virtualExpression2[position-1].begin()->first == lcLCircle && virtualExpression2[position+1].begin()->first == lcRCircle)
            {
                position -=1;
                virtualExpression2.erase(virtualExpression2.begin()+position);
                virtualExpression2.erase(virtualExpression2.begin()+position+1);
                return;
            }
        }
         break;
    }
    case lcEnter:
        this->result = virtualExpression2[0];
        return;

    }

    goto _begin;

}

map<LexClass, string> VariableExeption::Associativity(map<LexClass, string> leftOperend, map<LexClass, string> rightOperand, map<LexClass, string> operation)
{
    map<LexClass,string> agr;

    auto operationCopy = operation.begin()->first;
    auto leftOperendCopy = leftOperend.begin()->first;
    auto rightOperandCopy = rightOperand.begin()->first;

    string  result = "undef";

    switch (operationCopy)
    {
    case lcPlus:
    {
        result = to_string(atoi(leftOperend.begin()->second.data()) + atoi(rightOperand.begin()->second.data()));

        agr.insert(make_pair(lcIntNum,result));
    }
    }

    cout << "result = " <<  result << endl;

    return agr;
}

void VariableExeption::findParentheses()
{
    map<LexClass,string> agr;

    vector<map<LexClass,string>>* item;



    for(unsigned int i=0; i < expression.size();i++)
    {
        auto parentheses = expression[i].begin()->first;

        if(parentheses == lcLCircle)
        {
            virtualExpression.push_back(new vector<map<LexClass,string>>);
            item = virtualExpression.at(virtualExpression.size()-1);
            continue;
        }

        if(parentheses == lcRCircle)
        {
            virtualExpression.push_back(new vector<map<LexClass,string>>);
            item = virtualExpression.at(virtualExpression.size()-1);
            continue;
        }

        if(virtualExpression.size() < 0)
        {
            virtualExpression.push_back(new vector<map<LexClass,string>>);
            item = virtualExpression.at(virtualExpression.size()-1);
        }

        item->push_back(expression[i]);
    }

}

bool VariableExeption::right()
{
    int position = 0;

    for(int i=0; i < mapOperationsAndPriority.size();i++)
    {
        auto search = mapOperationsAndPriority[i];

        if(search.size()  == 0)
        {
            continue;
        }

        if(search[2].size() > 0)
        {
            for(auto item: search[2])
            {
                if ( item == expression[position].rbegin()->second)
                {
                    return true;
                }
            }
        }
    }


    return false;

}

map<LexClass, string> VariableExeption::getResult()
{
    return result;
}


