//
// Created by Qian Chen on 5/24/17.
//

#ifndef SGF2GO_H
#define SGF2GO_H

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

    node _sgf2Node(string sgf);
    string _writeSgfTree(json* j);
    void _writeJsonTree(json* j, node* root);
    void _writeJsonTreeMain(json* j, node* root);

}


#endif //SGF2GO_H
