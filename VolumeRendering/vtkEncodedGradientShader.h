/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkEncodedGradientShader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkEncodedGradientShader - Compute shading tables for encoded normals.
//
// .SECTION Description
// vtkEncodedGradientShader computes shading tables for encoded normals 
// that indicates the amount of diffuse and specular illumination that is 
// received from all light sources at a surface location with that normal.
// For diffuse illumination this is accurate, but for specular illumination
// it is approximate for perspective projections since the center view
// direction is always used as the view direction. Since the shading table is
// dependent on the volume (for the transformation that must be applied to
// the normals to put them into world coordinates) there is a shading table
// per volume. This is necessary because multiple volumes can share a 
// volume mapper.

#ifndef __vtkEncodedGradientShader_h
#define __vtkEncodedGradientShader_h

#include "vtkObject.h"

class vtkVolume;
class vtkRenderer;
class vtkEncodedGradientEstimator;

#define VTK_MAX_SHADING_TABLES   100

class VTK_VOLUMERENDERING_EXPORT vtkEncodedGradientShader : public vtkObject
{
public:
  static vtkEncodedGradientShader *New();
  vtkTypeMacro(vtkEncodedGradientShader,vtkObject);

  // Description:
  // Print the vtkEncodedGradientShader
  void PrintSelf( ostream& os, vtkIndent indent );

  // Description:
  // Set / Get the intensity diffuse / specular light used for the
  // zero normals. 
  vtkSetClampMacro( ZeroNormalDiffuseIntensity,  float, 0.0, 1.0);
  vtkGetMacro( ZeroNormalDiffuseIntensity, float );
  vtkSetClampMacro( ZeroNormalSpecularIntensity, float, 0.0, 1.0);
  vtkGetMacro( ZeroNormalSpecularIntensity, float );

  // Description:
  // Cause the shading table to be updated
  void UpdateShadingTable( vtkRenderer *ren, vtkVolume *vol,
                           vtkEncodedGradientEstimator *gradest);

  // Description:
  // Get the red/green/blue shading table.
  float *GetRedDiffuseShadingTable(    vtkVolume *vol );
  float *GetGreenDiffuseShadingTable(  vtkVolume *vol );
  float *GetBlueDiffuseShadingTable(   vtkVolume *vol );
  float *GetRedSpecularShadingTable(   vtkVolume *vol );
  float *GetGreenSpecularShadingTable( vtkVolume *vol );
  float *GetBlueSpecularShadingTable(  vtkVolume *vol );

  // Description:
  // Set the active component for shading. This component's 
  // ambient / diffuse / specular / specular power values will
  // be used to create the shading table. The default is 1.0
  vtkSetClampMacro( ActiveComponent, int, 0, 3 );
  vtkGetMacro( ActiveComponent, int );
  
protected:
  vtkEncodedGradientShader();
  ~vtkEncodedGradientShader();

  // Description:
  // Build a shading table for a light with the specified direction,
  // and color for an object of the specified material properties.
  // material[0] = ambient, material[1] = diffuse, material[2] = specular
  // and material[3] = specular exponent.  If the ambient flag is 1, 
  // then ambient illumination is added. If not, then this means we 
  // are calculating the "other side" of two sided lighting, so no 
  // ambient intensity is added in. If the update flag is 0,
  // the shading table is overwritten with these new shading values.
  // If the updateFlag is 1, then the computed light contribution is
  // added to the current shading table values. There is one shading
  // table per volume, and the index value indicated which index table
  // should be used. It is computed in the UpdateShadingTable method.
  void  BuildShadingTable( int index,
                           double lightDirection[3],
                           double lightColor[3],
                           double lightIntensity,
                           double viewDirection[3],
                           double material[4],
                           int twoSided,
                           vtkEncodedGradientEstimator *gradest,
                           int updateFlag );
  
  // The six shading tables (r diffuse ,g diffuse ,b diffuse, 
  // r specular, g specular, b specular ) - with an entry for each
  // encoded normal plus one entry at the end for the zero normal
  // There is one shading table per volume listed in the ShadingTableVolume
  // array. A null entry indicates an available slot.
  float                        *ShadingTable[VTK_MAX_SHADING_TABLES][6];
  vtkVolume                    *ShadingTableVolume[VTK_MAX_SHADING_TABLES];
  int                          ShadingTableSize[VTK_MAX_SHADING_TABLES];

  int                          ActiveComponent;
  
  // The intensity of light used for the zero normals, since it
  // can not be computed from the normal angles. Defaults to 0.0.
  float    ZeroNormalDiffuseIntensity;
  float    ZeroNormalSpecularIntensity;
private:
  vtkEncodedGradientShader(const vtkEncodedGradientShader&);  // Not implemented.
  void operator=(const vtkEncodedGradientShader&);  // Not implemented.
}; 


#endif