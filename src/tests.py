import unittest
import courseParser

class TestRegexp(unittest.TestCase):
 def setUp(self):
  self.maxDiff = None

 def test_pmath465(self):
  q = r"""<center><table border=0 width=80%><tr><td align=left><B><a name = "PMATH465"></a>PMATH 465 LEC 0.50</b></td><td align=right>Course ID: 003350</td></tr><tr><td colspan=2><b>Differential Geometry</B></td></tr><tr><td colspan=2>An introduction to differentiable manifolds. The tangent and cotangent bundles. Vector fields and differential forms. The Lie bracket and Lie derivative of vector fields. Exterior differentiation, integration of differential forms, and Stokes's Theorem. Riemannian manifolds, affine connections, and the Riemann curvature tensor. </td></tr><tr><td colspan=2><i>[Note: Offered in the Winter of even years.]</i></td></tr><tr><td colspan=2><i> </i></td></tr><tr><td colspan=2><i>Prereq: AMATH 333/PMATH 365; Not open to General Mathematics students</i></td></tr><p></table></center><Br><P> """
  a = ("PMATH465", "PMATH 465 LEC 0.50", "003350", "Differential Geometry", "An introduction to differentiable manifolds. The tangent and cotangent bundles. Vector fields and differential forms. The Lie bracket and Lie derivative of vector fields. Exterior differentiation, integration of differential forms, and Stokes's Theorem. Riemannian manifolds, affine connections, and the Riemann curvature tensor. ", ["[Note: Offered in the Winter of even years.]", " ", "Prereq: AMATH 333/PMATH 365; Not open to General Mathematics students"])
  self.assertEqual(courseParser.extractCourseInfo(q)[0], a)

 def test_pmath332(self):
  q = r"""<center><table border=0 width=80%><tr><td align=left><B><a name = "PMATH332"></a>PMATH 332 LEC 0.50</b></td><td align=right>Course ID: 003324</td></tr><tr><td colspan=2><b>Applied Complex Analysis</B></td></tr><tr><td colspan=2>Complex numbers, Cauchy-Riemann equations, analytic functions, conformal maps and applications to the solution of Laplace's equation, contour integrals, Cauchy integral formula, Taylor and Laurent expansions, residue calculus and applications. </td></tr><tr><td colspan=2><i>[Note: PMATH 352 may be substituted for AMATH/PMATH 332 whenever the latter is a requirement in an Honours plan. Offered: W,S]</i></td></tr><tr><td colspan=2><i> </i></td></tr><tr><td colspan=2><i>Prereq: MATH 237 or 247; Not open to General Mathematics students.</i></td></tr><tr><td colspan=2><i> Antireq: PHYS 365</i></td></tr><tr><td colspan=2><i>(Cross-listed with AMATH 332)</i></td></tr><P></table></center><Br><P> """
  a = ("PMATH332", "PMATH 332 LEC 0.50", "003324", "Applied Complex Analysis", "Complex numbers, Cauchy-Riemann equations, analytic functions, conformal maps and applications to the solution of Laplace's equation, contour integrals, Cauchy integral formula, Taylor and Laurent expansions, residue calculus and applications. ", ["[Note: PMATH 352 may be substituted for AMATH/PMATH 332 whenever the latter is a requirement in an Honours plan. Offered: W,S]", " ", "Prereq: MATH 237 or 247; Not open to General Mathematics students.", " Antireq: PHYS 365", "(Cross-listed with AMATH 332)"])
  self.assertEqual(courseParser.extractCourseInfo(q)[0], a)

if __name__ == "__main__":
 unittest.main()
