package(default_visibility = ["//visibility:public"])

cc_library(
    name = "config",
    hdrs = ["config.h"],
)

cc_library(
    name = "utils",
    srcs = ["utils.cc"],
    hdrs = ["utils.h"],
    deps = [
        "//base:logging",
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_library(
    name = "mmap_file",
    srcs = ["mmap_file.cc"],
    hdrs = ["mmap_file.h"],
    deps = [
        "//base:logging",
        "//base/file",
        "//utils/status",
        "//utils/status:compat_errno_macros",
    ],
)

cc_test(
    name = "mmap_file_test",
    srcs = ["mmap_file_test.cc"],
    deps = [
        ":mmap_file",
        "//utils:scoped_directory",
        "//utils/status:testing",
        "//utils/testing:test_main",
        "@com_google_absl//absl/memory",
    ],
)

cc_library(
    name = "file_handle_wrapper",
    srcs = ["file_handle_wrapper.cc"],
    hdrs = ["file_handle_wrapper.h"],
    deps = [
        "//base/file",
        "//utils/status",
    ],
)

cc_test(
    name = "file_handle_wrapper_test",
    srcs = ["file_handle_wrapper_test.cc"],
    deps = [
        ":file_handle_wrapper",
        "//utils:scoped_directory",
        "//utils/status:testing",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "base_db",
    hdrs = ["base_db.h"],
    deps = [
        "//utils/status",
    ],
)

cc_library(
    name = "db_impl",
    srcs = ["db_impl.cc"],
    hdrs = ["db_impl.h"],
    deps = [
        ":base_db",
        ":config",
        ":file_handle_wrapper",
        ":mmap_file",
        ":utils",
        "//base/file",
        "//ghost/mutex",
        "//utils:scope_guard",
        "//utils:time_utils",
        "//utils/strings:resize_uninitialized",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "db_test",
    srcs = ["db_test.cc"],
    linkstatic = True,
    deps = [
        ":db_impl",
        "//utils:scoped_directory",
        "//utils:thread_pool",
        "//utils/status:testing",
        "//utils/testing:test_main",
    ],
)

cc_binary(
    name = "bitcask_benchmark",
    srcs = ["bitcask_benchmark.cc"],
    deps = [
        ":db_impl",
        "//utils:scoped_directory",
        "@benchmark",
    ],
)
