"""This program illustrates the use of the DOLFIN nonlinear solver for solving 
problems of the form F(u) = 0. The user must provide functions for the 
function (Fu) and update of the (approximate) Jacobian.  

This simple program solves a nonlinear variant of Poisson's equation

     - div (1+u^2) grad u(x, y) = f(x, y)

on the unit square with source f given by

     f(x, y) = t * x * sin(y)

and boundary conditions given by

     u(x, y)     = t  for x = 0
     du/dn(x, y) = 0  otherwise

where t is pseudo time.

This is equivalent to solving: 
F(u) = (grad(v), (1-u^2)*grad(u)) - f(x,y) = 0

Original implementation: ../cpp/main.cpp by Garth N. Wells.
"""

__author__ = "Kristian B. Oelgaard (k.b.oelgaard@tudelft.nl) and Garth N. Wells (gnw20@cam.ac.uk)"
__date__ = "2007-11-15 -- 2008-12-12"
__copyright__ = "Copyright (C) 2007-2008 Kristian B. Oelgaard and Garth N. Wells"
__license__  = "GNU LGPL Version 2.1"

# Modified by Anders Logg, 2008.

from dolfin import *

# Create mesh and define function space
mesh = UnitSquare(64, 64)
V = FunctionSpace(mesh, "CG", 1)

# Sub domain for Dirichlet boundary condition
class DirichletBoundary(SubDomain):
    def inside(self, x, on_boundary):
        return abs(x[0] - 1.0) < DOLFIN_EPS and on_boundary

# User defined nonlinear problem 
class MyNonlinearProblem(NonlinearProblem):
    def __init__(self, V, bc, u0, f):
        NonlinearProblem.__init__(self)

        # Define variational problem
        v = TestFunction(V)
        u = TrialFunction(V)
        a = (1.0 + u0*u0)*dot(grad(v), grad(u))*dx + 2.0*u0*u*dot(grad(v), grad(u0))*dx
        L = v*f*dx - (1.0 + u0*u0)*dot(grad(v), grad(u0))*dx

        self.a = a
        self.L = L
        self.mesh = V.mesh()
        self.bc = bc
 
    # User defined assemble of residual 
    def F(self, b, x):
        assemble(self.L, tensor=b, reset_tensor=True)
        self.bc.apply(b, x)

    # User defined assemble of Jacobian 
    def J(self, A, x):
        assemble(self.a, mesh, tensor=A, reset_tensor=True)
        self.bc.apply(A)

# Solve nonlinear problem in a series of steps using pseudo time 
t = Time(0.0)
dt = 1.0
T = 3.0

# Create source function
f = Function(V, "t*x[0]*sin(x[1])")
f.t = 0.0

# Dirichlet boundary conditions
dirichlet_boundary = DirichletBoundary()
g = Function(V, "t")
g.t = 0.0
bc = DirichletBC(V, g, dirichlet_boundary)

# Create solution Function
u = Function(V)
x = u.vector()

# Create user-defined nonlinear problem
nonlinear_problem = MyNonlinearProblem(V, bc, u, f)

# Create nonlinear solver and set parameters
nonlinear_solver = NewtonSolver(bicgstab, amg_hypre)
nonlinear_solver.set("Newton maximum iterations", 50)
nonlinear_solver.set("Newton relative tolerance", 1e-10)
nonlinear_solver.set("Newton absolute tolerance", 1e-10)

# Solve 
while t < T:
    t += dt
    nonlinear_solver.solve(nonlinear_problem, x)

# Plot solution
plot(u, interactive=True)
interactive()

# Save function to file
file = File("nonlinear_poisson.pvd")
file << u
