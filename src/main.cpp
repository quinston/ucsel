#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/topological_sort.hpp>
#include<string>
#include<list>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<set>
#include "Course.h"

// Maps course enums to either the course ID (e.g. 1139)
// or NEVER_TAKEN
typedef std::unordered_map<int, int> WholePlan;


enum 
{
CS135 = 0,
CS145,
CS135orCS145,
CS146,
CS136,
n_courses
};

std::string noms[] = {"135","145","135 or 145","146","136"};

typedef Course::CourseType CourseType;

const CourseType cts[] = {CourseType::Normal, CourseType::Normal, CourseType::Disjunction, CourseType::Normal, CourseType::Normal };

typedef std::pair<int, int> EdgePair;


EdgePair prereqTo[] = 
{
{CS135, CS135orCS145},
{CS145, CS135orCS145},
{CS135orCS145, CS136},
{CS145, CS146}
};



typedef boost::adjacency_list<boost::vecS, 
boost::vecS, 
boost::bidirectionalS,
Course> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;




bool is_taken(const Vertex&, const Graph&);

/**
Returns true if the Course associated with v can be taken, according
to the current state of the graph g.
*/
bool is_takeable(const Vertex& v,const Graph& g ) {

 Graph::in_edge_iterator i, end; 
std::tie(i, end) = boost::in_edges(v, g);
 switch (g[v].ct_) {
 case CourseType::Normal:
 for (; i != end; ++i) {
  if (!is_taken(boost::source(*i, g),g) ) {
   return false;
  }
 }
 return true;

 case CourseType::Disjunction:
  // can't manually take a disjunction.
  // or change paradigm: can take a disjunction but not manually
   for (; i != end; ++i) {
  if (is_taken(boost::source(*i, g), g)) {
   return true;
  }
 }
 return false;

 }

}

bool is_taken(const Vertex& v, const Graph& g) {
 switch (g[v].ct_) {
  case CourseType::Normal:
   return g[v].is_taken_;
  case CourseType::Disjunction:
   return is_takeable(v,g);
}
}



/**
Returns a list of vertices that can be reached from any vertex in
 extreme_vertices due to the fulfilment of their prerequisites.

extreme_vertices should contain no disjunction vertices.

extreme_vertices may be destroyed.
*/
std::list<Vertex> relax(

 std::list<Vertex>& extreme_vertices,
 Graph& g) {
 std::list<Vertex> ret;
 for (Vertex v : extreme_vertices) {
  std::cout << "considering an extreme " << g[v].name_ << std::endl;
  if ( is_taken(v,g) ) {
   Graph::out_edge_iterator i, end;
   for (std::tie(i, end) = boost::out_edges(v, g); i != end; ++i) {
    auto successor = boost::target(*i, g);
    std::cout << "considering a successor " << g[successor].name_ << std::endl;
    if (is_takeable(successor, g)) {

std::cout << "pushed/enqueued the scucessor" << std::endl;
     switch (g[successor].ct_) {
      case Course::CourseType::Normal:
       ret.push_back(successor);
       break;
      /* We can't explicitly take a disjunction course, so we must relax 
further and consider its own successors */
      case Course::CourseType::Disjunction:
       extreme_vertices.push_back(successor);
       break;
     }
    } 
   } 
  }
  else if (is_takeable(v,g)) {
std::cout << "is takeable!: " << g[v].name_ << std::endl;
   ret.push_back(v);
  }
 }


std::cout << "takeable courses: " << ret.size() << std::endl;

return ret;
}


std::ostream& operator<<(std::ostream& out, std::pair<std::vector<Vertex>*, Graph*> vvg) {
std::vector<Vertex>& vv = * (vvg.first);
Graph& g = * (vvg.second);
out << "{" << std::endl;
for (auto v : vv) {
out << g[v].name_ << std::endl;
}
out << "}" << std::endl;
return out;
}


