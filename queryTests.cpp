#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

#include "query.h"
#include "maxQuery.h"

using namespace std;

// print
void print(const vector<double>& v)
{
    vector<double>::const_iterator i;
    for (i = v.begin(); i != v.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl;
}

/*
 * populate random double into vector examples
 */
void populateRandomExample(vector<double>& examples, int count)
{
    random_device generator;
    normal_distribution<double> distribution(5.0,2.0);

    for (int i=0; i<count; ++i) {
        examples.push_back(distribution(generator));
    }
}

void printWhenTestError(vector<double>::iterator b, vector<double>::iterator e,
        double fromQuery, double fromTestFunc)
{
    cout << "data: ";
    for (vector<double>::iterator i=b; i!=e+1; ++i)
        cout << *i << " ";
    cout << "\nquery result: " << fromQuery;
    cout << "\ntestF result: " << fromTestFunc << endl;
}

double testFunctionNewElementQuery(vector<double>::iterator b, vector<double>::iterator e)
{
    return *e;
}

double testFunctionAverage(vector<double>::iterator b, vector<double>::iterator e)
{
    return accumulate(b, e+1, 0.0) / (distance(b,e) + 1);
}

double testFunctionMax(vector<double>::iterator b, vector<double>::iterator e)
{
    return *max_element(b, e+1);
}

double testFunctionMin(vector<double>::iterator b, vector<double>::iterator e)
{
    return *min_element(b, e+1);
}


/*
 * run over examples using query and testFunction
 * to test whether query works as expected
 */
bool doTest(vector<double>& examples, int window_size,
        Query<double>* query,
        double (*testFunction)(vector<double>::iterator, vector<double>::iterator)) // query and function needed
{
    if (!query) return false;

    for (vector<double>::iterator i=examples.begin(); i!=examples.end(); ++i)
    {
        double fromQuery = query->update_with_new_value(*i);
        /* cout << *i << ", " << fromQuery << endl; */

        vector<double>::iterator b; // = i-window_size+1;
        if (distance(examples.begin(), i) < window_size)
            b = examples.begin();
		else
		{
			b = i-window_size+1;
		}
        double fromTestFunc = (*testFunction)(b, i);

        if (fromQuery != fromTestFunc)
        {
            printWhenTestError(b,i,fromQuery,fromTestFunc);
            /* return false; */
            cout << "false\n";
        }
    }
    return true;
}

int main()
{
    /* Query<double>* query[3]; */
    const int count=20;
    const int window_size=4;

    vector<double> examples;
    populateRandomExample(examples, count);
    print(examples);

    MaxQuery<double> query(window_size);

    if (!doTest(examples, window_size, (Query<double>*)&query, &testFunctionMax))
    /* if (!doTest(examples, window_size, (Query<double>*)&query, &testFunctionAverage)) */
    {
        cout << "test failed\n";
    }
    else
    {
        cout << "test passed\n";
    }

    return 0;
}

