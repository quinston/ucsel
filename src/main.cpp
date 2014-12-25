#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/topological_sort.hpp>
#include<string>
#include<list>
#include<iostream>
#include<vector>

enum 
{
CS135,
CS145,
CS135orCS145,
CS146,
CS136,
n_courses
};

const char* noms[] = {"135","145","135 or 145","146","136"};

typedef std::pair<int, int> Edge;


Edge prereqTo[] = 
{
{CS135, CS135orCS145},
{CS145, CS135orCS145},
{CS135orCS145, CS136},
{CS145, CS146}};



typedef boost::adjacency_list<boost::vecS, 
boost::vecS, 
boost::bidirectionalS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

Graph g(prereqTo, prereqTo + sizeof(prereqTo) / sizeof(Edge), n_courses);

int main() {
std::list<Vertex> topo_order;
boost::topological_sort(g, std::front_inserter(topo_order));
for (Vertex v : topo_order) {
 std::cout << noms[v] << std::endl;
}
std::cout << std::endl;

std::vector<int> height(n_courses, 0);

for (Vertex v : topo_order) {
 if (in_degree(v, g) != 0) {
  Graph::in_edge_iterator begin, end;
  std::tie(begin, end) = boost::in_edges(v, g);
  auto highestVertex = boost::source(*std::max_element(begin, end, [&height](const boost::graph_traits<Graph>::edge_descriptor& a, const boost::graph_traits<Graph>::edge_descriptor& b) { return height[boost::source(a,g)] < height[boost::source(b,g)]; }), g);
  height[v] = height[highestVertex] + 1;
 }
}

for (int i = 0; i != n_courses; ++i) {
 std::cout << noms[i] << " can be taken no earlier than term " << (height[i] + 1) << std::endl;
}

return 0;
}
