#pragma once

#include <string>
#include <vector>

class Node {
private:
    int nodeId;
    std::string nodeName;

public:
    Node(int id, const std::string& name);

    ~Node();


    int getId() const;
    std::string getName() const;

    void setId(int newId);
    void setName(const std::string& newName);

};
