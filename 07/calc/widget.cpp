#include "widget.h"
#include "ui_widget.h"
#include "externValue.h"

#include<iostream>
#include<string>
#include<vector>	//STL容器，动态数组
#include<iomanip>	//I/O控制流头文件
#include <stack>	//先进后出，栈
#include <map>
#include <QFile>
//STL(标准模板库)的一个关联容器，提供一对一的hash  key-value对应，每个key只能出现一次map内部实现一颗红黑树，对数据由自动排序的功能
//红黑树≈平衡的二叉排序树;
using namespace std;

// 用于存储语法分析的过程 最对100行，每行默认5各部分:步骤 状态栈 符号栈 输入串 操作
string yufaAna_string[100];
int ys_legnth=0;  //yufaAna_string 实际长度
//printfInfoEX(step, state, symbol, str1, "移入" + str);

//后缀表达式 存储后缀表达式
string houzhui_string;

/*
 结合编译原理的词法分析、语法分析及语义分析方法，给出表达式计算器的系统设计过程，并实现表达式计算器。课题要求如下：
（1）计算器中能实现基本功能如下：支持加、减、乘、除等运算；支持括号运算；对用户输入的表达式是否正确进行判断并给出错误提示；可以对上述功能进行扩充，例如增加支持三角函数和对数函数以及幂运算等；
（2）词法分析、语法分析、语义处理模块结构清晰，可以借助Lex、Yacc或JavaCC、Antlr等工具自动生成词法分析和语法分析程序；
（3）界面美观；
（4）可根据自己能力，在完成以上基本要求后，对程序功能进行适当扩充；
（5）撰写报告，对所采用的算法、程序结构和主要函数过程以及关键变量进行详细的说明；对程序的调试过程所遇到的问题进行回顾和分析，对测试和运行结果进行分析；总结软件设计和实习的经验和体会，进一步改进的设想。
（6）提供关键程序的清单、源程序及可执行文件和相关的软件说明。
*/


//*********************词法分析器******************************//
char SEPARATER[2] = { '(', ')' };    //分隔符
char OPERATOR[5] = { '+', '-', '*', '/', '=' };     //运算符
char FILTER[4] = { ' ', '\t', '\r', '\n' };                    //过滤符

/**判断是否为分隔符**/
bool IsSeparater(char ch) {
    for (int i = 0; i < 2; i++) {
        if (SEPARATER[i] == ch) {
            return true;
        }
    }
    return false;
}

/**判断是否为运算符**/
bool IsOperator(char ch) {
    for (int i = 0; i < 5; i++) {
        if (OPERATOR[i] == ch) {
            return true;
        }
    }
    return false;
}
/**判断是否为过滤符**/
bool IsFilter(char ch) {
    for (int i = 0; i < 4; i++) {
        if (FILTER[i] == ch) {
            return true;
        }
    }
    return false;
}
/**判断是否为数字**/
bool IsDigit(char ch) {
    if (ch >= '0' && ch <= '9') return true;
    return false;
}

