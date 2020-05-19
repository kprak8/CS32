//
//  eval.cpp
//  Infix
//
//  Created by Komal Prakash on 2/4/20.
//  Copyright © 2020 Komal Prakash. All rights reserved.
//
#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool isOp(char op) {return (op=='+'||op=='-'||op=='*'||op=='/');} //check if it is an operator

bool isLower(char letter){return (letter<='z'&&letter>='a');} //check if it a lowercase letter

string removeSpaces(string infix){ //removes spaces from string
    string noSpaces="";
    for (int i=0; i<infix.size(); i++){
        if (infix[i]!=' '){
            noSpaces+=infix[i];
        }
    }
    return noSpaces;
}

int precedence(char ch){ //determines precedence of operators
    int prec=0;
    if (ch=='+' ||ch=='-') prec=1;
    else if(ch=='*'||ch=='/') prec=2;
    return prec;
}

bool isValid(string infix){ //checks if infix is valid
    stack<char> parens;
    char prev=NULL;
    if (infix=="") return false;
    for (int i=0; i<infix.size();i++){
        if (i==0){
            if (infix[i]=='(') parens.push(infix[i]);
            if (infix[i]!='('&&!isLower(infix[i])) return false;
        }
        else if (isLower(infix[i])){
            if (prev!='('&& !isOp(prev)) return false;
        }
        else if (infix[i]=='('){
            parens.push(infix[i]);
            if (prev!='('&&!isOp(prev)) return false;
        }
        else if (infix[i]==')'){
            if (parens.empty()) return false;
            else parens.pop();
            if (prev!=')'&&!isLower(prev)) return false;
        }
        else if (isOp(infix[i])){
            if (prev!=')'&&!isLower(prev)) return false;
        }
        else return false;
        prev=infix[i];
    }
    if (prev!=')'&& !isLower(prev)) return false;
    return parens.empty();
}

bool convert(string infix, string& postfix){ //checks if valid, and then converts
    string noSpaces=removeSpaces(infix);
    if (!isValid(noSpaces)) return false;
    postfix="";
    stack<char> myOperators;
    for (int i=0; i<noSpaces.size(); i++){
        if(isLower(noSpaces[i])) postfix+=noSpaces[i];
        switch(noSpaces[i]){
            case '(':
                myOperators.push(noSpaces[i]);
                break;
            case ')':
                while (myOperators.top()!='('){
                    postfix+=myOperators.top();
                    myOperators.pop();
                }
                myOperators.pop();
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                while (!myOperators.empty() && myOperators.top()!='('&& precedence(noSpaces[i])<=precedence(myOperators.top())){
                    postfix+=myOperators.top();
                    myOperators.pop();
                }
                myOperators.push(noSpaces[i]);
                break;
        }
    }
    while (!myOperators.empty()){
        postfix+=myOperators.top();
        myOperators.pop();
    }
    return true;
}

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    stack<char> operand;
    int operand1,operand2;
    int val;
    
    if (!convert(infix,postfix)){
        return 1;
    }
    else{
        for (int i=0; i<infix.size();i++){
            if (isLower(infix[i])){
                if (!values.contains(infix[i])) return 2;
            }
        }
        for (int i=0; i<postfix.size();i++){
            if (isLower(postfix[i])){
                values.get(postfix[i],val);
                operand.push(val);
            }
            else{
                operand2=operand.top();
                operand.pop();
                operand1=operand.top();
                operand.pop();
                if (postfix[i]=='+') operand.push(operand1+operand2);
                else if (postfix[i]=='-') operand.push(operand1-operand2);
                else if (postfix[i]=='/'){
                    if (operand2==0){
                        return 3;
                    }
                    operand.push(operand1/operand2);
                }
                else if (postfix[i]=='*') operand.push(operand1*operand2);
            }
        }
        result = operand.top();
    }
    return 0;
}
                    
