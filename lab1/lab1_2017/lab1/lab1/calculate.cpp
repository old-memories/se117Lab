/*
name: zhang ziyang
ID: 515030910038
*/

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

void calculate(); //calculate function 
double basic();   // deal with singe '+'  and  '-'
double primary(); // deal with 'R' ,number,   '(' , ')' and '!'
double term();    //deal with '*' , '/'
double expression();  //dela with '+','-'
double parse(); //deal with 'M'
void clean_up_mess();  //clean the stream when error occurs
double factorial(double n);  //calculate factorial 

//same as code on the book 
class Token
{
public:
	char kind;
	double value;
	Token(char ch) :kind(ch), value(0) {}
	Token(char ch, double val) :kind(ch), value(val) {}

};
class Token_stream
{
public:
	Token_stream();
	Token get();
	void putback(Token t);
	void ignore(char c);
private:
	bool full;	
	Token buffer;

};



Token_stream::Token_stream() :full(false), buffer(0) {}
//ignore all the tokens till get c
void Token_stream::ignore(char c) {
	if (full&&c == buffer.kind) {
		full = false;
		return;
	}
	full = false;
	char ch = 0;
	while (cin >> ch)
		if (ch == c)return;
}

void Token_stream::putback(Token t)
{
	buffer = t;
	full = true;
}
Token Token_stream::get()
{
	if (full)
	{
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch;

	switch (ch)
	{
	case';':
	case'q':
	case'(':case')':case'+':case'-':case'*':case'/':case'%':case'!':case'R':case'M': 
		return Token(ch);
	case'.':
	case'0':case'1':case'2':case'3':case'4':case'5':
	case'6':case'7':case'8':case'9':
	{
		cin.putback(ch);
		double val;
		cin >> val;
		return Token('n', val);//note:  'n' represents number
	}
	default:
		throw(1);

	}
}

Token_stream ts;
double save;  //save the number when using 'R' and 'M'


double primary()
{
	double left = 0;
	Token t = ts.get();
	switch (t.kind) {
	case'(':
	{
		left = expression();
		t = ts.get();
		if (t.kind != ')') {   // if no ')'  
			ts.putback(t);
			throw(1); 
		}
		Token t2 = ts.get();
		if (t2.kind == '!') {
			Token t3 = ts.get();
			if ((t3.kind == 'R') || (t3.kind == '!') || (t3.kind == 'n')) {   //invalid expression like: 2!2, 3!R, R!4
				ts.putback(t3);
				throw(1);
			}
			ts.putback(t3);
				if (left < 0)
					throw(1);
				int i1 = int(left);
				if (i1 != left)
					throw(1);

				left = factorial(left);
				return left;
			}
		else {
			ts.putback(t2);
			return left;

		}
	}
	case'R':
	{
		left = save;
		Token t2 = ts.get();
		if (t2.kind == 'n') {
			ts.putback(t2);
			throw(1);
		}
		if (t2.kind == '!') {
			Token t3 = ts.get();
			if ((t3.kind == 'R') || (t3.kind == '!') || (t3.kind == 'n')) { //invalid expression like: 2!2, 3!R, R!4
				ts.putback(t3);
				throw(1);
			}
			ts.putback(t3);

			{
				if (left < 0)
					throw(1);
				int i1 = int(left);
				if (i1 != left)
					throw(1);

				left = factorial(left);
				return left;
			}
		}
		else {
			ts.putback(t2);
			return left;

		}
	}
	case'n':
	{
		left = t.value;
		Token t2 = ts.get();
		if (t2.kind == 'R') {
			ts.putback(t2);
			throw(1);
		}
		if (t2.kind == '!') {
			Token t3 = ts.get();
			if ((t3.kind == 'R') || (t3.kind == '!') || (t3.kind == 'n')) { //invalid expression like: 2!2, 3!R, R!4
				ts.putback(t3);
				throw(1);
			}
			ts.putback(t3);
			{
				if (left < 0)
					throw(1);
				int i1 = int(left);
				if (i1 != left)
					throw(1);

				left = factorial(left);  //calculate factorial of left
				return left;
			}
		}
		else {
			ts.putback(t2);
			return left;
		}
	}
	default:
		ts.putback(t);
		throw(1);
	}
}

double basic() {
	double left = 1;
	Token t = ts.get();
	while (true) {
		switch (t.kind)
		{
		case'-':
			left = -left;
			t = ts.get();
			break;
		case'+':
			t = ts.get();
			break;
		case'R':
		case'n':case'(':    //if next token is R, number or '(' ,we will read it as a primary;
			ts.putback(t);
			return  left*primary(); //left equals to +1 or -1; 
		default:
			ts.putback(t);
			throw(1);
		}
	}
}



double term()
{


	double left = basic();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case'*':
			left *= basic();
			t = ts.get();
			break;
		case'/':
		{
			double d = basic();
			if (d == 0) //deal with divided by zero
			{
				throw(1);
			}
			left /= d;
			t = ts.get();
			break;
		}
		case'%':
		{
			double d = basic();
			int i1 = int(left);
			if (i1 != left)
				throw(1);
			int i2 = int(d);
			if (i2 != d)
				throw(1);  //i1 an i2 should be both int
			if (i2 == 0)// deal with divided by zero
				throw(1);
			left = i1%i2;
			t = ts.get();
			break;
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}
double expression()
{
	double left = term();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case'+':
			left += term();
			t = ts.get();
			break;
		case'-':
			left -= term();
			t = ts.get();
			break;
		default:
			ts.putback(t);
			return left;
		}


	}
}

double parse() {
	double left = expression();
	Token t1 = ts.get();
	if ((t1.kind == 'M')) {
		Token t2 = ts.get();
		if (t2.kind == ';') {//M should be the last token of  the expression
			save = left;
			ts.putback(t2);
		}
		else {
			ts.putback(t2);
			throw(1);
		}
	}
	return left;
}

int main()
{
	cout << "NOTE: Enter 'q' to quit.\n";
	try {
		calculate();
		return 0;
	}
	catch (...) {
		cerr << "error\n";
		clean_up_mess();
		return 1;
	}
}

void calculate() {	
	save = 0;
	while (cin)
	{
		try{
			cout << "> ";
		    Token t = ts.get();
			while (t.kind == ';') t = ts.get();
			if (t.kind == 'q')
				return;
			ts.putback(t);
			cout << "= " << parse() << endl;
		   }
		catch (...) { //deal with errors, just print "error" and clean the stream,then the calculator will wait for next expression
			cerr << "error\n";
			clean_up_mess();
		}
	}
}

void clean_up_mess() {
	ts.ignore(';');
}


double factorial(double n)
{
	double num = 1;
	while (n>1)
	{
		num *= n;
		n--;
	}
	return num;
}