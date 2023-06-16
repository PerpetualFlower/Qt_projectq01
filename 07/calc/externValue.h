#ifndef EXTERNVALUE_H
#define EXTERNVALUE_H

#include<string>

#define wenfaLength 9
/*
cout << "归纳算术表达式文法如下：\n" << endl;
cout << "0  E’-> E " << endl;
cout << "1  E  -> E + T" << endl;
cout << "2  E  -> E - T" << endl;
cout << "3  E  -> T " << endl;
cout << "4  T  -> T * F" << endl;
cout << "5  T  -> T / F" << endl;
cout << "6  T  -> F " << endl;
cout << "7  F  -> ( E ) " << endl;
cout << "8  F  -> id " << endl;
cout << "" << endl;
 */
std::string suansuwenfa[wenfaLength]; //用于存储算数表达式文法

bool is_cifa=true; //词法是否正确
char error_cifa;  //存储错误的词语


#endif // EXTERNVALUE_H
