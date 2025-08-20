#include "Node.h"
#include <iostream>
#include <utility>

Node::Node(int id, const std::string& name) : nodeId(id), nodeName(name) {
}

Node::~Node() {
}

int Node::getId() const {
    return this->nodeId;
}

std::string Node::getName() const {
    return this->nodeName;
}

void Node::setId(int newId) {
    this->nodeId = newId;
}

void Node::setName(const std::string& newName) {
    this->nodeName = newName;
}