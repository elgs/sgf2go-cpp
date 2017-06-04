//
// Created by Qian Chen on 5/24/17.
//

#include "sgf2go.h"
#include <stack>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
using json = nlohmann::json;

namespace sgf2go {
    string _writeSgfTree(json* j) {
        string ret;
        for (json list : *j) {
            if (list.is_array() && list.size() > 0) {
                json first = list.at(0);
                if (first.is_array()) {
                    ret += "(";
                    ret += _writeSgfTree(&list);
                    ret += ")";
                } else {
                    ret += ";";
                    for (json propertiy : list) {
                        ret += propertiy["key"];
                        json value = propertiy["value"];
                        for (json v: value) {
                            ret += "[";
                            ret += v;
                            ret += "]";
                        }
                    }
                }
            }
        }
        return ret;
    };

    void _writeJsonTree(json* j, node* root) {
        json ja = json::array();
        for (deque<string> property: root->properties) {
            string key = property[0];
            property.pop_front();
            json value(property);
            json p = {{"key",   key},
                      {"value", value}};
            ja.push_back(p);
        }
        if (ja.size() > 0) {
            j->push_back(ja);
        }
        if (root->children.size() == 1) {
            for (node child : root->children) {
                _writeJsonTree(j, &child);
            }
        } else if (root->children.size() > 1) {
            for (node child : root->children) {
                j->push_back(json::array());
                _writeJsonTree(&j->back(), &child);
            }
        }
    };

    void _writeJsonTreeMain(json* j, node* root) {
        json ja = json::array();
        for (deque<string> property: root->properties) {
            string key = property[0];
            property.pop_front();
            json value(property);
            json p = {{"key",   key},
                      {"value", value}};
            ja.push_back(p);
        }
        if (ja.size() > 0) {
            j->push_back(ja);
        }
        for (node child : root->children) {
            _writeJsonTreeMain(j, &child);
            break;
        }
    };

    node _sgf2Node(string sgf) {
        node root;
        node* current = &root;
        stack<node*> listStack;
        bool inSqBracket = false;
        string buf;

        for (size_t i = 0; i < sgf.length(); ++i) {
            char a = sgf.at(i);
            if (inSqBracket) {
                if (a == ']' && sgf.at(i - 1) != '\\') {
                    inSqBracket = false;
                    current->properties.back().push_back(buf);
                    buf = "";
                } else {
                    buf += a;
                }
            } else {
                if (a == '(') {
                    listStack.push(current);
                } else if (a == ';') {
                    current->children.push_back(node());
                    current = &(current->children.back());
                } else if (a == ')') {
                    current = listStack.top();
                    listStack.pop();
                } else if (a == '[') {
                    inSqBracket = true;
                    if (buf.length() > 0) {
                        deque<string> v;
                        v.push_back(buf);
                        current->properties.push_back(v);
                        buf = "";
                    }
                } else {
                    buf += a;
                    trim(buf);
                }
            }
        }
        return root;
    };

    string sgf2json(string sgf) {
        node root = _sgf2Node(sgf);
        json j;
        _writeJsonTree(&j, &root);
        string js = j.dump();
        if (starts_with(js, "[[{")) {
            js = "[" + js + "]";
        }
        return js;
    };

    string sgf2jsonMain(string sgf) {
        node root = _sgf2Node(sgf);
        json j;
        _writeJsonTreeMain(&j, &root);
        string js = j.dump();
        if (starts_with(js, "[[{")) {
            js = "[" + js + "]";
        }
        return js;
    };

    string json2sgf(string js) {
        json j = json::parse(js);
        return _writeSgfTree(&j);
    }
}