//申请一块 3*(expression.length+1) 的二维数组，3行分别存储expression的整型数、运算符、分隔符
string record_arr[3][10];
int record_arr_length[3] = {0,0,0}; //分别记录record_arr的3行长度
/**词法分析**/
vector<string> analyse(string expression) {
    vector<string> vec;

    char ch = ' ';
    for (int i = 0; i < expression.length(); i++)
    {
        string arr = "";
        ch = expression[i];
        if (IsFilter(ch)) {}              //判断是否为过滤符
        else if (IsDigit(ch)) {           //判断是否为数字
            while (IsDigit(ch) || IsFilter(ch)) {
                if (IsDigit(ch))
                    arr += ch;
                i++;
                ch = expression[i];
            }
            i--;
            //printf("%3d    ", CONSTANT);
            //cout << "\t\t\t< 整形数 , " << arr << ">" << endl;
            record_arr[0][record_arr_length[0]++] = arr;  //记录arr整形数
            vec.push_back(arr);		//将一个新的元素加到vector后面，位置为当前最后一个元素的下一个元素
        }
        else if (IsOperator(ch))
        {
            arr += ch;
            //printf("%3d    ", value(OPERATOR, 8, *arr.data()));
            //cout << "\t\t\t< 运算符 , " << arr << ">" << endl;
            record_arr[1][record_arr_length[1]++] = arr;  //记录arr运算符
            vec.push_back(arr);
        }
        else if (IsSeparater(ch))
        {
            arr += ch;
            //printf("%3d    ", value(SEPARATER, 8, *arr.data()));
            //cout << "\t\t\t< 分隔符 , " << arr << ">" << endl;
            record_arr[2][record_arr_length[2]++] = arr;  //记录arr分隔符
            vec.push_back(arr);
        }
        else
        {
            is_cifa=false;  //表示词法分析出错
            error_cifa=ch; //存储出错的词语
            cout << "\t\t\t\"" << ch << "\":无法识别的字符！" << endl;
            vec.clear();
            return vec;
        }
    }
    return vec;
}
//*********************中间代码生成器******************************//
string InversePolish(string s_mid)  //生成后缀表达式
{
    string s_beh = "";
    stack<char> stk;

    map<char, int> op;//利用map来实现运算符对应其优先级
    op['('] = 0;
    op[')'] = 0;
    op['+'] = 1;
    op['-'] = 1;
    op['*'] = 2;
    op['/'] = 2;
    string::iterator it = s_mid.begin();
    //迭代器，遍历并选择序列中的对象，他提供了一种访问一个容器对象的中各个元素，而又不必保留该对象内部细节的方法。
    while (it != s_mid.end())
    {
        if (op.count(*it))//判断该元素是否为运算符
        {
            if (*it == ')')//若为’）‘，把栈中的运算符依次加入后缀表达式，直到出现'（'，’（‘出栈，退出该次循环
            {
                while (stk.top() != '(')
                {
                    s_beh += stk.top();		//取出栈顶元素
                    s_beh += " ";
                    stk.pop();
                }
                stk.pop();		//弹出栈顶元素
            }
            else if (stk.empty() || *it == '(' || op[*it] > op[stk.top()])
                //若为‘（’，入栈 ; 要入栈的运算符优先级大于等于栈顶的运算符的优先级，直接入栈
            {
                stk.push(*it);	//在栈顶增加元素
            }
            else if (op[*it] <= op[stk.top()])
                // 入栈的运算符优先级小于等于栈顶的运算符的优先级，栈顶运算符出栈，再次比较，直到出现优先级低的运算符，或者栈为空，退出
            {
                while (op[*it] <= op[stk.top()] && (!stk.empty()))
                {
                    s_beh += stk.top();
                    s_beh += " ";
                    stk.pop();
                    if (stk.empty()) break;
                }
                stk.push(*it);
            }
        }
        else
        {
            s_beh += *it;
            it++;
            if (it != s_mid.end() && op.count(*it))
                //count，返回被查找元素个数，返回1/0，有/无；find 返回被查找元素的位置，没有返回map.end()
                s_beh += " ";
            it--;
        }
        it++;

        if (it == s_mid.end())//当中缀表达式输出完成，所有元素出栈
        {
            while (!stk.empty())
            {
                s_beh += " ";
                s_beh += stk.top();
                stk.pop();
            }
            break;
        }
    }
    cout << "逆波兰表达式：" << s_beh << endl;
    houzhui_string=s_beh;
    return s_beh;
}

//*********************后缀表达式计算******************************//
int Op(int a, char op, int b)
{
    switch (op)
    {
    case '+':return a + b; break;
    case '-':return a - b; break;
    case '*':return a * b; break;
    case '/':return a / b; break;
    }
}

