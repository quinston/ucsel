extern crate graph; 

use self::graph::{Graph, NodeIdentifier};
use solver::course::Course;
use solver::course::Course::{NormalCourse, DisjunctionCourse};
use std::collections::{HashSet, BTreeMap, HashMap};

type TermCode_t = u16;

#[deriving(Clone,Show)]
pub struct Solution {
 /// Contains courses that are taken, in the order they were taken.
 /// Use term_to_starting_index to divide this monolithic list into terms.
 courses_taken_ordered : Vec<NodeIdentifier>,
 /// Contains courses that are taken
 courses_taken_set : HashSet<NodeIdentifier>
}

impl Solution {
 pub fn new() -> Solution {
  Solution{courses_taken_ordered: Vec::new(),
courses_taken_set: HashSet::new()}
 }
}

pub type TermToTermSize_t = BTreeMap<TermCode_t, u16>;
pub type TermToStartingIndex_t = BTreeMap<TermCode_t, u16>;
pub type VertexToInnodes_t = HashMap<NodeIdentifier, Vec<NodeIdentifier>>;
pub type Graph_t = Graph<'static, Course, ()>;

/// V is the data to hold in edges.
pub struct SolverParams {
 /// A map of term codes to quantity of courses the user plans to take
 /// in that term.
 pub term_to_term_size : TermToTermSize_t,
 /// A map of term codes to the zero-indexed
 /// position of the first course in each term
 /// in a list of all the courses the user has taken. 
 /// e.g. If term 100 contains four courses, and term 101 contains
 /// nine courses, then the pair (101, 4) will exist in the map.
 pub term_to_starting_index : TermToStartingIndex_t,
 /// A map of vertices to their in-nodes
 pub vertex_to_innodes : VertexToInnodes_t,
 /// A graph representing course prerequisites by directed edges from the
 /// prerequisite to the course in question
 pub g: Graph_t
}

pub fn add_edge_and_update_inedge_map(g: &mut Graph_t, 
 from_index: NodeIdentifier,
 to_index: NodeIdentifier, 
 vertex_to_innodes: &mut VertexToInnodes_t)
{
 g.connect(from_index, to_index, ());
 


 if (vertex_to_innodes.contains_key(&to_index)) {
  vertex_to_innodes.get_mut(&to_index).unwrap().push(from_index);
 }
 else {vertex_to_innodes.insert(to_index, vec![from_index]); }
}


pub fn calculate_term_starting_indices( 
term_to_term_size : &TermToTermSize_t)
-> TermToStartingIndex_t {
 // atm typedef static method calls are not supported.
 let mut ret: TermToStartingIndex_t = BTreeMap::new();
 // Minimum element, so that the first start index is 0
 let mut acc = -term_to_term_size.keys().next().unwrap().clone();
 for (term_no, size) in term_to_term_size.iter() {
  acc += size.clone();
  ret.insert(term_no.clone(), acc);
 }
 ret
}

/// Returns true if the Course associated with v can be taken
/// A vertex is _takeable_ if it can be considered as the next trial node
/// in a partial solution.
/// Disjunction nodes are not takeable.
fn is_takeable(v: &NodeIdentifier, 
sp: &SolverParams, 
s: &Solution) -> bool
{
 let ref g = sp.g;
 match *g.get(*v) {
  NormalCourse(_, reqs) => 
reqs.iter().all(|r| match r {
  TakeAllPrereqCourses =>
   match sp.vertex_to_innodes.get(v) {
    // No prereqs
    None => true,
    Some(pr) => pr.iter().all(|x| is_taken(x, sp, s))
   }
  }),
  DisjunctionCourse(_) => false
 }
}

/// A vertex is _taken_ if its outnodes can be considered for addition
/// to the partial solution.
fn is_taken(v: &NodeIdentifier, 
sp: &SolverParams,
s: &Solution) -> bool
{
 let ref g = sp.g;
 match *g.get(*v) {
  NormalCourse(_,_) => 
   s.courses_taken_set.contains(v),
  DisjunctionCourse(_) => 
   // This cannot panic as a disjunction node must not be a leaf.
   sp.vertex_to_innodes[*v].iter().any(|x| is_taken(x, sp, s))
 }
}

/// Returns the list of all the vertices corresponding to the courses
/// that can be immediately taken.
fn relax(extreme_vertices: &Vec<NodeIdentifier>,
sp: &SolverParams,
s: &Solution) -> Vec<NodeIdentifier> {
 let mut ret = Vec::new();
 let ref g = sp.g;

 let mut ev = extreme_vertices.clone(); 
 let mut i = 0u;

 while i < ev.len() {
  let v = ev[i];

  if is_taken(&v, sp, s) {
   for successor in g.connections(v).iter()
.map(|e| e.dest) {
    match *g.get(successor) {
     NormalCourse(_,_) => 
if is_takeable(&successor, sp, s) {
 ret.push(successor);    
},
     DisjunctionCourse(_) => {
      for out_edge in g.connections(successor).iter() {
       ev.push(out_edge.dest);
      }
}
    }
   }
  }
  else if is_takeable(&v, sp, s) {
   ret.push(v);
  }

  i += 1;
 }
 ret
}

/// Finds all course configurations that have the courses in 
/// required_courses. The solutions are accumulated in found_solutions.
/// extreme_vertices should contain all courses that can be taken immediately
/// (i.e. no outstanding prereqs).
/// required_courses contains course codes.
pub fn find_solutions(
 sp: &SolverParams,
 s: &mut Solution, 
 required_courses: &Vec<NodeIdentifier>, 
 extreme_vertices: &Vec<NodeIdentifier>,
 found_solutions: &mut Vec<Solution>) {
 // No need to keep investigating; the remaining empty slots are the user's
 // to fill
 if required_courses.iter().all(|cc| s.courses_taken_set.contains(cc)) {
  found_solutions.push(s.clone());
 }
 else {
  let new_extreme_vertices = relax(extreme_vertices, sp, s);
  // If not out of choices
  for v in new_extreme_vertices.iter() {
   s.courses_taken_ordered.push(*v);
   s.courses_taken_set.insert(*v);
   find_solutions(sp, s, required_courses, &new_extreme_vertices,  
found_solutions); 
   s.courses_taken_ordered.pop();
   s.courses_taken_set.remove(v);
  }
 }
}

