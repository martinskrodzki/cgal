// Copyright (c) 1997  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Nico Kruithof <Nico@nghk.nl>

#ifndef CGAL_PERIODIC_2_DELAUNAY_TRIANGULATION_2_H
#define CGAL_PERIODIC_2_DELAUNAY_TRIANGULATION_2_H

#include <CGAL/Periodic_2_triangulation_2.h>
#include <CGAL/iterator.h>

#ifndef CGAL_TRIANGULATION_2_DONT_INSERT_RANGE_OF_POINTS_WITH_INFO
#include <CGAL/Spatial_sort_traits_adapter_2.h>
#include <CGAL/internal/info_check.h>

#include <boost/iterator/zip_iterator.hpp>
#include <boost/mpl/and.hpp>
#endif //CGAL_TRIANGULATION_2_DONT_INSERT_RANGE_OF_POINTS_WITH_INFO


namespace CGAL {

template <
  class Gt, 
  class Tds = Triangulation_data_structure_2<
                     Periodic_2_triangulation_ds_vertex_base_2<Gt>,
                     Triangulation_face_base_2<Gt, 
                       Periodic_2_triangulation_ds_face_base_2<> > > >
class Periodic_2_Delaunay_triangulation_2 : public Periodic_2_triangulation_2<Gt,Tds>
{
  typedef Periodic_2_Delaunay_triangulation_2<Gt,Tds>          Self;
public:
  typedef Periodic_2_triangulation_2<Gt,Tds>                   Triangulation;
  
public:
  typedef Tds                                  Triangulation_data_structure;
  typedef Gt                                   Geom_traits;
  
  typedef typename Gt::Periodic_2_offset_2     Offset;
  typedef typename Gt::Iso_rectangle_2         Iso_rectangle;
  typedef array<int, 2>                        Covering_sheets;
  
  typedef typename Gt::FT                      FT;
  typedef typename Gt::Point_2                 Point;
  typedef typename Gt::Segment_2               Segment;
  typedef typename Gt::Triangle_2              Triangle;
  
  typedef std::pair<Point,Offset>              Periodic_point;
  typedef array< std::pair<Point,Offset>, 2>   Periodic_segment;
  typedef array< std::pair<Point,Offset>, 3>   Periodic_triangle;
  typedef array< std::pair<Point,Offset>, 4>   Periodic_tetrahedron;
  
  typedef typename Triangulation::size_type             size_type;
  typedef typename Triangulation::Locate_type           Locate_type;
  typedef typename Triangulation::Face_handle           Face_handle;
  typedef typename Triangulation::Vertex_handle         Vertex_handle;
  typedef typename Triangulation::Edge                  Edge;
  typedef typename Triangulation::Edge_circulator       Edge_circulator;
  typedef typename Triangulation::Face_circulator       Face_circulator;
  typedef typename Triangulation::Vertex_circulator     Vertex_circulator;
  typedef typename Triangulation::Finite_edges_iterator Finite_edges_iterator;
  typedef typename Triangulation::Finite_faces_iterator Finite_faces_iterator;
  typedef typename Triangulation::Finite_vertices_iterator 
                                                     Finite_vertices_iterator;
  typedef typename Triangulation::All_faces_iterator    All_faces_iterator;

  typedef typename Triangulation::Edge_iterator    Edge_iterator;
  typedef typename Triangulation::Face_iterator    Face_iterator;
  typedef typename Triangulation::Vertex_iterator Vertex_iterator;


public:
#ifndef CGAL_CFG_USING_BASE_MEMBER_BUG_2  
  using Triangulation::empty;
  using Triangulation::cw;
  using Triangulation::ccw;
  using Triangulation::tds;
  using Triangulation::is_infinite;
  using Triangulation::get_offset;
  using Triangulation::int_to_off;
  using Triangulation::geom_traits;
  using Triangulation::is_1_cover;
  using Triangulation::dimension;
  using Triangulation::number_of_vertices;
  using Triangulation::faces_begin;
  using Triangulation::finite_edges_begin;
  using Triangulation::finite_edges_end;
  using Triangulation::get_neighbor_offset;
  using Triangulation::combine_offsets;
  using Triangulation::locate;
  using Triangulation::number_of_sheets;
  using Triangulation::orientation;
  using Triangulation::side_of_oriented_circle;
  using Triangulation::remove_degree_init;
#endif

  /// \name Constructors
  // \{
  /// Constructor
  /// NGHK: implemented
  Periodic_2_Delaunay_triangulation_2(const Iso_rectangle & domain = Iso_rectangle(0,0,1,1),
                                     const Gt& gt = Gt())
    : Periodic_2_triangulation_2<Gt,Tds>(domain, gt) {}

  /// Copy constructor
  /// \n NGHK: implemented
  Periodic_2_Delaunay_triangulation_2(
	       const Periodic_2_Delaunay_triangulation_2<Gt,Tds> &tr)
    : Periodic_2_triangulation_2<Gt,Tds>(tr) {
    CGAL_triangulation_postcondition( is_valid() );
  }

  /// Constructor with insertion of points
  template < class InputIterator >
  Periodic_2_Delaunay_triangulation_2(InputIterator first, InputIterator last,
                                      const Iso_rectangle & domain = Iso_rectangle(0,0,1,1),
                                      const Gt& gt = Gt())
    : Periodic_2_triangulation_2<Gt,Tds>(domain, gt) {
    insert(first, last);
  }
  
  // \}

  /// \name Insertion-Removal
  // \{
  /// NGHK: implemented
  Vertex_handle insert(const Point  &p, 
		       Face_handle start = Face_handle() );
  /// NGHK: implemented
  Vertex_handle insert(const Point& p,
		       Locate_type lt,
		       Face_handle loc, int li );


  /// Inserts a point in the triangulation.
  /// NGHK: Implemented
  Vertex_handle push_back(const Point &p);

  /// NGHK: implemented
#ifndef CGAL_TRIANGULATION_2_DONT_INSERT_RANGE_OF_POINTS_WITH_INFO
  template < class InputIterator >
  std::ptrdiff_t
  insert(InputIterator first, InputIterator last,
         bool is_large_point_set = true,
         typename boost::enable_if<
           boost::is_convertible<
             typename std::iterator_traits<InputIterator>::value_type,
             Point
             > >::type* = NULL)
#else
  template < class InputIterator >
  std::ptrdiff_t
  insert(InputIterator first, InputIterator last, bool is_large_point_set = true)
#endif //CGAL_TRIANGULATION_2_DONT_INSERT_RANGE_OF_POINTS_WITH_INFO 
  {
    if (first == last) return 0;

    size_type n = number_of_vertices();

    // The heuristic discards the existing triangulation so it can only be
    // applied to empty triangulations.
    if (n!=0) is_large_point_set = false;

    std::set<Vertex_handle> dummy_points;
    std::vector<Point> points(first, last);
    typename std::vector<Point>::iterator pbegin = points.begin();

    if (is_large_point_set) {
      std::vector<Vertex_handle> tmp_dummy_points = this->insert_dummy_points();
      std::copy(tmp_dummy_points.begin(), tmp_dummy_points.end(),
                std::inserter(dummy_points, dummy_points.begin()));
    } else {
      std::random_shuffle (points.begin(), points.end());
      pbegin = points.begin();

      // The empty triangulation is a 1-cover by definition, insert at least one point
      insert(*pbegin); ++pbegin;
      while (!is_1_cover()) {
        if (pbegin == points.end())
          return number_of_vertices() - n;
        insert(*pbegin);
        ++pbegin;
      }
    }

    CGAL_assertion(is_1_cover());

    // Insert the points
    spatial_sort (pbegin, points.end(), geom_traits());

    Face_handle f; Locate_type lt; int li;

    for (typename std::vector<Point>::const_iterator p = pbegin, end = points.end();
         p != end; ++p)
    {
      f = locate(*p, lt, li, f);

      if (lt == Triangulation::VERTEX) {
        dummy_points.erase(f->vertex(li));
      } else {
        insert(*p, lt, f, li);
      }
    }

    if (is_large_point_set) {
      for (typename std::set<Vertex_handle>::const_iterator it = dummy_points.begin();
          it != dummy_points.end(); ++it) {
        remove(*it);
      }
    }

    return number_of_vertices() - n;
  }
#ifndef CGAL_TRIANGULATION_2_DONT_INSERT_RANGE_OF_POINTS_WITH_INFO
private:  
  //top stands for tuple-or-pair
  template <class Info>
  const Point& top_get_first(const std::pair<Point,Info>& pair) const { return pair.first; }
  template <class Info>
  const Info& top_get_second(const std::pair<Point,Info>& pair) const { return pair.second; }
  template <class Info>
  const Point& top_get_first(const boost::tuple<Point,Info>& tuple) const { return boost::get<0>(tuple); }
  template <class Info>
  const Info& top_get_second(const boost::tuple<Point,Info>& tuple) const { return boost::get<1>(tuple); }

  template <class Tuple_or_pair,class InputIterator>
  std::ptrdiff_t insert_with_info(InputIterator first,InputIterator last, bool is_large_point_set)
  {
    if (first == last) return 0;

    std::vector<std::ptrdiff_t> indices;
    std::vector<Point> points;
    std::vector<typename Tds::Vertex::Info> infos;
    std::ptrdiff_t index=0;
    for (InputIterator it=first;it!=last;++it){
      Tuple_or_pair value=*it;
      points.push_back( top_get_first(value)  );
      infos.push_back ( top_get_second(value) );
      indices.push_back(index++);
    }

    typedef Spatial_sort_traits_adapter_2<Geom_traits,Point*> Search_traits;
    
    size_type n = number_of_vertices();

    // The heuristic discards the existing triangulation so it can only be
    // applied to empty triangulations.
    if (n!=0) is_large_point_set = false;

    std::set<Vertex_handle> dummy_points;
    typename std::vector<std::ptrdiff_t>::iterator pbegin = indices.begin();

    if (is_large_point_set) {
      std::vector<Vertex_handle> tmp_dummy_points = this->insert_dummy_points();
      std::copy(tmp_dummy_points.begin(), tmp_dummy_points.end(),
                std::inserter(dummy_points, dummy_points.begin()));
    } else {
      std::random_shuffle(indices.begin(),indices.end());
      pbegin = indices.begin();

      Vertex_handle v_new;

      // The empty triangulation is a 1-cover by definition, insert at least one point
      v_new = insert(points[*pbegin]);
      v_new->info() = infos[*pbegin];
      ++pbegin;

      while (!is_1_cover()) {
        if (pbegin == indices.end())
          return number_of_vertices() - n;
        v_new = insert(points[*pbegin]);
        v_new->info() = infos[*pbegin];
        ++pbegin;
      }
    }

    CGAL_assertion(is_1_cover());

    // Insert the points
    spatial_sort(indices.begin(),indices.end(),Search_traits(&(points[0]),geom_traits()));

    Face_handle f; Locate_type lt; int li;

    Face_handle hint;
    for (typename std::vector<std::ptrdiff_t>::const_iterator it = pbegin, end = indices.end();
      it != end; ++it){
      f = locate(points[*it], lt, li, f);

      if (lt == Triangulation::VERTEX) {
        // Always copy the info, it might be a dummy vertex
        f->vertex(li)->info() = infos[*it];
        dummy_points.erase(f->vertex(li));
      } else {
        Vertex_handle v_new = insert(points[*it], lt, f, li);
        v_new->info() = infos[*it];
      }
    }

    if (is_large_point_set) {
      for (typename std::set<Vertex_handle>::const_iterator it = dummy_points.begin();
           it != dummy_points.end(); ++it) {
        remove(*it);
      }
    }

    return number_of_vertices() - n;
  }
  
 
public:

  template < class InputIterator >
  std::ptrdiff_t
  insert( InputIterator first,
          InputIterator last,
          bool is_large_point_set = true,
          typename boost::enable_if<
            boost::is_convertible<
              typename std::iterator_traits<InputIterator>::value_type,
              std::pair<Point,typename internal::Info_check<typename Tds::Vertex>::type>
            > >::type* =NULL
  )
  {
    return insert_with_info< std::pair<Point,typename internal::Info_check<typename Tds::Vertex>::type> >(first,last, is_large_point_set);
  }

  template <class  InputIterator_1,class InputIterator_2>
  std::ptrdiff_t
  insert( boost::zip_iterator< boost::tuple<InputIterator_1,InputIterator_2> > first,
          boost::zip_iterator< boost::tuple<InputIterator_1,InputIterator_2> > last,
          bool is_large_point_set = true,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_convertible< typename std::iterator_traits<InputIterator_1>::value_type, Point >,
              boost::is_convertible< typename std::iterator_traits<InputIterator_2>::value_type, typename internal::Info_check<typename Tds::Vertex>::type >
            > >::type* =NULL)
  {
    return insert_with_info< boost::tuple<Point,typename internal::Info_check<typename Tds::Vertex>::type> >(first,last,is_large_point_set);
  }
#endif //CGAL_TRIANGULATION_2_DONT_INSERT_RANGE_OF_POINTS_WITH_INFO

  void  remove(Vertex_handle v );
  // \}

  /// \name Displacement
  // \{
  
  /// NGHK: Not yet implemented
  Vertex_handle move_if_no_collision(Vertex_handle v, const Point &p);
  /// NGHK: Not yet implemented
  Vertex_handle move_point(Vertex_handle v, const Point &p);
  // \}

  /// \name Check - Query
  // \{
  /// Returns the vertex closest to p, the point location will start from f
  /// NGHK: Not yet implemented
  Vertex_handle
  nearest_vertex(const Point& p, Face_handle f= Face_handle()) const;

  /// NGHK: Not yet implemented
  template <class OutputItFaces, class OutputItBoundaryEdges>
  std::pair<OutputItFaces,OutputItBoundaryEdges>
  get_conflicts_and_boundary(const Point  &p,
			     OutputItFaces fit,
			     OutputItBoundaryEdges eit,
			     Face_handle start = Face_handle()) const {
    CGAL_triangulation_precondition( dimension() == 2);
    int li;
    Locate_type lt;
    Face_handle fh = locate(p,lt,li, start);
    switch(lt) {
      //case Triangulation::EMPTY:
    case Triangulation::VERTEX:
      return std::make_pair(fit,eit);
    case Triangulation::FACE:
    case Triangulation::EDGE:
    case Triangulation::EMPTY:
      *fit++ = fh; //put fh in OutputItFaces
      std::pair<OutputItFaces,OutputItBoundaryEdges> pit = std::make_pair(fit,eit);
      pit = propagate_conflicts(p,fh,0,pit);
      pit = propagate_conflicts(p,fh,1,pit);
      pit = propagate_conflicts(p,fh,2,pit);
      return pit;
    }
    CGAL_triangulation_assertion(false);
    return std::make_pair(fit,eit);
  }

  /// NGHK: Not yet implemented
  template <class OutputItFaces>
  OutputItFaces
  get_conflicts (const Point  &p,
		 OutputItFaces fit,
		 Face_handle start= Face_handle()) const {
    std::pair<OutputItFaces,Emptyset_iterator> pp =
      get_conflicts_and_boundary(p,fit,Emptyset_iterator(),start);
    return pp.first;
  }

  /// NGHK: Not yet implemented
  template <class OutputItBoundaryEdges>
  OutputItBoundaryEdges
  get_boundary_of_conflicts(const Point  &p,
			    OutputItBoundaryEdges eit,
			    Face_handle start= Face_handle()) const {
    std::pair<Emptyset_iterator, OutputItBoundaryEdges> pp =
      get_conflicts_and_boundary(p,Emptyset_iterator(),eit,start);
    return pp.second;
  }
  // \}


  /// \name Dual
  // \{
  /// Returns the dual of f, which is the circumcenter of f.
  Point dual(Face_handle f) const;
  /// Returns the dual of e, which is always a segment in the periodic triangulation.
  Segment dual(const Edge &e) const ;
  /// Returns the dual of the edge pointed to by ec.
  Segment dual(const Edge_circulator& ec) const;
  /// Returns the dual of the edge pointed to by ei.
  Segment dual(const Edge_iterator& ei) const;

  template < class Stream>
  Stream& draw_dual(Stream & ps) {
    Finite_edges_iterator eit= finite_edges_begin();
    for (; eit != finite_edges_end(); ++eit) {
      ps << dual(eit);
    }
    return ps;
  }
  // \}

  /// \name Checking
  // \{
  /// NGHK: Not yet implemented
  bool is_valid(bool verbose = false, int level = 0) const;

  /// NGHK: Not yet implemented
  bool is_valid(Face_handle f, bool verbose = false, int level = 0) const;
  // \}

private:
  /// Not in the documentation
  /// NGHK: Not yet implemented
  inline void restore_Delaunay(Vertex_handle v);

  inline bool locally_Delaunay(const Face_handle &f, int i, const Face_handle &nb);

  /// NGHK: Not yet implemented
  template <class OutputItFaces>
  Vertex_handle insert_and_give_new_faces(const Point  &p,
                                          OutputItFaces fit,
                                          Face_handle start = Face_handle() );
  /// NGHK: Not yet implemented
  template <class OutputItFaces>
  Vertex_handle insert_and_give_new_faces(const Point& p,
                                          Locate_type lt,
                                          Face_handle loc, int li,
                                          OutputItFaces fit);

  /// NGHK: Not yet implemented
  template <class OutputItFaces>
  Vertex_handle move_if_no_collision_and_give_new_faces(Vertex_handle v,
                                                        const Point &p,
                                                        OutputItFaces fit);

  /// NGHK: Not yet implemented
  template <class OutputItFaces>
  void remove_and_give_new_faces(Vertex_handle v,
                                 OutputItFaces fit);

  /// NGHK: Not yet implemented
  bool is_delaunay_after_displacement(Vertex_handle v,
                                      const Point &p) const;

#ifndef CGAL_DT2_USE_RECURSIVE_PROPAGATING_FLIP
  void non_recursive_propagating_flip(Face_handle f,int i);
  void propagating_flip(const Face_handle& f,int i, int depth=0);
#else
  void propagating_flip(const Face_handle& f,int i);
#endif

  /// NGHK: Not yet implemented
  void remove_2D(Vertex_handle v );

  // auxilliary functions for remove
  // returns false if we first need to convert to a 9-cover before the vertex can be removed
  bool remove_single_vertex(Vertex_handle v, const Offset &v_o);
  /// NGHK: implemented
  void remove_degree_triangulate(Vertex_handle v, std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w,
                                 std::vector<int> &i,int d);
  void remove_degree_triangulate(Vertex_handle v, std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<Offset> &offset_w,
                                 std::vector<int> &i,int d);
  void remove_degree_d(Vertex_handle v, std::vector<Face_handle> &f,
                       std::vector<Vertex_handle> &w, 
                       std::vector<int> &i,int d);
  void remove_degree_d(Vertex_handle v, std::vector<Face_handle> &f,
                       std::vector<Vertex_handle> &w, std::vector<Offset> &offset_w,
                       std::vector<int> &i,int d);
  /// NGHK: Implemented
  /// Assumes that all offsets are (0,0)
  void fill_hole_delaunay(std::list<Edge> & hole);
  /// Fill hole over a periodic boundary
  void fill_hole_delaunay(std::list<Edge> & hole,
                          std::map<Vertex_handle, Offset> &vertex_offsets);

