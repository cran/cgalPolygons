#ifndef _HEADER_
#include "cgalPolygons.h"
#endif

class CGALpolygonWithHoles {
public:
  Polygon2WithHoles polygonwh;
  Rcpp::XPtr<Polygon2WithHoles> xptr;

  CGALpolygonWithHoles(
    const Rcpp::NumericMatrix outer, const Rcpp::List holes
  )
    : polygonwh(makePolygonWithHoles(outer, holes)),
      xptr(Rcpp::XPtr<Polygon2WithHoles>(&polygonwh, false)) {}

  CGALpolygonWithHoles(Rcpp::XPtr<Polygon2WithHoles> xptr_)
    : polygonwh(*(xptr_.get())), 
      xptr(Rcpp::XPtr<Polygon2WithHoles>(&polygonwh, false)) {}
  

  // -------------------------------------------------------------------------- //
  // -------------------------------------------------------------------------- //
  double area() {
    Polygon2 outer = polygonwh.outer_boundary();
    if(!outer.is_simple()) {
      Rcpp::stop("The outer polygon is not simple.");
    }
    EK::FT a = outer.area();
    int h = 1;
    for(
      auto hit = polygonwh.holes_begin(); hit != polygonwh.holes_end(); ++hit
    ) {
      Polygon2 hole = *hit;
      if(!hole.is_simple()) {
        Rcpp::stop("Hole " + std::to_string(h) + " is not simple.");
      }
      a += hole.area(); // we *add* the area since it is negative
      h++;
    }
    return CGAL::to_double<EK::FT>(a);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_intersection(Rcpp::XPtr<Polygon2WithHoles> plg2XPtr) {
    Polygon2WithHoles plg2 = *(plg2XPtr.get());
    return Intersection(polygonwh, plg2);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_subtract(Rcpp::XPtr<Polygon2WithHoles> plg2XPtr) {
    Polygon2WithHoles plg2 = *(plg2XPtr.get());
    return Subtract(polygonwh, plg2);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_symdiff(Rcpp::XPtr<Polygon2WithHoles> plg2XPtr) {
    Polygon2WithHoles plg2 = *(plg2XPtr.get());
    return Symdiff(polygonwh, plg2);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_union(Rcpp::XPtr<Polygon2WithHoles> plg2XPtr) {
    Polygon2WithHoles plg2 = *(plg2XPtr.get());
    return Union(polygonwh, plg2);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_intersection2(Rcpp::XPtr<Polygon> plg2XPtr) {
    Polygon plg2 = *(plg2XPtr.get());
    Polygon2WithHoles pwh2 = polygonToPolygon2WithHoles(plg2);
    return Intersection(polygonwh, pwh2);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_subtract2(Rcpp::XPtr<Polygon> plg2XPtr) {
    Polygon plg2 = *(plg2XPtr.get());
    Polygon2WithHoles pwh2 = polygonToPolygon2WithHoles(plg2);
    return Subtract(polygonwh, pwh2);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_symdiff2(Rcpp::XPtr<Polygon> plg2XPtr) {
    Polygon plg2 = *(plg2XPtr.get());
    Polygon2WithHoles pwh2 = polygonToPolygon2WithHoles(plg2);
    return Symdiff(polygonwh, pwh2);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List boolop_union2(Rcpp::XPtr<Polygon> plg2XPtr) {
    Polygon plg2 = *(plg2XPtr.get());
    Polygon2WithHoles pwh2 = polygonToPolygon2WithHoles(plg2);
    return Union(polygonwh, pwh2);
  }
  
    
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::NumericMatrix boundingBox() {
    CGAL::Bbox_2 bbox = polygonwh.bbox();
    Rcpp::NumericVector minCorner = {bbox.xmin(), bbox.ymin()};
    Rcpp::NumericVector maxCorner = {bbox.xmax(), bbox.ymax()};
    Rcpp::NumericMatrix Corners(2, 2);
    Corners(0, Rcpp::_) = minCorner;
    Corners(1, Rcpp::_) = maxCorner;
    Rcpp::CharacterVector rownames = {"min", "max"};
    Rcpp::rownames(Corners) = rownames;
    return Corners;
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List convexPartsT() {

    checkPWH(polygonwh);
    
    PTD decomposition;
    std::list<Polygon2> convexParts;
    decomposition(polygonwh, std::back_inserter(convexParts));

    int nparts = convexParts.size();
    
    std::string msg;
    if(nparts == 1) {
      msg = "Only one convex part found.";
    } else {
      msg = "Found " + std::to_string(nparts) + " convex parts.";
    }
    Message(msg);
    
    Rcpp::List Out(nparts);
    int i = 0;
    for(Polygon2 cpolygon : convexParts) {
      Out(i++) = getVertices<Polygon2>(cpolygon);
    }
    
    return Out;
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List convexPartsV() {
    
    checkPWH(polygonwh);
    
    PVD decomposition;
    std::list<Polygon2> convexParts;
    decomposition(polygonwh, std::back_inserter(convexParts));
    
    int nparts = convexParts.size();
    
    std::string msg;
    if(nparts == 1) {
      msg = "Only one convex part found.";
    } else {
      msg = "Found " + std::to_string(nparts) + " convex parts.";
    }
    Message(msg);
    
    Rcpp::List Out(nparts);
    int i = 0;
    for(Polygon2 cpolygon : convexParts) {
      Out(i++) = getVertices<Polygon2>(cpolygon);
    }
    
    return Out;
  }

  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List minkowskiC(Rcpp::XPtr<Polygon2WithHoles> polygonwh2XPtr) {
    
    Polygon2WithHoles polygonwh2 = *(polygonwh2XPtr.get());
    
    Polygon2WithHoles msum = 
      CGAL::minkowski_sum_by_reduced_convolution_2(polygonwh, polygonwh2);
    
    return returnPolygonWithHoles(msum);
  }

  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List minkowskiC2(Rcpp::XPtr<Polygon> polygon2XPtr) {
    
    Polygon polygon2             = *(polygon2XPtr.get());
    Polygon2WithHoles polygonwh2 = polygonToPolygon2WithHoles(polygon2); 
    
    Polygon2WithHoles msum = 
      CGAL::minkowski_sum_by_reduced_convolution_2(polygonwh, polygonwh2);
    
    return returnPolygonWithHoles(msum);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List minkowskiO(Rcpp::XPtr<Polygon2WithHoles> polygonwh2XPtr) {
    
    Polygon2WithHoles polygonwh2 = *(polygonwh2XPtr.get());

    SSABD decomp_no_hole;
    PTD   decomp_with_holes;
    Polygon2WithHoles msum = minkowski_sum_by_decomposition_2(
      polygonwh, polygonwh2, decomp_no_hole, decomp_with_holes
    );
    
    return returnPolygonWithHoles(msum);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List minkowskiT(Rcpp::XPtr<Polygon2WithHoles> polygonwh2XPtr) {
    
    Polygon2WithHoles polygonwh2 = *(polygonwh2XPtr.get());
    
    PTD decomposition;
    Polygon2WithHoles msum = 
      CGAL::minkowski_sum_2(polygonwh, polygonwh2, decomposition);
    
    return returnPolygonWithHoles(msum);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  Rcpp::List minkowskiV(Rcpp::XPtr<Polygon2WithHoles> polygonwh2XPtr) {
    
    Polygon2WithHoles polygonwh2 = *(polygonwh2XPtr.get());
    
    PVD decomposition;
    Polygon2WithHoles msum = 
      CGAL::minkowski_sum_2(polygonwh, polygonwh2, decomposition);
    
    return returnPolygonWithHoles(msum);
  }
  
  
  // ------------------------------------------------------------------------ //
  // ------------------------------------------------------------------------ //
  void print() {
    int nholes = polygonwh.number_of_holes();
    if(nholes == 0) {
      Rcpp::Rcout << "Polygon with zero hole.\n";
    } else if(nholes == 1) {
      Rcpp::Rcout << "Polygon with one hole.\n";
    } else {
      Rcpp::Rcout << "Polygon with " << nholes << " holes.\n";
    }
  }
  
};
