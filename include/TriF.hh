/*=========================================================================

  Program:   Visualization Library
  Module:    TriF.hh
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

This file is part of the Visualization Library. No part of this file
or its contents may be copied, reproduced or altered in any way
without the express written consent of the authors.

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen 1993, 1994 

=========================================================================*/
// .NAME vlTriangleFilter - create triangle polygons from input polygons and triangle strips
// .SECTION Description
// vlTriangleFilter generates triangles from input polygons and triangle 
// strips. The filter will also pass through vertices and lines, if
// requested.

#ifndef __vlTriangleFilter_h
#define __vlTriangleFilter_h

#include "P2PF.hh"

class vlTriangleFilter : public vlPolyToPolyFilter
{
public:
  vlTriangleFilter() : PassVerts(1), PassLines(1) {};
  ~vlTriangleFilter() {};
  char *GetClassName() {return "vlTriangleFilter";};
  void PrintSelf(ostream& os, vlIndent indent);

  // Description:
  // Turn on/off passing vertices through filter.
  vlBooleanMacro(PassVerts,int);
  vlSetMacro(PassVerts,int);
  vlGetMacro(PassVerts,int);

  // Description:
  // Turn on/off passing lines through filter.
  vlBooleanMacro(PassLines,int);
  vlSetMacro(PassLines,int);
  vlGetMacro(PassLines,int);

protected:
  // Usual data generation method
  void Execute();

  int PassVerts;
  int PassLines;
};

#endif


