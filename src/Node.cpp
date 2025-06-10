#include "Node.h"
#include <iostream>
#include <utility>

using namespace std;

Node::Node(int id, const string& name) : nodeId(id), nodeName(name) {
}


Node::~Node() {
}

int Node::getId() const {
    return this->nodeId;
}

string Node::getName() const {
    return this->nodeName;
}


void Node::setId(int newId) {
    this->nodeId = newId;
}

void Node::setName(const string& newName) {
    this->nodeName = newName;
}
