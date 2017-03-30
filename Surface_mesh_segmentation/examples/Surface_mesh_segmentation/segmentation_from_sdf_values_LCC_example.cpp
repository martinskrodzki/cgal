#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/boost/graph/graph_traits_Linear_cell_complex.h>
#include <CGAL/mesh_segmentation.h>

#include <CGAL/property_map.h>

#include <iostream>
#include <fstream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Linear_cell_complex_traits<3, Kernel> MyTraits;

struct Myitem
{
  template<class Refs>
  struct Dart_wrapper
  {
    typedef CGAL::Tag_true Darts_with_id;
    typedef CGAL::Cell_attribute_with_point_and_id< Refs > Vertex_attribute;
    typedef CGAL::Cell_attribute_with_id< Refs > Face_attribute;
    typedef CGAL::cpp11::tuple<Vertex_attribute, void, Face_attribute> Attributes;
  };
};

typedef CGAL::Linear_cell_complex_for_combinatorial_map<2, 3, MyTraits, Myitem> LCC;

int main()
{
    // create and read Polyhedron
    LCC mesh;
    CGAL::load_off_v2("data/cactus.off");

    // create a property-map for SDF values
    typedef std::map<Polyhedron::Facet_const_handle, double> Facet_double_map;
    Facet_double_map internal_sdf_map;
    boost::associative_property_map<Facet_double_map> sdf_property_map(internal_sdf_map);

    // compute SDF values using default parameters for number of rays, and cone angle
    CGAL::sdf_values(mesh, sdf_property_map);

    // create a property-map for segment-ids
    typedef std::map<Polyhedron::Facet_const_handle, std::size_t> Facet_int_map;
    Facet_int_map internal_segment_map;
    boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map);

    // segment the mesh using default parameters for number of levels, and smoothing lambda
    // Any other scalar values can be used instead of using SDF values computed using the CGAL function
    std::size_t number_of_segments = CGAL::segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map);

    std::cout << "Number of segments: " << number_of_segments << std::endl;
    // print segment-ids
    for(Polyhedron::Facet_const_iterator facet_it = mesh.facets_begin();
        facet_it != mesh.facets_end(); ++facet_it) {
        // ids are between [0, number_of_segments -1]
        std::cout << segment_property_map[facet_it] << " ";
    }
    std::cout << std::endl;

    const std::size_t number_of_clusters = 4;       // use 4 clusters in soft clustering
    const double smoothing_lambda = 0.3;  // importance of surface features, suggested to be in-between [0,1]

    // Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
    // This feature is relevant for segmenting the mesh several times with different parameters.
    CGAL::segmentation_from_sdf_values(
      mesh, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda);
}
