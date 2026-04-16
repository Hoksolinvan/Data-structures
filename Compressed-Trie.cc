#include <iostream>
#include <string_view>
#include <map>
#include <string>
#include <algorithm>

int longestCommonPrefixLen(std::string_view a, std::string_view b) {
    int n = std::min(a.size(), b.size());
    int i = 0;
    while (i < n && a[i] == b[i]) i++;
    return i;
}

struct Node {
    std::map<std::string, Node*> edges;
    bool is_end;
    Node(bool is_end = true) : is_end(is_end) {}
};

void insert(Node* root, std::string_view current_string) {
    auto& edge_list = root->edges;

    if (edge_list.empty()) {
        edge_list[std::string(current_string)] = new Node(true);
        return;
    }

    std::string best_match;
    int current_max = 0;
    for (const auto& [key, _] : edge_list) {
        int len = longestCommonPrefixLen(key, current_string);
        if (len > current_max) {
            current_max = len;
            best_match = key;
        }
    }

    // No prefix match at all
    if (current_max == 0) {
        edge_list[std::string(current_string)] = new Node();
        return;
    }

    std::string_view first_half  = std::string_view(best_match).substr(0, current_max); // shared prefix
    std::string_view second_half = std::string_view(best_match).substr(current_max);    // remainder of existing edge
    std::string_view third_half  = current_string.substr(current_max);                  // remainder of new string

    Node* old_child = edge_list[best_match];
    edge_list.erase(best_match);

    if (second_half.empty()) {
        edge_list[std::string(first_half)] = old_child;
        insert(old_child, third_half);
        return;
    }

    if (third_half.empty()) {
        Node* intermediate = new Node(true);            
        intermediate->edges[std::string(second_half)] = old_child;
        edge_list[std::string(first_half)] = intermediate;
        return;
    }

    Node* intermediate = new Node(false);        
    intermediate->edges[std::string(second_half)] = old_child;
    intermediate->edges[std::string(third_half)]  = new Node(true);
    edge_list[std::string(first_half)] = intermediate;
}

void print(Node* node, const std::string& prefix = "") {
    if (node->is_end) std::cout << prefix << "\n";
    for (const auto& [key, child] : node->edges)
        print(child, prefix + key);
}
