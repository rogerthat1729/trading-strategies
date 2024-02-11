#include <bits/stdc++.h>
#include "main.h"
using namespace std;

// Passing a 0 as first element of args to indicate that it has come from executer
// Passing a 1 as first element of args to indicate that it has come from best

int main(int argc, char *argv[])
{
    vector<string> args;
    args.push_back("0");
    for (int i = 0; i < argc; ++i){
        args.push_back(argv[i]);}
    work(args);
    return 0;
}