#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>

void cp(const char *msg) {
    static int x = 0;
    std::cout << ++x << " - " << msg << '\n';
}

class GraphNode {
private:
    std::string name_;
    std::vector<GraphNode *> links_;
    bool large;
    short visited = 0;
public:
    GraphNode(const std::string &name) : name_(name) {
        large = (name[0] >= 'A' && name[0] <= 'Z');
        links_ = std::vector<GraphNode *>();
        //std::cout << name_ << " constr\n"; 
    }
    
    void link_to(GraphNode &other) {
        links_.push_back(&other);
    }

    inline const std::string &name() const { return name_; }
    inline const std::vector<GraphNode *> &links() const { return links_; }
    inline bool is_large() const { return large; }
    inline short visits() const { return visited; }
    inline void visit() { visited += !large; /*std::cout << '#' << name_ << " visited\n";*/ }
    inline void unvisit() { visited -= !large; /*std::cout << '#' << name_ << " unvisited\n";*/ }
    
    friend std::ostream& operator<<(std::ostream &ost, const GraphNode &node) {
        ost << node.name_ << " -> [ ";
        for (const auto &n : node.links_)
            ost << n->name() << ' ';        
        ost << ']';     
        return ost;
    }
    
    //~GraphNode() { std::cout << name_ << " destr\n"; }
};

class Graph {   
private:
    GraphNode *start = nullptr, *end = nullptr;
    std::vector<std::unique_ptr<GraphNode>> nodes{};    
    std::unordered_set<std::string> names{};
public:
    void add_link(const std::string &str) {
        const std::size_t end_index = str.find_first_of('-');
        const std::string begin_str = str.substr(0, end_index);
        const std::string end_str = str.substr(end_index + 1);
        
        GraphNode &begin = add_node(begin_str);
        GraphNode &end = add_node(end_str);

        begin.link_to(end);
        end.link_to(begin);
    }
    
    GraphNode &add_node(const std::string &str) {
        if (names.contains(str))
            return get_node(str);
        names.insert(str);
        nodes.push_back(std::make_unique<GraphNode>(str));
        
        if (str == "start")
            start = nodes.back().get();
        else if (str == "end")
            end = nodes.back().get();
        
        return *nodes.back();
    }
    
    GraphNode &get_node(const std::string &str) {
        for (const auto &p : nodes)
            if (p->name() == str)
                return *p;
        throw 1;
    }
    
    std::vector<std::vector<GraphNode *>> find_paths() const {
        std::vector<std::vector<GraphNode *>> paths{};
        std::vector<GraphNode *> path{ start };
        
        start->visit();
        find_paths_rec(*start, path, paths);
        start->unvisit();
        
        return paths;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const Graph &g) {
        for (const auto &node : g.nodes)
            ost << *node << '\n';       
        return ost;
    }
private:
    void find_paths_rec(GraphNode &node, std::vector<GraphNode *> &path,
        std::vector<std::vector<GraphNode *>> &paths, bool has_double = false) const
    {
        for (const auto &n : node.links()) {
            if (n == start || n->visits() > 1 || (n->visits() == 1 && has_double))
                continue;
            bool added_double = false;

            n->visit();
            path.push_back(n);
            
            if (n->visits() == 2)
                added_double = has_double = true;
            if (n == end)
                paths.emplace_back(path);
            else
                find_paths_rec(*n, path, paths, has_double);
                
            n->unvisit();
            path.pop_back();    
            if (added_double)
                has_double = false;
        }
    }
    
    static void print_path(const std::vector<GraphNode *> &path) {
        for (const auto &a : path) 
            std::cout << a->name() << ' ';
        std::cout << '\n';
    }
};

int main() {
    std::string line;
    Graph caves{};
    
    while (std::cin >> line)
        caves.add_link(line);   
    std::cout << caves;
    
    const auto paths = caves.find_paths();
    std::cout << "\npath count = " << paths.size() << "\n\n";   
    
    for (const auto &path : paths) {
        for (const auto &node : path)
            std::cout << node->name() << ' ';
        std::cout << '\n';
    }
}
















