use solver::requirement::Requirement;

pub enum CourseType {
 Normal,
 Disjunction,
 Null
}

pub type ReqsList = [Requirement];

/// NormalCourse contains its name and requirements.
/// DisjunctionCourse is just named.
#[deriving(Show)]
pub enum Course {
 NormalCourse(&'static str, 
              &'static ReqsList),
 DisjunctionCourse(&'static str)
}
