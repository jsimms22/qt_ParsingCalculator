#ifndef CALCULATOR_H
#define CALCULATOR_H

// #include "std_lib_facilities.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <sstream>

#include <QApplication>

// error() simply disguises throws:
inline void error(const std::string& s)
{
    throw std::runtime_error(s);
}

inline void error(const std::string& s, const std::string& s2)
{
    error(s + s2);
}

inline void error(const std::string& s, int i)
{
    std::ostringstream os;
    os << s << ": " << i;
    error(os.str());
}

// run-time checked narrowing cast (type conversion). See ???.
template<class R, class A> R narrow_cast(const A& a)
{
    R r = R(a);
    if (A(r) != a) error(std::string("info loss"));
    return r;
}

constexpr char let = 'L';
constexpr char quit = 'Q';
constexpr char sq_rt = 'S';         // arbitrary token type for square roots
constexpr char power = '^';         // arbitrary token type for powers
constexpr char print = ';';
constexpr char number = '8';        // arbitrary token type for numbers
constexpr char name = 'a';          // arbitrary token type for variables
const std::string prompt = "> ";    // prompt line indicator symbol
const std::string result = "= ";    // result line indicator symbol
const std::string declkey = "let";
const std::string sqrtkey = "sqrt";
const std::string powkey = "pow";
const std::string quitkey = "quit";
std::stringstream ss{};             // handles the stream for our input to calculate

//------------------------------------------------------------------------------

struct Token
{
    char kind;      // what kind of token
    double value;   // for numbers: a value
    std::string name;
    Token() :kind(0) { }                                    // default constructor
    Token(char ch) :kind(ch), value(0) { }                  // make a Token from a char
    Token(char ch, double val) :kind(ch), value(val) { }    // make a Token from a char and a double
    Token(char ch, std::string s) :kind(ch), name(s) { }    // make a Token from a char and a string
};

//------------------------------------------------------------------------------

class Token_stream
{
private:
    bool full;      // is there a Token in the buffer?
    Token buffer;   // here is where we keep a Token put back using unget()
public:
    Token_stream() :full(0), buffer(0) { }              // make a Token_stream that reads from cin
    Token get();                                        // get a Token (get() is defined elsewhere)
    void unget(Token t) { buffer = t; full = true; }    // put a Token back
    void ignore(char c);                                // discard characters up to and including char c
};

//------------------------------------------------------------------------------

Token Token_stream::get()
{
    if (full) {                     // do we already have a Token ready?
        full = false;               // remove token from buffer
        return buffer;
    }

    char ch;
    ss >> ch;

    switch (ch) {
    case print:
    case quit:
    case '!': case ',':
    case '{': case '}':
    case '(': case ')':
    case '+': case '-':
    case '*': case '/':
    case '%': case '=':
        return Token(ch);           // let each character represent itself
    case '.':                       // a floating-point literal can start with a dot
    case '0': case '1':
    case '2': case '3':
    case '4': case '5':
    case '6': case '7':
    case '8': case '9':
    {
        ss.unget();                 // put digit back into the input stream
        double val;
        ss >> val;                  // read a floating-point number
        return Token(number, val);  // let '8' represent "a number"
    }
    default:
        if (isalpha(ch) || ch == '_') {
            std::string s;
            s += ch;
            while (ss.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) s += ch;
            ss.unget();
            if (s == declkey) return Token(let);
            if (s == quitkey) return Token(name);
            if (s == sqrtkey) return Token(sq_rt);
            if (s == powkey) return Token(power);
            return Token(name, s);
        } // else { error("Bad token"); }
    }
    return Token();
}

//------------------------------------------------------------------------------

void Token_stream::ignore(char c)
{
    // first, look in buffer
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;

    // now search input
    char ch;
    while (std::cin >> ch)
        if (ch == c) return;
}

//------------------------------------------------------------------------------

struct Variable
{
    std::string name;
    double value;
    bool constant;
    Variable(std::string n, double v) :name(n), value(v) { constant = false; }
    Variable(std::string n, double v, bool c) :name(n), value(v), constant(c) { }
};

class Symbol_Table {
private:
    std::vector<Variable> var_table;
public:
    Symbol_Table() { }
    double get(std::string);
    void set(std::string, double);
    void push_back(Variable v) { var_table.push_back(v); }
    bool is_declared(std::string);
    double define_name(std::string, double);
    double define_const_name(std::string, double);
};

Symbol_Table Sym_Tbl = Symbol_Table(); // provides vector of variables

//------------------------------------------------------------------------------

// return the value of the Variable named s
double Symbol_Table::get(std::string s)
{
    for (Variable& v : Sym_Tbl.var_table) {
        if (v.name == s) { return v.value; }
    }
    // error("Get: undefined variable ", s);
    exit(1);
}

//------------------------------------------------------------------------------

// set the Variable named s to d
void Symbol_Table::set(std::string s, double d) {
    for (Variable& v : Sym_Tbl.var_table) {
        if (v.name == s) {
            if (v.constant == true) {
                // error("this variable cannot be rewritten");
            } else {
                v.value = d;
                return;
            }
        }
    }
    // error("undefined variable ", s);
}

//------------------------------------------------------------------------------

// is variable s already in the table?
bool Symbol_Table::is_declared(std::string s)
{
    for (const Variable& v : Sym_Tbl.var_table) {
        if (v.name == s) { return true; }
    }
    return false;
}