int calculate(char* str)
{
    stack<int> s;//暂存操作数
    int i = 0;
    int offset = 0;
    int flag = 0;
    int Length = strlen(str);
    int Result;
    while (i < Length)
    {
        string One = "";
        for (i; i < Length; i++)
        {

            One += str[i];
            if (((str[i] == ' ') && (str[i + 1] != ' ')) || (i == Length - 1))
            {
                char* OneNumber = (char*)One.data();
                int Number = atoi(One.c_str());
                //atoi字符串转整数，c_str返回一个指向正规c字符串的指针常量，内容与其中string同。（为了与c语言兼容）

                if (IsOperator(*OneNumber))
                {
                    Number = 0;
                }
                s.push(Number);

                if (IsOperator(*OneNumber))
                {
                    s.pop();
                    int x = s.top();
                    s.pop();
                    int y = s.top();
                    s.pop();
                    Result = Op(y, *OneNumber, x);
                    s.push(Result);
                }
                offset = i + 1;
                break;
            }
        }
        i = offset;
    }
    return s.top();
}


//*********************语法分析器******************************//
//Action表
int action[16][8] = { { 0, 0, 0, 0, 4, 0, 5, 0 },//0
{ 6, 7, 0, 0, 0, 0, 0, 100 },//1
{ -3, -3, 8, 9, 0, -3, 0, -3 },//2
{ -6, -6, -6, -6,  0, -6, 0, -6 },//3
{ 0, 0, 0, 0, 4, 0, 5, 0 },//4
{ -8, -8, -8, -8, 0, -8, 0, -8 },//5
{ 0, 0, 0, 0, 4, 0, 5, 0 },//6
{ 0, 0, 0, 0, 4, 0, 5, 0 },//7
{ 0, 0, 0, 0, 4, 0, 5, 0 },//8
{ 0, 0, 0, 0, 4, 0, 5, 0 },//9
{ 6, 7, 0, 0, 0, 15, 0, 0 },//10
{ -1, -1, 8, 9, 0, -1, 0, -1 },//11
{ -2, -2, 8, 9, 0, -2, 0, -2 },//12
{ -4, -4, -4, -4, 0, -4, 0, -4 },//13
{ -5, -5, -5, -5, 0, -5, 0, -5 },//14
{ -7, -7, -7, -7, 0, -7, 0, -7 }//15
};//17
//Goto表
int gotol[16][3] = { { 1, 2, 3 },//0
{ 0, 0, 0 },//1
{ 0, 0, 0 },//2
{ 0, 0, 0 },//3
{ 10, 2, 3 },//4
{ 0, 0, 0 },//5
{ 0, 11, 3 },//6
{ 0, 12, 3 },//7
{ 0, 0, 13 },//8
{ 0, 0,14 },//9
{ 0, 0, 0 },//10
{ 0, 0, 0 },//11
{ 0, 0, 0 },//12
{ 0, 0, 0 },//13
{ 0, 0, 0 },//14
{ 0, 0, 0 } };//17
//终结符集合
string endls[9] = { "+", "-", "*", "/", "(", ")", "=","id","#" };
//非终结符集合
string noends[3] = { "E", "T", "F" };
//产生式集合
string products[9] = { "E", "E+T", "E-T", "T", "T*F", "T/F", "F", "(E)", "id" };
/*
   E=> T+F | T-F |T
   T=> F*T | F/T |F
   F=> (E) | id
*/

//状态栈
class statestack
{
public:
    int* base;//栈底指针
    int* top;//栈顶指针
    int size;//栈内元素个数
    int stacksize;//栈的大小
public:
    statestack()
    {
        size = 0;
        stacksize = 20;
        base = new int[stacksize];
        top = base;
    }
    int getTop()//获取栈顶的元素。
    {
        if (base == top)
        {
            return -1;
        }
        else
        {
            return *(top - 1);
        }
    }
    bool statePush(int elem)//元素入栈
    {
        ++size;
        (*top) = elem;
        ++top;
        return true;
    }
    void statePop(int time)//元素出栈
    {
        for (int i = 0; i < time; i++)
        {
            --top;
            --size;
        }
    }
    void printState()//输出栈内的所有元素
    {
        string str = " ";
        int* pre;
        for (pre = base; pre < top; pre++)
        {
            if (*pre > 9)   // 栈中保存的10转换成1、0
            {
                char ch1 = (*pre / 10) + 48;
                char ch2 = (*pre % 10) + 48;
                str += ch1;
                str += ch2;
            }
            else
            {
                char ch = *pre + 48;
                str += ch;
            }
        }
        cout << setw(15) << setiosflags(ios_base::left) << str;
        yufaAna_string[ys_legnth]+="\t\t\t"+str;
    }
};

