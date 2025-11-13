#pragma once

#include <vector>

namespace ecim {
    class Node {
    public:
        static int nextId;

        int Id;
        double Voltage;

        Node() : Id(nextId++), Voltage(0.0) {}
    };
    
    // Initialize static member
    inline int Node::nextId = 0; // 0 reserved for ground node
}
