/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDendrogramItem.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkDendrogramItem - A 2D graphics item for rendering a tree as
// a dendrogram
//
// .SECTION Description
// Draw a tree as a dendrogram
// The input tree's vertex data must contain at least two arrays.
// The first required array is a vtkStringArray called "node name".
// This array is used to label the leaf nodes of the tree.
// The second required array is a scalar array called "node weight".
// This array is used by vtkTreeLayoutStrategy to set any particular
// node's distance from the root of the tree.
//
// The vtkNewickTreeReader automatically initializes both of these
// required arrays in its output tree.
//
// .SEE ALSO
// vtkTree vtkNewickTreeReader

#ifndef __vtkDendrogramItem_h
#define __vtkDendrogramItem_h

#include "vtkViewsInfovisModule.h" // For export macro
#include "vtkContextItem.h"

#include "vtkNew.h"               // For vtkNew ivars
#include "vtkSmartPointer.h"      // For vtkSmartPointer ivars
#include "vtkVector.h"            // For vtkVector2f ivar

class vtkDoubleArray;
class vtkGraphLayout;
class vtkLookupTable;
class vtkTree;
class vtkPruneTreeFilter;

class VTKVIEWSINFOVIS_EXPORT vtkDendrogramItem : public vtkContextItem
{
public:
  static vtkDendrogramItem *New();
  vtkTypeMacro(vtkDendrogramItem, vtkContextItem);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Set the tree that this item draws.  Note that this tree's vertex data
  // must contain a vtkStringArray called "node name".  The vtkNewickTreeReader
  // automatically creates this required array for you.
  virtual void SetTree(vtkTree *tree);

  // Description:
  // Get the tree that this item draws.
  vtkTree * GetTree();

  // Description:
  // Collapse subtrees until there are only n leaf nodes left in the tree.
  // The leaf nodes that remain are those that are closest to the root.
  // Any subtrees that were collapsed prior to this function being called
  // may be re-expanded.
  void CollapseToNumberOfLeafNodes(unsigned int n);

  // Description:
  // Get the collapsed tree
  vtkTree * GetPrunedTree();

  // Description:
  // Indicate which array within the Tree's VertexData should be used to
  // color the tree.  The specified array must be a vtkDoubleArray.
  // By default, the tree will be drawn in black.
  void SetColorArray(const char *arrayName);

  // Description:
  // Get/set whether or not leaf nodes should be extended so that they all line
  // up vertically.  The default is to NOT extend leaf nodes.  When extending
  // leaf nodes, the extra length is drawn in grey so as to distinguish it from
  // the actual length of the leaf node.
  vtkSetMacro(ExtendLeafNodes, bool);
  vtkGetMacro(ExtendLeafNodes, bool);
  vtkBooleanMacro(ExtendLeafNodes, bool);

  // Description:
  // Set which way the tree should face within the visualization.  The default
  // is for the tree to be drawn left to right.
  void SetOrientation(int orientation);

  // Description:
  // Get the current tree orientation.
  int GetOrientation();

  // Description:
  // Get the rotation angle (in degrees) that corresponds to the given
  // tree orientation.  For the default orientation (LEFT_TO_RIGHT), this
  // is 90 degrees.
  double GetAngleForOrientation(int orientation);

  // Description:
  // Get the angle that vertex labels should be rotated for the correponding
  // tree orientation.  For the default orientation (LEFT_TO_RIGHT), this
  // is 0 degrees.
  double GetTextAngleForOrientation(int orientation);

  // Description:
  // Get/Set whether or not leaf nodes should be labeled by this class.
  // Default is true.
  vtkSetMacro(DrawLabels, bool);
  vtkGetMacro(DrawLabels, bool);
  vtkBooleanMacro(DrawLabels, bool);

  // Description:
  // Set the position of the dendrogram.
  vtkSetVector2Macro(Position, float);
  void SetPosition(const vtkVector2f &pos);

  // Description:
  // Get position of the dendrogram.
  vtkGetVector2Macro(Position, float);
  vtkVector2f GetPositionVector();

  // Description:
  // Get/Set the spacing between the leaf nodes in our dendrogram.
  // Default is 18 pixels.
  vtkGetMacro(LeafSpacing, double);
  vtkSetMacro(LeafSpacing, double);

  // Description:
  // This function calls RebuildBuffers() if necessary.
  // Once PrepareToPaint() has been called, GetBounds() is guaranteed
  // to provide useful information.
  void PrepareToPaint(vtkContext2D *painter);

  // Description:
  // Get the bounds for this item as (Xmin,Xmax,Ymin,Ymax).
  // These bounds are only guaranteed to be accurate after Paint() or
  // PrepareToPaint() has been called.
  virtual void GetBounds(double bounds[4]);

  // Description:
  // Compute the width of the longest leaf node label.
  void ComputeLabelWidth(vtkContext2D *painter);

  // Description:
  // Get the width of the longest leaf node label.
  float GetLabelWidth();

  // Description:
  // Find the position of the vertex with the specified name.  Store
  // this information in the passed array.  Returns true if the vertex
  // was found, false otherwise.
  bool GetPositionOfVertex(std::string vertexName, double position[2]);

  // Description:
  // Paints the input tree as a dendrogram.
  virtual bool Paint(vtkContext2D *painter);