//符号栈
class symbolstack
{
public:
    string* base;
    string* top;
    int size;
    int stacksize;
public:
    symbolstack()
    {
        size = 0;
        stacksize = 20;
        base = new string[stacksize];
        top = base;
    }
    string getTop()//获取栈顶的元素。
    {
        if (base == top)
        {
            return "";
        }
        else
        {
            return *(top - 1);
        }
    }
    //元素入栈
    bool symbolPush(string elem)
    {
        ++size;
        *top = elem;
        ++top;
        return true;
    }
    //元素出栈
    void symbolPop(int time)
    {
        for (int i = 0; i < time; i++)
        {
            --top;
            --size;
        }
    }
    //输出栈内的所有元素
    void printSymbol()
    {
        string str = "";
        string* pre;
        for (pre = base; pre < top; pre++)
        {
            str += *pre;
        }
        cout << setw(15) << setiosflags(ios_base::left) << str;
        yufaAna_string[ys_legnth]+="\t\t\t"+str;

    }
};


class analyseLR
{
public:
    int step;
    string inputstr;//布尔表达式
    statestack state;//状态栈
    symbolstack symbol;//符号栈
    statestack data;//数据栈

    //vector<string> fors;//四元式
public:
    //构造函数
    analyseLR()
    {
        step = 0;
        inputstr = "";
        state = statestack();
        symbol = symbolstack();
    }
    //初始化两个栈
    void initstack()
    {
        state.statePush(0);
        symbol.symbolPush("=");
    }
    //这是一个输出提示函数
    void printInfo(string str)
    {
        cout << str << endl;
    }
    //这是两个输出提示函数的增强版
    void printInfoEX(string str1, string str2, string str3, string str4, string str5)
    {
        cout << setw(6) << setiosflags(ios_base::left) << str1;	// setiosflags(ios_base::left) 左对齐值，用填充字符填充右边
        cout << setw(15) << setiosflags(ios_base::left) << str2;
        cout << setw(15) << setiosflags(ios_base::left) << str3;
        cout << setw(30) << setiosflags(ios_base::left) << str4;
        cout << setw(10) << setiosflags(ios_base::left) << str5 << endl;
    }
    void printfInfoEX(int str1, statestack state, symbolstack symbol, string str4, string str5) //写入yufaAna_string
    {
        cout << setw(6) << setiosflags(ios_base::left) << str1;
        yufaAna_string[ys_legnth]="\t\t\t"+to_string(str1);
        state.printState();
        symbol.printSymbol();
        cout << setw(30) << setiosflags(ios_base::left) << str4;
        yufaAna_string[ys_legnth]+="\t\t\t"+str4;
        cout << setw(10) << setiosflags(ios_base::left) << str5 << endl;
        yufaAna_string[ys_legnth++]+="\t\t\t"+str5;    }
    //初始化界面函数
    void initstageInfo()
    {
        cout << "\n<<<<<基于编译原理的表达式计算器>>>>>" << endl;
        cout << "" << endl;
        cout << "归纳算术表达式文法如下：\n" << endl;
        cout << "0  E’-> E " << endl;
        suansuwenfa[0]="0  E’-> E ";  //存储算术文法文法
        cout << "1  E  -> E + T" << endl;
        suansuwenfa[1]="1  E  -> E + T";
        cout << "2  E  -> E - T" << endl;
        suansuwenfa[2]="2  E  -> E - T";
        cout << "3  E  -> T " << endl;
        suansuwenfa[3]="3  E  -> T ";
        cout << "4  T  -> T * F" << endl;
        suansuwenfa[4]="4  T  -> T * F";
        cout << "5  T  -> T / F" << endl;
        suansuwenfa[5]="5  T  -> T / F";
        cout << "6  T  -> F " << endl;
        suansuwenfa[6]="6  T  -> F ";
        cout << "7  F  -> ( E ) " << endl;
        suansuwenfa[7]="7  F  -> ( E ) ";
        cout << "8  F  -> id " << endl;
        suansuwenfa[8]="8  F  -> id ";
        cout << "" << endl;
    }

