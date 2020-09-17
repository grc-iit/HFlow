# CMake generated Testfile for 
# Source directory: /home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit
# Build directory: /home/lukemartinlogan/Documents/Projects/PhD/rhea/cmake-build-debug/scs_io_common/test/unit
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(args_conv "/home/lukemartinlogan/Documents/Projects/PhD/rhea/cmake-build-debug/scs_io_common/test/unit/arg_test" "-test" "arg_conv" "-a" "hello" "-b" "24245" "-c" "35k" "-d" "2.0")
set_tests_properties(args_conv PROPERTIES  _BACKTRACE_TRIPLES "/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;23;add_test;/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;0;")
add_test(args_list "/home/lukemartinlogan/Documents/Projects/PhD/rhea/cmake-build-debug/scs_io_common/test/unit/arg_test" "-test" "arg_list" "-b" "24245" "-b" "24245" "-b" "24245")
set_tests_properties(args_list PROPERTIES  _BACKTRACE_TRIPLES "/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;25;add_test;/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;0;")
add_test(args_default "/home/lukemartinlogan/Documents/Projects/PhD/rhea/cmake-build-debug/scs_io_common/test/unit/arg_test" "-test" "default_args" "-e" "24245" "-e" "24245" "-e" "24245")
set_tests_properties(args_default PROPERTIES  _BACKTRACE_TRIPLES "/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;27;add_test;/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;0;")
add_test(args_invalid "/home/lukemartinlogan/Documents/Projects/PhD/rhea/cmake-build-debug/scs_io_common/test/unit/arg_test" "-test" "invalid_args" "-asdf")
set_tests_properties(args_invalid PROPERTIES  _BACKTRACE_TRIPLES "/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;29;add_test;/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;0;")
add_test(args_invalid2 "/home/lukemartinlogan/Documents/Projects/PhD/rhea/cmake-build-debug/scs_io_common/test/unit/arg_test" "-test" "invalid_args" "-e" "-e" "24245" "-e" "24245")
set_tests_properties(args_invalid2 PROPERTIES  _BACKTRACE_TRIPLES "/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;31;add_test;/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;0;")
add_test(args_no_params "/home/lukemartinlogan/Documents/Projects/PhD/rhea/cmake-build-debug/scs_io_common/test/unit/arg_test" "-test" "no_param_args" "-g" "-h")
set_tests_properties(args_no_params PROPERTIES  _BACKTRACE_TRIPLES "/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;33;add_test;/home/lukemartinlogan/Documents/Projects/PhD/rhea/scs_io_common/test/unit/CMakeLists.txt;0;")
