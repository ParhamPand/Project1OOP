#pragma once

#include <string>
#include <vector>

using namespace std;

class Node {
private:
    int nodeId;
    string nodeName;

public:
    Node(int id, const string& name);

    ~Node();


    int getId() const;
    string getName() const;

    void setId(int newId);
    void setName(const string& newName);

};

