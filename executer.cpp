#include<bits/stdc++.h>
#include "main.h"
using namespace std;

int main(int argc, char *argv[])
{
    vector<string> args;
    for(int i = 0 ; i < argc ; ++i)
        args.push_back(argv[i]);
    work(argc, args);
    return 0;
}