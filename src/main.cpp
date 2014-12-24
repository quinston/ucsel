#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/topological_sort.hpp>
#include<string>
#include<list>
#include<iostream>

enum 
{
CS135,
CS145,
CS146,
CS136,
n_courses
};

const char* noms[] = {"135","145","146","136"};

typedef std::pair<int, int> Edge;


Edge prereqTo[] = 
{{CS135, CS136},
{CS145, CS136},
{CS145, CS146}};



typedef boost::adjacency_list<boost::vecS, 
boost::vecS, 
boost::directedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

Graph g(prereqTo, prereqTo + sizeof(prereqTo) / sizeof(Edge), n_courses);

int main() {
std::list<Vertex> topo_order;
boost::topological_sort(g, std::front_inserter(topo_order));
for (Vertex v : topo_order) {
 std::cout << noms[v] << std::endl;
}
std::cout << std::endl;
return 0;
}
