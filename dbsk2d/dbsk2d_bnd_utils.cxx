// This is file shp/dbsk2d/dbsk2d_bnd_contour.cxx

//:
// \file


#include "dbsk2d_bnd_utils.h"

#include <vtol/vtol_list_functions.h>

#include "../dbgl/algo/dbgl_circ_arc.h"

#include "dbsk2d_bnd_edge.h"
#include "dbsk2d_boundary.h"
#include "dbsk2d_ishock_bline.h"
#include "dbsk2d_ishock_barc.h"



//---------------------------------------------------
//: Create a vertex at coordinate of `pt'
// Id's of new vertex and the bpoint inside are obtained from boundary, if given.
// Return smart pointer to the new vertex
dbsk2d_bnd_vertex_sptr dbsk2d_bnd_utils::
new_vertex(const vgl_point_2d<double >& pt,
  const dbsk2d_boundary_sptr& boundary)
{
  dbsk2d_ishock_bpoint* bp = 
    new dbsk2d_ishock_bpoint(pt.x(), pt.y(),
    (boundary) ? boundary->nextAvailableID() : -1);
  return new dbsk2d_bnd_vertex(bp,
    (boundary) ? boundary->nextAvailableID() : -1);
}



//---------------------------------------------------------------
//: Create a singular point (degenerate edge)
// Return a smart pointer to the new degenerate edge
dbsk2d_bnd_edge_sptr dbsk2d_bnd_utils::
new_stand_alone_point(const vgl_point_2d<double >& pt,
  const dbsk2d_boundary_sptr& boundary)
{
  dbsk2d_bnd_vertex_sptr newv = 
    dbsk2d_bnd_utils::new_vertex(pt, boundary);
  return new dbsk2d_bnd_edge(newv,
    (boundary)? boundary->nextAvailableID() : -1);
  return 0;
}


//---------------------------------------------------
//: Connect two vertices with a line edge
// id's of the elements will be generated by `boundary', if given.
// otherwise default id (-1) is used.
// Return a pointer to the new edge
dbsk2d_bnd_edge_sptr dbsk2d_bnd_utils::
new_line_between(const dbsk2d_bnd_vertex_sptr& v1,
  const dbsk2d_bnd_vertex_sptr& v2, 
  const dbsk2d_boundary_sptr& boundary)
{
  dbsk2d_ishock_bline* left = 
    new dbsk2d_ishock_bline(v1->bpoint(), v2->bpoint(),
    (boundary)? boundary->nextAvailableID() : -1, true);

  dbsk2d_ishock_bline* right = 
    new dbsk2d_ishock_bline(v2->bpoint(), v1->bpoint(),
    (boundary)? boundary->nextAvailableID() : -1, false);

  left->set_twinLine(right);

  return new dbsk2d_bnd_edge(v1, v2, left, right, 
    (boundary)? boundary->nextAvailableID() : -1);
}


//------------------------------------------------------------------------
//: Connect two vertices with an arc edge
// id's of new elements will be generated by `boundary', if given.
// otherwise default id (-1) is used.
// Return a pointer to the new edge
dbsk2d_bnd_edge_sptr dbsk2d_bnd_utils::
new_arc_between(const dbsk2d_bnd_vertex_sptr& v1,
                const dbsk2d_bnd_vertex_sptr& v2, double curvature, 
                const dbsk2d_boundary_sptr& boundary)
{
  dbgl_circ_arc arc(v1->point(), v2->point(), curvature);
  vgl_point_2d<double > center = arc.center();
  dbsk2d_ishock_barc* left = 
    new dbsk2d_ishock_barc(v1->bpoint(), v2->bpoint(),
    (boundary)? boundary->nextAvailableID() : -1, (curvature>0) ? false : true, //GUIElm is always the one on the outside, i.e., CW
    center, 
    vcl_abs(1/curvature), 
    (curvature>0) ? ARC_NUD_CCW : ARC_NUD_CW);

  dbsk2d_ishock_barc* right = 
    new dbsk2d_ishock_barc(v2->bpoint(), v1->bpoint(),
    (boundary)? boundary->nextAvailableID() : -1, (curvature>0) ? true : false, //GUIElm is always the one on the outside, i.e., CW
    center, 
    vcl_abs(1/curvature), 
    (curvature>0) ? ARC_NUD_CW : ARC_NUD_CCW );

  left->set_twinArc(right);

  return new dbsk2d_bnd_edge(v1, v2, left, right, 
    (boundary)? boundary->nextAvailableID() : -1);
}






// ================== BND_CONTOUR ======================================

