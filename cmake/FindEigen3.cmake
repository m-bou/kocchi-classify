# Try to find Eigen and validate that it is installed as it should be
# Once done it will define
# - EIGEN_FOUND
# - EIGEN_INCLUDE_DIRS

# We won't be using PkgConfig and maybe this should change in the future, all
# we are about to do is check if we can find the file <Eigen/Core> and add that
# in the include dirs

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Eigen "Eigen not found" EIGEN_INCLUDE_DIRS)