bool find_solution(std::vector<Vertex>& base, std::vector<Vertex> required_courses,
int n_courses_completed, std::list<Vertex>& extreme_vertices,
Graph& g) {
std::cout << make_pair(&base, &g) ;


 bool is_finished = true;
 // If all the required courses have been taken, we have found a solution! 
 for (Vertex v : required_courses) {
  is_finished = g[v].is_taken_;
  if (!is_finished) break;
 }
 if (is_finished) {
 std::cout << "solution found!" << std::endl;
  return true;
 }

 // Keep going.
 
 // Collect all possible options by relaxing frontier vertices.
 std::list<Vertex> new_extreme_vertices = relax(extreme_vertices, g);
 std::cout << "after moving, # of takeables: " << new_extreme_vertices.size() << std::endl;
 if (new_extreme_vertices.empty()) {
  std::cout << "Out of choices " << std::endl;
  return false;
 }

 for (Vertex v : new_extreme_vertices) {
  g[v].is_taken_ = true;
  base.push_back(v);
  if (find_solution(base, required_courses, n_courses_completed + 1,
   new_extreme_vertices, g)) {
   std::cout << "solution found below." << std::endl;
   return true;
  }
  else {
   std::cout << "came up with nothing, try another extreme vertex."<<std::endl;
   g[v].is_taken_ = false;
   base.pop_back();
  }
 }
 
 return false;
}

int main() {
std::map<std::string, Vertex> id_to_vertex;


Graph g(0);

for (EdgePair ep : prereqTo) {

decltype(id_to_vertex)::iterator iter;
Vertex u, v;
 /* Try inserting source vertex. */
 iter = id_to_vertex.find(noms[ep.first]);
 if (iter == id_to_vertex.end()) {
  u = id_to_vertex[noms[ep.first]] = boost::add_vertex(g);
std::cout << boost::num_vertices(g) << std::endl;
  g[u].name_ = noms[ep.first];
  g[u].ct_ = cts[ep.first];
 }
 else {
 u = iter->second;
 }
 
 /* Try inserting target vertex */
iter = id_to_vertex.find(noms[ep.second]);
 if (iter == id_to_vertex.end()) {
   v = id_to_vertex[noms[ep.second]] = boost::add_vertex(g);
std::cout << boost::num_vertices(g) << std::endl;
  g[v].name_ = noms[ep.second];
  g[v].ct_ = cts[ep.second];
 }
 else {
 v = iter->second;
 }

 boost::add_edge(u, v, g);
}

std::cout << "I got me " << boost::num_vertices(g) << " vertices" << std::endl;


std::list<Vertex> topo_order;

boost::topological_sort(g, std::front_inserter(topo_order));

int cccccount = 0;

for (Vertex v : topo_order) {
 std::cout << "#" << ++cccccount << std::endl;
 std::cout << g[v].name_ << std::endl;
}
std::cout << std::endl;

std::cout << "what is a vertex: " << sizeof(Vertex) << "B" << std::endl;
std::map<Vertex, int> height;

for (Vertex v : topo_order) {
 if (in_degree(v, g) != 0) {
  Graph::in_edge_iterator begin, end;
  std::tie(begin, end) = boost::in_edges(v, g);
  auto highestVertex = boost::source(*std::max_element(begin, end, [&height, &g](const Edge& a, const Edge& b) { return height[boost::source(a,g)] < height[boost::source(b,g)]; }), g);
  height[v] = height[highestVertex] + 1;
 }
}

for (auto p : height) {
 std::cout << g[p.first].name_ << " can be taken no earlier than term " << (p.second + 1) << std::endl;
}


std::vector<Vertex> schedule;
std::list<Vertex> extreme_vertices_0 = {id_to_vertex["135"]
, id_to_vertex["145"]};
find_solution(schedule, {id_to_vertex["136"]}, 0, 
extreme_vertices_0, g);

std::cout << "THE PLAN!!!"<<std::endl;


return 0;
}