//: Connect the points with line segments
dbsk2d_bnd_contour_sptr dbsk2d_bnd_utils::
new_polyline_contour(const vcl_vector<vgl_point_2d<double > > &vertices,
                     bool closed, 
                     const dbsk2d_boundary_sptr& boundary)
{   
  if (vertices.size() < 2) return 0;
  if (closed && vertices.size() < 3) return 0;

  // convert the pts into bnd_vertex and put into a list
  vcl_vector<dbsk2d_bnd_vertex_sptr > bv_list;
  bv_list.reserve(vertices.size());
  for (unsigned int i = 0; i < vertices.size(); ++i)
  {
    bv_list.push_back(dbsk2d_bnd_utils::new_vertex(vertices[i], boundary));
  }

  // special treatment when the contour is closed
  if (closed && vertices.size() > 2)
    bv_list.push_back(bv_list[0]);

  // now link all these vertices into a chain and save as a contour
  vcl_vector<dbsk2d_bnd_edge_sptr > bnd_edges;
  for (unsigned int i=0; i<bv_list.size()-1; ++i)
  {
    bnd_edges.push_back(dbsk2d_bnd_utils::new_line_between(bv_list[i], 
      bv_list[i+1], boundary));
  }
  vcl_vector<signed char > directions(bnd_edges.size(), 1);

  
  // form a new contour
  return (new dbsk2d_bnd_contour(bnd_edges, directions, 
    (boundary)? boundary->nextAvailableID() : -1));
}









//----------------------------------------------------------------------
//: Determine directions of the `edges' (such that they are connected)
// Return false if the edges are not connected in a chain
bool dbsk2d_bnd_utils::
determine_edge_directions(const vcl_vector<dbsk2d_bnd_edge_sptr >& edges,
                             vcl_vector<signed char >& directions)
{
  // no connectedness check when there is only one edge
  directions.clear();
  if (edges.size() < 2)
  {
    directions.push_back((signed char)1);
    return true;
  }
  
  // check for connectedness and determine the directions list
  // of the bnd_edge list

  vtol_vertex_sptr last_vertex = 0;

  // determine the direction of the first edge
  // direction is +1
  if (! edges[1]->is_endpoint(edges[0]->v1()))
  {
    directions.push_back((signed char)(1));
    last_vertex = edges[0]->v2();
  }

  // direction is -1
  else
  {
    directions.push_back((signed char)(-1));
    last_vertex = edges[0]->v1();
  }

  // direction of the remaining edges
  for (unsigned int i = 1; i<edges.size(); i++)
  {
    dbsk2d_bnd_edge_sptr cur_edge = edges[i];
    if (last_vertex == cur_edge->v1())
    {
      directions.push_back((signed char)(1));
      last_vertex = cur_edge->v2();
    }
    // direction of cur_edge is -1
    else if (last_vertex == cur_edge->v2())
    {
      directions.push_back((signed char)(-1));
      last_vertex = cur_edge->v1();
    }
    // cur_edge is not connected to previous one.
    else
    {
      directions.clear();
      return false;
    }
  }
  return true;
}




//--------------------------------------------------------------------------
//: Extract edge list from a list of contours
void dbsk2d_bnd_utils::
extract_edge_list(const vcl_list<dbsk2d_bnd_contour_sptr >& contours,
                              vcl_list<dbsk2d_bnd_edge_sptr >& edges)
{
  edges.clear();
  for (bnd_contour_list::const_iterator cit = contours.begin();
    cit != contours.end(); ++cit)
  {
    dbsk2d_bnd_contour_sptr cur_con = (*cit);
    
    // check for non-existing contour
    if (!cur_con) continue;

    // edges of `cur_con'
    for (int i = 0; i < cur_con->num_edges(); ++i)
    {
      edges.push_back(cur_con->bnd_edge(i));
    }
  }
  tagged_union(&edges);
  return;
}


