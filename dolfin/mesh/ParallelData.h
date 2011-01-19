// Copyright (C) 2011 Anders Logg and Garth N. Wells.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2011-01-17
// Last changed: 2011-01-17

#ifndef __PARALLEL_DATA_H
#define __PARALLEL_DATA_H

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include "MeshFunction.h"

namespace dolfin
{

  class Mesh;

  /// This class stores auxiliary mesh data for parallel computing.

  class ParallelData
  {
  public:

    /// Constructor
    ParallelData(const Mesh& mesh);

    /// Destructor
    ~ParallelData();

    //--- Data for distributed memory parallelism ---


    //--- Data for shared memory parallelism (multicore) ---

    /// Return the number of colors for entities of dimension D
    /// colored by entities of dimension d and coloring distance rho.
    uint num_colors(uint D, uint d, uint rho) const;

    /// Return colors for entities of dimension D colored by entities
    /// of dimension d and coloring distance rho (const version).
    MeshFunction<uint>& entity_colors(uint D, uint d, uint rho);

    /// Return colors for entities of dimension D colored by entities
    /// of dimension d and coloring distance rho (const version).
    const MeshFunction<uint>& entity_colors(uint D, uint d, uint rho) const;

    /// Return an array of colored entities for each color in the
    /// range 0, 1, ..., num_colors -1 for entities of dimension D
    /// colored by entities of dimension d and coloring distance rho.
    std::vector<std::vector<uint > >& colored_entities(uint D, uint d, uint rho);

    /// Return an array of colored entities for each color in the
    /// range 0, 1, ..., num_colors for entities of dimension D
    /// colored by entities of dimension d and coloring distance rho
    /// (const version).
    const std::vector<std::vector<uint > >& colored_entities(uint D, uint d, uint rho) const;

  private:

    // Some typedefs for complex types
    typedef boost::tuple<uint, uint, uint> tuple_type;
    typedef std::map<tuple_type, MeshFunction<uint> > entity_colors_map_type;
    typedef std::map<tuple_type, std::vector<std::vector<uint> > > colored_entities_map_type;

    // The mesh
    const Mesh& _mesh;

    // Map to entity colors
    entity_colors_map_type _entity_colors;

    // Map to colored entities
    colored_entities_map_type _colored_entities;

  };

}

#endif