
mod solver;

#[cfg(test)]
mod tests{

use solver::search::{SolverParams, Solution, TermToTermSize_t,
TermToStartingIndex_t, VertexToInnodes_t, Graph_t};
use solver::search::graph::{Graph, NodeIdentifier};
use solver::course::Course;
use solver::requirement::Requirement;
use solver::search;
 use std::collections::{HashMap, BTreeMap, HashSet};
use solver::course::ReqsList;
 
const empty: &'static [Requirement,..1] = &[Requirement::TakeAllPrereqCourses];

 #[test]
 fn small_search() {
  let (cs135,cs145,cs146,cs136,cs1X5) = 
  (Course::NormalCourse("cs135", empty),
   Course::NormalCourse("cs145", empty),
   Course::NormalCourse("cs146", empty),
   Course::NormalCourse("cs136", empty),
   Course::DisjunctionCourse("cs135 or cs145"));
  
  let mut vi : VertexToInnodes_t  = HashMap::new();
  let mut g : Graph_t = Graph::new();
  let (v_cs135,v_cs136,v_cs146,v_cs1X5,v_cs145) = (g.insert(cs135), 
g.insert(cs136),
  g.insert(cs146), g.insert(cs1X5),
   g.insert(cs145));
search::add_edge_and_update_inedge_map(&mut g, v_cs135, v_cs1X5, &mut vi);
search::add_edge_and_update_inedge_map(&mut g, v_cs145, v_cs1X5, &mut vi);
search::add_edge_and_update_inedge_map(&mut g, v_cs145, v_cs146, &mut vi);
search::add_edge_and_update_inedge_map(&mut g, v_cs1X5, v_cs136, &mut vi);

 for (i,course) in g.iter().enumerate() {
  println!("{} => {}", i, course);
 } 

/*
Find solutions for CS 146
*/
  
  let sp = search::SolverParams{ term_to_term_size: BTreeMap::new(),
term_to_starting_index: BTreeMap::new(), 
vertex_to_innodes: vi,
g: g};
  let mut s = search::Solution::new();
  let required_courses = vec![v_cs146];
  let ev = vec![v_cs135, v_cs145];
  let mut slns = Vec::new();

  search::find_solutions(&sp, &mut s, &required_courses, &ev, &mut slns);

 for s in slns.iter() {
  println!("{}", s);
 }

/*
Find solutions for CS 136
*/
 let mut s2 = search::Solution::new();
 let rc2 = vec![v_cs136];
 let mut slns2 = Vec::new();

 search::find_solutions(&sp, &mut s2, &rc2, &ev, &mut slns2);
 
 for s in slns2.iter() {
  println!("{}", s);
 }

 

 }
}