    //提示输入的函数
    void initInputstr()
    {
        cout << "\n输入表达式:";
        getline(cin, inputstr);
    }

    //将终结符和非终结符转换为action和gotol表中对应的列号
    int strtoInt(string str)
    {
        if (str == "+")
            return 0;
        if (str == "-")
            return 1;
        if (str == "*")
            return 2;
        if (str == "/")
            return 3;
        if (str == "(")
            return 4;
        if (str == ")")
            return 5;
        //if (str == "id")
            //return 6;
        if (str == "=")  //$
            return 7;
        if (str == "E")
            return 0;
        if (str == "T")
            return 1;
        if (str == "F")
            return 2;
        //return 111;
        return 6;
    }
    //判断字符是终结符还是非终结符
    bool judgeSymbol(string str)
    {
        for (int i = 0; i < 3; i++)
        {
            if (noends[i] == str)
                return false;
        }
        return true;
    }
    //根据产生式选择非终结符
    int chooseNoEnds(int num)
    {
        if (num == 1 || num == 2 || num == 3)
            return 0;//选择“E”
        if (num == 4 || num == 5 || num == 6)
            return 1;//选择“T”
        return 2;//选择“F”
    }
    //计算字符串中元素的个数
    int calculatenum(string str)
    {
        int num = 0;
        if (str == "id") num = 1;
        else num = str.size();
        return num;
    }
    //将vector容器中的字符串连起来
    string linkVectorstr(vector<string>& vecs, vector<string>::iterator iter)
    {
        string str = "";
        vector<string>::iterator it;
        it = iter;
        for (it; it < vecs.end(); it++)
        {
            //cout<<*it;
            str += *it;
        }
        return str;
    }



    void Start()//开始函数.
    {
        initstageInfo();
        initstack();//初始化两个栈
        //initInputstr();
        printInfo("\n\t\t\t<<<<词法分析>>>>");

        //printInfo("\t\t\t记号名\t属性值");
        vector<string> vec = analyse(inputstr);//分开布尔表达式
        if (vec.empty()) {}
        else {
            vector<string>::iterator iter = vec.begin();
            printInfo("\n\t\t\t<<<<语法分析>>>>");
            printInfoEX("步骤", "状态栈", "符号栈", "输入串", "操作");
            bool flag;
            for (iter; iter != vec.end(); ++iter)//依次遍历字符
            {
                flag = 1;
                string str1 = linkVectorstr(vec, iter);
                actionGoto(*iter, str1, vec, flag);
                if (!flag)
                    return;
            }
            iter--;
            if (*iter != "=")
            {
                flag = 0;
                cout << "请以'='结束！" << endl;
            }
            if (flag)
            {
                cout << "语法分析成功！" << endl;
                //printInfo("\n<<<<中间代码>>>>");
                vector<string>::iterator iter2 = vec.begin();
                string s_mid = linkVectorstr(vec, iter2);   //连接成一个字符串
                s_mid = s_mid.substr(0, s_mid.length() - 1);  //删除最后的等于，用substr实现截取功能
                string HZ = InversePolish(s_mid);//转后缀表达式
                //printInfo("\n---------------------------计算结果----------------------------------");
                char* stringofchar = (char*)HZ.data();//加const  或者用char * p=(char*)str.data();的形式
                int Result = calculate(stringofchar);
                cout << "逆波兰表达式计算结果：" << Result << endl;
            }
            //printForsInfo();
        }
    }

