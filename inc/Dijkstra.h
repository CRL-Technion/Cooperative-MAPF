#include "Instance.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/compute/detail/lru_cache.hpp>
 
typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS,boost::undirectedS,boost::no_property,EdgeWeightProperty> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator edge_iterator;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_descriptor;
typedef std::vector<vertex_descriptor> graph_path;

using namespace boost::compute::detail;

class Dijkstra
{
private:
    const Instance& instance;
    UndirectedGraph m_graph;
    lru_cache<int, std::map<int, int>>* len_cache; // LRU cache to hold length of paths to a specific location

public:
    Dijkstra(const Instance& instance_);
    std::map<int, graph_path> search(int start);
    std::map<int, int> search_l(int start); // returns the length of paths instead of actuall paths
    graph_path construct_path(vertex_descriptor start, vertex_descriptor goal, std::vector<vertex_descriptor> p);
    ~Dijkstra() { delete len_cache; };
};