# Try to find Eigen and validate that it is installed as it should be
# Once done it will define
# - LDA_INCLUDE_DIRS

# We won't be using PkgConfig and maybe this should change in the future, all
# we are about to do is check if we can find the file <Eigen/Core> and add that
# in the include dirs

# find_path(LDA_INCLUDE_DIRS ldaplusplus)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LDA++ "LDA++ not found" LDA_INCLUDE_DIRS)