  // Description:
  // Get/Set how wide the edges of this dendrogram should be.  Default is one pixel.
  vtkGetMacro(LineWidth, float);
  vtkSetMacro(LineWidth, float);

  // Description:
  // Get/set whether or not the number of collapsed leaf nodes should be written
  // inside the triangle representing a collapsed subtree.  Default is true.
  vtkSetMacro(DisplayNumberOfCollapsedLeafNodes, bool);
  vtkGetMacro(DisplayNumberOfCollapsedLeafNodes, bool);
  vtkBooleanMacro(DisplayNumberOfCollapsedLeafNodes, bool);

  // this struct & class allow us to generate a priority queue of vertices.
  struct WeightedVertex
  {
    vtkIdType ID;
    double weight;
  };
  class CompareWeightedVertices
  {
    public:
    // Returns true if v2 is higher priority than v1
    bool operator()(WeightedVertex& v1, WeightedVertex& v2)
    {
    if (v1.weight < v2.weight)
      {
      return false;
      }
     return true;
    }
  };

  //BTX
  // Description:
  // Enum for Orientation.
  enum
    {
    LEFT_TO_RIGHT,
    UP_TO_DOWN,
    RIGHT_TO_LEFT,
    DOWN_TO_UP
    };

  // Description:
  // Returns true if the transform is interactive, false otherwise.
  virtual bool Hit(const vtkContextMouseEvent &mouse);

  // Description:
  // Collapse or expand a subtree when the user double clicks on an
  // internal node.
  virtual bool MouseDoubleClickEvent( const vtkContextMouseEvent &event);
  //ETX

protected:
  vtkDendrogramItem();
  ~vtkDendrogramItem();

  vtkVector2f PositionVector;
  float* Position;

  // Description:
  // Generate some data needed for painting.  We cache this information as
  // it only needs to be generated when the input data changes.
  virtual void RebuildBuffers();

  // Description:
  // This function does the bulk of the actual work in rendering our dendrogram
  virtual void PaintBuffers(vtkContext2D *painter);

  // Description:
  // This function returns a bool indicating whether or not we need to rebuild
  // our cached data before painting.
  virtual bool IsDirty();

  // Description:
  // Compute how to scale our data so that text labels will fit within the
  // bounds determined by the spacing between the leaf nodes of the tree.
  void ComputeMultipliers();

  // Description:
  // Compute the bounds of our tree in pixel coordinates.
  void ComputeBounds();

  // Description:
  // Count the number of leaf nodes in the tree
  void CountLeafNodes();

  // Description:
  // Count the number of leaf nodes that descend from a given vertex.
  int CountLeafNodes(vtkIdType vertex);

  // Description:
  // Get the tree vertex closest to the specified coordinates.
  vtkIdType GetClosestVertex(double x, double y);

  // Description:
  // Collapse the subtree rooted at vertex.
  void CollapseSubTree(vtkIdType vertex);

  // Description:
  // Expand the previously collapsed subtree rooted at vertex.
  void ExpandSubTree(vtkIdType vertex);

  // Description:
  // Look up the original ID of a vertex in the pruned tree.
  vtkIdType GetOriginalId(vtkIdType vertex);

  // Description:
  // Look up the ID of a vertex in the pruned tree from a vertex ID
  // of the input tree.
  vtkIdType GetPrunedIdForOriginalId(vtkIdType originalId);

  // Description:
  // Check if the click at (x, y) should be considered as a click on
  // a collapsed subtree.  Returns the vtkIdType of the pruned subtree
  // if so, -1 otherwise.
  vtkIdType GetClickedCollapsedSubTree(double x, double y);

  // Description:
  // Calculate the extent of the data that is visible within the window.
  // This information is used to ensure that we only draw details that
  // will be seen by the user.  This improves rendering speed, particularly
  // for larger data.
  void UpdateVisibleSceneExtent(vtkContext2D *painter);

  // Description:
  // Returns true if any part of the line segment defined by endpoints
  // (x0, y0), (x1, y1) falls within the extent of the currently
  // visible scene.  Returns false otherwise.
  bool LineIsVisible(double x0, double y0, double x1, double y1);

  // Description:
  // Internal function.  Use SetOrientation(int orientation) instead.
  void SetOrientation(vtkTree *tree, int orientation);

  vtkSmartPointer<vtkTree> Tree;
  vtkSmartPointer<vtkTree> LayoutTree;

private:
  vtkDendrogramItem(const vtkDendrogramItem&); // Not implemented
  void operator=(const vtkDendrogramItem&); // Not implemented

  vtkSmartPointer<vtkTree> PrunedTree;
  unsigned long DendrogramBuildTime;
  vtkNew<vtkGraphLayout> Layout;
  vtkNew<vtkPruneTreeFilter> PruneFilter;
  vtkNew<vtkLookupTable> TriangleLookupTable;
  vtkNew<vtkLookupTable> TreeLookupTable;
  vtkDoubleArray* ColorArray;
  double MultiplierX;
  double MultiplierY;
  int NumberOfLeafNodes;
  double LeafSpacing;

  double MinX;
  double MinY;
  double MaxX;
  double MaxY;
  double SceneBottomLeft[3];
  double SceneTopRight[3];
  float LabelWidth;
  float LineWidth;
  bool ColorTree;
  bool ExtendLeafNodes;
  bool DrawLabels;
  bool DisplayNumberOfCollapsedLeafNodes;
};

#endif
