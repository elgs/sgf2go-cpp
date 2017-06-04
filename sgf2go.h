//
// Created by Qian Chen on 5/24/17.
//

#ifndef HELLO_SGF2GO_H
#define HELLO_SGF2GO_H

#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

namespace sgf2go {
    class node {
    public:
        vector<node> children;
        vector<deque<string>> properties;
    };

    string sgf2json(string sgf);
    string sgf2jsonMain(string sgf);
    string json2sgf(string js);

    node sgf2Node(string sgf);
    string writeSgfTree(json* j);
    void writeJsonTree(json* j, node root);
    void writeJsonTreeMain(json* j, node root);

}


#endif //HELLO_SGF2GO_H