//--------------------------------------------------------------------------
//: Extract edge list from a boundary
void dbsk2d_bnd_utils::
extract_edge_list(const dbsk2d_boundary_sptr& boundary,
    vcl_list<dbsk2d_bnd_edge_sptr >& edges)
{
  // goal: avoid using all_contours(...) because it involves list copying
  // and avoid calling tagged_union(...) multiple times
  // (a simple way to implement this function is
  // 1. extract edges (list1) from preprocessed contours (tagged_union once)
  // 2. extract edges (list2) from scratched contours (tagged_union once)
  // 3. merge list1 and list2 then tagged_union again
  // --> result: NOT efficient)
  
  // The following algo is more efficient, tagged_union is run only once.

  // clear old stuffs
  edges.clear();

  // edges from preprocessed contours
  for (bnd_contour_list::const_iterator cit = 
    boundary->preproc_contours().begin();
    cit != boundary->preproc_contours().end(); ++cit)
  {
    dbsk2d_bnd_contour_sptr cur_con = (*cit);
    
    // check for non-existing contour
    if (!cur_con) continue;

    // edges of `cur_con'
    for (int i = 0; i < cur_con->num_edges(); ++i)
    {
      edges.push_back(cur_con->bnd_edge(i));
    }
  }

  // edges from scratch contours
  for (bnd_contour_list::const_iterator cit = 
    boundary->scratch_contours().begin();
    cit != boundary->scratch_contours().end(); ++cit)
  {
    dbsk2d_bnd_contour_sptr cur_con = (*cit);
    
    // check for non-existing contour
    if (!cur_con) continue;

    // edges of `cur_con'
    for (int i = 0; i < cur_con->num_edges(); ++i)
    {
      edges.push_back(cur_con->bnd_edge(i));
    }
  }

  tagged_union(&edges);
  return;
}



//-----------------------------------------------------------------------------
//: Extract edges that have `vertices' as their endpoints
void dbsk2d_bnd_utils::
extract_edge_list(const vcl_list<dbsk2d_bnd_vertex_sptr >& vertices,
                  vcl_list<dbsk2d_bnd_edge_sptr >& edges)
{
  edges.clear();
  for (bnd_vertex_list::const_iterator vit = 
    vertices.begin(); vit != vertices.end(); ++vit)
  {
    dbsk2d_bnd_vertex_sptr v = *vit;
    edge_list elist;
    v->edges(elist);
    for (unsigned int i=0; i<elist.size(); ++i)
    {
      edges.push_back(static_cast<dbsk2d_bnd_edge* >(elist[i].ptr()));
    }
  }
  tagged_union<dbsk2d_bnd_edge_sptr >(&edges);
  return;
}















//--------------------------------------------------------------------------
//: Extract edge list from a list of contours
void dbsk2d_bnd_utils::
extract_vertex_list(const vcl_list<dbsk2d_bnd_edge_sptr >& edges, 
  vcl_list<dbsk2d_bnd_vertex_sptr >& vertices)
{
  vertices.clear();
  for (bnd_edge_list::const_iterator eit = 
    edges.begin(); eit != edges.end(); ++eit)
  {
    vertices.push_back((*eit)->bnd_v1());
    if (!(*eit)->is_a_point())
    {
      vertices.push_back((*eit)->bnd_v2());
    }
  }
  tagged_union<dbsk2d_bnd_vertex_sptr >(&vertices);
  return;  
}





// -----------------------------------------------------------------------
//: Extract list of belms from a list of edges
void dbsk2d_bnd_utils::
extract_belm_list(const vcl_list<dbsk2d_bnd_edge_sptr >& edges, 
                  vcl_vector<dbsk2d_ishock_belm* >& ret_belms)
{
  // clear old stuffs
  ret_belms.clear();

  // collect all bcurves and bpoints from the edges
  // there might be duplicated bpoints, so we need to handle them separately
  vcl_list<dbsk2d_ishock_bpoint* > all_bpoints;
  for (vcl_list<dbsk2d_bnd_edge_sptr >::const_iterator eit = 
    edges.begin(); eit != edges.end(); ++eit)
  {
    dbsk2d_bnd_edge_sptr e = *eit;
    // Stand-alone points - bpoint is not duplicated
    if (e->is_a_point())
    {
      ret_belms.push_back(e->bnd_v1()->bpoint());
    }
    // normal edge - bcurves are unique but bpoints are not
    else
    {
      ret_belms.push_back(e->left_bcurve());
      ret_belms.push_back(e->right_bcurve());
      all_bpoints.push_back(e->left_bcurve()->s_pt());
      all_bpoints.push_back(e->left_bcurve()->e_pt());
    }
  }

  // eliminate bpoint duplication in `all_bpoints'
    all_bpoints.sort();
    all_bpoints.unique();

//	vcl_ofstream out("bpoints.txt", vcl_ios::out);
//	for (vcl_list<dbsk2d_ishock_bpoint* >::iterator bit = all_bpoints.begin();
//	     bit != all_bpoints.end(); ++bit)
//	{
//		out << (*bit)->id() << " ";
//		out << (*bit)->pt().x() << " " << (*bit)->pt().y() << " " << (*bit) << vcl_endl;
//	}
//	out.close();

  
  
  // add the bpoints to the `belms' list
  for (vcl_list<dbsk2d_ishock_bpoint* >::iterator bit = all_bpoints.begin();
    bit != all_bpoints.end(); ++bit)
  {
    ret_belms.push_back(*bit);
  }

  return;
}