    //语法分析结果(计算结果)
    int calc_result=0; //初始化为0
    //LR分析函数
    int actionGoto(string str, string str1, vector<string> vec, bool& flag)
    {
        int x = state.getTop();//当前栈顶状态
        int y = strtoInt(str);//当前将要输入的字符
        if (action[x][y] == 0)
        {
            cout << "语法分析出错！" << endl;
            flag = 0;
        }
        if (action[x][y] > 0 && judgeSymbol(str) && (str != "="))//移进
        {
            printfInfoEX(step, state, symbol, str1, "移入" + str);

            state.statePush(action[x][y]);
            symbol.symbolPush(str);
            ++step;
        }
        if (action[x][y] < 0 && judgeSymbol(str))//规约
        {
            int z = -action[x][y];//用-action[x][y]对应的产生式规约
            string lenstr = products[z];//生成需要规约的产生式
            int n = calculatenum(lenstr);//计算产生式的长度，进行规约
            int c = chooseNoEnds(z);
            string ch = noends[c];//生成规约后的非终结符
            printfInfoEX(step, state, symbol, str1, "规约" + noends[c] + "->" + products[z]);

            switch (z)
            {
            case 1:
            {
                int a = data.getTop();
                data.statePop(1);
                int b = data.getTop();
                data.statePop(1);
                b += a;
                data.statePush(b);
                break;
            }
            case 2:
            {
                int a = data.getTop();
                data.statePop(1);
                int b = data.getTop();
                data.statePop(1);
                b -= a;
                data.statePush(b);
                break;
            }
            case 4:
            {
                int a = data.getTop();
                data.statePop(1);
                int b = data.getTop();
                data.statePop(1);
                b *= a;
                data.statePush(b);
                break;
            }
            case 5:
            {
                int a = data.getTop();
                if (a == 0)
                {
                    flag = 0;
                    cout << "\n除数为0！" << endl;
                    return flag;
                }
                data.statePop(1);
                int b = data.getTop();
                data.statePop(1);
                b /= a;
                data.statePush(b);
                break;
            }
            case 8:
            {
                string b = symbol.getTop();
                int c = atoi(b.c_str());
                data.statePush(c);
                break;
            }

            default:
                break;
            }

            state.statePop(n);
            symbol.symbolPop(n);
            int m = state.getTop();//获取规约后的栈顶状态
            if (gotol[m][c] > 0)
            {
                int g = gotol[m][c];
                state.statePush(g);
                symbol.symbolPush(ch);
            }
            ++step;
            actionGoto(str, str1, vec, flag);
        }
        //cout << str<<" "<<str1 << endl;
        if ((action[x][y] == 100) && (str1 == "="))
        {
            flag = 1;
            printfInfoEX(step, state, symbol, str1, "分析完成");
            cout << "\n语法分析计算结果：" << data.getTop() << endl;
            calc_result = data.getTop();
        }
        else if ((action[x][y] == 100))
        {
            cout << "语法分析失败！" << endl;
            flag = 0;
        }

        return flag;
    }
};


//int main()
//{
//	while (1) {
//		string x;
//		analyseLR ana = analyseLR();
//		ana.Start();
//		cout << "\n是否继续？(y/n)";
//		getline(cin, x);
//		if (x == "n" || x == "N") break;
//	}
//	system("pause");
//	return 0;
//}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


analyseLR ana = analyseLR();


