// Copyright (C) 2010-2011 Anders Logg, Marie Rognes and Garth N. Wells
//
// This file is part of DOLFIN.
//
// DOLFIN is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// DOLFIN is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with DOLFIN. If not, see <http://www.gnu.org/licenses/>.
//
// First added:  2010-02-10
// Last changed: 2011-10-11
//
// This file defines free functions for refinement/adaption of meshes,
// function spaces, functions etc.

#ifndef __DOLFIN_ADAPT_H
#define __DOLFIN_ADAPT_H

#include <vector>

namespace dolfin
{

  // Forward declarations
  class DirichletBC;
  class ErrorControl;
  class Form;
  class Function;
  class FunctionSpace;
  class GenericFunction;
  class LinearVariationalProblem;
  class Mesh;
  template <typename T> class MeshFunction;
  class NonlinearVariationalProblem;

  //--- Refinement of meshes ---

  /// Refine mesh uniformly
  std::shared_ptr<Mesh> adapt(const Mesh& mesh);

  /// Refine mesh based on cell markers
  std::shared_ptr<Mesh> adapt(const Mesh& mesh,
                              const MeshFunction<bool>& cell_markers);

  //--- Refinement of function spaces ---

  /// Refine function space uniformly
  std::shared_ptr<FunctionSpace> adapt(const FunctionSpace& space);

  /// Refine function space based on cell markers
  std::shared_ptr<FunctionSpace> adapt(const FunctionSpace& space,
                                       const MeshFunction<bool>& cell_markers);

  /// Refine function space based on refined mesh
  std::shared_ptr<FunctionSpace> adapt(const FunctionSpace& space,
                                       std::shared_ptr<const Mesh> adapted_mesh);

  //--- Refinement of functions ---

  /// Adapt Function based on adapted mesh
  ///
  /// *Arguments*
  ///     function  (_Function_)
  ///         The function that should be adapted
  ///     adapted_mesh  (_Mesh_)
  ///         The new mesh
  ///     interpolate (bool)
  ///         Optional argument, default is true. If false, the
  ///         function's function space is adapted, but the values are
  ///         not interpolated.
  ///
  /// *Returns*
  ///     _Function__
  ///         The adapted function
  std::shared_ptr<Function> adapt(const Function& function,
                                  std::shared_ptr<const Mesh> adapted_mesh,
                                  bool interpolate=true);

  /// Refine GenericFunction based on refined mesh
  std::shared_ptr<GenericFunction>
    adapt(std::shared_ptr<const GenericFunction> function,
          std::shared_ptr<const Mesh> adapted_mesh);

  /// Refine mesh function<std::size_t> based on mesh
  std::shared_ptr<MeshFunction<std::size_t>>
    adapt(const MeshFunction<std::size_t>& mesh_function,
          std::shared_ptr<const Mesh> adapted_mesh);

  //--- Refinement of boundary conditions ---

  /// Refine Dirichlet bc based on refined mesh
  std::shared_ptr<DirichletBC> adapt(const DirichletBC& bc,
                                     std::shared_ptr<const Mesh> adapted_mesh,
                                     const FunctionSpace& S);

  /// Helper function for refinement of boundary conditions
  void adapt_markers(std::vector<std::size_t>& refined_markers,
                     const Mesh& adapted_mesh,
                     const std::vector<std::size_t>& markers,
                     const Mesh& mesh);

  //--- Refinement of forms ---

  /// Adapt form based on adapted mesh
  ///
  /// *Arguments*
  ///     form  (_Form_)
  ///         The form that should be adapted
  ///     adapted_mesh  (_Mesh_)
  ///         The new mesh
  ///     adapt_coefficients (bool)
  ///         Optional argument, default is true. If false, the form
  ///         coefficients are not explicitly adapted, but pre-adapted
  ///         coefficients will be transferred.
  ///
  /// *Returns*
  ///     _Form__
  ///         The adapted form
  std::shared_ptr<Form> adapt(const Form& form,
                              std::shared_ptr<const Mesh> adapted_mesh,
                              bool adapt_coefficients=true);

  //--- Refinement of variational problems ---

  /// Refine linear variational problem based on mesh
  std::shared_ptr<LinearVariationalProblem>
    adapt(const LinearVariationalProblem& problem,
          std::shared_ptr<const Mesh> adapted_mesh);

  /// Refine nonlinear variational problem based on mesh
  std::shared_ptr<NonlinearVariationalProblem>
    adapt(const NonlinearVariationalProblem& problem,
          std::shared_ptr<const Mesh> adapted_mesh);

  /// Adapt error control object based on adapted mesh
  ///
  /// *Arguments*
  ///     ec  (_ErrorControl_)
  ///         The error control object to be adapted
  ///     adapted_mesh  (_Mesh_)
  ///         The new mesh
  ///     adapt_coefficients (bool)
  ///         Optional argument, default is true. If false, any form
  ///         coefficients are not explicitly adapted, but pre-adapted
  ///         coefficients will be transferred.
  ///
  /// *Returns*
  ///     _ErrorControl__
  ///         The adapted error control object
  std::shared_ptr<ErrorControl> adapt(const ErrorControl& ec,
                                      std::shared_ptr<const Mesh> adapted_mesh,
                                      bool adapt_coefficients=true);


}

#endif
