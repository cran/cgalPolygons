// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "cgalPolygons_types.h"
#include <RcppEigen.h>
#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif


RcppExport SEXP _rcpp_module_boot_class_CGALpolygon();
RcppExport SEXP _rcpp_module_boot_class_CGALpolygonWithHoles();

static const R_CallMethodDef CallEntries[] = {
    {"_rcpp_module_boot_class_CGALpolygon", (DL_FUNC) &_rcpp_module_boot_class_CGALpolygon, 0},
    {"_rcpp_module_boot_class_CGALpolygonWithHoles", (DL_FUNC) &_rcpp_module_boot_class_CGALpolygonWithHoles, 0},
    {NULL, NULL, 0}
};

RcppExport void R_init_cgalPolygons(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}