  /// NGHK: implemented
  void remove_degree3(Vertex_handle v, std::vector<Face_handle> &f,
                      std::vector<Vertex_handle> &w,
                      std::vector<int> &i);
  void remove_degree3(Vertex_handle v, std::vector<Face_handle> &f,
                      std::vector<Vertex_handle> &w, std::vector<Offset> &o,
                      std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree4(Vertex_handle v, std::vector<Face_handle> &f,
		      std::vector<Vertex_handle> &w, std::vector<Offset> &o,
                      std::vector<int> &i);
  void remove_degree4(Vertex_handle v, std::vector<Face_handle> &f,
		      std::vector<Vertex_handle> &w,
                      std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree5(Vertex_handle v, std::vector<Face_handle> &f,
                      std::vector<Vertex_handle> &w,
                      std::vector<int> &i);
  void remove_degree5(Vertex_handle v, std::vector<Face_handle> &f,
                      std::vector<Vertex_handle> &w, std::vector<Offset> &o,
                      std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree5_star   (Vertex_handle &v,
                              Face_handle & ,Face_handle & ,Face_handle & ,Face_handle & ,Face_handle & ,
                              Vertex_handle&,Vertex_handle&,Vertex_handle&,Vertex_handle&,Vertex_handle&,
                              int           ,int           ,int           ,int           ,int );
  void remove_degree5_star   (Vertex_handle &v,
                              Face_handle & ,Face_handle & ,Face_handle & ,Face_handle & ,Face_handle & ,
                              Vertex_handle&,Vertex_handle&,Vertex_handle&,Vertex_handle&,Vertex_handle&,
                              Offset&,Offset&,Offset&,Offset&,Offset&,
                              int           ,int           ,int           ,int           ,int );
  /// NGHK: implemented
  void remove_degree6(Vertex_handle v, std::vector<Face_handle> &f,
		      std::vector<Vertex_handle> &w,
                      std::vector<int> &i);
  void remove_degree6(Vertex_handle v, std::vector<Face_handle> &f,
		      std::vector<Vertex_handle> &w, std::vector<Offset> &o,
                      std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree6_star   (Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      int           ,int           ,int           ,
			      int           ,int           ,int );
  void remove_degree6_star   (Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Offset&,Offset&,Offset&,
			      Offset&,Offset&,Offset&,
			      int           ,int           ,int           ,
			      int           ,int           ,int );
  /// NGHK: implemented
  void remove_degree6_N      (Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      int           ,int           ,int           ,
			      int           ,int           ,int  );
  void remove_degree6_N      (Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Offset&,Offset&,Offset&,
			      Offset&,Offset&,Offset&,
			      int           ,int           ,int           ,
			      int           ,int           ,int  );
  /// NGHK: implemented
  void remove_degree6_antiN  (Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      int           ,int           ,int           ,
			      int           ,int           ,int  );
  /// NGHK: implemented
  void remove_degree6_antiN  (Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Offset&,Offset&,Offset&,
			      Offset&,Offset&,Offset&,
			      int           ,int           ,int           ,
			      int           ,int           ,int  );
  /// NGHK: implemented
  void remove_degree6_diamond(Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      int           ,int           ,int           ,
			      int           ,int           ,int  );
  void remove_degree6_diamond(Vertex_handle &v,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Face_handle & ,Face_handle & ,Face_handle & ,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Vertex_handle&,Vertex_handle&,Vertex_handle&,
			      Offset&,Offset&,Offset&,
			      Offset&,Offset&,Offset&,
			      int           ,int           ,int           ,
			      int           ,int           ,int  );
  /// NGHK: implemented
  void remove_degree7(Vertex_handle v,std::vector<Face_handle> &f,
		      std::vector<Vertex_handle> &w,
                      std::vector<int> &i);
  void remove_degree7(Vertex_handle v,std::vector<Face_handle> &f,
		      std::vector<Vertex_handle> &w, std::vector<Offset> &o,
                      std::vector<int> &i);

  /// NGHK: implemented
  void rotate7(int j, std::vector<Vertex_handle> &w,
	       std::vector<Face_handle> &f,
               std::vector<int> &i);
  void rotate7(int j, std::vector<Vertex_handle> &w,
	       std::vector<Face_handle> &f, std::vector<Offset> &o,
               std::vector<int> &i);
  /// NGHK: implemented
  /// Returns whether the simplicity criterion is satisfied
  void get_offset_degree7(std::vector<Offset> &in_o, int out_o[]);
  /// NGHK: implemented
  void remove_degree7_star      (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<int> &i);
  void remove_degree7_star      (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree7_zigzag    (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<int> &i);
  void remove_degree7_zigzag    (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree7_leftdelta (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<int> &i);
  void remove_degree7_leftdelta (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree7_rightdelta(Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<int> &i);
  void remove_degree7_rightdelta(Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree7_leftfan   (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<int> &i);
  void remove_degree7_leftfan   (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i);
  /// NGHK: implemented
  void remove_degree7_rightfan  (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<int> &i);
  void remove_degree7_rightfan  (Vertex_handle&,int,std::vector<Face_handle> &f,
                                 std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i);

  bool incircle(int x, int j, int k, int l, std::vector<Face_handle> &,
		std::vector<Vertex_handle> &w, std::vector<int> &) {
    
    return side_of_oriented_circle(w[j]->point(), w[k]->point(), w[l]->point(), w[x]->point(),
                                   true) ==  ON_POSITIVE_SIDE;
  }
  bool incircle(int x, int j, int k, int l, std::vector<Face_handle> &,
		std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &) {
    
    return side_of_oriented_circle(w[j]->point(), w[k]->point(), w[l]->point(), w[x]->point(),
                                   o[j], o[k], o[l], o[x],
                                   true) ==  ON_POSITIVE_SIDE;
  }

// end of auxilliary functions for remove




  /// NGHK: Not yet implemented
  Vertex_handle nearest_vertex_2D(const Point& p, Face_handle f) const;

  /// NGHK: Not yet implemented
  void  look_nearest_neighbor(const Point& p,
			      Face_handle f,
			      int i,
			      Vertex_handle& nn) const;

  /// NGHK: Not yet implemented
  template <class OutputItFaces, class OutputItBoundaryEdges>
  std::pair<OutputItFaces,OutputItBoundaryEdges>
  propagate_conflicts (const Point  &p,
		       Face_handle fh,
		       int i,
		       std::pair<OutputItFaces,OutputItBoundaryEdges>
		       pit)  const {
    Face_handle fn = fh->neighbor(i);
    if (! test_conflict(p,fn)) {
      *(pit.second)++ = Edge(fn, fn->index(fh));
    } else {
      *(pit.first)++ = fn;
      int j = fn->index(fh);
      pit = propagate_conflicts(p,fn,ccw(j),pit);
      pit = propagate_conflicts(p,fn,cw(j), pit);
    }
    return pit;
  }

  bool test_conflict(const Point &p, Face_handle fh) const {
    return side_of_oriented_circle(fh, p, true) ==  ON_POSITIVE_SIDE;
  }

  /// NGHK: Not yet implemented
  void restore_edges(Vertex_handle v)
  {
    NGHK_NYI;
    std::list<Edge> edges;
    Face_circulator fc = incident_faces(v), done(fc);
    int degree = 0;
    do {
      if((++degree) > 3) break;
    } while(++fc != done);
    fc = incident_faces(v);
    done = fc;
    if(degree == 3) {
      do {
        int i = fc->index(v);
        edges.push_back(Edge(fc, i));
      } while(++fc != done);
    } else {
      do {
        int i = fc->index(v);
        edges.push_back(Edge(fc, i));
        edges.push_back(Edge(fc, cw(i)));
      } while(++fc != done);
    }
    while(!edges.empty()) {
      const Edge &e = edges.front();
      Face_handle f = e.first;
      int i = e.second;
      edges.pop_front();
      if(is_infinite(f->vertex(i))) continue;
      Face_handle fi = f->neighbor(i);
      int mi = this->_tds.mirror_index(f, i);
      Vertex_handle vm = this->_tds.mirror_vertex(f, i);
      if(is_infinite(vm)) continue;
      if(this->side_of_oriented_circle(f, vm->point(),true) == ON_POSITIVE_SIDE) {
        this->_tds.flip(f, i);
        edges.push_back(Edge(f, i));
        edges.push_back(Edge(f, cw(i)));
        edges.push_back(Edge(fi, cw(mi)));
        edges.push_back(Edge(fi, mi));
      }
    }
  }

  /// NGHK: Not yet implemented
  void restore_edges(Vertex_handle v, std::set<Face_handle> &faces)
  {
    NGHK_NYI;
    typedef std::list<Edge> Edges_list;
    Edges_list edges;
    Face_circulator fc = incident_faces(v), done(fc);
    int degree = 0;
    do {
      if((++degree) > 3) break;
    } while(++fc != done);
    fc = incident_faces(v);
    done = fc;
    if(degree == 3) {
      do {
        int i = fc->index(v);
        edges.push_back(Edge(fc, i));
      } while(++fc != done);
    } else {
      do {
        int i = fc->index(v);
        edges.push_back(Edge(fc, i));
        edges.push_back(Edge(fc, cw(i)));
      } while(++fc != done);
    }
    while(!edges.empty()) {
      const Edge &e = edges.front();
      Face_handle f = e.first;
      int i = e.second;
      edges.pop_front();
      faces.insert(f);
      if(is_infinite(f->vertex(i))) continue;
      Face_handle fi = f->neighbor(i);
      int mi = this->_tds.mirror_index(f, i);
      Vertex_handle vm = this->_tds.mirror_vertex(f, i);
      if(is_infinite(vm)) continue;
      if(this->side_of_oriented_circle(f, vm->point()) == ON_POSITIVE_SIDE) {
        this->_tds.flip(f, i);
        edges.push_back(Edge(f, i));
        edges.push_back(Edge(f, cw(i)));
        edges.push_back(Edge(fi, cw(mi)));
        edges.push_back(Edge(fi, mi));
      }
    }
  }

};

template < class Gt, class Tds >
bool
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
is_valid(bool verbose, int level) const
{
  // Check the parent
  bool result = Periodic_2_triangulation_2<Gt,Tds>::is_valid(verbose, level);

  // Check in_sphere:
  if (dimension()==2) {
    const Point *p[4]; Offset off[4];
    for (Face_iterator fit = faces_begin();
         fit != this->faces_end(); ++fit) {
      for (int i=0; i<3; i++) {
        p[i] = &fit->vertex(i)->point();
        off[i] = get_offset(fit,i);
      }

      /// Check whether the vertices of the neighbor lie outside the circumcircle of the face
      for (int i=0; i<3; ++i) {
        p[3]   = &fit->vertex(i)->point();
        off[3] = combine_offsets(get_offset(fit,i), get_neighbor_offset(fit, i));

        result &= ON_POSITIVE_SIDE !=
          side_of_oriented_circle(*p[0], *p[1], *p[2], *p[3],
                                  off[0], off[1], off[2], off[3],
                                  false);
        CGAL_triangulation_assertion(result);
      }
    }
  }

  return result;
}

template < class Gt, class Tds >
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
nearest_vertex(const Point  &p, Face_handle f) const
{
  switch (dimension()) {
  case 0:
    return Vertex_handle();
    //break;
  case 2:
    return nearest_vertex_2D(p,f);
    //break;
  }
  return Vertex_handle();
}

template < class Gt, class Tds >
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
nearest_vertex_2D(const Point& p, Face_handle f) const
{
  CGAL_triangulation_precondition(dimension() == 2);
  f = locate(p,f);

  typename Geom_traits::Compare_distance_2 compare_distance =
    geom_traits().compare_distance_2_object();
  Vertex_handle nn =  f->vertex(0);
  if (compare_distance(p, f->vertex(1)->point(), nn->point()) == SMALLER)
    nn = f->vertex(1);
  if (compare_distance(p, f->vertex(2)->point(), nn->point()) == SMALLER)
    nn = f->vertex(2);

  look_nearest_neighbor(p,f,0,nn);
  look_nearest_neighbor(p,f,1,nn);
  look_nearest_neighbor(p,f,2,nn);

  return nn;
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
look_nearest_neighbor(const Point& p,
                      Face_handle f,
		      int i,
		      Vertex_handle& nn) const
{
  Face_handle  ni=f->neighbor(i);
  if ( this->side_of_oriented_circle(ni,p,true) != ON_POSITIVE_SIDE )
    return;

  typename Geom_traits::Compare_distance_2 compare_distance =
    geom_traits().compare_distance_2_object();
  i = ni->index(f);
  if (compare_distance(p, ni->vertex(i)->point(), nn->point()) == SMALLER)
    nn=ni->vertex(i);

  // recursive exploration of triangles whose circumcircle contains p
  look_nearest_neighbor(p, ni, ccw(i), nn);
  look_nearest_neighbor(p, ni, cw(i), nn);
}

//DUALITY
template<class Gt, class Tds>
inline
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Point
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
dual (Face_handle f) const
{
  CGAL_triangulation_precondition (dimension()==2);
  return Triangulation::circumcenter(f);
}


template < class Gt, class Tds >
inline typename Gt::Segment_2
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
dual(const Edge &e) const
{
    // dimension==2
    Face_handle nb = e.first->neighbor(e.second);
    Point p0 = dual(e.first);
    Point p1 = dual(nb);
    Offset o = combine_offsets( Offset(), get_neighbor_offset(e.first, e.second));
    Segment s = geom_traits().construct_segment_2_object()(p0, p1, o, Offset());
    
    return s;
}

template < class Gt, class Tds >
inline typename Gt::Segment_2
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
dual(const Edge_circulator& ec) const
{
  return dual(*ec);
}
template < class Gt, class Tds >
inline typename Gt::Segment_2
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
dual(const Edge_iterator& ei) const {
  return dual(*ei);
}

///////////////////////////////////////////////////////////////
//  INSERT

template < class Gt, class Tds >
inline
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
insert(const Point  &p,  Face_handle start)
{
  CGAL_triangulation_assertion((this->domain().xmin() <= p.x()) &&
                               (p.x() < this->domain().xmax()));
  CGAL_triangulation_assertion((this->domain().ymin() <= p.y()) &&
                               (p.y() < this->domain().ymax()));

  if (empty()) {
    return this->insert_first(p);
  }

  if (start == Face_handle()) {
    start = this->faces_begin();
  }

  Locate_type lt;
  int li;
  Face_handle loc = locate (p, lt, li, start);

  /// Call the insert function with the located simplex
  return insert(p, lt, loc, li);
}

template < class Gt, class Tds >
inline
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
push_back(const Point &p)
{
  return insert(p);
}

template < class Gt, class Tds >
inline
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
insert(const Point  &p, Locate_type lt, Face_handle loc, int li)
{
  Vertex_handle vh = Triangulation::insert(p,lt,loc,li);

  if (lt != Triangulation::VERTEX) {
    restore_Delaunay(vh);

    if (!is_1_cover()) {
      typename Triangulation::Virtual_vertex_reverse_map_it vertices_it =
        this->virtual_vertices_reverse().find(vh);
      CGAL_triangulation_assertion(vertices_it != this->virtual_vertices_reverse().end());
      const std::vector<Vertex_handle> &virtual_vertices = vertices_it->second;
      for (size_t i=0; i<virtual_vertices.size(); ++i) {
        restore_Delaunay(virtual_vertices[i]);
      }

      this->try_to_convert_to_one_cover();
      if (is_1_cover()) {
        CGAL_triangulation_assertion(is_valid());
      }
    }
  }

  return vh;
}

template < class Gt, class Tds >
template < class OutputItFaces >
inline
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
insert_and_give_new_faces(const Point  &p,
                          OutputItFaces oif,
                          Face_handle start)
{
    NGHK_NYI;
  Vertex_handle v = insert(p, start);
  int dim = dimension();
  if(dim == 2)
  {
    Face_circulator fc = this->incident_faces(v), done(fc);
    do {
      *oif++ = fc;
    } while(++fc != done);
  }
  else if(dim == 1)
  {
    Face_handle c = v->face();
    *oif++ = c;
    *oif++ = c->neighbor((~(c->index(v)))&1);
  }
  else *oif++ = v->face(); // dimension == 0
  return v;
}

template < class Gt, class Tds >
template < class OutputItFaces >
inline
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
insert_and_give_new_faces(const Point  &p,
                          Locate_type lt,
                          Face_handle loc, int li,
                          OutputItFaces oif)
{
    NGHK_NYI;
  Vertex_handle v = insert(p, lt, loc, li);
  int dim = dimension();
  if(dim == 2)
  {
    Face_circulator fc = this->incident_faces(v), done(fc);
    do {
      *oif++ = fc;
    } while(++fc != done);
  }
  else if(dim == 1)
  {
    Face_handle c = v->face();
    *oif++ = c;
    *oif++ = c->neighbor((~(c->index(v)))&1);
  }
  else *oif++ = v->face(); // dimension == 0
  return v;
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
restore_Delaunay(Vertex_handle v)
{
  Face_handle f=v->face();
  Face_handle next;
  int i;
  Face_handle start(f);
  do {
    i = f->index(v);
    next = f->neighbor(ccw(i));  // turn ccw around v
    propagating_flip(f,i);
    f=next;
  } while(next != start);
}


#ifndef CGAL_DT2_USE_RECURSIVE_PROPAGATING_FLIP
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
non_recursive_propagating_flip(Face_handle f , int i)
{
  std::stack<Edge> edges;
  const Vertex_handle& vp = f->vertex(i);
  edges.push(Edge(f,i));

  while(! edges.empty()){
    const Edge& e = edges.top();
    f = e.first; 
    i = e.second;
    const Face_handle& n = f->neighbor(i);
    
    if (locally_Delaunay(f, i, n)) {
      edges.pop();
      continue;
    }
    this->flip_single_edge(f, i);
    // As we haven't popped it, we don't have to push it
    edges.push(Edge(n,n->index(vp)));
  }
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
propagating_flip(const Face_handle& f,int i, int depth)
{
#ifdef CGAL_DT2_IMMEDIATELY_NON_RECURSIVE_PROPAGATING_FLIP
  non_recursive_propagating_flip(f,i);
#else
  int max_depth = 100;
  if(depth==max_depth){
    non_recursive_propagating_flip(f,i);
    return;
  }
  Face_handle n = f->neighbor(i);
      
  if (locally_Delaunay(f, i, n))
    return;

  this->flip_single_edge(f, i);
  propagating_flip(f,i,depth+1);
  i = n->index(f->vertex(i));
  propagating_flip(n,i,depth+1);
#endif
}
#else 
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
propagating_flip(Face_handle& f,int i)
{
  Face_handle nb = f->neighbor(i);

  if (locally_Delaunay(f, nb))
    return;

  this->flip_single_edge(f, i);
  propagating_flip(f,i);
  i = nb->index(f->vertex(i));
  propagating_flip(nb,i);
}
#endif

template < class Gt, class Tds >
bool
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
locally_Delaunay(const Face_handle &f, int i, const Face_handle &nb) {
  CGAL_BRANCH_PROFILER("locally_Delaunay(), simplicity check failures", tmp);

  bool simplicity_criterion = is_1_cover() && f->has_zero_offsets() && nb->has_zero_offsets();

  const Point *p[4];
  for (int index=0; index<3; ++index) {
    p[index]   = &nb->vertex(index)->point();
  }
  p[3]   = &f->vertex(i)->point();

  Oriented_side os;
  if (simplicity_criterion) {
    // No periodic offsets
    os = side_of_oriented_circle(*p[0], *p[1], *p[2], *p[3], true);
  } else {
    CGAL_BRANCH_PROFILER_BRANCH(tmp);

    Offset off[4];

    for (int index=0; index<3; ++index) {
      off[index] = get_offset(nb,index);
    }
    off[3] = combine_offsets(get_offset(f,i), get_neighbor_offset(f, i));

    os = side_of_oriented_circle(*p[0], *p[1], *p[2], *p[3],
                                 off[0], off[1], off[2], off[3], true);
  }

  return (ON_POSITIVE_SIDE != os);
}

///////////////////////////////////////////////////////////////
//  REMOVE    see INRIA RResearch Report 7104

template < class Gt, class Tds >
template <class OutputItFaces>
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_and_give_new_faces(Vertex_handle v, OutputItFaces fit)
{
  NGHK_NYI;
  CGAL_triangulation_precondition( v != Vertex_handle());
  CGAL_triangulation_precondition( !is_infinite(v));

  if(this->number_of_vertices() == 1) this->remove_first(v);
  else if(this->number_of_vertices() == 2) this->remove_second(v);
  else if( dimension() == 1)
  {
    Point p = v->point();
    Triangulation::remove(v);
    *fit++ = locate(p);
  }
  else if (this->test_dim_down(v)) {
    this->_tds.remove_dim_down(v);
    for(All_faces_iterator afi = this-> all_faces_begin();
        afi != this->all_faces_end();
        afi++) *fit++ = afi;
  }
  else {
    static int maxd=30;
    static std::vector<Face_handle> f(maxd);
    static std::vector<int> i(maxd);
    static std::vector<Vertex_handle> w(maxd);
    int d;
    remove_degree_init(v,f,w,i,d,maxd);
    remove_degree_triangulate(v,f,w,i,d);
    this->delete_vertex(v);
    Face_circulator fc(v[0]),done;
    do *fit++ = fc++; while (fc!=done);
  }
  return;
}


template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove(Vertex_handle v)
{
  // Make sure we have the original vertex
  CGAL_assertion(v == this->get_original_vertex(v));

  CGAL_triangulation_precondition(v != Vertex_handle());
  CGAL_triangulation_precondition(dimension() == 2);

  if ( this->number_of_vertices() == 1) {
    // Last vertex
    Triangulation::remove_first(v);
    return;
  }

  if (!remove_single_vertex(v, Offset())) {
    // The vertex was not removed as we need to revert to the 9-cover first
    this->convert_to_9_sheeted_covering();

    remove_single_vertex(v, Offset());
  }

  if (!is_1_cover()) {
    CGAL_assertion(this->virtual_vertices_reverse().find(v) != this->virtual_vertices_reverse().end());

    const std::vector<Vertex_handle> &virtual_copies = this->virtual_vertices_reverse().find(v)->second;
    for (size_t i=0; i<8; ++i) {
      remove_single_vertex(virtual_copies[i], Offset((i+1)/3, (i+1)%3));
    }

    this->remove_from_virtual_copies(v);
  }
}

template < class Gt, class Tds >
bool
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_single_vertex(Vertex_handle v, const Offset &v_o) {
  static int maxd=30;
  static std::vector<Face_handle> f(maxd);
  static std::vector<int> i(maxd);
  static std::vector<Vertex_handle> w(maxd);
  static std::vector<Offset> offset_w(maxd);
  int d;
  bool simplicity_criterion;

  if (remove_degree_init(v,v_o, f,w,offset_w,i,d,maxd, simplicity_criterion)) {
    if (is_1_cover()) {
      // Don't delete if the hole is too big and the triangulation is a 1-cover
      return false;
    }
  }

  if (simplicity_criterion)
    remove_degree_triangulate(v,f,w,i,d);
  else
    remove_degree_triangulate(v,f,w,offset_w,i,d);

  this->delete_vertex(v);

  return true;
}


template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree_triangulate(Vertex_handle v,
                          std::vector<Face_handle> &f,
                          std::vector<Vertex_handle> &w,
                          std::vector<int> &i,int d)
{
  // degree: 3: 1%, 4: 9%, 5: 23%, 6: 35%, 7: 19%, r: 10%

  switch (d) {
  case 3:
    remove_degree3(v,f,w,i);    break;
  case 4:
    remove_degree4(v,f,w,i);    break;
  case 5:
    remove_degree5(v,f,w,i);    break;
  case 6:
    remove_degree6(v,f,w,i);    break;
  case 7:
    remove_degree7(v,f,w,i);    break;
  default:
    remove_degree_d(v,f,w,i,d); break;
  }
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree_triangulate(Vertex_handle v,
                          std::vector<Face_handle> &f,
                          std::vector<Vertex_handle> &w,
                          std::vector<Offset> &offset_w,
                          std::vector<int> &i,int d)
{
  // degree: 3: 1%, 4: 9%, 5: 23%, 6: 35%, 7: 19%, r: 10%

  switch (d) {
  case 3:
    remove_degree3(v,f,w,offset_w,i);    break;
  case 4:
    remove_degree4(v,f,w,offset_w,i);    break;
  case 5:
    remove_degree5(v,f,w,offset_w,i);    break;
  case 6:
    remove_degree6(v,f,w,offset_w,i);    break;
  case 7:
    remove_degree7(v,f,w,offset_w,i);    break;
  default:
    remove_degree_d(v,f,w,offset_w,i,d); break;
  }
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree_d(Vertex_handle v, std::vector<Face_handle> &,
                std::vector<Vertex_handle> &,
                std::vector<int> &, int)
{
  std::list<Edge> hole;
  this->make_hole(v, hole);

  fill_hole_delaunay(hole);

  return;
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree_d(Vertex_handle v, std::vector<Face_handle> &,
                std::vector<Vertex_handle> &w, std::vector<Offset> &offset_w,
                std::vector<int> &,int d)
{
  std::list<Edge> hole;
  this->make_hole(v, hole);

  std::map<Vertex_handle, Offset> vertex_offsets;
  for (int idx=0; idx<d; ++idx) {
    vertex_offsets[w[idx]] = offset_w[idx];
  }

  fill_hole_delaunay(hole, vertex_offsets);

  return;
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree3(Vertex_handle, std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &,
               std::vector<int> &i)
{
  // modify the triangulation
  Face_handle nn= f[1]->neighbor( i[1] );
  tds().set_adjacency(f[0], ccw(i[0]) , nn , nn->index(f[1])  );
  nn= f[2]->neighbor( i[2] );
  tds().set_adjacency(f[0], cw(i[0]) , nn , nn->index(f[2])  );
  f[0]->set_vertex  (            i[0] , f[1]->vertex( cw(i[1]) ) );

  // clean container
  tds().delete_face(f[1]);
  tds().delete_face(f[2]);

  this->set_offsets(f[0], 0,0,0);

  return;
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree3(Vertex_handle, std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &, std::vector<Offset> &o,
               std::vector<int> &i)
{
  // modify the triangulation
  Face_handle nn= f[1]->neighbor( i[1] );
  tds().set_adjacency(f[0], ccw(i[0]) , nn , nn->index(f[1])  );
  nn= f[2]->neighbor( i[2] );
  tds().set_adjacency(f[0], cw(i[0]) , nn , nn->index(f[2])  );
  f[0]->set_vertex  (            i[0] , f[1]->vertex( cw(i[1]) ) );

  // clean container
  tds().delete_face(f[1]);
  tds().delete_face(f[2]);

  Offset oo[3];
  oo[    i[0] ] = o[2];
  oo[ cw(i[0])] = o[1];
  oo[ccw(i[0])] = o[0];

  if (oo[0].x() < 0 || oo[1].x() < 0 || oo[2].x() < 0) {
    Offset o(number_of_sheets()[0], 0);
    oo[0] += o; oo[1] += o; oo[2] += o;
  }
  if (oo[0].y() < 0 || oo[1].y() < 0 || oo[2].y() < 0) {
    Offset o(0, number_of_sheets()[1]);
    oo[0] += o; oo[1] += o; oo[2] += o;
  }
  this->set_offsets(f[0], 
                    (oo[0].x() >= number_of_sheets()[0] ? 2 : 0) + (oo[0].y() >= number_of_sheets()[1] ? 1 : 0),
                    (oo[1].x() >= number_of_sheets()[0] ? 2 : 0) + (oo[1].y() >= number_of_sheets()[1] ? 1 : 0),
                    (oo[2].x() >= number_of_sheets()[0] ? 2 : 0) + (oo[2].y() >= number_of_sheets()[1] ? 1 : 0));

  return;
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree4(Vertex_handle, std::vector<Face_handle> &f,
	       std::vector<Vertex_handle> &w, 
               std::vector<int> &i )
{
  // removing a degree 4 vertex

  Face_handle nn;

  if ( !incircle(2,0,1,3,f,w,i) )  {
    // diagonal 1 3
    f[0]->set_vertex( i[0], w[3] ); //w0 w1 w3
    f[1]->set_vertex( i[1], w[3] ); //w1 w2 w3
    nn = f[3]->neighbor( i[3] );
    tds().set_adjacency(f[0], cw(i[0]) , nn , nn->index(f[3])  );
    nn = f[2]->neighbor( i[2] );
    tds().set_adjacency(f[1], ccw(i[1]) , nn , nn->index(f[2]) );
    // clean container
    tds().delete_face(f[2]);
    tds().delete_face(f[3]);

    f[0]->set_offsets(0,0,0);
    f[1]->set_offsets(0,0,0);

    this->insert_too_long_edge(f[0], ccw(i[0]));
  }else{
    // diagonal 0 2
    f[0]->set_vertex( i[0], w[2]); //w0 w1 w2
    f[3]->set_vertex( i[3], w[2]); //w3 w0 w2
    nn = f[1]->neighbor( i[1] );
    tds().set_adjacency(f[0], ccw(i[0]) , nn , nn->index(f[1])  );
    nn = f[2]->neighbor( i[2] );
    tds().set_adjacency(f[3], cw(i[3]) , nn , nn->index(f[2])  );
    // clean container
    tds().delete_face(f[1]);
    tds().delete_face(f[2]);

    f[0]->set_offsets(0,0,0);
    f[3]->set_offsets(0,0,0);
  }
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree4(Vertex_handle, std::vector<Face_handle> &f,
	       std::vector<Vertex_handle> &w, std::vector<Offset> &o,
               std::vector<int> &i )
{
  // removing a degree 4 vertex

  Face_handle nn;

  int oo[4];
  if ((o[0] == o[1]) && (o[0] == o[2]) && (o[0] == o[3])) {
    for (int i=0; i<4; ++i) oo[i] = 0;
  } else {
    Covering_sheets cover = number_of_sheets();
    if ((o[0].x() < 0) || (o[1].x() < 0) || (o[2].x() < 0) || (o[3].x() < 0))
      for (int i=0; i<4; ++i)
        o[i] += Offset(cover[0], 0);

    if ((o[0].y() < 0) || (o[1].y() < 0) || (o[2].y() < 0) || (o[3].y() < 0))
      for (int i=0; i<4; ++i)
        o[i] += Offset(0, cover[1]);

    for (int i=0; i<4; ++i) {
      oo[i] = (o[i].x() >= cover[0] ? 2 : 0) + (o[i].y() >= cover[1] ? 1 : 0); 
    }
  }

  if ( !incircle(2,0,1,3,f,w,o,i) )  {
    // diagonal 1 3
    f[0]->set_vertex( i[0], w[3] ); //w0 w1 w3
    f[1]->set_vertex( i[1], w[3] ); //w1 w2 w3
    nn = f[3]->neighbor( i[3] );
    tds().set_adjacency(f[0], cw(i[0]) , nn , nn->index(f[3])  );
    nn = f[2]->neighbor( i[2] );
    tds().set_adjacency(f[1], ccw(i[1]) , nn , nn->index(f[2]) );
    // clean container
    tds().delete_face(f[2]);
    tds().delete_face(f[3]);

    int o_face[3];
    o_face[i[0]]      = oo[3];
    o_face[ccw(i[0])] = oo[0];
    o_face[ cw(i[0])] = oo[1];
    this->set_offsets(f[0], o_face[0], o_face[1], o_face[2]);
    o_face[i[1]]      = oo[3];
    o_face[ccw(i[1])] = oo[1];
    o_face[ cw(i[1])] = oo[2];
    this->set_offsets(f[1], o_face[0], o_face[1], o_face[2]);

    this->insert_too_long_edge(f[0], ccw(i[0]));
  }else{
    // diagonal 0 2
    f[0]->set_vertex( i[0], w[2]); //w0 w1 w2
    f[3]->set_vertex( i[3], w[2]); //w3 w0 w2
    nn = f[1]->neighbor( i[1] );
    tds().set_adjacency(f[0], ccw(i[0]) , nn , nn->index(f[1])  );
    nn = f[2]->neighbor( i[2] );
    tds().set_adjacency(f[3], cw(i[3]) , nn , nn->index(f[2])  );
    // clean container
    tds().delete_face(f[1]);
    tds().delete_face(f[2]);

    int o_face[3];
    o_face[i[0]]      = oo[2];
    o_face[ccw(i[0])] = oo[0];
    o_face[ cw(i[0])] = oo[1];
    this->set_offsets(f[0], o_face[0], o_face[1], o_face[2]);
    o_face[i[3]]      = oo[2];
    o_face[ccw(i[3])] = oo[3];
    o_face[ cw(i[3])] = oo[0];
    this->set_offsets(f[3], o_face[0], o_face[1], o_face[2]);

    this->insert_too_long_edge(f[3], ccw(i[3]));
  }
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree5(Vertex_handle v, std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &w,
               std::vector<int> &i)
{
  // removing a degree 5 vertex
  this->remove_too_long_edges_in_star(v);

  if (incircle(3,0,1,2,f,w,i)) {
    if (incircle(4,0,1,3,f,w,i)) {
      if (incircle(4,1,2,3,f,w,i)) {
	// star from 4
	remove_degree5_star(v,f[4],f[0],f[1],f[2],f[3],
			      w[4],w[0],w[1],w[2],w[3],
			      i[4],i[0],i[1],i[2],i[3]);
      }else{
	//star from 1
	remove_degree5_star(v,f[1],f[2],f[3],f[4],f[0],
			      w[1],w[2],w[3],w[4],w[0],
			      i[1],i[2],i[3],i[4],i[0]);


      }
    }else{
      // star from 3
      remove_degree5_star(v,f[3],f[4],f[0],f[1],f[2],
			    w[3],w[4],w[0],w[1],w[2],
			    i[3],i[4],i[0],i[1],i[2]);
    }
  } else {
    if (incircle(4,2,3,0,f,w,i)){
      if (incircle(4,0,1,2,f,w,i)){
	// star from 4
	remove_degree5_star(v,f[4],f[0],f[1],f[2],f[3],
			      w[4],w[0],w[1],w[2],w[3],
			      i[4],i[0],i[1],i[2],i[3]);
      }else{
	//star from 2
	remove_degree5_star(v,f[2],f[3],f[4],f[0],f[1],
			      w[2],w[3],w[4],w[0],w[1],
			      i[2],i[3],i[4],i[0],i[1]);
      }
    }else{
      // star from 0
      remove_degree5_star(v,f[0],f[1],f[2],f[3],f[4],
			    w[0],w[1],w[2],w[3],w[4],
			    i[0],i[1],i[2],i[3],i[4]);
    }
  }
  return;
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree5(Vertex_handle v, std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &w, std::vector<Offset> &o,
               std::vector<int> &i)
{
  // removing a degree 5 vertex
  this->remove_too_long_edges_in_star(v);

  if (incircle(3,0,1,2,f,w,o,i)) {
    if (incircle(4,0,1,3,f,w,o,i)) {
      if (incircle(4,1,2,3,f,w,o,i)) {
	// star from 4
	remove_degree5_star(v,f[4],f[0],f[1],f[2],f[3],
			      w[4],w[0],w[1],w[2],w[3],
			      o[4],o[0],o[1],o[2],o[3],
			      i[4],i[0],i[1],i[2],i[3]);
      }else{
	//star from 1
	remove_degree5_star(v,f[1],f[2],f[3],f[4],f[0],
			      w[1],w[2],w[3],w[4],w[0],
			      o[1],o[2],o[3],o[4],o[0],
			      i[1],i[2],i[3],i[4],i[0]);


      }
    }else{
      // star from 3
      remove_degree5_star(v,f[3],f[4],f[0],f[1],f[2],
			    w[3],w[4],w[0],w[1],w[2],
			    o[3],o[4],o[0],o[1],o[2],
			    i[3],i[4],i[0],i[1],i[2]);
    }
  } else {
    if (incircle(4,2,3,0,f,w,o,i)){
      if (incircle(4,0,1,2,f,w,o,i)){
	// star from 4
	remove_degree5_star(v,f[4],f[0],f[1],f[2],f[3],
			      w[4],w[0],w[1],w[2],w[3],
			      o[4],o[0],o[1],o[2],o[3],
			      i[4],i[0],i[1],i[2],i[3]);
      }else{
	//star from 2
	remove_degree5_star(v,f[2],f[3],f[4],f[0],f[1],
			      w[2],w[3],w[4],w[0],w[1],
			      o[2],o[3],o[4],o[0],o[1],
			      i[2],i[3],i[4],i[0],i[1]);
      }
    }else{
      // star from 0
      remove_degree5_star(v,f[0],f[1],f[2],f[3],f[4],
			    w[0],w[1],w[2],w[3],w[4],
			    o[0],o[1],o[2],o[3],o[4],
			    i[0],i[1],i[2],i[3],i[4]);
    }
  }
  return;
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree5_star
(Vertex_handle &,
 Face_handle &f0, Face_handle &f1, Face_handle &f2, Face_handle &f3, Face_handle &f4,
 Vertex_handle &v0, Vertex_handle &, Vertex_handle &, Vertex_handle &, Vertex_handle &,
 int i0, int i1, int i2, int i3, int i4 )
{ // removing a degree 5 vertex, starring from v0
  Face_handle nn;
  f1->set_vertex( i1, v0) ;  // f1 = v1v2v0
  f2->set_vertex( i2, v0) ;  // f2 = v2v3v0
  f3->set_vertex( i3, v0) ;  // f3 = v3v4v0

  nn = f0->neighbor( i0 );
  tds().set_adjacency(f1, cw(i1) , nn , nn->index(f0) );
  nn = f4->neighbor( i4 );
  tds().set_adjacency(f3, ccw(i3) , nn , nn->index(f4) );
  tds().delete_face(f0);
  tds().delete_face(f4);

  f1->set_offsets(0,0,0);
  f2->set_offsets(0,0,0);
  f3->set_offsets(0,0,0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree5_star
(Vertex_handle &,
 Face_handle &f0, Face_handle &f1, Face_handle &f2, Face_handle &f3, Face_handle &f4,
 Vertex_handle &v0, Vertex_handle &, Vertex_handle &, Vertex_handle &, Vertex_handle &,
 Offset &o0, Offset &o1, Offset &o2, Offset &o3, Offset &o4,
 int i0, int i1, int i2, int i3, int i4 )
{ // removing a degree 5 vertex, starring from v0
  Face_handle nn;
  f1->set_vertex( i1, v0) ;  // f1 = v1v2v0
  f2->set_vertex( i2, v0) ;  // f2 = v2v3v0
  f3->set_vertex( i3, v0) ;  // f3 = v3v4v0

  nn = f0->neighbor( i0 );
  tds().set_adjacency(f1, cw(i1) , nn , nn->index(f0) );
  nn = f4->neighbor( i4 );
  tds().set_adjacency(f3, ccw(i3) , nn , nn->index(f4) );
  tds().delete_face(f0);
  tds().delete_face(f4);

  if (o0.x() < 0 || o1.x() < 0 || o2.x() < 0 || o3.x() < 0 || o4.x() < 0) {
    o0 += Offset(number_of_sheets()[0], 0);
    o1 += Offset(number_of_sheets()[0], 0);
    o2 += Offset(number_of_sheets()[0], 0);
    o3 += Offset(number_of_sheets()[0], 0);
    o4 += Offset(number_of_sheets()[0], 0);
  }
  if (o0.y() < 0 || o1.y() < 0 || o2.y() < 0 || o3.y() < 0 || o4.y() < 0) {
    o0 += Offset(0, number_of_sheets()[1]);
    o1 += Offset(0, number_of_sheets()[1]);
    o2 += Offset(0, number_of_sheets()[1]);
    o3 += Offset(0, number_of_sheets()[1]);
    o4 += Offset(0, number_of_sheets()[1]);
  }
  int oo0 = (o0.x() >= number_of_sheets()[0] ? 2 : 0) + (o0.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo1 = (o1.x() >= number_of_sheets()[0] ? 2 : 0) + (o1.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo2 = (o2.x() >= number_of_sheets()[0] ? 2 : 0) + (o2.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo3 = (o3.x() >= number_of_sheets()[0] ? 2 : 0) + (o3.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo4 = (o4.x() >= number_of_sheets()[0] ? 2 : 0) + (o4.y() >= number_of_sheets()[1] ? 1 : 0);

  int oo[3];
  oo[i1]      = oo0;
  oo[ccw(i1)] = oo1;
  oo[ cw(i1)] = oo2;
  this->set_offsets(f1, oo[0], oo[1], oo[2]);
  oo[i2]      = oo0;
  oo[ccw(i2)] = oo2;
  oo[ cw(i2)] = oo3;
  this->set_offsets(f2, oo[0], oo[1], oo[2]);
  oo[i3]      = oo0;
  oo[ccw(i3)] = oo3;
  oo[ cw(i3)] = oo4;
  this->set_offsets(f3, oo[0], oo[1], oo[2]);

  //this->insert_too_long_edges_in_star(f1->vertex(i1));
  this->insert_too_long_edge(f1, ccw(i1));
  this->insert_too_long_edge(f2, ccw(i2));
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree6(Vertex_handle v, std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &w, 
               std::vector<int> &i)
{
  if(incircle(1,2,3,0,f,w,i)){
    if(incircle(4,2,3,5,f,w,i)){
      if(incircle(1,2,3,4,f,w,i)){
	if(incircle(4,0,1,3,f,w,i)){
	  if(incircle(5,0,1,4,f,w,i)){
	    remove_degree6_star(v,
                                f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	  }else{
	    remove_degree6_N(v,
                             f[1],f[2],f[3],f[4],f[5],f[0],
                             w[1],w[2],w[3],w[4],w[5],w[0],
                             i[1],i[2],i[3],i[4],i[5],i[0]);
	  }}else{
	  remove_degree6_antiN(v,
                               f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	}}else{
	if(incircle(5,1,2,4,f,w,i)){
	  remove_degree6_N(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	}else{
	  if(incircle(5,0,1,4,f,w,i)){
	    remove_degree6_antiN(v,
                                 f[1],f[2],f[3],f[4],f[5],f[0],
                                 w[1],w[2],w[3],w[4],w[5],w[0],
                                 i[1],i[2],i[3],i[4],i[5],i[0]);
	  }else{
	    remove_degree6_star(v,f[4],f[5],f[0],f[1],f[2],f[3],
				w[4],w[5],w[0],w[1],w[2],w[3],
				i[4],i[5],i[0],i[1],i[2],i[3]);
	  }}}}else{
      if(incircle(1,2,3,5,f,w,i)){
	if(incircle(1,3,4,5,f,w,i)){
	  if(incircle(4,0,1,3,f,w,i)){
	    if(incircle(5,0,1,4,f,w,i)){
	    remove_degree6_star(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	    }else{
	    remove_degree6_N(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	    }}else{
	  remove_degree6_antiN(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }}else{
	  if(incircle(5,0,1,3,f,w,i)){
	    remove_degree6_diamond(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	  }else{
	    if(incircle(4,5,0,3,f,w,i)){
	  remove_degree6_antiN(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	    }else{
	    remove_degree6_star(v,f[3],f[4],f[5],f[0],f[1],f[2],
				w[3],w[4],w[5],w[0],w[1],w[2],
				i[3],i[4],i[5],i[0],i[1],i[2]);
	    }}}}else{
	    remove_degree6_star(v,f[5],f[0],f[1],f[2],f[3],f[4],
				w[5],w[0],w[1],w[2],w[3],w[4],
				i[5],i[0],i[1],i[2],i[3],i[4]);
      }}}else{
    if(incircle(4,2,3,5,f,w,i)){
      if(incircle(4,2,3,0,f,w,i)){
	if(incircle(4,0,1,2,f,w,i)){
	  if(incircle(4,1,2,5,f,w,i)){
	    if(incircle(4,0,1,5,f,w,i)){
	    remove_degree6_star(v,f[4],f[5],f[0],f[1],f[2],f[3],
				w[4],w[5],w[0],w[1],w[2],w[3],
				i[4],i[5],i[0],i[1],i[2],i[3]);
	    }else{
	    remove_degree6_antiN(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	    }}else{
	  remove_degree6_N(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	  }}else{
	  if(incircle(4,5,0,2,f,w,i)){
	  remove_degree6_diamond(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }else{
	    if(incircle(5,0,1,2,f,w,i)){
	  remove_degree6_N(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	    }else{
	  remove_degree6_star(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	    }}}}else{
	  remove_degree6_star(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
      }}else{
      if(incircle(5,2,3,0,f,w,i)){
	if(incircle(5,0,1,2,f,w,i)){
	  remove_degree6_star(v,f[5],f[0],f[1],f[2],f[3],f[4],
			       w[5],w[0],w[1],w[2],w[3],w[4],
                               i[5],i[0],i[1],i[2],i[3],i[4]);
	}else{
	  remove_degree6_antiN(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	}}else{
	  if(incircle(4,5,0,3,f,w,i)){
	  remove_degree6_star(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }else{
            remove_degree6_N(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }}}}
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree6(Vertex_handle v, std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &w, std::vector<Offset> &o,
               std::vector<int> &i)
{
  // removing a degree 6 vertex
  this->remove_too_long_edges_in_star(v);

  if(incircle(1,2,3,0,f,w,o,i)){
    if(incircle(4,2,3,5,f,w,o,i)){
      if(incircle(1,2,3,4,f,w,o,i)){
	if(incircle(4,0,1,3,f,w,o,i)){
	  if(incircle(5,0,1,4,f,w,o,i)){
	    remove_degree6_star(v,
                                f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				o[1],o[2],o[3],o[4],o[5],o[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	  }else{
	    remove_degree6_N(v,
                             f[1],f[2],f[3],f[4],f[5],f[0],
                             w[1],w[2],w[3],w[4],w[5],w[0],
                             o[1],o[2],o[3],o[4],o[5],o[0],
                             i[1],i[2],i[3],i[4],i[5],i[0]);
	  }}else{
	  remove_degree6_antiN(v,
                               f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       o[0],o[1],o[2],o[3],o[4],o[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	}}else{
	if(incircle(5,1,2,4,f,w,o,i)){
	  remove_degree6_N(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   o[2],o[3],o[4],o[5],o[0],o[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	}else{
	  if(incircle(5,0,1,4,f,w,o,i)){
	    remove_degree6_antiN(v,
                                 f[1],f[2],f[3],f[4],f[5],f[0],
                                 w[1],w[2],w[3],w[4],w[5],w[0],
                                 o[1],o[2],o[3],o[4],o[5],o[0],
                                 i[1],i[2],i[3],i[4],i[5],i[0]);
	  }else{
	    remove_degree6_star(v,f[4],f[5],f[0],f[1],f[2],f[3],
				w[4],w[5],w[0],w[1],w[2],w[3],
				o[4],o[5],o[0],o[1],o[2],o[3],
				i[4],i[5],i[0],i[1],i[2],i[3]);
	  }}}}else{
      if(incircle(1,2,3,5,f,w,o,i)){
	if(incircle(1,3,4,5,f,w,o,i)){
	  if(incircle(4,0,1,3,f,w,o,i)){
	    if(incircle(5,0,1,4,f,w,o,i)){
	    remove_degree6_star(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				o[1],o[2],o[3],o[4],o[5],o[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	    }else{
	    remove_degree6_N(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				o[1],o[2],o[3],o[4],o[5],o[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	    }}else{
	  remove_degree6_antiN(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       o[0],o[1],o[2],o[3],o[4],o[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }}else{
	  if(incircle(5,0,1,3,f,w,o,i)){
	    remove_degree6_diamond(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				o[1],o[2],o[3],o[4],o[5],o[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	  }else{
	    if(incircle(4,5,0,3,f,w,o,i)){
	  remove_degree6_antiN(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       o[0],o[1],o[2],o[3],o[4],o[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	    }else{
	    remove_degree6_star(v,f[3],f[4],f[5],f[0],f[1],f[2],
				w[3],w[4],w[5],w[0],w[1],w[2],
				o[3],o[4],o[5],o[0],o[1],o[2],
				i[3],i[4],i[5],i[0],i[1],i[2]);
	    }}}}else{
	    remove_degree6_star(v,f[5],f[0],f[1],f[2],f[3],f[4],
				w[5],w[0],w[1],w[2],w[3],w[4],
				o[5],o[0],o[1],o[2],o[3],o[4],
				i[5],i[0],i[1],i[2],i[3],i[4]);
      }}}else{
    if(incircle(4,2,3,5,f,w,o,i)){
      if(incircle(4,2,3,0,f,w,o,i)){
	if(incircle(4,0,1,2,f,w,o,i)){
	  if(incircle(4,1,2,5,f,w,o,i)){
	    if(incircle(4,0,1,5,f,w,o,i)){
	    remove_degree6_star(v,f[4],f[5],f[0],f[1],f[2],f[3],
				w[4],w[5],w[0],w[1],w[2],w[3],
				o[4],o[5],o[0],o[1],o[2],o[3],
				i[4],i[5],i[0],i[1],i[2],i[3]);
	    }else{
	    remove_degree6_antiN(v,f[1],f[2],f[3],f[4],f[5],f[0],
				w[1],w[2],w[3],w[4],w[5],w[0],
				o[1],o[2],o[3],o[4],o[5],o[0],
				i[1],i[2],i[3],i[4],i[5],i[0]);
	    }}else{
	  remove_degree6_N(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   o[2],o[3],o[4],o[5],o[0],o[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	  }}else{
	  if(incircle(4,5,0,2,f,w,o,i)){
	  remove_degree6_diamond(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       o[0],o[1],o[2],o[3],o[4],o[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }else{
	    if(incircle(5,0,1,2,f,w,o,i)){
	  remove_degree6_N(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   o[2],o[3],o[4],o[5],o[0],o[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	    }else{
	  remove_degree6_star(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   o[2],o[3],o[4],o[5],o[0],o[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	    }}}}else{
	  remove_degree6_star(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       o[0],o[1],o[2],o[3],o[4],o[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
      }}else{
      if(incircle(5,2,3,0,f,w,o,i)){
	if(incircle(5,0,1,2,f,w,o,i)){
	  remove_degree6_star(v,f[5],f[0],f[1],f[2],f[3],f[4],
			       w[5],w[0],w[1],w[2],w[3],w[4],
			       o[5],o[0],o[1],o[2],o[3],o[4],
			      i[5],i[0],i[1],i[2],i[3],i[4]);
	}else{
	  remove_degree6_antiN(v,f[2],f[3],f[4],f[5],f[0],f[1],
			   w[2],w[3],w[4],w[5],w[0],w[1],
			   o[2],o[3],o[4],o[5],o[0],o[1],
			   i[2],i[3],i[4],i[5],i[0],i[1]);
	}}else{
	  if(incircle(4,5,0,3,f,w,o,i)){
	  remove_degree6_star(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       o[0],o[1],o[2],o[3],o[4],o[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }else{
	  remove_degree6_N(v,f[0],f[1],f[2],f[3],f[4],f[5],
			       w[0],w[1],w[2],w[3],w[4],w[5],
			       o[0],o[1],o[2],o[3],o[4],o[5],
			       i[0],i[1],i[2],i[3],i[4],i[5]);
	  }}}}
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_star
(Vertex_handle &,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &, Vertex_handle &,
 Vertex_handle &, Vertex_handle &, Vertex_handle &,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, staring from v0
  Face_handle nn;
  f1->set_vertex( i1, v0) ;  // f1 = v1v2v0
  f2->set_vertex( i2, v0) ;  // f2 = v2v3v0
  f3->set_vertex( i3, v0) ;  // f3 = v3v4v0
  f4->set_vertex( i4, v0) ;  // f4 = v4v5v0
  nn = f0->neighbor( i0 );
  tds().set_adjacency(f1, cw(i1), nn, nn->index(f0));
  nn = f5->neighbor( i5 );
  tds().set_adjacency(f4, ccw(i4), nn,  nn->index(f5));
  tds().delete_face(f0);
  tds().delete_face(f5);

  f1->set_offsets(0,0,0);
  f2->set_offsets(0,0,0);
  f3->set_offsets(0,0,0);
  f4->set_offsets(0,0,0);
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_star
(Vertex_handle &v,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &v1, Vertex_handle &v2,
 Vertex_handle &v3, Vertex_handle &v4, Vertex_handle &v5,
 Offset &o0, Offset &o1, Offset &o2,
 Offset &o3, Offset &o4, Offset &o5,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, staring from v0
  remove_degree6_star(v,
                      f0, f1, f2, f3, f4, f5,
                      v0, v1, v2, v3, v4, v5,
                      i0, i1, i2, i3, i4, i5);
  
  if (o0.x() < 0 || o1.x() < 0 || o2.x() < 0 || o3.x() < 0 || o4.x() < 0 || o5.x() < 0) {
    o0 += Offset(number_of_sheets()[0], 0);
    o1 += Offset(number_of_sheets()[0], 0);
    o2 += Offset(number_of_sheets()[0], 0);
    o3 += Offset(number_of_sheets()[0], 0);
    o4 += Offset(number_of_sheets()[0], 0);
    o5 += Offset(number_of_sheets()[0], 0);
  }
  if (o0.y() < 0 || o1.y() < 0 || o2.y() < 0 || o3.y() < 0 || o4.y() < 0 || o5.y() < 0) {
    o0 += Offset(0, number_of_sheets()[1]);
    o1 += Offset(0, number_of_sheets()[1]);
    o2 += Offset(0, number_of_sheets()[1]);
    o3 += Offset(0, number_of_sheets()[1]);
    o4 += Offset(0, number_of_sheets()[1]);
    o5 += Offset(0, number_of_sheets()[1]);
  }
  int oo0 = (o0.x() >= number_of_sheets()[0] ? 2 : 0) + (o0.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo1 = (o1.x() >= number_of_sheets()[0] ? 2 : 0) + (o1.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo2 = (o2.x() >= number_of_sheets()[0] ? 2 : 0) + (o2.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo3 = (o3.x() >= number_of_sheets()[0] ? 2 : 0) + (o3.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo4 = (o4.x() >= number_of_sheets()[0] ? 2 : 0) + (o4.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo5 = (o5.x() >= number_of_sheets()[0] ? 2 : 0) + (o5.y() >= number_of_sheets()[1] ? 1 : 0);

  int oo[3];
  oo[i1]      = oo0;
  oo[ccw(i1)] = oo1;
  oo[ cw(i1)] = oo2;
  this->set_offsets(f1, oo[0], oo[1], oo[2]);
  oo[i2]      = oo0;
  oo[ccw(i2)] = oo2;
  oo[ cw(i2)] = oo3;
  this->set_offsets(f2, oo[0], oo[1], oo[2]);
  oo[i3]      = oo0;
  oo[ccw(i3)] = oo3;
  oo[ cw(i3)] = oo4;
  this->set_offsets(f3, oo[0], oo[1], oo[2]);
  oo[i4]      = oo0;
  oo[ccw(i4)] = oo4;
  oo[ cw(i4)] = oo5;
  this->set_offsets(f4, oo[0], oo[1], oo[2]);

  this->insert_too_long_edge(f1, ccw(i1));
  this->insert_too_long_edge(f2, ccw(i2));
  this->insert_too_long_edge(f3, ccw(i3));
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_N
(
 Vertex_handle &,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &, Vertex_handle &,
 Vertex_handle &v3, Vertex_handle &, Vertex_handle &,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, N configuration with diagonal v0v3
  Face_handle nn;
  f1->set_vertex( i1, v0) ;  // f1 = v1v2v0
  f2->set_vertex( i2, v0) ;  // f2 = v2v3v0
  f4->set_vertex( i4, v3) ;  // f4 = v4v5v3
  f5->set_vertex( i5, v3) ;  // f5 = v5v0v3
  nn = f0->neighbor( i0 );
  tds().set_adjacency(f1, cw(i1) , nn , nn->index(f0)  );
  nn = f3->neighbor( i3 );
  tds().set_adjacency(f4, cw(i4) , nn, nn->index(f3) );
  tds().set_adjacency(f2, ccw(i2) , f5 , ccw(i5)  );
  tds().delete_face(f0);
  tds().delete_face(f3);

  f1->set_offsets(0,0,0);
  f2->set_offsets(0,0,0);
  f4->set_offsets(0,0,0);
  f5->set_offsets(0,0,0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_N
(
 Vertex_handle &v,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &v1, Vertex_handle &v2,
 Vertex_handle &v3, Vertex_handle &v4, Vertex_handle &v5,
 Offset &o0, Offset &o1, Offset &o2,
 Offset &o3, Offset &o4, Offset &o5,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, N configuration with diagonal v0v3

  remove_degree6_N(v,
                   f0,f1,f2,f3,f4,f5,
                   v0,v1,v2,v3,v4,v5,
                   i0,i1,i2,i3,i4,i5);

  if (o0.x() < 0 || o1.x() < 0 || o2.x() < 0 || o3.x() < 0 || o4.x() < 0 || o5.x() < 0) {
    o0 += Offset(number_of_sheets()[0], 0);
    o1 += Offset(number_of_sheets()[0], 0);
    o2 += Offset(number_of_sheets()[0], 0);
    o3 += Offset(number_of_sheets()[0], 0);
    o4 += Offset(number_of_sheets()[0], 0);
    o5 += Offset(number_of_sheets()[0], 0);
  }
  if (o0.y() < 0 || o1.y() < 0 || o2.y() < 0 || o3.y() < 0 || o4.y() < 0 || o5.y() < 0) {
    o0 += Offset(0, number_of_sheets()[1]);
    o1 += Offset(0, number_of_sheets()[1]);
    o2 += Offset(0, number_of_sheets()[1]);
    o3 += Offset(0, number_of_sheets()[1]);
    o4 += Offset(0, number_of_sheets()[1]);
    o5 += Offset(0, number_of_sheets()[1]);
  }
  int oo0 = (o0.x() >= number_of_sheets()[0] ? 2 : 0) + (o0.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo1 = (o1.x() >= number_of_sheets()[0] ? 2 : 0) + (o1.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo2 = (o2.x() >= number_of_sheets()[0] ? 2 : 0) + (o2.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo3 = (o3.x() >= number_of_sheets()[0] ? 2 : 0) + (o3.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo4 = (o4.x() >= number_of_sheets()[0] ? 2 : 0) + (o4.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo5 = (o5.x() >= number_of_sheets()[0] ? 2 : 0) + (o5.y() >= number_of_sheets()[1] ? 1 : 0);

  int oo[3];
  oo[i1]      = oo0;
  oo[ccw(i1)] = oo1;
  oo[ cw(i1)] = oo2;
  this->set_offsets(f1, oo[0], oo[1], oo[2]);
  oo[i2]      = oo0;
  oo[ccw(i2)] = oo2;
  oo[ cw(i2)] = oo3;
  this->set_offsets(f2, oo[0], oo[1], oo[2]);
  oo[i4]      = oo3;
  oo[ccw(i4)] = oo4;
  oo[ cw(i4)] = oo5;
  this->set_offsets(f4, oo[0], oo[1], oo[2]);
  oo[i5]      = oo3;
  oo[ccw(i5)] = oo5;
  oo[ cw(i5)] = oo0;
  this->set_offsets(f5, oo[0], oo[1], oo[2]);

  this->insert_too_long_edge(f1, ccw(i1));
  this->insert_too_long_edge(f2, ccw(i2));
  this->insert_too_long_edge(f4, ccw(i4));
  this->insert_too_long_edge(f5, ccw(i5));
}


template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_antiN
(
 Vertex_handle &,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &, Vertex_handle &,
 Vertex_handle &v3, Vertex_handle &, Vertex_handle &,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, antiN configuration with diagonal v0v3
  Face_handle nn;
  f0->set_vertex( i0, v3) ;  // f0 = v0v1v3
  f1->set_vertex( i1, v3) ;  // f1 = v1v2v3
  f3->set_vertex( i3, v0) ;  // f3 = v3v4v0
  f4->set_vertex( i4, v0) ;  // f4 = v4v5v0
  nn = f2->neighbor( i2 );
  tds().set_adjacency(f1, ccw(i1) , nn , nn->index(f2)  );
  nn = f5->neighbor( i5 );
  tds().set_adjacency(f4, ccw(i4) , nn , nn->index(f5) );
  tds().set_adjacency(f0, cw(i0) , f3, cw(i3) );
  tds().delete_face(f2);
  tds().delete_face(f5);

  f0->set_offsets(0,0,0);
  f1->set_offsets(0,0,0);
  f3->set_offsets(0,0,0);
  f4->set_offsets(0,0,0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_antiN
(
 Vertex_handle &v,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &v1, Vertex_handle &v2,
 Vertex_handle &v3, Vertex_handle &v4, Vertex_handle &v5,
 Offset &o0, Offset &o1, Offset &o2,
 Offset &o3, Offset &o4, Offset &o5,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, antiN configuration with diagonal v0v3
  remove_degree6_antiN(v,
                       f0,f1,f2,f3,f4,f5,
                       v0,v1,v2,v3,v4,v5,
                       i0,i1,i2,i3,i4,i5);

  if (o0.x() < 0 || o1.x() < 0 || o2.x() < 0 || o3.x() < 0 || o4.x() < 0 || o5.x() < 0) {
    o0 += Offset(number_of_sheets()[0], 0);
    o1 += Offset(number_of_sheets()[0], 0);
    o2 += Offset(number_of_sheets()[0], 0);
    o3 += Offset(number_of_sheets()[0], 0);
    o4 += Offset(number_of_sheets()[0], 0);
    o5 += Offset(number_of_sheets()[0], 0);
  }
  if (o0.y() < 0 || o1.y() < 0 || o2.y() < 0 || o3.y() < 0 || o4.y() < 0 || o5.y() < 0) {
    o0 += Offset(0, number_of_sheets()[1]);
    o1 += Offset(0, number_of_sheets()[1]);
    o2 += Offset(0, number_of_sheets()[1]);
    o3 += Offset(0, number_of_sheets()[1]);
    o4 += Offset(0, number_of_sheets()[1]);
    o5 += Offset(0, number_of_sheets()[1]);
  }
  int oo0 = (o0.x() >= number_of_sheets()[0] ? 2 : 0) + (o0.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo1 = (o1.x() >= number_of_sheets()[0] ? 2 : 0) + (o1.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo2 = (o2.x() >= number_of_sheets()[0] ? 2 : 0) + (o2.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo3 = (o3.x() >= number_of_sheets()[0] ? 2 : 0) + (o3.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo4 = (o4.x() >= number_of_sheets()[0] ? 2 : 0) + (o4.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo5 = (o5.x() >= number_of_sheets()[0] ? 2 : 0) + (o5.y() >= number_of_sheets()[1] ? 1 : 0);

  int oo[3];
  oo[i0]      = oo3;
  oo[ccw(i0)] = oo0;
  oo[ cw(i0)] = oo1;
  this->set_offsets(f0, oo[0], oo[1], oo[2]);
  oo[i1]      = oo3;
  oo[ccw(i1)] = oo1;
  oo[ cw(i1)] = oo2;
  this->set_offsets(f1, oo[0], oo[1], oo[2]);
  oo[i3]      = oo0;
  oo[ccw(i3)] = oo3;
  oo[ cw(i3)] = oo4;
  this->set_offsets(f3, oo[0], oo[1], oo[2]);
  oo[i4]      = oo0;
  oo[ccw(i4)] = oo4;
  oo[ cw(i4)] = oo5;
  this->set_offsets(f4, oo[0], oo[1], oo[2]);

  this->insert_too_long_edge(f0, ccw(i0));
  this->insert_too_long_edge(f1, ccw(i1));
  this->insert_too_long_edge(f3, ccw(i3));
  this->insert_too_long_edge(f4, ccw(i4));
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_diamond
(
 Vertex_handle &,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &, Vertex_handle &v2,
 Vertex_handle &, Vertex_handle &v4, Vertex_handle &,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, with chords v0v2 v2v4 v4v0
  Face_handle nn;
  f0->set_vertex( i0, v2) ;  // f0 = v0v1v2
  f2->set_vertex( i2, v4) ;  // f2 = v2v3v4
  f4->set_vertex( i4, v0) ;  // f4 = v4v5v0
  f1->set_vertex( i1, v4) ;
  f1->set_vertex( ccw(i1), v0) ;  // f1 = v0v2v4
  nn = f1->neighbor( i1 );
  tds().set_adjacency(f0, ccw(i0) , nn , nn->index(f1) );
  nn = f3->neighbor( i3 );
  tds().set_adjacency(f2, ccw(i2) , nn , nn->index(f3) );
  nn = f5->neighbor( i5 );
  tds().set_adjacency(f4, ccw(i4) , nn , nn->index(f5) );
  tds().set_adjacency(f0, cw(i0) , f1 , i1  );
  tds().set_adjacency(f4, cw(i4) , f1 , cw(i1) );

  tds().delete_face(f3);
  tds().delete_face(f5);


  f0->set_offsets(0,0,0);
  f1->set_offsets(0,0,0);
  f2->set_offsets(0,0,0);
  f4->set_offsets(0,0,0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::remove_degree6_diamond
(
 Vertex_handle &v,
 Face_handle &  f0, Face_handle &  f1, Face_handle &  f2,
 Face_handle &  f3, Face_handle &  f4, Face_handle &  f5,
 Vertex_handle &v0, Vertex_handle &v1, Vertex_handle &v2,
 Vertex_handle &v3, Vertex_handle &v4, Vertex_handle &v5,
 Offset &o0, Offset &o1, Offset &o2,
 Offset &o3, Offset &o4, Offset &o5,
 int i0, int i1, int i2, int i3, int i4, int i5 )
{ // removing a degree 6 vertex, with chords v0v2 v2v4 v4v0
  remove_degree6_diamond(v,
                         f0,f1,f2,f3,f4,f5,
                         v0,v1,v2,v3,v4,v5,
                         i0,i1,i2,i3,i4,i5);

  if (o0.x() < 0 || o1.x() < 0 || o2.x() < 0 || o3.x() < 0 || o4.x() < 0 || o5.x() < 0) {
    o0 += Offset(number_of_sheets()[0], 0);
    o1 += Offset(number_of_sheets()[0], 0);
    o2 += Offset(number_of_sheets()[0], 0);
    o3 += Offset(number_of_sheets()[0], 0);
    o4 += Offset(number_of_sheets()[0], 0);
    o5 += Offset(number_of_sheets()[0], 0);
  }
  if (o0.y() < 0 || o1.y() < 0 || o2.y() < 0 || o3.y() < 0 || o4.y() < 0 || o5.y() < 0) {
    o0 += Offset(0, number_of_sheets()[1]);
    o1 += Offset(0, number_of_sheets()[1]);
    o2 += Offset(0, number_of_sheets()[1]);
    o3 += Offset(0, number_of_sheets()[1]);
    o4 += Offset(0, number_of_sheets()[1]);
    o5 += Offset(0, number_of_sheets()[1]);
  }
  int oo0 = (o0.x() >= number_of_sheets()[0] ? 2 : 0) + (o0.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo1 = (o1.x() >= number_of_sheets()[0] ? 2 : 0) + (o1.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo2 = (o2.x() >= number_of_sheets()[0] ? 2 : 0) + (o2.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo3 = (o3.x() >= number_of_sheets()[0] ? 2 : 0) + (o3.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo4 = (o4.x() >= number_of_sheets()[0] ? 2 : 0) + (o4.y() >= number_of_sheets()[1] ? 1 : 0);
  int oo5 = (o5.x() >= number_of_sheets()[0] ? 2 : 0) + (o5.y() >= number_of_sheets()[1] ? 1 : 0);

  int oo[3];
  oo[i0]      = oo2;
  oo[ccw(i0)] = oo0;
  oo[ cw(i0)] = oo1;
  this->set_offsets(f0, oo[0], oo[1], oo[2]);
  oo[i2]      = oo4;
  oo[ccw(i2)] = oo2;
  oo[ cw(i2)] = oo3;
  this->set_offsets(f2, oo[0], oo[1], oo[2]);
  oo[i4]      = oo0;
  oo[ccw(i4)] = oo4;
  oo[ cw(i4)] = oo5;
  this->set_offsets(f4, oo[0], oo[1], oo[2]);
  oo[i1]      = oo4;
  oo[ccw(i1)] = oo0;
  oo[ cw(i1)] = oo2;
  this->set_offsets(f1, oo[0], oo[1], oo[2]);

  this->insert_too_long_edge(f1, 0);
  this->insert_too_long_edge(f1, 1);
  this->insert_too_long_edge(f1, 2);
}


template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7(Vertex_handle v,std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &w,
               std::vector<int> &i)
{
  // removing a degree 7 vertex

  if (incircle(2,0,1,3,f,w,i)) { // sweeping from above
    if (incircle(2,3,4,0,f,w,i)) {
      if (incircle(5,3,4,6,f,w,i)) {
	if (incircle(5,3,4,2,f,w,i)) {
	  if (incircle(6,2,3,5,f,w,i)) {
	    if (incircle(6,0,1,2,f,w,i)) {
	      remove_degree7_leftfan(v,  6  ,f,w,i);
	    }else{
	      remove_degree7_zigzag(v,  6  ,f,w,i);
	    }}else{
	    if (incircle(5,0,1,2,f,w,i)) {
	      if (incircle(6,1,2,5,f,w,i)) {
		remove_degree7_zigzag(v, 2   ,f,w,i);
	      }else{
		if (incircle(6,0,1,5,f,w,i)) {
		  remove_degree7_rightfan(v, 5   ,f,w,i);
		}else{
		  remove_degree7_star(v,  5  ,f,w,i);
		}}}else{
	      if (incircle(2,5,6,0,f,w,i)) {
		if (incircle(6,0,1,2,f,w,i)) {
		  remove_degree7_zigzag(v,  2  ,f,w,i);
		}else{
		  remove_degree7_rightfan(v,  2  ,f,w,i);
		}}else{
		remove_degree7_rightdelta(v,  5  ,f,w,i);
	      }}}}else{
	  if (incircle(4,0,1,2,f,w,i)) {
	    if (incircle(5,1,2,4,f,w,i)) {
	      if (incircle(6,1,2,5,f,w,i)) {
		remove_degree7_leftfan(v,  2  ,f,w,i);
	      }else{
		if (incircle(6,0,1,5,f,w,i)) {
		  remove_degree7_zigzag(v,  5  ,f,w,i);
		}else{
		  remove_degree7_leftfan(v,  5  ,f,w,i);
		}}}else{
	      if (incircle(5,0,1,4,f,w,i)) {
		if (incircle(6,0,1,5,f,w,i)) {
		  remove_degree7_rightfan(v,  1  ,f,w,i);
		}else{
		  remove_degree7_zigzag(v,  1  ,f,w,i);
		}}else{
		remove_degree7_rightfan(v,  4  ,f,w,i);
	      }}}else{
	    if (incircle(2,4,5,0,f,w,i)) {
	      if (incircle(5,0,1,2,f,w,i)) {
		if (incircle(6,1,2,5,f,w,i)) {
		  remove_degree7_leftfan(v,  2  ,f,w,i);
		}else{
		  if (incircle(6,0,1,5,f,w,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,i);
		  }else{
		    remove_degree7_leftfan(v,  5  ,f,w,i);
		  }}}else{
		if (incircle(2,5,6,0,f,w,i)) {
		  if (incircle(6,0,1,2,f,w,i)) {
		    remove_degree7_leftfan(v,  2  ,f,w,i);
		  }else{
		    remove_degree7_star(v,  2  ,f,w,i);
		  }}else{
		  remove_degree7_leftdelta(v,  2  ,f,w,i);
		}}}else{
	      remove_degree7_rightdelta(v,  0  ,f,w,i);
	    }}}}else{
	if (incircle(6,3,4,2,f,w,i)) {
	  if (incircle(6,0,1,2,f,w,i)) {
	    remove_degree7_star(v,  6  ,f,w,i);
	  }else{
	    remove_degree7_rightfan(v,  6  ,f,w,i);
	  }}else{
	  if (incircle(4,0,1,2,f,w,i)) {
	    if (incircle(2,4,5,6,f,w,i)) {
	      if (incircle(5,1,2,4,f,w,i)) {
		if (incircle(6,1,2,5,f,w,i)) {
		  remove_degree7_leftfan(v,  2  ,f,w,i);
		}else{
		  if (incircle(6,0,1,5,f,w,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,i);
		  }else{
		    remove_degree7_leftfan(v,  5  ,f,w,i);
		  }}}else{
		if (incircle(5,0,1,4,f,w,i)) {
		  if (incircle(6,0,1,5,f,w,i)) {
		    remove_degree7_rightfan(v,  1  ,f,w,i);
		  }else{
		    remove_degree7_zigzag(v,  1  ,f,w,i);
		  }} else{
		  remove_degree7_rightfan(v,  4  ,f,w,i);
		}}} else {
	      if (incircle(6,1,2,4,f,w,i)) {
		remove_degree7_leftdelta(v,  6  ,f,w,i);
	      }else{
		if (incircle(1,4,5,6,f,w,i)) {
		  if (incircle(1,4,5,0,f,w,i)) {
		    if (incircle(6,0,1,5,f,w,i)) {
		      remove_degree7_rightfan(v,  1  ,f,w,i);
		    }else{
		      remove_degree7_zigzag(v,  1  ,f,w,i);
		    }}else{
		    remove_degree7_rightfan(v,  4  ,f,w,i);
		  }} else {
		  if (incircle(6,0,1,4,f,w,i)) {
		    remove_degree7_rightdelta(v,  4  ,f,w,i);
		  }else{
		    if (incircle(6,4,5,0,f,w,i)) {
		      remove_degree7_star(v,  4  ,f,w,i);
		    }else{
		      remove_degree7_rightfan(v,  4  ,f,w,i);
		    }}}}}}else{
	    if (incircle(2,4,5,6,f,w,i)) {
	      if (incircle(2,4,5,0,f,w,i)) {
		if (incircle(5,0,1,2,f,w,i)) {
		  if (incircle(6,1,2,5,f,w,i)) {
		    remove_degree7_leftfan(v,  2  ,f,w,i);
		  }else{
		    if (incircle(6,0,1,5,f,w,i)) {
		      remove_degree7_zigzag(v,  5  ,f,w,i);
		    }else{
		      remove_degree7_leftfan(v,  5  ,f,w,i);
		    }}}else{
		  if (incircle(2,5,6,0,f,w,i)) {
		    if (incircle(6,0,1,2,f,w,i)) {
		      remove_degree7_leftfan(v,  2  ,f,w,i);
		    }else{
		      remove_degree7_star(v,  2  ,f,w,i);
		    }}else{
		    remove_degree7_leftdelta(v,  2  ,f,w,i);
		  }}}else{
		remove_degree7_rightdelta(v,  0  ,f,w,i);
	      }}else{
	      if (incircle(2,6,0,4,f,w,i)) {
		if (incircle(6,0,1,2,f,w,i)) {
		  remove_degree7_leftdelta(v,  6  ,f,w,i);
		}else{
		  remove_degree7_rightdelta(v,  2  ,f,w,i);
		}}else{
		if (incircle(6,4,5,0,f,w,i)) {
		  remove_degree7_leftdelta(v,  4  ,f,w,i);
		}else{
		  remove_degree7_rightdelta(v,  0  ,f,w,i);
		}}}}}}} else{
      if (incircle(5,3,4,6,f,w,i)) {
	if (incircle(5,3,4,0,f,w,i)) {
	  if (incircle(5,2,3,0,f,w,i)) {
	    if (incircle(6,2,3,5,f,w,i)) {
	      if (incircle(6,0,1,2,f,w,i)) {
		remove_degree7_leftfan(v,  6  ,f,w,i);
	      }else{
		remove_degree7_zigzag(v,  6  ,f,w,i);
	      }}else
	      if (incircle(5,0,1,2,f,w,i)) {
	  	if (incircle(6,1,2,5,f,w,i)) {
		  remove_degree7_zigzag(v,  2  ,f,w,i);
		}else{
		  if (incircle(6,0,1,5,f,w,i)) {
		    remove_degree7_rightfan(v,  5  ,f,w,i);
		  }else{
		    remove_degree7_star(v,  5  ,f,w,i);
		  }}}else{
		if (incircle(2,5,6,0,f,w,i)) {
		  if (incircle(6,0,1,2,f,w,i)) {
		    remove_degree7_zigzag(v,  2  ,f,w,i);
		  }else{
		    remove_degree7_rightfan(v,  2  ,f,w,i);
		  }}else{
		  remove_degree7_rightdelta(v,  5  ,f,w,i);
		}}}else{
	    if (incircle(3,5,6,0,f,w,i)) {
	      if (incircle(6,2,3,0,f,w,i)) {
		if (incircle(6,0,1,2,f,w,i)) {
		  remove_degree7_leftfan(v,  6  ,f,w,i);
		}else{
		  remove_degree7_zigzag(v,  6  ,f,w,i);
		}}else{
		remove_degree7_leftfan(v,  3  ,f,w,i);
	      }}else{
	      remove_degree7_leftdelta(v,  0  ,f,w,i);
	    }}}else{
	  remove_degree7_star(v,  0  ,f,w,i);
	}}else{
	if (incircle(6,3,4,0,f,w,i)) {
	  if (incircle(6,2,3,0,f,w,i)) {
	    if (incircle(6,0,1,2,f,w,i)) {
	      remove_degree7_star(v,  6  ,f,w,i);
	    }else{
	      remove_degree7_rightfan(v,  6  ,f,w,i);
	    }}else{
	    remove_degree7_zigzag(v,  3  ,f,w,i);
	  }}else{
	  if (incircle(6,4,5,0,f,w,i)) {
	    remove_degree7_leftfan(v,  0  ,f,w,i);
	  }else{
	    remove_degree7_star(v,  0  ,f,w,i);
	  }}}}}else{  //sweeping from below
    if (incircle(1,6,0,3,f,w,i)) {
      if (incircle(5,6,0,4,f,w,i)) {
	if (incircle(5,6,0,1,f,w,i)) {
	  if (incircle(4,0,1,5,f,w,i)) {
	    if (incircle(4,2,3,1,f,w,i)) {
	      remove_degree7_rightfan(v,  4  ,f,w,i);
	    }else{
	      remove_degree7_zigzag(v,  4  ,f,w,i);
	    }}else{
	    if (incircle(5,2,3,1,f,w,i)) {
	      if (incircle(4,1,2,5,f,w,i)) {
		remove_degree7_zigzag(v, 1   ,f,w,i);
	      }else{
		if (incircle(4,2,3,5,f,w,i)) {
		  remove_degree7_leftfan(v, 5   ,f,w,i);
		}else{
		  remove_degree7_star(v,  5  ,f,w,i);
		}}}else{
	      if (incircle(1,4,5,3,f,w,i)) {
		if (incircle(4,2,3,1,f,w,i)) {
		  remove_degree7_zigzag(v,  1  ,f,w,i);
		}else{
		  remove_degree7_leftfan(v,  1  ,f,w,i);
		}}else{
		remove_degree7_leftdelta(v,  5  ,f,w,i);
	      }}}}else{
	  if (incircle(6,2,3,1,f,w,i)) {
	    if (incircle(5,1,2,6,f,w,i)) {
	      if (incircle(4,1,2,5,f,w,i)) {
		remove_degree7_rightfan(v,  1  ,f,w,i);
	      }else{
		if (incircle(4,2,3,5,f,w,i)) {
		  remove_degree7_zigzag(v,  5  ,f,w,i);
		}else{
		  remove_degree7_rightfan(v,  5  ,f,w,i);
		}}}else{
	      if (incircle(5,2,3,6,f,w,i)) {
		if (incircle(4,2,3,5,f,w,i)) {
		  remove_degree7_leftfan(v,  2  ,f,w,i);
		}else{
		  remove_degree7_zigzag(v,  2  ,f,w,i);
		}}else{
		remove_degree7_leftfan(v,  6  ,f,w,i);
	      }}}else{
	    if (incircle(1,5,6,3,f,w,i)) {
	      if (incircle(5,2,3,1,f,w,i)) {
		if (incircle(4,1,2,5,f,w,i)) {
		  remove_degree7_rightfan(v,  1  ,f,w,i);
		}else{
		  if (incircle(4,2,3,5,f,w,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,i);
		  }else{
		    remove_degree7_rightfan(v,  5  ,f,w,i);
		  }}}else{
		if (incircle(1,4,5,3,f,w,i)) {
		  if (incircle(4,2,3,1,f,w,i)) {
		    remove_degree7_rightfan(v,  1  ,f,w,i);
		  }else{
		    remove_degree7_star(v,  1  ,f,w,i);
		  }}else{
		  remove_degree7_rightdelta(v,  1  ,f,w,i);
		}}}else{
	      remove_degree7_leftdelta(v,  3  ,f,w,i);
	    }}}}else{
	if (incircle(4,6,0,1,f,w,i)) {
	  if (incircle(4,2,3,1,f,w,i)) {
	    remove_degree7_star(v,  4  ,f,w,i);
	  }else{
	    remove_degree7_leftfan(v,  4  ,f,w,i);
	  }}else{
	  if (incircle(6,2,3,1,f,w,i)) {
	    if (incircle(1,5,6,4,f,w,i)) {
	      if (incircle(5,1,2,6,f,w,i)) {
		if (incircle(4,1,2,5,f,w,i)) {
		  remove_degree7_rightfan(v,  1  ,f,w,i);
		}else{
		  if (incircle(4,2,3,5,f,w,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,i);
		  }else{
		    remove_degree7_rightfan(v,  5  ,f,w,i);
		  }}}else{
		if (incircle(5,2,3,6,f,w,i)) {
		  if (incircle(4,2,3,5,f,w,i)) {
		    remove_degree7_leftfan(v,  2  ,f,w,i);
		  }else{
		    remove_degree7_zigzag(v,  2  ,f,w,i);
		  }} else{
		  remove_degree7_leftfan(v,  6  ,f,w,i);
		}}} else {
	      if (incircle(4,1,2,6,f,w,i)) {
		remove_degree7_rightdelta(v,  4  ,f,w,i);
	      }else{
		if (incircle(2,5,6,4,f,w,i)) {
		  if (incircle(2,5,6,3,f,w,i)) {
		    if (incircle(4,2,3,5,f,w,i)) {
		      remove_degree7_leftfan(v,  2  ,f,w,i);
		    }else{
		      remove_degree7_zigzag(v,  2  ,f,w,i);
		    }}else{
		    remove_degree7_leftfan(v,  6  ,f,w,i);
		  }} else {
		  if (incircle(4,2,3,6,f,w,i)) {
		    remove_degree7_leftdelta(v,  6  ,f,w,i);
		  }else{
		    if (incircle(4,5,6,3,f,w,i)) {
		      remove_degree7_star(v,  6  ,f,w,i);
		    }else{
		      remove_degree7_leftfan(v,  6  ,f,w,i);
		    }}}}}}else{
	    if (incircle(1,5,6,4,f,w,i)) {
	      if (incircle(1,5,6,3,f,w,i)) {
		if (incircle(5,2,3,1,f,w,i)) {
		  if (incircle(4,1,2,5,f,w,i)) {
		    remove_degree7_rightfan(v,  1  ,f,w,i);
		  }else{
		    if (incircle(4,2,3,5,f,w,i)) {
		      remove_degree7_zigzag(v,  5  ,f,w,i);
		    }else{
		      remove_degree7_rightfan(v,  5  ,f,w,i);
		    }}}else{
		  if (incircle(1,4,5,3,f,w,i)) {
		    if (incircle(4,2,3,1,f,w,i)) {
		      remove_degree7_rightfan(v,  1  ,f,w,i);
		    }else{
		      remove_degree7_star(v,  1  ,f,w,i);
		    }}else{
		    remove_degree7_rightdelta(v,  1  ,f,w,i);
		  }}}else{
		remove_degree7_leftdelta(v,  3  ,f,w,i);
	      }}else{
	      if (incircle(1,3,4,6,f,w,i)) {
		if (incircle(4,2,3,1,f,w,i)) {
		  remove_degree7_rightdelta(v,  4  ,f,w,i);
		}else{
		  remove_degree7_leftdelta(v,  1  ,f,w,i);
		}}else{
		if (incircle(4,5,6,3,f,w,i)) {
		  remove_degree7_rightdelta(v,  6  ,f,w,i);
		}else{
		  remove_degree7_leftdelta(v,  3  ,f,w,i);
		}}}}}}} else{
      if (incircle(5,6,0,4,f,w,i)) {
	if (incircle(5,6,0,3,f,w,i)) {
	  if (incircle(5,0,1,3,f,w,i)) {
	    if (incircle(4,0,1,5,f,w,i)) {
	      if (incircle(4,2,3,1,f,w,i)) {
		remove_degree7_rightfan(v,  4  ,f,w,i);
	      }else{
		remove_degree7_zigzag(v,  4  ,f,w,i);
	      }}else
	      if (incircle(5,2,3,1,f,w,i)) {
	  	if (incircle(4,1,2,5,f,w,i)) {
		  remove_degree7_zigzag(v,  1  ,f,w,i);
		}else{
		  if (incircle(4,2,3,5,f,w,i)) {
		    remove_degree7_leftfan(v,  5  ,f,w,i);
		  }else{
		    remove_degree7_star(v,  5  ,f,w,i);
		  }}}else{
		if (incircle(1,4,5,3,f,w,i)) {
		  if (incircle(4,2,3,1,f,w,i)) {
		    remove_degree7_zigzag(v,  1  ,f,w,i);
		  }else{
		    remove_degree7_leftfan(v,  1  ,f,w,i);
		  }}else{
		  remove_degree7_leftdelta(v,  5  ,f,w,i);
		}}}else{
	    if (! incircle(3,4,5,0,f,w,i)) {
	      if (incircle(4,0,1,3,f,w,i)) {
		if (incircle(4,2,3,1,f,w,i)) {
		  remove_degree7_rightfan(v,  4  ,f,w,i);
		}else{
		  remove_degree7_zigzag(v,  4  ,f,w,i);
		}}else{
		remove_degree7_rightfan(v,  0  ,f,w,i);
	      }}else{
	      remove_degree7_rightdelta(v,  3  ,f,w,i);
	    }}}else{
	  remove_degree7_star(v,  3  ,f,w,i);
	}}else{
	if (incircle(4,6,0,3,f,w,i)) {
	  if (incircle(4,0,1,3,f,w,i)) {
	    if (incircle(4,2,3,1,f,w,i)) {
	      remove_degree7_star(v,  4  ,f,w,i);
	    }else{
	      remove_degree7_leftfan(v,  4  ,f,w,i);
	    }}else{
	    remove_degree7_zigzag(v,  0  ,f,w,i);
	  }}else{
	  if (incircle(4,5,6,3,f,w,i)) {
	    remove_degree7_rightfan(v,  3  ,f,w,i);
	  }else{
	    remove_degree7_star(v,  3  ,f,w,i);
	  }}}}}
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7(Vertex_handle v,std::vector<Face_handle> &f,
               std::vector<Vertex_handle> &w, std::vector<Offset> &o,
               std::vector<int> &i)
{
  // removing a degree 7 vertex

  if (incircle(2,0,1,3,f,w,o,i)) { // sweeping from above
    if (incircle(2,3,4,0,f,w,o,i)) {
      if (incircle(5,3,4,6,f,w,o,i)) {
	if (incircle(5,3,4,2,f,w,o,i)) {
	  if (incircle(6,2,3,5,f,w,o,i)) {
	    if (incircle(6,0,1,2,f,w,o,i)) {
	      remove_degree7_leftfan(v,  6  ,f,w,o,i);
	    }else{
	      remove_degree7_zigzag(v,  6  ,f,w,o,i);
	    }}else{
	    if (incircle(5,0,1,2,f,w,o,i)) {
	      if (incircle(6,1,2,5,f,w,o,i)) {
		remove_degree7_zigzag(v, 2   ,f,w,o,i);
	      }else{
		if (incircle(6,0,1,5,f,w,o,i)) {
		  remove_degree7_rightfan(v, 5   ,f,w,o,i);
		}else{
		  remove_degree7_star(v,  5  ,f,w,o,i);
		}}}else{
	      if (incircle(2,5,6,0,f,w,o,i)) {
		if (incircle(6,0,1,2,f,w,o,i)) {
		  remove_degree7_zigzag(v,  2  ,f,w,o,i);
		}else{
		  remove_degree7_rightfan(v,  2  ,f,w,o,i);
		}}else{
		remove_degree7_rightdelta(v,  5  ,f,w,o,i);
	      }}}}else{
	  if (incircle(4,0,1,2,f,w,o,i)) {
	    if (incircle(5,1,2,4,f,w,o,i)) {
	      if (incircle(6,1,2,5,f,w,o,i)) {
		remove_degree7_leftfan(v,  2  ,f,w,o,i);
	      }else{
		if (incircle(6,0,1,5,f,w,o,i)) {
		  remove_degree7_zigzag(v,  5  ,f,w,o,i);
		}else{
		  remove_degree7_leftfan(v,  5  ,f,w,o,i);
		}}}else{
	      if (incircle(5,0,1,4,f,w,o,i)) {
		if (incircle(6,0,1,5,f,w,o,i)) {
		  remove_degree7_rightfan(v,  1  ,f,w,o,i);
		}else{
		  remove_degree7_zigzag(v,  1  ,f,w,o,i);
		}}else{
		remove_degree7_rightfan(v,  4  ,f,w,o,i);
	      }}}else{
	    if (incircle(2,4,5,0,f,w,o,i)) {
	      if (incircle(5,0,1,2,f,w,o,i)) {
		if (incircle(6,1,2,5,f,w,o,i)) {
		  remove_degree7_leftfan(v,  2  ,f,w,o,i);
		}else{
		  if (incircle(6,0,1,5,f,w,o,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,o,i);
		  }else{
		    remove_degree7_leftfan(v,  5  ,f,w,o,i);
		  }}}else{
		if (incircle(2,5,6,0,f,w,o,i)) {
		  if (incircle(6,0,1,2,f,w,o,i)) {
		    remove_degree7_leftfan(v,  2  ,f,w,o,i);
		  }else{
		    remove_degree7_star(v,  2  ,f,w,o,i);
		  }}else{
		  remove_degree7_leftdelta(v,  2  ,f,w,o,i);
		}}}else{
	      remove_degree7_rightdelta(v,  0  ,f,w,o,i);
	    }}}}else{
	if (incircle(6,3,4,2,f,w,o,i)) {
	  if (incircle(6,0,1,2,f,w,o,i)) {
	    remove_degree7_star(v,  6  ,f,w,o,i);
	  }else{
	    remove_degree7_rightfan(v,  6  ,f,w,o,i);
	  }}else{
	  if (incircle(4,0,1,2,f,w,o,i)) {
	    if (incircle(2,4,5,6,f,w,o,i)) {
	      if (incircle(5,1,2,4,f,w,o,i)) {
		if (incircle(6,1,2,5,f,w,o,i)) {
		  remove_degree7_leftfan(v,  2  ,f,w,o,i);
		}else{
		  if (incircle(6,0,1,5,f,w,o,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,o,i);
		  }else{
		    remove_degree7_leftfan(v,  5  ,f,w,o,i);
		  }}}else{
		if (incircle(5,0,1,4,f,w,o,i)) {
		  if (incircle(6,0,1,5,f,w,o,i)) {
		    remove_degree7_rightfan(v,  1  ,f,w,o,i);
		  }else{
		    remove_degree7_zigzag(v,  1  ,f,w,o,i);
		  }} else{
		  remove_degree7_rightfan(v,  4  ,f,w,o,i);
		}}} else {
	      if (incircle(6,1,2,4,f,w,o,i)) {
		remove_degree7_leftdelta(v,  6  ,f,w,o,i);
	      }else{
		if (incircle(1,4,5,6,f,w,o,i)) {
		  if (incircle(1,4,5,0,f,w,o,i)) {
		    if (incircle(6,0,1,5,f,w,o,i)) {
		      remove_degree7_rightfan(v,  1  ,f,w,o,i);
		    }else{
		      remove_degree7_zigzag(v,  1  ,f,w,o,i);
		    }}else{
		    remove_degree7_rightfan(v,  4  ,f,w,o,i);
		  }} else {
		  if (incircle(6,0,1,4,f,w,o,i)) {
		    remove_degree7_rightdelta(v,  4  ,f,w,o,i);
		  }else{
		    if (incircle(6,4,5,0,f,w,o,i)) {
		      remove_degree7_star(v,  4  ,f,w,o,i);
		    }else{
		      remove_degree7_rightfan(v,  4  ,f,w,o,i);
		    }}}}}}else{
	    if (incircle(2,4,5,6,f,w,o,i)) {
	      if (incircle(2,4,5,0,f,w,o,i)) {
		if (incircle(5,0,1,2,f,w,o,i)) {
		  if (incircle(6,1,2,5,f,w,o,i)) {
		    remove_degree7_leftfan(v,  2  ,f,w,o,i);
		  }else{
		    if (incircle(6,0,1,5,f,w,o,i)) {
		      remove_degree7_zigzag(v,  5  ,f,w,o,i);
		    }else{
		      remove_degree7_leftfan(v,  5  ,f,w,o,i);
		    }}}else{
		  if (incircle(2,5,6,0,f,w,o,i)) {
		    if (incircle(6,0,1,2,f,w,o,i)) {
		      remove_degree7_leftfan(v,  2  ,f,w,o,i);
		    }else{
		      remove_degree7_star(v,  2  ,f,w,o,i);
		    }}else{
		    remove_degree7_leftdelta(v,  2  ,f,w,o,i);
		  }}}else{
		remove_degree7_rightdelta(v,  0  ,f,w,o,i);
	      }}else{
	      if (incircle(2,6,0,4,f,w,o,i)) {
		if (incircle(6,0,1,2,f,w,o,i)) {
		  remove_degree7_leftdelta(v,  6  ,f,w,o,i);
		}else{
		  remove_degree7_rightdelta(v,  2  ,f,w,o,i);
		}}else{
		if (incircle(6,4,5,0,f,w,o,i)) {
		  remove_degree7_leftdelta(v,  4  ,f,w,o,i);
		}else{
		  remove_degree7_rightdelta(v,  0  ,f,w,o,i);
		}}}}}}} else{
      if (incircle(5,3,4,6,f,w,o,i)) {
	if (incircle(5,3,4,0,f,w,o,i)) {
	  if (incircle(5,2,3,0,f,w,o,i)) {
	    if (incircle(6,2,3,5,f,w,o,i)) {
	      if (incircle(6,0,1,2,f,w,o,i)) {
		remove_degree7_leftfan(v,  6  ,f,w,o,i);
	      }else{
		remove_degree7_zigzag(v,  6  ,f,w,o,i);
	      }}else
	      if (incircle(5,0,1,2,f,w,o,i)) {
	  	if (incircle(6,1,2,5,f,w,o,i)) {
		  remove_degree7_zigzag(v,  2  ,f,w,o,i);
		}else{
		  if (incircle(6,0,1,5,f,w,o,i)) {
		    remove_degree7_rightfan(v,  5  ,f,w,o,i);
		  }else{
		    remove_degree7_star(v,  5  ,f,w,o,i);
		  }}}else{
		if (incircle(2,5,6,0,f,w,o,i)) {
		  if (incircle(6,0,1,2,f,w,o,i)) {
		    remove_degree7_zigzag(v,  2  ,f,w,o,i);
		  }else{
		    remove_degree7_rightfan(v,  2  ,f,w,o,i);
		  }}else{
		  remove_degree7_rightdelta(v,  5  ,f,w,o,i);
		}}}else{
	    if (incircle(3,5,6,0,f,w,o,i)) {
	      if (incircle(6,2,3,0,f,w,o,i)) {
		if (incircle(6,0,1,2,f,w,o,i)) {
		  remove_degree7_leftfan(v,  6  ,f,w,o,i);
		}else{
		  remove_degree7_zigzag(v,  6  ,f,w,o,i);
		}}else{
		remove_degree7_leftfan(v,  3  ,f,w,o,i);
	      }}else{
	      remove_degree7_leftdelta(v,  0  ,f,w,o,i);
	    }}}else{
	  remove_degree7_star(v,  0  ,f,w,o,i);
	}}else{
	if (incircle(6,3,4,0,f,w,o,i)) {
	  if (incircle(6,2,3,0,f,w,o,i)) {
	    if (incircle(6,0,1,2,f,w,o,i)) {
	      remove_degree7_star(v,  6  ,f,w,o,i);
	    }else{
	      remove_degree7_rightfan(v,  6  ,f,w,o,i);
	    }}else{
	    remove_degree7_zigzag(v,  3  ,f,w,o,i);
	  }}else{
	  if (incircle(6,4,5,0,f,w,o,i)) {
	    remove_degree7_leftfan(v,  0  ,f,w,o,i);
	  }else{
	    remove_degree7_star(v,  0  ,f,w,o,i);
	  }}}}}else{  //sweeping from below
    if (incircle(1,6,0,3,f,w,o,i)) {
      if (incircle(5,6,0,4,f,w,o,i)) {
	if (incircle(5,6,0,1,f,w,o,i)) {
	  if (incircle(4,0,1,5,f,w,o,i)) {
	    if (incircle(4,2,3,1,f,w,o,i)) {
	      remove_degree7_rightfan(v,  4  ,f,w,o,i);
	    }else{
	      remove_degree7_zigzag(v,  4  ,f,w,o,i);
	    }}else{
	    if (incircle(5,2,3,1,f,w,o,i)) {
	      if (incircle(4,1,2,5,f,w,o,i)) {
		remove_degree7_zigzag(v, 1   ,f,w,o,i);
	      }else{
		if (incircle(4,2,3,5,f,w,o,i)) {
		  remove_degree7_leftfan(v, 5   ,f,w,o,i);
		}else{
		  remove_degree7_star(v,  5  ,f,w,o,i);
		}}}else{
	      if (incircle(1,4,5,3,f,w,o,i)) {
		if (incircle(4,2,3,1,f,w,o,i)) {
		  remove_degree7_zigzag(v,  1  ,f,w,o,i);
		}else{
		  remove_degree7_leftfan(v,  1  ,f,w,o,i);
		}}else{
		remove_degree7_leftdelta(v,  5  ,f,w,o,i);
	      }}}}else{
	  if (incircle(6,2,3,1,f,w,o,i)) {
	    if (incircle(5,1,2,6,f,w,o,i)) {
	      if (incircle(4,1,2,5,f,w,o,i)) {
		remove_degree7_rightfan(v,  1  ,f,w,o,i);
	      }else{
		if (incircle(4,2,3,5,f,w,o,i)) {
		  remove_degree7_zigzag(v,  5  ,f,w,o,i);
		}else{
		  remove_degree7_rightfan(v,  5  ,f,w,o,i);
		}}}else{
	      if (incircle(5,2,3,6,f,w,o,i)) {
		if (incircle(4,2,3,5,f,w,o,i)) {
		  remove_degree7_leftfan(v,  2  ,f,w,o,i);
		}else{
		  remove_degree7_zigzag(v,  2  ,f,w,o,i);
		}}else{
		remove_degree7_leftfan(v,  6  ,f,w,o,i);
	      }}}else{
	    if (incircle(1,5,6,3,f,w,o,i)) {
	      if (incircle(5,2,3,1,f,w,o,i)) {
		if (incircle(4,1,2,5,f,w,o,i)) {
		  remove_degree7_rightfan(v,  1  ,f,w,o,i);
		}else{
		  if (incircle(4,2,3,5,f,w,o,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,o,i);
		  }else{
		    remove_degree7_rightfan(v,  5  ,f,w,o,i);
		  }}}else{
		if (incircle(1,4,5,3,f,w,o,i)) {
		  if (incircle(4,2,3,1,f,w,o,i)) {
		    remove_degree7_rightfan(v,  1  ,f,w,o,i);
		  }else{
		    remove_degree7_star(v,  1  ,f,w,o,i);
		  }}else{
		  remove_degree7_rightdelta(v,  1  ,f,w,o,i);
		}}}else{
	      remove_degree7_leftdelta(v,  3  ,f,w,o,i);
	    }}}}else{
	if (incircle(4,6,0,1,f,w,o,i)) {
	  if (incircle(4,2,3,1,f,w,o,i)) {
	    remove_degree7_star(v,  4  ,f,w,o,i);
	  }else{
	    remove_degree7_leftfan(v,  4  ,f,w,o,i);
	  }}else{
	  if (incircle(6,2,3,1,f,w,o,i)) {
	    if (incircle(1,5,6,4,f,w,o,i)) {
	      if (incircle(5,1,2,6,f,w,o,i)) {
		if (incircle(4,1,2,5,f,w,o,i)) {
		  remove_degree7_rightfan(v,  1  ,f,w,o,i);
		}else{
		  if (incircle(4,2,3,5,f,w,o,i)) {
		    remove_degree7_zigzag(v,  5  ,f,w,o,i);
		  }else{
		    remove_degree7_rightfan(v,  5  ,f,w,o,i);
		  }}}else{
		if (incircle(5,2,3,6,f,w,o,i)) {
		  if (incircle(4,2,3,5,f,w,o,i)) {
		    remove_degree7_leftfan(v,  2  ,f,w,o,i);
		  }else{
		    remove_degree7_zigzag(v,  2  ,f,w,o,i);
		  }} else{
		  remove_degree7_leftfan(v,  6  ,f,w,o,i);
		}}} else {
	      if (incircle(4,1,2,6,f,w,o,i)) {
		remove_degree7_rightdelta(v,  4  ,f,w,o,i);
	      }else{
		if (incircle(2,5,6,4,f,w,o,i)) {
		  if (incircle(2,5,6,3,f,w,o,i)) {
		    if (incircle(4,2,3,5,f,w,o,i)) {
		      remove_degree7_leftfan(v,  2  ,f,w,o,i);
		    }else{
		      remove_degree7_zigzag(v,  2  ,f,w,o,i);
		    }}else{
		    remove_degree7_leftfan(v,  6  ,f,w,o,i);
		  }} else {
		  if (incircle(4,2,3,6,f,w,o,i)) {
		    remove_degree7_leftdelta(v,  6  ,f,w,o,i);
		  }else{
		    if (incircle(4,5,6,3,f,w,o,i)) {
		      remove_degree7_star(v,  6  ,f,w,o,i);
		    }else{
		      remove_degree7_leftfan(v,  6  ,f,w,o,i);
		    }}}}}}else{
	    if (incircle(1,5,6,4,f,w,o,i)) {
	      if (incircle(1,5,6,3,f,w,o,i)) {
		if (incircle(5,2,3,1,f,w,o,i)) {
		  if (incircle(4,1,2,5,f,w,o,i)) {
		    remove_degree7_rightfan(v,  1  ,f,w,o,i);
		  }else{
		    if (incircle(4,2,3,5,f,w,o,i)) {
		      remove_degree7_zigzag(v,  5  ,f,w,o,i);
		    }else{
		      remove_degree7_rightfan(v,  5  ,f,w,o,i);
		    }}}else{
		  if (incircle(1,4,5,3,f,w,o,i)) {
		    if (incircle(4,2,3,1,f,w,o,i)) {
		      remove_degree7_rightfan(v,  1  ,f,w,o,i);
		    }else{
		      remove_degree7_star(v,  1  ,f,w,o,i);
		    }}else{
		    remove_degree7_rightdelta(v,  1  ,f,w,o,i);
		  }}}else{
		remove_degree7_leftdelta(v,  3  ,f,w,o,i);
	      }}else{
	      if (incircle(1,3,4,6,f,w,o,i)) {
		if (incircle(4,2,3,1,f,w,o,i)) {
		  remove_degree7_rightdelta(v,  4  ,f,w,o,i);
		}else{
		  remove_degree7_leftdelta(v,  1  ,f,w,o,i);
		}}else{
		if (incircle(4,5,6,3,f,w,o,i)) {
		  remove_degree7_rightdelta(v,  6  ,f,w,o,i);
		}else{
		  remove_degree7_leftdelta(v,  3  ,f,w,o,i);
		}}}}}}} else{
      if (incircle(5,6,0,4,f,w,o,i)) {
	if (incircle(5,6,0,3,f,w,o,i)) {
	  if (incircle(5,0,1,3,f,w,o,i)) {
	    if (incircle(4,0,1,5,f,w,o,i)) {
	      if (incircle(4,2,3,1,f,w,o,i)) {
		remove_degree7_rightfan(v,  4  ,f,w,o,i);
	      }else{
		remove_degree7_zigzag(v,  4  ,f,w,o,i);
	      }}else
	      if (incircle(5,2,3,1,f,w,o,i)) {
	  	if (incircle(4,1,2,5,f,w,o,i)) {
		  remove_degree7_zigzag(v,  1  ,f,w,o,i);
		}else{
		  if (incircle(4,2,3,5,f,w,o,i)) {
		    remove_degree7_leftfan(v,  5  ,f,w,o,i);
		  }else{
		    remove_degree7_star(v,  5  ,f,w,o,i);
		  }}}else{
		if (incircle(1,4,5,3,f,w,o,i)) {
		  if (incircle(4,2,3,1,f,w,o,i)) {
		    remove_degree7_zigzag(v,  1  ,f,w,o,i);
		  }else{
		    remove_degree7_leftfan(v,  1  ,f,w,o,i);
		  }}else{
		  remove_degree7_leftdelta(v,  5  ,f,w,o,i);
		}}}else{
	    if (! incircle(3,4,5,0,f,w,o,i)) {
	      if (incircle(4,0,1,3,f,w,o,i)) {
		if (incircle(4,2,3,1,f,w,o,i)) {
		  remove_degree7_rightfan(v,  4  ,f,w,o,i);
		}else{
		  remove_degree7_zigzag(v,  4  ,f,w,o,i);
		}}else{
		remove_degree7_rightfan(v,  0  ,f,w,o,i);
	      }}else{
	      remove_degree7_rightdelta(v,  3  ,f,w,o,i);
	    }}}else{
	  remove_degree7_star(v,  3  ,f,w,o,i);
	}}else{
	if (incircle(4,6,0,3,f,w,o,i)) {
	  if (incircle(4,0,1,3,f,w,o,i)) {
	    if (incircle(4,2,3,1,f,w,o,i)) {
	      remove_degree7_star(v,  4  ,f,w,o,i);
	    }else{
	      remove_degree7_leftfan(v,  4  ,f,w,o,i);
	    }}else{
	    remove_degree7_zigzag(v,  0  ,f,w,o,i);
	  }}else{
	  if (incircle(4,5,6,3,f,w,o,i)) {
	    remove_degree7_rightfan(v,  3  ,f,w,o,i);
	  }else{
	    remove_degree7_star(v,  3  ,f,w,o,i);
	  }}}}}
}



template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
rotate7(int j,  std::vector<Vertex_handle> &w,
        std::vector<Face_handle> &f, std::vector<int> &i)
{
  if (j==0) return;
  Face_handle ff=f[0];
  int ii=i[0],k=0,kk=(6*j)%7;
  Vertex_handle ww=w[0];
  for (int jj=0; k!=kk; jj=k) { // 7 is prime
    k=(jj+j)%7;
    w[jj]=w[k]; f[jj]=f[k]; i[jj]=i[k];
  }
  w[kk]=ww;f[kk]=ff;i[kk]=ii;
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
rotate7(int j,  std::vector<Vertex_handle> &w,
        std::vector<Face_handle> &f, std::vector<Offset> &o, std::vector<int> &i)
{
  if (j==0) return;
  Face_handle ff=f[0];
  int ii=i[0],k=0,kk=(6*j)%7;
  Vertex_handle ww=w[0];
  Offset oo=o[0];
  for (int jj=0; k!=kk; jj=k) { // 7 is prime
    k=(jj+j)%7;
    w[jj]=w[k]; f[jj]=f[k]; o[jj] = o[k]; i[jj]=i[k];
  }
  w[kk]=ww;f[kk]=ff;o[kk]=oo;i[kk]=ii;
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
get_offset_degree7(std::vector<Offset> &in_o, int out_o[]) {
  bool add[2];
  
  add[0] = add[1] = false;

  for (int cnt=0; cnt<7; ++cnt) {
    add[0] |= in_o[cnt].x() < 0;
    add[1] |= in_o[cnt].y() < 0;
  }

  Covering_sheets c = number_of_sheets();
  if (add[0] || add[1]) {
    const Offset oo = Offset(add[0]?c[0]:0, add[1]?c[1]:0);
    for (int i=0; i<7; ++i) in_o[i] += oo;
  }

  for (int cnt=0; cnt<7; ++cnt)
    out_o[cnt] = (in_o[cnt].x() >= c[0] ? 2 : 0) + (in_o[cnt].y() >= c[1] ? 1 : 0);
}

template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_star   (Vertex_handle &, int j,
std::vector<Face_handle> &f, std::vector<Vertex_handle> &w, std::vector<int> &i)
{ // removing a degree 7 vertex, staring from w[j]
  rotate7(j,w,f,i);
  
  Face_handle nn;
  f[1]->set_vertex( i[1], w[0]) ;  // f1 = w1w2w0
  f[2]->set_vertex( i[2], w[0]) ;  // f2 = w2w3w0
  f[3]->set_vertex( i[3], w[0]) ;  // f3 = w3w4w0
  f[4]->set_vertex( i[4], w[0]) ;  // f4 = w4w5w0
  f[5]->set_vertex( i[5], w[0]) ;  // f5 = w5w6w0

  nn = f[0]->neighbor( i[0] );
  tds().set_adjacency(f[1], cw(i[1]) , nn , nn->index(f[0])  );
  nn = f[6]->neighbor( i[6] );
  tds().set_adjacency(f[5], ccw(i[5]) , nn , nn->index(f[6]) );
  tds().delete_face(f[0]);
  tds().delete_face(f[6]);
  

  f[1]->set_offsets(0, 0, 0);
  f[2]->set_offsets(0, 0, 0);
  f[3]->set_offsets(0, 0, 0);
  f[4]->set_offsets(0, 0, 0);
  f[5]->set_offsets(0, 0, 0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_star   (Vertex_handle &v, int j,
std::vector<Face_handle> &f, std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i)
{ // removing a degree 7 vertex, staring from w[j]

  // Rotate the offset as well
  rotate7(j,w,f,o,i);
  remove_degree7_star(v, /* !! */ 0, f, w, i);
  
  int oo[7];
  get_offset_degree7(o, oo);
  int o_face[3]; int ii;
  ii = i[1]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[1]; o_face[ cw(ii)] = oo[2];
  this->set_offsets(f[1], o_face[0], o_face[1], o_face[2]);
  ii = i[2]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[2]; o_face[ cw(ii)] = oo[3];
  this->set_offsets(f[2], o_face[0], o_face[1], o_face[2]);
  ii = i[3]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[3]; o_face[ cw(ii)] = oo[4];
  this->set_offsets(f[3], o_face[0], o_face[1], o_face[2]);
  ii = i[4]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[4]; o_face[ cw(ii)] = oo[5];
  this->set_offsets(f[4], o_face[0], o_face[1], o_face[2]);
  ii = i[5]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[5]; o_face[ cw(ii)] = oo[6];
  this->set_offsets(f[5], o_face[0], o_face[1], o_face[2]);
  
  this->insert_too_long_edge(f[1], ccw(i[1]));
  this->insert_too_long_edge(f[2], ccw(i[2]));
  this->insert_too_long_edge(f[3], ccw(i[3]));
  this->insert_too_long_edge(f[4], ccw(i[4]));
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_zigzag (Vertex_handle &, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<int> &i)
{ // removing a degree 7 vertex, zigzag, w[j] = middle point
  rotate7(j,w,f,i);

  Face_handle nn;
  f[1]->set_vertex(    i[1] , w[3]) ;  // f1 = w1w2w3
  f[2]->set_vertex(ccw(i[2]), w[1]) ;
  f[2]->set_vertex(    i[2] , w[0]) ;  // f2 = w1w3w0
  f[3]->set_vertex(    i[3] , w[0]) ;  // f3 = w3w4w0
  f[4]->set_vertex( cw(i[4]), w[6]) ;
  f[4]->set_vertex(    i[4] , w[0]) ;  // f4 = w4w6w0
  f[5]->set_vertex(    i[5] , w[4]) ;  // f5 = w5w6w4

  nn = f[2]->neighbor( i[2] );
  tds().set_adjacency(f[1], ccw(i[1]) , nn, nn->index(f[2]) );
  nn = f[0]->neighbor( i[0] );
  tds().set_adjacency(f[2], cw(i[2]) , nn , nn->index(f[0]) );
  nn = f[6]->neighbor( i[6] );
  tds().set_adjacency(f[4], ccw(i[4]) , nn , nn->index(f[6])  );
  nn = f[4]->neighbor( i[4] );
  tds().set_adjacency(f[5], cw(i[5]) , nn , nn->index(f[4])  );
  tds().set_adjacency(f[1], cw(i[1]) , f[2] , i[2]   );
  tds().set_adjacency(f[4], i[4]  , f[5] , ccw(i[5])  );

  tds().delete_face(f[0]);
  tds().delete_face(f[6]);


  f[1]->set_offsets(0, 0, 0);
  f[2]->set_offsets(0, 0, 0);
  f[3]->set_offsets(0, 0, 0);
  f[4]->set_offsets(0, 0, 0);
  f[5]->set_offsets(0, 0, 0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_zigzag (Vertex_handle &v, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i)
{ // removing a degree 7 vertex, zigzag, w[j] = middle point

  // Rotate the offset as well
  rotate7(j,w,f,o,i);
  remove_degree7_zigzag(v, /* !! */ 0, f, w, i);

  int oo[7];
  get_offset_degree7(o, oo);
  int o_face[3]; int ii;
  ii = i[1]; o_face[ii] = oo[3]; o_face[ccw(ii)] = oo[1]; o_face[ cw(ii)] = oo[2];
  this->set_offsets(f[1], o_face[0], o_face[1], o_face[2]);
  ii = i[2]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[1]; o_face[ cw(ii)] = oo[3];
  this->set_offsets(f[2], o_face[0], o_face[1], o_face[2]);
  ii = i[3]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[3]; o_face[ cw(ii)] = oo[4];
  this->set_offsets(f[3], o_face[0], o_face[1], o_face[2]);
  ii = i[4]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[4]; o_face[ cw(ii)] = oo[6];
  this->set_offsets(f[4], o_face[0], o_face[1], o_face[2]);
  ii = i[5]; o_face[ii] = oo[4]; o_face[ccw(ii)] = oo[5]; o_face[ cw(ii)] = oo[6];
  this->set_offsets(f[5], o_face[0], o_face[1], o_face[2]);
  
  this->insert_too_long_edge(f[1],  cw(i[1]));
  this->insert_too_long_edge(f[2], ccw(i[2]));
  this->insert_too_long_edge(f[3], ccw(i[3]));
  this->insert_too_long_edge(f[4],     i[4]);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_leftdelta(Vertex_handle &, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<int> &i)
{ // removing a degree 7 vertex, left delta from w[j]
  rotate7(j,w,f,i);

  Face_handle nn;
  f[1]->set_vertex(    i[1] , w[0]) ;  // f1 = w1w2w0
  f[2]->set_vertex(    i[2] , w[0]) ;  // f2 = w2w3w0
  f[3]->set_vertex( cw(i[3]), w[5]) ;
  f[3]->set_vertex(    i[3] , w[0]) ;  // f3 = w3w5w0
  f[4]->set_vertex(    i[4] , w[3]) ;  // f4 = w4w5w3
  f[5]->set_vertex(    i[5] , w[0]) ;  // f5 = w5w6w0

  nn = f[0]->neighbor( i[0] );
  tds().set_adjacency(f[1], cw(i[1]) , nn , nn->index(f[0])  );
  nn = f[3]->neighbor( i[3] );
  tds().set_adjacency(f[4], cw(i[4]) , nn , nn->index(f[3]) );
  nn = f[6]->neighbor( i[6] );
  tds().set_adjacency(f[5], ccw(i[5]) , nn , nn->index(f[6])  );
  tds().set_adjacency(f[3], i[3]  , f[4] , ccw(i[4])  );
  tds().set_adjacency(f[3], ccw(i[3]) , f[5] ,  cw(i[5]) );

  tds().delete_face(f[0]);
  tds().delete_face(f[6]);

  f[1]->set_offsets(0, 0, 0);
  f[2]->set_offsets(0, 0, 0);
  f[3]->set_offsets(0, 0, 0);
  f[4]->set_offsets(0, 0, 0);
  f[5]->set_offsets(0, 0, 0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_leftdelta(Vertex_handle &v, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i)
{ // removing a degree 7 vertex, left delta from w[j]

  // Rotate the offset as well
  rotate7(j,w,f,o,i);
  remove_degree7_leftdelta(v, /* !! */ 0, f, w, i);

  int oo[7];
  get_offset_degree7(o, oo);

  int o_face[3]; int ii;
  ii = i[1]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[1]; o_face[ cw(ii)] = oo[2];
  this->set_offsets(f[1], o_face[0], o_face[1], o_face[2]);
  ii = i[2]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[2]; o_face[ cw(ii)] = oo[3];
  this->set_offsets(f[2], o_face[0], o_face[1], o_face[2]);
  ii = i[3]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[3]; o_face[ cw(ii)] = oo[5];
  this->set_offsets(f[3], o_face[0], o_face[1], o_face[2]);
  ii = i[4]; o_face[ii] = oo[3]; o_face[ccw(ii)] = oo[4]; o_face[ cw(ii)] = oo[5];
  this->set_offsets(f[4], o_face[0], o_face[1], o_face[2]);
  ii = i[5]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[5]; o_face[ cw(ii)] = oo[6];
  this->set_offsets(f[5], o_face[0], o_face[1], o_face[2]);
    
  this->insert_too_long_edge(f[1], ccw(i[1]));
  this->insert_too_long_edge(f[2], ccw(i[2]));
  this->insert_too_long_edge(f[3],     i[3]);
  this->insert_too_long_edge(f[3], ccw(i[3]));
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_rightdelta(Vertex_handle &, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<int> &i)
{ // removing a degree 7 vertex, right delta from w[j]
  rotate7(j,w,f,i);

  Face_handle nn;
  f[1]->set_vertex(    i[1] , w[0]) ;  // f1 = w1w2w0
  f[2]->set_vertex(    i[2] , w[4]) ;  // f2 = w2w3w4
  f[3]->set_vertex(ccw(i[3]), w[2]) ;
  f[3]->set_vertex(    i[3] , w[0]) ;  // f3 = w2w4w0
  f[4]->set_vertex(    i[4] , w[0]) ;  // f4 = w4w5w0
  f[5]->set_vertex(    i[5] , w[0]) ;  // f5 = w5w6w0

  nn = f[0]->neighbor( i[0] );
  tds().set_adjacency(f[1], cw(i[1]) , nn , nn->index(f[0]) );
  nn = f[3]->neighbor( i[3] );
  tds().set_adjacency(f[2], ccw(i[2]) , nn, nn->index(f[3]) );
  nn = f[6]->neighbor( i[6] );
  tds().set_adjacency(f[5], ccw(i[5]) , nn , nn->index(f[6]) );
  tds().set_adjacency(f[1], ccw(i[1]) , f[3], cw(i[3])  );
  tds().set_adjacency(f[3], i[3]  , f[2], cw(i[2]) );

  tds().delete_face(f[0]);
  tds().delete_face(f[6]);

  f[1]->set_offsets(0, 0, 0);
  f[2]->set_offsets(0, 0, 0);
  f[3]->set_offsets(0, 0, 0);
  f[4]->set_offsets(0, 0, 0);
  f[5]->set_offsets(0, 0, 0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_rightdelta(Vertex_handle &v, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i)
{ // removing a degree 7 vertex, right delta from w[j]

  // Rotate the offset as well
  rotate7(j,w,f,o,i);
  remove_degree7_rightdelta(v, /* !! */ 0, f, w, i);

  int oo[7];
  get_offset_degree7(o, oo);
  int o_face[3]; int ii;
  ii = i[1]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[1]; o_face[ cw(ii)] = oo[2];
  this->set_offsets(f[1], o_face[0], o_face[1], o_face[2]);
  ii = i[2]; o_face[ii] = oo[4]; o_face[ccw(ii)] = oo[2]; o_face[ cw(ii)] = oo[3];
  this->set_offsets(f[2], o_face[0], o_face[1], o_face[2]);
  ii = i[3]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[2]; o_face[ cw(ii)] = oo[4];
  this->set_offsets(f[3], o_face[0], o_face[1], o_face[2]);
  ii = i[4]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[4]; o_face[ cw(ii)] = oo[5];
  this->set_offsets(f[4], o_face[0], o_face[1], o_face[2]);
  ii = i[5]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[5]; o_face[ cw(ii)] = oo[6];
  this->set_offsets(f[5], o_face[0], o_face[1], o_face[2]);
    
  this->insert_too_long_edge(f[1], ccw(i[1]));
  this->insert_too_long_edge(f[2],  cw(i[2]));
  this->insert_too_long_edge(f[3], ccw(i[3]));
  this->insert_too_long_edge(f[4], ccw(i[4]));
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_leftfan(Vertex_handle &, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<int> &i)
{ // removing a degree 7 vertex, left fan from w[j]
  rotate7(j,w,f,i);

  Face_handle nn;
  f[1]->set_vertex(    i[1] , w[0]) ;  // f1 = w1w2w0
  f[2]->set_vertex(    i[2] , w[0]) ;  // f2 = w2w3w0
  f[3]->set_vertex(    i[3] , w[0]) ;  // f3 = w3w4w0
  f[4]->set_vertex(    i[4] , w[6]) ;  // f4 = w4w5w6
  f[6]->set_vertex(    i[6] , w[4]) ;  // f6 = w6w0w4

  nn = f[0]->neighbor( i[0] );
  tds().set_adjacency(f[1], cw(i[1]) , nn, nn->index(f[0]) );
  nn = f[5]->neighbor( i[5] );
  tds().set_adjacency(f[4], ccw(i[4]) , nn, nn->index(f[5]) );
  tds().set_adjacency(f[3], ccw(i[3]) , f[6], ccw(i[6]) );
  tds().set_adjacency(f[6], cw(i[6]) , f[4], cw(i[4]) );

  tds().delete_face(f[0]);
  tds().delete_face(f[5]);

  f[1]->set_offsets(0, 0, 0);
  f[2]->set_offsets(0, 0, 0);
  f[3]->set_offsets(0, 0, 0);
  f[4]->set_offsets(0, 0, 0);
  f[6]->set_offsets(0, 0, 0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_leftfan(Vertex_handle &v, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i)
{ // removing a degree 7 vertex, left fan from w[j]

  // Rotate the offset as well
  rotate7(j,w,f,o,i);
  remove_degree7_leftfan(v, /* !! */ 0, f, w, i);

  int oo[7];
  get_offset_degree7(o, oo);

  int o_face[3]; int ii;
  ii = i[1]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[1]; o_face[ cw(ii)] = oo[2];
  this->set_offsets(f[1], o_face[0], o_face[1], o_face[2]);
  ii = i[2]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[2]; o_face[ cw(ii)] = oo[3];
  this->set_offsets(f[2], o_face[0], o_face[1], o_face[2]);
  ii = i[3]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[3]; o_face[ cw(ii)] = oo[4];
  this->set_offsets(f[3], o_face[0], o_face[1], o_face[2]);
  ii = i[4]; o_face[ii] = oo[6]; o_face[ccw(ii)] = oo[4]; o_face[ cw(ii)] = oo[5];
  this->set_offsets(f[4], o_face[0], o_face[1], o_face[2]);
  ii = i[6]; o_face[ii] = oo[4]; o_face[ccw(ii)] = oo[6]; o_face[ cw(ii)] = oo[0];
  this->set_offsets(f[6], o_face[0], o_face[1], o_face[2]);
  
  this->insert_too_long_edge(f[1], ccw(i[1]));
  this->insert_too_long_edge(f[2], ccw(i[2]));
  this->insert_too_long_edge(f[3], ccw(i[3]));
  this->insert_too_long_edge(f[4],  cw(i[4]));
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_rightfan(Vertex_handle &, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<int> &i)
{ // removing a degree 7 vertex, right fan from w[j]
  rotate7(j,w,f,i);

  Face_handle nn;
  f[0]->set_vertex(    i[0] , w[3]) ;  // f0 = w0w1w3
  f[2]->set_vertex(    i[2] , w[1]) ;  // f2 = w2w3w1
  f[3]->set_vertex(    i[3] , w[0]) ;  // f3 = w3w4w0
  f[4]->set_vertex(    i[4] , w[0]) ;  // f4 = w4w5w0
  f[5]->set_vertex(    i[5] , w[0]) ;  // f5 = w5w6w0

  nn = f[1]->neighbor( i[1] );
  tds().set_adjacency(f[2], cw(i[2]) , nn, nn->index(f[1]) );
  nn = f[6]->neighbor( i[6] );
  tds().set_adjacency(f[5], ccw(i[5]) , nn, nn->index(f[6]) );
  tds().set_adjacency(f[2], ccw(i[2]) , f[0], ccw(i[0])  );
  tds().set_adjacency(f[0], cw(i[0]) , f[3] , cw(i[3]) );

  tds().delete_face(f[1]);
  tds().delete_face(f[6]);

  f[0]->set_offsets(0, 0, 0);
  f[2]->set_offsets(0, 0, 0);
  f[3]->set_offsets(0, 0, 0);
  f[4]->set_offsets(0, 0, 0);
  f[5]->set_offsets(0, 0, 0);
}
template < class Gt, class Tds >
void
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
remove_degree7_rightfan(Vertex_handle &v, int j,
 std::vector<Face_handle> &f,std::vector<Vertex_handle> &w, std::vector<Offset> &o, std::vector<int> &i)
{ // removing a degree 7 vertex, right fan from w[j]

  // Rotate the offset as well
  rotate7(j,w,f,o,i);
  remove_degree7_rightfan(v, /* !! */ 0, f, w, i);

  int oo[7];
  get_offset_degree7(o, oo);
  int o_face[3]; int ii;
  ii = i[0]; o_face[ii] = oo[3]; o_face[ccw(ii)] = oo[0]; o_face[ cw(ii)] = oo[1];
  this->set_offsets(f[0], o_face[0], o_face[1], o_face[2]);
  ii = i[2]; o_face[ii] = oo[1]; o_face[ccw(ii)] = oo[2]; o_face[ cw(ii)] = oo[3];
  this->set_offsets(f[2], o_face[0], o_face[1], o_face[2]);
  ii = i[3]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[3]; o_face[ cw(ii)] = oo[4];
  this->set_offsets(f[3], o_face[0], o_face[1], o_face[2]);
  ii = i[4]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[4]; o_face[ cw(ii)] = oo[5];
  this->set_offsets(f[4], o_face[0], o_face[1], o_face[2]);
  ii = i[5]; o_face[ii] = oo[0]; o_face[ccw(ii)] = oo[5]; o_face[ cw(ii)] = oo[6];
  this->set_offsets(f[5], o_face[0], o_face[1], o_face[2]);
  
  this->insert_too_long_edge(f[0], ccw(i[0]));
  this->insert_too_long_edge(f[0],  cw(i[0]));
  this->insert_too_long_edge(f[3], ccw(i[3]));
  this->insert_too_long_edge(f[4], ccw(i[4]));
}




///////////////////////////////////////////////////////////////
//  DISPLACEMENT

template <class Gt, class Tds >
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
move_if_no_collision(Vertex_handle v, const Point &p) {
  Locate_type lt;
  int li;
  Vertex_handle inserted;
  Face_handle loc = locate(p, lt, li, v->face());

  if (lt == Triangulation::VERTEX)
    return v;
  else 
    /// This can be optimized by checking whether we can move v->point() to p
    return insert(p, lt, loc, li);
}

template <class Gt, class Tds >
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
move_point(Vertex_handle v, const Point &p) {
  if(v->point() == p) return v;
  Vertex_handle w = move_if_no_collision(v,p);
  if(w != v) {
    remove(v);
    return w;
  }
  return v;
}

template <class Gt, class Tds >
bool
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
is_delaunay_after_displacement(Vertex_handle v, const Point &p) const
{
    NGHK_NYI;
  CGAL_triangulation_precondition(!is_infinite(v));
  CGAL_triangulation_precondition(dimension() == 2);
  CGAL_triangulation_precondition(!this->test_dim_down(v));
	if(v->point() == p) return true;
  Point ant = v->point();
  v->set_point(p);
  if(!this->well_oriented(v))
  {
    v->set_point(ant);
    return false;
  }
  std::list<Edge> edges;
  Face_circulator fc = this->incident_faces(v), done(fc);
  int degree = 0;
  do {
    if((++degree) > 3) break;
  } while(++fc != done);
  fc = this->incident_faces(v);
  done = fc;
  if(degree == 3) {
    do {
      int i = fc->index(v);
      edges.push_back(Edge(fc, i));
    } while(++fc != done);
  } else {
    do {
      int i = fc->index(v);
      edges.push_back(Edge(fc, i));
      edges.push_back(Edge(fc, cw(i)));
    } while(++fc != done);
  }
  while(!edges.empty()) {
    const Edge &e = edges.front();
    Face_handle f = e.first;
    int i = e.second;
    edges.pop_front();
    if(is_infinite(f->vertex(i))) continue;
    Face_handle fi = f->neighbor(i);
    Vertex_handle vm = this->_tds.mirror_vertex(f, i);
    if(is_infinite(vm)) continue;
    if(this->side_of_oriented_circle(f, vm->point()) == ON_POSITIVE_SIDE) {
      v->set_point(ant);
      return false;
    }
  }
  v->set_point(ant);
  return true;
}

template <class Gt, class Tds >
template <class OutputItFaces>
typename Periodic_2_Delaunay_triangulation_2<Gt,Tds>::Vertex_handle
Periodic_2_Delaunay_triangulation_2<Gt,Tds>::
move_if_no_collision_and_give_new_faces(Vertex_handle v,
                                        const Point &p,
                                        OutputItFaces oif)
{
    NGHK_NYI;
  CGAL_triangulation_precondition(!is_infinite(v));
  if(v->point() == p) return v;

  typedef std::list<Face_handle>                        Faces_list;
  const int dim = dimension();

  if(dim == 2) {
    Point ant = v->point();
    v->set_point(p);
    // This option optimizes only when most of the
    // displacements would not break the orientation
    // of the faces.. we will consider this as an a priori,
    // because otherwise it is pointless to just do
    // not rebuild from scratch.
    if(well_oriented(v)) {
      std::set<Face_handle> faces_set;
      restore_edges(v, faces_set);
      for(typename std::set<Face_handle>::iterator ib = faces_set.begin(),
            iend = faces_set.end(); ib != iend; ib++) *oif++ = *ib;
      return v;
    }
    v->set_point(ant);
  }

  Locate_type lt;
  int li;
  Vertex_handle inserted;
  Face_handle loc = locate(p, lt, li, v->face());

  if(lt == Triangulation::VERTEX) return loc->vertex(li);

  if(dim == 0) {
    v->point() = p;
    return v;
  }

  size_type n_vertices = tds().number_of_vertices();

  if((lt == Triangulation::EMPTY) &&
     (dim == 1) && (n_vertices == 3)) {
    v->point() = p;
    for(All_faces_iterator afi = this-> all_faces_begin();
        afi != this->all_faces_end();
        afi++) *oif++ = afi;
    return v;
  }

  if((lt != Triangulation::EMPTY) && (dim == 1)) {
    if(loc->has_vertex(v)) {
      v->point() = p;
    } else {
      inserted = insert(p, lt, loc, li);
      Face_handle f = v->face();
      int i = f->index(v);
      if (i==0) {f = f->neighbor(1);}
      CGAL_triangulation_assertion(f->index(v) == 1);
      Face_handle g= f->neighbor(0);
      f->set_vertex(1, g->vertex(1));
      f->set_neighbor(0,g->neighbor(0));
      g->neighbor(0)->set_neighbor(1,f);
      g->vertex(1)->set_face(f);
      this->delete_face(g);
      *oif++ = f;
      Face_handle f_ins = inserted->face();
      i = f_ins->index(inserted);
      if (i==0) {f_ins = f_ins->neighbor(1);}
      CGAL_triangulation_assertion(f_ins->index(inserted) == 1);
      Face_handle g_ins = f_ins->neighbor(0);
      f_ins->set_vertex(1, v);
      g_ins->set_vertex(0, v);
      v->set_face(inserted->face());
    	v->set_point(p);
      this->delete_vertex(inserted);
    }
    *oif++ = v->face();
    if(v->face()->neighbor(0)->has_vertex(v))
      *oif++ = v->face()->neighbor(0);
    if(v->face()->neighbor(1)->has_vertex(v))
      *oif++ = v->face()->neighbor(1);
    return v;
  }

  if((lt != Triangulation::EMPTY) && this->test_dim_down(v)) {
    // verify if p and two static vertices are collinear in this case
    int iinf;
    Face_circulator finf = incident_faces(this->infinite_vertex()),
      fdone(finf);
    do {
      if(!finf->has_vertex(v))
      {
        iinf = ~(finf->index(this->infinite_vertex()));
        break;
      }
    } while(++finf != fdone);
    if(this->orientation(finf->vertex(iinf&1)->point(),
                         finf->vertex(iinf&2)->point(),
                         p) == COLLINEAR)
    {
      v->point() = p;
      tds().dim_down(loc, loc->index(v));
      for(All_faces_iterator afi = this-> all_faces_begin();
          afi != this->all_faces_end();
          afi++) *oif++ = afi;
      return v;
    }
  }

  std::set<Face_handle> faces_set;
  inserted = Periodic_2_Delaunay_triangulation_2<Gt,Tds>::insert(p, lt, loc, li);
  Face_circulator fc = this->incident_faces(inserted), done(fc);
  do { faces_set.insert(fc); } while(++fc != done);


  {
    static int maxd=30;
    static std::vector<Face_handle> f(maxd);
    static std::vector<int> i(maxd);
    static std::vector<Vertex_handle> w(maxd);
    int d;
    remove_degree_init(v,f,w,i,d,maxd);
    remove_degree_triangulate(v,f,w,i,d);
    this->delete_vertex(v);
    Face_circulator fc(v[0]),done;
    do *oif++ = fc++; while (fc!=done);
  }

  fc = this->incident_faces(inserted), done(fc);
  std::vector<Face_handle> faces_pt;
  faces_pt.reserve(16);
  do { faces_pt.push_back(fc); } while(++fc != done);
  int ss = faces_pt.size();
  for(int k=0; k<ss; k++)
  {
    Face_handle f = faces_pt[k];
    int i = f->index(inserted);
    f->set_vertex(i, v);
  }
  v->set_point(p);
  v->set_face(inserted->face());
  this->delete_vertex(inserted);

  for(typename std::set<Face_handle>::const_iterator ib = faces_set.begin(),
        iend = faces_set.end(); ib != iend; ib++) *oif++ = *ib;

  return v;
}

template<class Gt, class Tds>
void Periodic_2_Delaunay_triangulation_2<Gt, Tds>::fill_hole_delaunay(std::list<Edge> & first_hole) {
  typedef std::list<Edge> Hole;
  typedef std::list<Hole> Hole_list;

  Face_handle  f, ff, fn;
  int i, ii, in;
  Hole_list hole_list;
  Hole hole;

  hole_list.push_front(first_hole);

  while( ! hole_list.empty())
  {
    hole = hole_list.front();
    hole_list.pop_front();
    typename Hole::iterator hit = hole.begin();

    // if the hole has only three edges, create the triangle
    if (hole.size() == 3) {
      hit = hole.begin();
      f = (*hit).first;        i = (*hit).second;
      ff = (* ++hit).first;    ii = (*hit).second;
      fn = (* ++hit).first;    in = (*hit).second;
      Face_handle newf = create_face(f,i,ff,ii,fn,in);
      newf->set_offsets(0,0,0);
      
      continue;
    }

    // else find an edge with two finite vertices
    // on the hole boundary
    // and the new triangle adjacent to that edge
    //  cut the hole and push it back

    // take the first neighboring face and pop it;
    ff = (hole.front()).first;
    ii =(hole.front()).second;
    hole.pop_front();

    Vertex_handle v0 = ff->vertex(cw(ii));
    Vertex_handle v1 = ff->vertex(ccw(ii));
    Vertex_handle v2 = Vertex_handle();
    Vertex_handle v3 = Vertex_handle();
    const Point& p0 = v0->point();
    const Point& p1 = v1->point();

    typename Hole::iterator hdone = hole.end();
    hit =  hole.begin();
    typename Hole::iterator cut_after(hit);

    // if tested vertex is c with respect to the vertex opposite
    // to NULL neighbor,
    // stop at the before last face;
    hdone--;
    while( hit != hdone) {
      fn = (*hit).first;
      in = (*hit).second;
      Vertex_handle vv = fn->vertex(ccw(in));

      const Point &p = vv->point();
      Orientation orient = orientation(p0,p1,p);

      if (orient == COUNTERCLOCKWISE) {
        if (v2 == Vertex_handle()) {
          v2=vv;
          v3=vv;
          cut_after=hit;
        } else {
          Oriented_side side = side_of_oriented_circle(p0,p1,v3->point(),p,true);
          if (side == ON_POSITIVE_SIDE) {
            v2=vv;
            v3=vv;
            cut_after=hit;
          }
        }
      }

      ++hit;
    }

    // create new triangle and update adjacency relations
    Face_handle newf;

    //update the hole and push back in the Hole_List stack
    // if v2 belongs to the neighbor following or preceding *f
    // the hole remain a single hole
    // otherwise it is split in two holes

    fn = (hole.front()).first;
    in = (hole.front()).second;
    if (fn->has_vertex(v2, i) && i == fn->ccw(in)) {
      newf = create_face(ff,ii,fn,in);

      newf->set_offsets(0,0,0);

      hole.pop_front();
      hole.push_front(Edge(newf,1));
      hole_list.push_front(hole);
    }
    else{
      fn = (hole.back()).first;
      in = (hole.back()).second;
      if (fn->has_vertex(v2, i) && i== fn->cw(in)) {
        newf = create_face(fn,in,ff,ii);
        newf->set_offsets(0,0,0);

        hole.pop_back();
        hole.push_back(Edge(newf,1));
        hole_list.push_front(hole);
      } else {
        // split the hole in two holes
        CGAL_assertion(v2 != Vertex_handle());
        newf = create_face(ff,ii,v2);
        newf->set_offsets(0,0,0);

        Hole new_hole;
        ++cut_after;
        while( hole.begin() != cut_after )
        {
          new_hole.push_back(hole.front());
          hole.pop_front();
        }

        hole.push_front(Edge( newf,1));
        new_hole.push_front(Edge( newf,0));
        hole_list.push_front(hole);
        hole_list.push_front(new_hole);
      }
    }
  }
}

template<class Gt, class Tds>
void Periodic_2_Delaunay_triangulation_2<Gt, Tds>::fill_hole_delaunay(
    std::list<Edge> & first_hole,
    std::map<Vertex_handle, Offset> &vertex_offsets) {
  typedef std::list<Edge> Hole;
  typedef std::list<Hole> Hole_list;

  Face_handle  f, ff, fn;
  int i, ii, in;
  Hole_list hole_list;
  Hole hole;

  hole_list.push_front(first_hole);

  while( ! hole_list.empty())
  {
    hole = hole_list.front();
    hole_list.pop_front();
    typename Hole::iterator hit = hole.begin();

    // if the hole has only three edges, create the triangle
    if (hole.size() == 3) {
      hit = hole.begin();
      f = (*hit).first;        i = (*hit).second;
      ff = (* ++hit).first;    ii = (*hit).second;
      fn = (* ++hit).first;    in = (*hit).second;
      Face_handle newf = create_face(f,i,ff,ii,fn,in);
      Offset oo0(vertex_offsets[newf->vertex(0)]);
      Offset oo1(vertex_offsets[newf->vertex(1)]);
      Offset oo2(vertex_offsets[newf->vertex(2)]);
      if (oo0.x() < 0 || oo1.x() < 0 || oo2.x() < 0) {
        oo0 += Offset(number_of_sheets()[0], 0); oo1 += Offset(number_of_sheets()[0], 0); oo2 += Offset(number_of_sheets()[0], 0);
      }
      if (oo0.y() < 0 || oo1.y() < 0 || oo2.y() < 0) {
        oo0 += Offset(0, number_of_sheets()[1]); oo1 += Offset(0, number_of_sheets()[1]); oo2 += Offset(0, number_of_sheets()[1]);
      }
      set_offsets(newf, 
                  (oo0.x() >= number_of_sheets()[0] ? 2 : 0) + (oo0.y() >= number_of_sheets()[1] ? 1 : 0),
                  (oo1.x() >= number_of_sheets()[0] ? 2 : 0) + (oo1.y() >= number_of_sheets()[1] ? 1 : 0),
                  (oo2.x() >= number_of_sheets()[0] ? 2 : 0) + (oo2.y() >= number_of_sheets()[1] ? 1 : 0));
      
      insert_too_long_edge(newf, 0);
      insert_too_long_edge(newf, 1);
      insert_too_long_edge(newf, 2);

      continue;
    }

    // else find an edge with two finite vertices
    // on the hole boundary
    // and the new triangle adjacent to that edge
    //  cut the hole and push it back

    // take the first neighboring face and pop it;
    ff = (hole.front()).first;
    ii =(hole.front()).second;
    hole.pop_front();

    Vertex_handle v0 = ff->vertex(cw(ii));
    Vertex_handle v1 = ff->vertex(ccw(ii));
    Vertex_handle v2 = Vertex_handle();
    Vertex_handle v3 = Vertex_handle();
    const Point& p0 = v0->point();
    const Point& p1 = v1->point();
    const Offset o0 = vertex_offsets[v0];
    const Offset o1 = vertex_offsets[v1];
    bool simplicity_criterion = (o0 == o1);

    typename Hole::iterator hdone = hole.end();
    hit =  hole.begin();
    typename Hole::iterator cut_after(hit);

    // if tested vertex is c with respect to the vertex opposite
    // to NULL neighbor,
    // stop at the before last face;
    hdone--;
    while( hit != hdone) {
      fn = (*hit).first;
      in = (*hit).second;
      Vertex_handle vv = fn->vertex(ccw(in));

      const Point &p = vv->point();
      CGAL_assertion(vertex_offsets.find(vv) != vertex_offsets.end());
      const Offset o = vertex_offsets[vv];
      Orientation orient;
      simplicity_criterion &= (o == o0);
      if (simplicity_criterion)
        orient = orientation(p0,p1,p);
      else
        orient = orientation(p0,p1,p, o0,o1,o);

      if (orient == COUNTERCLOCKWISE) {
        if (v2 == Vertex_handle()) {
          v2=vv;
          v3=vv;
          cut_after=hit;
        } else {
          Offset o3 = vertex_offsets[v3];
          Oriented_side side;
          if (simplicity_criterion && (o3 == o0))
            side = side_of_oriented_circle(p0,p1,v3->point(),p,
                                           true);
          else
            side = side_of_oriented_circle(p0,p1,v3->point(),p,
                                           o0,o1,o3,o,
                                           true);
          if (side == ON_POSITIVE_SIDE) {
            v2=vv;
            v3=vv;
            cut_after=hit;
          }
        }
      }

      ++hit;
    }

    // create new triangle and update adjacency relations
    Face_handle newf;

    //update the hole and push back in the Hole_List stack
    // if v2 belongs to the neighbor following or preceding *f
    // the hole remain a single hole
    // otherwise it is split in two holes

    fn = (hole.front()).first;
    in = (hole.front()).second;
    if (fn->has_vertex(v2, i) && i == fn->ccw(in)) {
      newf = create_face(ff,ii,fn,in);

      Offset oo0 = o0;
      Offset oo1 = o1;
      Offset oo2 = vertex_offsets[v2];
      if (oo0.x() < 0 || oo1.x() < 0 || oo2.x() < 0) {
        oo0 += Offset(number_of_sheets()[0], 0); oo1 += Offset(number_of_sheets()[0], 0); oo2 += Offset(number_of_sheets()[0], 0);
      }
      if (oo0.y() < 0 || oo1.y() < 0 || oo2.y() < 0) {
        oo0 += Offset(0, number_of_sheets()[1]); oo1 += Offset(0, number_of_sheets()[1]); oo2 += Offset(0, number_of_sheets()[1]);
      }
      set_offsets(newf, 
                  (oo0.x() >= number_of_sheets()[0] ? 2 : 0) + (oo0.y() >= number_of_sheets()[1] ? 1 : 0),
                  (oo1.x() >= number_of_sheets()[0] ? 2 : 0) + (oo1.y() >= number_of_sheets()[1] ? 1 : 0),
                  (oo2.x() >= number_of_sheets()[0] ? 2 : 0) + (oo2.y() >= number_of_sheets()[1] ? 1 : 0));
      // set_offsets(newf, o0, o1, o2);
      insert_too_long_edge(newf, 0);
      insert_too_long_edge(newf, 1);

      hole.pop_front();
      hole.push_front(Edge(newf,1));
      hole_list.push_front(hole);
    }
    else{
      fn = (hole.back()).first;
      in = (hole.back()).second;
      if (fn->has_vertex(v2, i) && i== fn->cw(in)) {
        newf = create_face(fn,in,ff,ii);
        Offset oo0 = o0;
        Offset oo1 = o1;
        Offset oo2 = vertex_offsets[v2];
        if (oo0.x() < 0 || oo1.x() < 0 || oo2.x() < 0) {
          oo0 += Offset(number_of_sheets()[0], 0); oo1 += Offset(number_of_sheets()[0], 0); oo2 += Offset(number_of_sheets()[0], 0);
        }
        if (oo0.y() < 0 || oo1.y() < 0 || oo2.y() < 0) {
          oo0 += Offset(0, number_of_sheets()[1]); oo1 += Offset(0, number_of_sheets()[1]); oo2 += Offset(0, number_of_sheets()[1]);
        }
        set_offsets(newf,
                    (oo2.x() >= number_of_sheets()[0] ? 2 : 0) + (oo2.y() >= number_of_sheets()[1] ? 1 : 0), 
                    (oo0.x() >= number_of_sheets()[0] ? 2 : 0) + (oo0.y() >= number_of_sheets()[1] ? 1 : 0),
                    (oo1.x() >= number_of_sheets()[0] ? 2 : 0) + (oo1.y() >= number_of_sheets()[1] ? 1 : 0));
        insert_too_long_edge(newf, 1);
        insert_too_long_edge(newf, 2);
        hole.pop_back();
        hole.push_back(Edge(newf,1));
        hole_list.push_front(hole);
      } else {
        // split the hole in two holes
        CGAL_assertion(v2 != Vertex_handle());
        newf = create_face(ff,ii,v2);
        Offset oo0 = o0;
        Offset oo1 = o1;
        Offset oo2 = vertex_offsets[v2];
        if (oo0.x() < 0 || oo1.x() < 0 || oo2.x() < 0) {
          oo0 += Offset(number_of_sheets()[0], 0); oo1 += Offset(number_of_sheets()[0], 0); oo2 += Offset(number_of_sheets()[0], 0);
        }
        if (oo0.y() < 0 || oo1.y() < 0 || oo2.y() < 0) {
          oo0 += Offset(0, number_of_sheets()[1]); oo1 += Offset(0, number_of_sheets()[1]); oo2 += Offset(0, number_of_sheets()[1]);
        }
        set_offsets(newf, 
                    (oo0.x() >= number_of_sheets()[0] ? 2 : 0) + (oo0.y() >= number_of_sheets()[1] ? 1 : 0),
                    (oo1.x() >= number_of_sheets()[0] ? 2 : 0) + (oo1.y() >= number_of_sheets()[1] ? 1 : 0),
                    (oo2.x() >= number_of_sheets()[0] ? 2 : 0) + (oo2.y() >= number_of_sheets()[1] ? 1 : 0));


        // set_offsets(newf, o0, o1, o2);
        insert_too_long_edge(newf, 0);
        insert_too_long_edge(newf, 1);

        Hole new_hole;
        ++cut_after;
        while( hole.begin() != cut_after )
        {
          new_hole.push_back(hole.front());
          hole.pop_front();
        }

        hole.push_front(Edge( newf,1));
        new_hole.push_front(Edge( newf,0));
        hole_list.push_front(hole);
        hole_list.push_front(new_hole);
      }
    }
  }
}
} //namespace CGAL

#endif // CGAL_PERIODIC_2_DELAUNAY_TRIANGULATION_2_H
