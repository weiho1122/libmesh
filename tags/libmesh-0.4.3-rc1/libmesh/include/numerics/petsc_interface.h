// $Id: petsc_interface.h,v 1.3 2004-04-17 03:02:50 benkirk Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2004  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __petsc_interface_h__
#define __petsc_interface_h__

// C++ includes

// Local includes
#include "linear_solver_interface.h"
#include "petsc_vector.h"
#include "petsc_matrix.h"


/**
 * Petsc include files.
 */
#ifdef HAVE_PETSC

#ifndef USE_COMPLEX_NUMBERS
extern "C" {
# include <petscversion.h>
# if (PETSC_VERSION_MAJOR == 2) && (PETSC_VERSION_MINOR <= 1)
#   include <petscsles.h>
# else
#   include <petscksp.h>
# endif
}
#else
# include <petscversion.h>
# if (PETSC_VERSION_MAJOR == 2) && (PETSC_VERSION_MINOR <= 1)
#   include <petscsles.h>
# else
#   include <petscksp.h>
# endif
#endif



/**
 * This class provides a deal.II interface to the Petsc
 * iterative solver library.
 *
 * @author Benjamin Kirk, 2002
 */

template <typename T>
class PetscInterface : public LinearSolverInterface<T>
{
public:
  /**
   *  Constructor. Initializes Petsc data structures
   */
  PetscInterface ();
    
  /**
   * Destructor.
   */
  ~PetscInterface ();
  
  /**
   * Release all memory and clear data structures.
   */
  void clear ();

  /**
   * Initialize data structures if not done so already.
   */
  void init ();
  
  /**
   * Call the Petsc solver
   */    
  std::pair<unsigned int, Real> 
  solve (SparseMatrix<T> &matrix,
	 NumericVector<T> &solution,
	 NumericVector<T> &rhs,
	 const double tol,
	 const unsigned int m_its);
   
private:

  /**
   * Tells PETSC to use the user-specified solver stored in
   * \p _solver_type
   */
  void set_petsc_solver_type ();

  /**
   * Tells PETSC to use the user-specified preconditioner stored in
   * \p _preconditioner_type
   */
  void set_petsc_preconditioner_type ();

  // SLES removed from >= PETSc 2.2.0
#if (PETSC_VERSION_MAJOR == 2) && (PETSC_VERSION_MINOR <= 1)
  
  /**
   * Linear solver context
   */
  SLES _sles;

#endif
  
  /**
   * Preconditioner context
   */
  PC _pc; 

  /**
   * Krylov subspace context
   */
  KSP _ksp;
};


/*----------------------- functions ----------------------------------*/
template <typename T>
inline
PetscInterface<T>::PetscInterface ()
{
  if (libMesh::n_processors() == 1)
    this->_preconditioner_type = ILU_PRECOND;
  else
    this->_preconditioner_type = BLOCK_JACOBI_PRECOND;
}



template <typename T>
inline
PetscInterface<T>::~PetscInterface ()
{
  this->clear ();
}



#endif // #ifdef HAVE_PETSC
#endif // #ifdef __petsc_interface_h__