//------------------------------------------------------------------------------

// add variable {s,d} to the table
double Symbol_Table::define_name(std::string s, double d)
{
    if (Sym_Tbl.is_declared(s)) { error(s, " declared twice"); }
    Sym_Tbl.push_back(Variable(s,d));
    return d;
}

double Symbol_Table::define_const_name(std::string s, double d)
{
    if (Sym_Tbl.is_declared(s)) { error(s, " declared twice"); }
    Sym_Tbl.push_back(Variable(s,d,true));
    return d;
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback()

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

double sq_rt_handler(Token t)
{
    t = ts.get();
    if (t.kind != '(') error("'(' expected");
    double d = expression();
    if (d < 0) error("cannot take square root of negative values");
    t = ts.get();
    if (t.kind != ')') error("')' expected");
    return sqrt(d);
}

//------------------------------------------------------------------------------

double pow_handler(Token t)
{
    t = ts.get();
    if (t.kind != '(') error("'(' expected");
    double d = expression();
    t = ts.get();
    if (t.kind != ',') error("',' expected");
    double temp = expression();
    int i = narrow_cast<int>(temp);
    t = ts.get();
    if (t.kind != ')') error("')' expected");
    if (i == 0) { return 1.0; }
    return pow(d,i);
}

//------------------------------------------------------------------------------

// deal with numbers, unary operators, parentheses, and brackets
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '{':   // handle '{' expression '}'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != '}') error("'}' expected");
        return d;
    }
    case '(':   	// handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("'(' expected");
        return d;
    }
    case '-':
        return -primary();          // returns unary minus
    case '+':
        return + primary();         // returns unary plus
    case number:
        return t.value;             // return the number's value
    case name:
        return Sym_Tbl.get(t.name);   // return the variable's value of name t.name
    case sq_rt:
        return sq_rt_handler(t);	// handle square roots
    case power:
        return pow_handler(t);		// handle powers
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------

// deal with factorials
double factorial()
{
    double left = primary();

    while (true) {
        Token t = ts.get(); // get the next token from token stream
        switch (t.kind) {
        case '!':
        {
            if (t.kind != '!') error("'!' expected");
            // try calculating factorial
            try {
                int temp = narrow_cast<int>(left);  // fail if left value is not a whole number
                if (temp == 0) {
                    left = 1;
                    break;
                } else {
                    for (int i = (int) (temp - 1); i > 0; --i) {
                        temp *= i;
                    }
                    left = temp;
                }
            } catch (std::exception& e) {
                error(std::string("attempted to use ! on a non-whole number value"));
            }
            break;
        }
        default:
            ts.unget(t);
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
    double left = factorial();

    while (true) {
        Token t = ts.get(); // get the next token from token stream
        switch (t.kind) {
        case '*':
            left *= factorial();
            break;
        case '/':
        {
            double d = factorial();
            if (d == 0) error("divide by zero");
            left /= d;
            break;
        }
        case '%':
        {
            double d = factorial();
            if (d == 0) error("%: divide by zero");
            left = fmod(left,d);
            break;
        }
        default:
            ts.unget(t);    // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with adding and subtraction
double expression()
{
    double left = term();   // read and evaluate a Term

    while (true) {
        Token t = ts.get(); // get the next token from token stream
        switch (t.kind) {
        case '+':
            left += term(); // evaluate Term and add
            break;
        case '-':
            left -= term(); // evaluate Term and subtract
            break;
        default:
            ts.unget(t);    // put t back into the token stream
            return left;    // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

// assume we have seen "let"
// handle: name = expression
// declare a variable called "name" with the initial value "expression"
double declaration()
{
    Token t = ts.get();
    if (t.kind != 'a') error("name expected in declaration");

    std::string name = t.name;
    if (Sym_Tbl.is_declared(name) == false) {
        Token t2 = ts.get();
        if (t2.kind != '=') error("= missing in declaration of ", name);

        double d = expression();
        Sym_Tbl.push_back(Variable(name, d)); // add variable {s,d} to the table

        return d;
    }

    if (Sym_Tbl.is_declared(name) == true) {
        Token t2 = ts.get();
        if (t2.kind != '=') error("= missing in declaration of ", name);

        double d = expression();
        Sym_Tbl.set(name, d);

        return d;
    }
    error("failed to write a value to a variable");
    return 0;
}

//------------------------------------------------------------------------------

// entry path for our calculator
// helps different between variable intialization or raw math;
double statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case let:
        return declaration();   // begin steps to initialize variable with the name folowing 'let '
    default:
        ts.unget(t);
        return expression();    // otherwise calculate math
    }
}

//------------------------------------------------------------------------------

void clean_up_mess()
{
    ts.ignore(print);
}

//------------------------------------------------------------------------------

// handler for the user inputs and calculations
QString calculate(QString& input)
{
    ss = std::stringstream(input.toStdString());    // initialize our stream
    while (true) {
        try {
            // std::cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get();   // first discard all "prints"
            // if (t.kind == quit) return "";          // if prompted to quit, quit
            ts.unget(t);
            return QString::number(statement());
        }
        catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;     // write error message
            clean_up_mess();
            return "";
        }
    }

}

//------------------------------------------------------------------------------

#endif // CALCULATOR_H