void Widget::on_btnStart_clicked()
{
    ui->textBrowser->clear();    //清空输出窗口
    //ui->textBrowser->setText("我不好！");
    //ana.inputstr = ui->lineInput->text();
    ana.inputstr = ui->lineInput->text().toStdString(); //从输入窗口读入，QString->string，赋予ana.inputstr
    string inputstr_string = ana.inputstr;
    QString inputstr_QString = ui->lineInput->text();
    
    ui->textBrowser->setText(inputstr_QString);

    ana.initstageInfo();
    ana.initstack();//初始化两个栈
    
    //显示算术文法
    ui->textEdit_wenfa->append("算术表达式文法：");
    for(int i=0;i<wenfaLength;i++)
        ui->textEdit_wenfa->append(QString::fromStdString(suansuwenfa[i]));

    ui->textBrowser->append("\n<<<<词法分析>>>>");
    vector<string> vec = analyse(ana.inputstr);//分开布尔表达式
    //打印record_arr
    for (int i=0; i < 3; i++)
    {
        if (i == 0)
            cout << "\n整型数:";
        if (i == 1)
            cout << "\n运算符:";
        if (i == 2)
            cout << "\n分隔符:";
        for (int j = 0; j < record_arr_length[i]; j++)
        {
            cout << record_arr[i][j] << " ";
        }
    }
    //打印词法分析结果 record_arr 到界面上
    if(!is_cifa)
    {
        ui->textBrowser->append(QString::fromStdString("###词法分析发生错误###"));
        ui->textBrowser->append(QString::fromStdString("以下为错误："));
        QString error_cifa_QString;
        error_cifa_QString=QString(error_cifa);
        ui->textBrowser->append(error_cifa_QString);
        return;
    }
    for (int i=0; i < 3; i++)
    {
        if (i == 0)
            //cout << "\n整型数:";
            ui->textBrowser->append("\n整型数:");
        if (i == 1)
            //cout << "\n运算符:";
            ui->textBrowser->append("\n运算符:");
        if (i == 2)
            //cout << "\n分隔符:";
            ui->textBrowser->append("\n分隔符:");
        for (int j = 0; j < record_arr_length[i]; j++)
        {
            //cout << record_arr[i][j] << " ";
            ui->textBrowser->append(QString::fromStdString(record_arr[i][j]));
        }
    }
    if (vec.empty()) {}
    else {
        vector<string>::iterator iter = vec.begin();
        //printInfo("\n\t\t\t<<<<语法分析>>>>")  被替代
        ui->textBrowser->append("\n<<<<语法分析>>>>");
        //printInfoEX("步骤", "状态栈", "符号栈", "输入串", "操作");  被替代
        ui->textBrowser->append(QString::fromStdString("\n\t\t\t步骤\t\t\t状态栈\t\t\t符号栈\t\t\t\输入串\t\t\t\操作"));
        bool flag;
        for (iter; iter != vec.end(); ++iter)//依次遍历字符
        {
            flag = 1;
            string str1 = ana.linkVectorstr(vec, iter);
            ana.actionGoto(*iter, str1, vec, flag);
            if (!flag)
                return;
        }
        //在界面输出
        for(int i=0;i<ys_legnth;i++)
        {
            QString tmp = QString::fromStdString(yufaAna_string[i]);  //第i条语法分析
            cout<<yufaAna_string[i]<<endl;
            ui->textBrowser->append(tmp);
        }
        iter--;
        if (*iter != "=")
        {
            flag = 0;
            cout << "发生错误，请以'='结束！" << endl;
            ui->textBrowser->append("发生错误，请以'='结束！");
        }
        if (flag)
        {
            cout << "语法分析成功！" << endl;
            ui->textBrowser->append("语法分析成功！\n");
            //printInfo("\n<<<<中间代码>>>>");
            vector<string>::iterator iter2 = vec.begin();
            string s_mid = ana.linkVectorstr(vec, iter2);   //连接成一个字符串
            s_mid = s_mid.substr(0, s_mid.length() - 1);  //删除最后的等于，用substr实现截取功能
            string HZ = InversePolish(s_mid);//转后缀表达式
            ui->textBrowser->append(QString::fromStdString("后缀表达式："+houzhui_string));
            //printInfo("\n---------------------------计算结果----------------------------------");
            char* stringofchar = (char*)HZ.data();//加const  或者用char * p=(char*)str.data();的形式
            int Result = calculate(stringofchar);
            cout << "逆波兰表达式计算结果：" << Result << endl;
            ui->textBrowser->append(QString::fromStdString("后缀表达式计算结果:"+to_string(Result)));
        }
        //printForsInfo();
    }
    //ana.Start()
}

