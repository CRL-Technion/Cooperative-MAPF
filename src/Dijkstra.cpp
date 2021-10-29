#include "Dijkstra.h"
#include <map>

Dijkstra::Dijkstra(const Instance& instance_)
: instance(instance_)
{
    // build the graph based on instance
    list<int> neighbors;
    for (int i = 0; i < instance.map_size; ++i)
    {
        if (instance.isObstacle(i))
            continue;
        neighbors = instance.getNeighbors(i);
        for (int n : neighbors)
            boost::add_edge(i, n, 1, m_graph);
    }

    len_cache = new lru_cache<int, std::map<int, int>>(instance.getDefaultNumberOfTasks() * 4); // we need at most 4*num_of_tasks cache entries

    // std::pair<edge_iterator, edge_iterator> ei = edges(m_graph);
 
    // std::cout << "Number of edges = " << num_edges(m_graph) << "\n";
    // std::cout << "Edge list:\n";
 
    // for (edge_iterator it = ei.first; it != ei.second; ++it )
    // {
    //     std::cout << *it << std::endl;
    // }
 
    // std::cout << std::endl;
}


std::map<int, graph_path> Dijkstra::search(int start)
{
    std::vector<vertex_descriptor> p(boost::num_vertices(m_graph));
    std::vector<int> d(boost::num_vertices(m_graph));
    vertex_descriptor s = boost::vertex(start, m_graph);

    boost::dijkstra_shortest_paths(m_graph, s, 
      boost::predecessor_map(&p[0]).distance_map(&d[0]));

    std::map<int, graph_path> paths;
    for (int loc = 0; loc < p.size(); ++loc)
    {
        if (!instance.isObstacle(loc))
        {
            graph_path path = construct_path(s, boost::vertex(loc, m_graph), p);
            paths.insert(std::pair<int, graph_path>(loc, path));
        } 
    }

    return paths;

    // vertex_descriptor goal = vertex(654, m_graph);
    
 
    // // Prints the path obtained in reverse
    // std::cout << "Path from " << s << " to " << goal << std::endl;
    // std::vector<vertex_descriptor>::reverse_iterator it;
 
    // for (it = path.rbegin(); it != path.rend(); ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;
}

std::map<int, int> Dijkstra::search_l(int start)
{
    if (len_cache->contains(start))
        return *len_cache->get(start);

    std::vector<vertex_descriptor> p(boost::num_vertices(m_graph));
    std::vector<int> d(boost::num_vertices(m_graph));
    vertex_descriptor s = boost::vertex(start, m_graph);

    boost::dijkstra_shortest_paths(m_graph, s, 
      boost::predecessor_map(&p[0]).distance_map(&d[0]));

    std::map<int, int> lengths;
    for (int loc = 0; loc < p.size(); ++loc)
    {
        if (!instance.isObstacle(loc))
        {
            graph_path path = construct_path(s, boost::vertex(loc, m_graph), p);
            lengths.insert(std::pair<int, int>(loc, path.size() - 1));
        } 
    }

    len_cache->insert(start, lengths);

    return lengths;
}

graph_path Dijkstra::construct_path(vertex_descriptor start, vertex_descriptor goal, std::vector<vertex_descriptor> p)
{
    graph_path path;
    vertex_descriptor current = goal;

    while(current != start) 
    {
        path.push_back(current);
        current = p[current];
    }
    path.push_back(start);

    return path;
}