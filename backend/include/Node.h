#pragma once

#include <string>
#include <memory>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

class Node : public std::enable_shared_from_this<Node> {
private:
    int nodeId;
    std::string nodeName;

public:
    Node() : nodeId(-1), nodeName("uninitialized") {}
    Node(int id, const std::string& name);
    ~Node();

    int getId() const;
    std::string getName() const;
    void setId(int newId);
    void setName(const std::string& newName);

    template<class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(nodeId), CEREAL_NVP(nodeName));
    }
};
